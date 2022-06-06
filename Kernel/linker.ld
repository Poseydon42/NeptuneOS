ENTRY(_start)

KERNEL_OFFSET = 0xFFFFFF8000000000;
KERNEL_START = 0x100000;

SECTIONS
{
    . = KERNEL_START + KERNEL_OFFSET;
    StartOfKernelMemory = .;

    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_OFFSET)
    {
        *(.multiboot)
        *(.boot)
        *(.text)
    }
    
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_OFFSET)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_OFFSET)
	{
		*(.data)
	}

    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_OFFSET)
    {
        *(.bss)
    }

    EndOfKernelMemory = .;
}