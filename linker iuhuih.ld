OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

SECTIONS
{
    /* Multiboot header au début du fichier ELF (GRUB le lit là) */
    .multiboot ALIGN(4) : {
        KEEP(*(.multiboot))
    } : AT(0x0)

    /* GDT dans la RAM à 0x800 */
    . = 0x800;
    .gdt ALIGN(8) : {
        KEEP(*(.gdt))
    }

    /* Code du kernel */
    . = 0x1000;
    .text ALIGN(2K) : {
        *(.text)
    }

    /* Sections de données */
    .rodata ALIGN(4K) : { *(.rodata) }
    .data   ALIGN(4K) : { *(.data) }
    .bss    ALIGN(4K) : { *(.bss) *(COMMON) }
}
