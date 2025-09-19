.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    # Initialisation pile
    mov $stack_top, %esp

    # -------------------------
    # Copier GDT à 0x800
    # -------------------------
    mov $gdt_start, %esi        # source dans le fichier
    mov $0x800, %edi            # destination en RAM
    mov $(gdt_end - gdt_start), %ecx
    shr $2, %ecx                # nombre de mots de 4 octets
1:
    lodsl
    stosl
    loop 1b

    # -------------------------
    # Charger GDT depuis 0x800
    # -------------------------
    lgdt [gdt_descriptor_ram]

    # Charger segments
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Far jump pour flush CS
    ljmp $0x08, $flush

flush:
    call kernel_main   # votre kernel
    cli
1:  hlt
    jmp 1b

.size _start, . - _start

# ==========================
# GDT originale dans le fichier
# ==========================
.section .gdt
.align 8
.global gdt_start, gdt_end

gdt_start:
    # Null segment
    .long 0x00000000
    .long 0x00000000

    # Code segment 0x00CF9A000000FFFF
    .long 0x0000FFFF
    .long 0x00CF9A00

    # Data segment 0x00CF92000000FFFF
    .long 0x0000FFFF
    .long 0x00CF9200
gdt_end:

# --------------------------
# Descripteur pour lgdt
# --------------------------
gdt_descriptor_ram:
    .word gdt_end - gdt_start - 1   # limite
    .long 0x800                     # base = 0x800
