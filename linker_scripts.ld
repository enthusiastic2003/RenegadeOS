ENTRY(start)

/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

SECTIONS {
    . = 1M;
    
    .mb2_hdr :
    {
        *(.mb2_hdr)
    }

    .mb2_text :
    {
        *(.mb2_text)
    }
    
    /* The higher-half kernel virtual address offset */
    kernOffset = 0xFFFFFFFF80000000;
	. += kernOffset; /* Add the offset so that all future lables are linked here */

	/* Add a symbol that indicates the start address of the kernel. */
	_kernel_start = .;
	_kernel_start_phys = . - kernOffset;
  	
	
	
	

	.text ALIGN (4K) : AT (ADDR (.text) - kernOffset)
	{
		*(.text)
    	code = .; _code = .; __code = .;
	}
	
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - kernOffset)
	{
		*(.rodata)
    	rodata = .; _rodata = .; __rodata = .;
	}
	
	.data ALIGN (4K) : AT (ADDR (.data) - kernOffset)
	{
		*(.data)
    	data = .; _data = .; __data = .;
	}

	.bss  ALIGN (4K): AT (ADDR (.bss) - kernOffset)
	{
		*(COMMON)
		*(.bss)
    	bss = .; _bss = .; __bss = .;
	}

	/* Add a symbol that indicates the end address of the kernel. */
	_kernel_end = .; /* The virtual end address of the kernel */
	_kernel_end_phys = . - kernOffset; /* The Physical end address of the kernel */
  	end = .; _end = .; __end = .;
}