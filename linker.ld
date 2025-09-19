OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

SECTIONS
{
    /* Placer la GDT à 0x800 */
    . = 0x800;
    .gdt :
    {
        KEEP(*(.gdt))
    }

    /* Aligner le reste du bootloader et kernel */
    . = 2M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss)
        *(COMMON)
    }
}
