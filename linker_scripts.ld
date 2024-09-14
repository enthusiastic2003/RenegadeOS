ENTRY(start)

/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

SECTIONS {
    . = 1M;
    
	_kernel_start = .;

    .mb2_hdr :
    {
        *(.mb2_hdr)
    }
	
	.mb2_text BLOCK(4K) : ALIGN(4K)
	{
		*(.mb2_text)
	}

	. += 0xC0000000;

	.text ALIGN(4K) : AT (ADDR (.text) - 0xC0000000)
	{	
		
		*(.text)
	}
	

	/* Read-only data. */
	.rodata ALIGN(4K) : AT (ADDR (.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT (ADDR (.data) - 0xC0000000)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT (ADDR (.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
	}
	_kernel_end = .;
}


