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
	.text BLOCK(4K) : ALIGN(4K)
	{	
		
		*(.text)
	}
	

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}


