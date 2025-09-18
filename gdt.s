    .section .gdt
    .align 8
    .global gdt_start, gdt_end
    .org 0x800 # décale les offsets internes de la section,
    # mais pas l’adresse virtuelle finale du binaire
    # fixée par le linker.

gdt_start:
    # Null segment
    .long 0x00000000
    .long 0x00000000

    # Code segment 0x00CF9A000000FFFF
    .long 0x0000FFFF   # low 32 bits
    .long 0x00CF9A00   # high 32 bits

    # Data segment 0x00CF92000000FFFF
    .long 0x0000FFFF
    .long 0x00CF9200
gdt_end:

    .global gdt_descriptor
gdt_descriptor:
    .word gdt_end - gdt_start - 1 # taille (limit)
    .long gdt_start               # base (adresse)
