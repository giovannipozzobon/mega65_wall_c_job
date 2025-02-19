	.rtmodel version,"1"
	.rtmodel codeModel,"plain"
	.rtmodel core,"45gs02"
	.rtmodel target,"mega65"
	.extern _Zp

    .section code,text
    .public wait_very_briefly
    .public wait_raster



// per PAL 50 volte per secondo per NTCS 60 volte al secondo
// Wait for bit 8 of the raster count to flip negative then positive,
// for a delay between 1 and 2 frames.
wait_very_briefly:
        // A have the value of call 
        tax
loop:
--     
        bit 0xd011
        bpl --
---    
        bit 0xd011
        bmi ---
        dex
        bne loop
        rts


 wait_raster:
        //lda contains the number of raster lines to wait
--
        cmp 0xd012
        bne --
        bit 0xd011
        bmi --  // rch is high, not our stop
        rts