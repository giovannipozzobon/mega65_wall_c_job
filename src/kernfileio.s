
	.rtmodel version,"1"
	.rtmodel codeModel,"plain"
	.rtmodel core,"45gs02"
	.rtmodel target,"mega65"
	.extern _Zp
    .extern map_regs
    .extern m65_dirent_exchange

    .section code,text
    .public fill, setbank, setbank_attic, setbank_map_regs, setfssave, setfsload, setname, savefile1, loadfile1, setname1, savefilewithpars, loadfilewithpars, opendir, closedir, readdir


kcall  .macro  kaddr 
    pha
    tba 
    sta actual_base
    lda kernal_base_page
    tab
    pla
    jsr \kaddr
    pha
    lda actual_base
    tab
    pla
    .endm

fill:
    ; fill memory with data to test saving
      ldx #0 
loop: txa
      sta 0x4500 ,x 
      inx
      bne loop
      rts 

setbank:
    sei
    lda #0x04 
    ldx #00 
    ;jsr 0xff6b ; setbank
    kcall 0xff6b
    rts

setbank_attic:
    sei
    lda #0x88
    ldy #0x03
    ldx #0x00
    jsr 0xff6b
    rts

setbank_map_regs:
    sei
    lda zp:map_regs
    ldy zp:map_regs+2
    ldx zp:map_regs+1
    ;brk
    jsr 0xff6b
    rts

setfssave:    
    lda #2
    ldx #1
    ldy #2
    ;jsr 0xffba ;setlfs
    kcall 0xffba
    rts
    ; memory in bank 0
    ; filename in bank 0
    ; (not used by save) ; device number

setfsload:    
    lda #00
    ldx #0x08
    ldy #00
    jsr 0xffba ;setlfs
    rts

setname:    
    ; save
  
    lda #filename_end - filename
    ldx #.byte0 filename
    ldy #.byte1 filename   
    jsr 0xffbd ;setname
    rts

setname1:    
    ; A has len of filename
    ; x & y the address of string of filename
    ldx zp:_Zp
    ldy zp:_Zp+1   
    jsr 0xffbd ;setname
    rts


savefile1:   
    ; Save 0.4500 to 0.45FF lda #$00
    lda #0x0
    sta 0xfe
    lda #0x45
    sta 0xff 
    lda #0xfe 
    ldx #00 
    ldy #0x46
    clc
    ;jsr 0xffd8 ;save 
    kcall 0xffd8
    bcs errsf ; jmp if error (Carry Flag Clear) A has error code
    lda #0
errsf:
    cli
    rts

 
savefilewithpars:   
    lda zp:map_regs+3
    sta 0xfe
    lda zp:map_regs
    sta 0xff 
    lda #0xfe 
    ldx zp:map_regs+1
    ldy zp:map_regs+2
    ;brk
    clc
    jsr 0xffd8 ;save 
    bcs errsfp ; jmp if error (Carry Flag Clear) A has error code
    lda #0
errsfp:
    cli
    rts

loadfilewithpars:
    lda zp:map_regs
    ldy zp:map_regs+2
    sty load_add_msb ;save the load address
    ldx zp:map_regs+1
    stx load_add_lsb
    clc
    jsr 0xffd5
    bcs errlfp   ;jmp if error (Carry Flag Clear) A has error code
    lda #0
    sta zp:map_regs
    ; calculate the number of char read
    ;stx save_add_lsb 
    ;sty save_add_msb
    txa
    sec
    sbc load_add_lsb
    sta zp:map_regs+1  
    tya
    sbc load_add_msb
    sta zp:map_regs+2 
    lda #0         
errlfp:
    cli
    rts

loadfile1:
    lda #0
    ldx #0x00
    ldy #0x45  ;address is NOT equal to the save address
    clc
    jsr 0xffd5
    bcs errlf   ;jmp if error (Carry Flag Clear) A has error code
    lda #0
errlf:
    cli
    rts


	;; Opendir takes no arguments and returns File descriptor in A
opendir:
	lda #0x12
	sta 0xD640
	clv
    bcc errhypopendir
    sta zp:map_regs
    jmp donehypopendir
errhypopendir:
    ldx #0x00
    stx zp:map_regs+1
    lda #0xff
	sta zp:map_regs
donehypopendir:
    nop
	rts


	;; closedir takes file descriptor as argument (appears in A)
closedir:
	ldx zp:map_regs+1 
	lda #0x16
	sta 0xD640
	clv
    bcc errhypclosedir
    stx zp:map_regs+1 
	jmp donehypclosedir
errhypclosedir:
    sta zp:map_regs
donehypclosedir:
    nop
	rts


	;; readdir takes the file descriptor returned by opendir as argument
	;; and gets a pointer to a MEGA65 DOS dirent structure.
	;; Again, the annoyance of the MEGA65 Hypervisor requiring a page aligned
	;; transfer area is a nuisance here. We will use 0x0400-0x04FF, and then
	;; copy the result into a regular C dirent structure
	;;
	;; d_ino = first cluster of file
	;; d_off = offset of directory entry in cluster
	;; d_reclen = size of the dirent on disk (32 bytes)
	;; d_type = file/directory type
	;; d_name = name of file
readdir:
    ;phx
    ; save the MSB address of m65 dir struct
    ldx zp:map_regs+1
    ldy zp:map_regs+2
    stx filedesc
    sty m65_struct_dir_addr+1
    ;; First, clear out the dirent
	ldx #0
	txa
l1:
	sta m65_struct_dir_addr,x	
	dex
	bne l1

	;; Third, call the hypervisor trap
	;; File descriptor gets passed in in X.
	;; Result gets written to transfer area we setup at 0x0400
	;plx
    ldx filedesc
;	ldy #>0x0400 		; write dirent to 0x0400 
	ldy #.byte1 0x0400 		
	lda #0x14
	sta 0xD640
	clv
	bcs readDirSuccess
	;;  Return end of directory
	lda #0x00
	ldx #0x00
	rts

readDirSuccess:
	;;  Copy file name
	ldx #0x3f
l2:
	lda 0x0400,x
	sta (m65_dirent_exchange)+4+2+4+2,x
	dex
	bpl l2
	;; make sure it is null terminated
	ldx 0x0400+64
	lda #0x00
	sta zp:map_regs+4+2+4+2,x

	;; Inode = cluster from offset 64+1+12 = 77
	ldx #0x03
l3:
	lda 0x0477,x
	sta zp:map_regs+0,x
	dex
	bpl l3
	;; d_off stays zero as it is not meaningful here
	;; d_reclen we preload with the length of the file (this saves calling stat() on the MEGA65)
	ldx #3
l4:
	lda 0x0400+64+1+12+4,x
	sta zp:map_regs+4+2,x
	dex
	bpl l4
	;; File type and attributes
	;; XXX - We should translate these to C style meanings
	lda 0x0400+64+1+12+4+4
	sta zp:map_regs+4+2+4
	;; Return address of dirent structure
	;lda #<zp:m65_dirent_exchange
	;ldx #>zp:m65_dirent_exchange	
	lda #.byte0 map_regs
	ldx #.byte1 map_regs	

	rts


    .section data, data
    filename:
    ;.byte 0x66, 0x69, 0x6C, 0x65, 0x2E, 0x73, 0x65, 0x71, 0x00
    ;.byte 0x66, 0x69, 0x6C, 0x65, 0x2C, 0x73, 0x00
    .byte 64, 58, 70, 73, 76, 69, 69, 68, 73, 84, 44, 83, 0x00
    filename_end:
    kernal_base_page: .byte 0x00
    actual_base: .byte 0x00
    load_add_lsb: .byte 0x00
    load_add_msb: .byte 0x00
    save_add_lsb: .byte 0x00
    save_add_msb: .byte 0x00
    filedesc: .byte 0x00 
    m65_struct_dir_addr: .word 0x00    
    //attenzione a non mettere nessuna variabile dopo perché verrà azzerata da una routine

