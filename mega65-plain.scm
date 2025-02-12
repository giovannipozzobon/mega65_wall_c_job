(define memories '((memory program
            (address (#x2001 . #x9fff)) (type any)
            (section 
                (programStart #x2001) 
                (startup #x200e)
                (diskio_copy #x6000)
            )
            )
        (memory zeropage (address (#x2 . #xff))  (qualifier zpage)
	        (section (registers (#x2. #x7f)))
                (section zzpage)
        )
    
        (memory stackpage (address (#x100 . #x1ff)) 
                (section stack)
        )
    ))