ENTRY(_start)
 
SECTIONS
{
	/* Begin putting sections at 64 KiB, a conventional place for bootloaders to be loaded at. */
	. = 0x10000;
 
	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text ALIGN(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
 
	/* Read-only data. */
	.rodata ALIGN(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data ALIGN(4K) : ALIGN(4K)
	{
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) (NOLOAD) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		. = ALIGN(4K);
		*(.heap)
	}
}
