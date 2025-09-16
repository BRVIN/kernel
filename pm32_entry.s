    .section .data
    .align 8

# -------------------------------
# Global Descriptor Table (GDT)
# -------------------------------
gdt_start:
    # Null descriptor
    .long 0x0
    .long 0x0

    # Kernel Code Segment (base=0, limit=4G, ring0)
    .short 0xFFFF         # limit low
    .short 0x0000         # base low
    .byte 0x00            # base mid
    .byte 0x9A            # access byte
    .byte 0xCF            # flags + limit high
    .byte 0x00            # base high

    # Kernel Data Segment (base=0, limit=4G, ring0)
    .short 0xFFFF
    .short 0x0000
    .byte 0x00
    .byte 0x92
    .byte 0xCF
    .byte 0x00

gdt_end:

gdt_descriptor:
    .short gdt_end - gdt_start - 1   # limit
    .long gdt_start                  # base

# -------------------------------
# Stack
# -------------------------------
    .align 16
stack_bottom:
    .skip 16384                      # 16 KB stack
stack_top:

# -------------------------------
# Protected mode entry
# -------------------------------
    .globl pm32_start
    .section .text
pm32_start:
    cli
    lgdt gdt_descriptor               # charger GDTR

    # Passer en mode protégé
    movl %cr0, %eax
    orl $1, %eax                     # activer PE
    movl %eax, %cr0

    # Far jump pour recharger CS
    ljmp $0x08, $pm32_continue

pm32_continue:
    # Initialiser segments data et pile
    movw $0x10, %ax                   # Kernel Data segment
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    movl $stack_top, %esp             # initialiser la pile

    ret                                # retour vers le kernel C ou boucle infinie
