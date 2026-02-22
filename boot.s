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
.global stack_bottom, stack_top
stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    cli
    mov $stack_top, %esp # stack init temporaire

    # charger GDTR
    lgdt gdt_descriptor

    # 0x10 kernel data segment
    # selecteurs speciaux es = fs = gs = ds = 0x10 (standard recommandé)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # 0x18 kernel stack segment puis notre stack init
    mov $0x18, %ax
    mov %ax, %ss
    mov $stack_top, %esp

    ljmp $0x08, $kernel_main
    # 0x08 kernel code segment

    cli
1:  hlt
    jmp 1b

.size _start, .-_start

.section .gdt
.align 8
.global gdt_start, gdt_end, gdt_descriptor

gdt_start:
    # 0x00 - 0o - Null segment
    # sécurité en cas de selecteur invalide (valeur sentinelle)
    .quad 0x0000000000000000

    # 0x08 - +8o = 16o - Kernel Code Segment (ring 0)
    .quad 0x00CF9A000000FFFF

    # 0x10 - +8o = 24o - Kernel Data Segment (ring 0)
    .quad 0x00CF92000000FFFF

    # 0x18 - +8o = 32o - Kernel Stack Segment (ring 0)
    .quad 0x00CF92000000FFFF

    # 0x20 - +8o = 40o - User Code Segment (ring 3)
    .quad 0x00CFFA000000FFFF

    # 0x28 - +8o = 48o - User Data Segment (ring 3)
    .quad 0x00CFF2000000FFFF

    # 0x30 - +8o = 56o - User Stack Segment (ring 3)
    .quad 0x00CFF2000000FFFF

gdt_end: # 0x38 = 56 -> 56o 7 segments × 8o
gdt_descriptor:
    .word gdt_end - gdt_start - 1 # taille (limit)
    .long gdt_start               # base (adresse)

    # 9A code segment, executable, readable, access =0
    # 92 data segment, writable, access =0
    # FA/F2 same as above mais DPL = (ring 3)
    # CF granularity = 4 KB, 32 bits
    # FFFF limit d'un segment (limit x granularity = taille max memoire)
    # 0x00000000 toutes les bases (= flat memory model)

    # Pour 0x00CF9A000000FFFF :
    # Bits 0-15   : Segment Limit [15:0]      FFFF
    # Bits 16-31  : Base Address [15:0]       0000
    # Bits 32-39  : Base Address [23:16]      00
    # Bits 40-47  : Access Byte (type, S, DPL, P)  9A
    # Bits 48-51  : Segment Limit [19:16]     F
    # Bits 52-55  : Flags (AVL, L, D/B, G)   C
    # Bits 56-63  : Base Address [31:24]     00

    # P : Present = segment valide
    # DPL : descriptor Privilege Level = 0 (kernel ring 0)
    # type : (code segment exec + visible)
    # S : Descriptor type = 1 (code/data)
    # AVL sert a rien
    # D/B : (default operation size)
    # L = 0 (32bits i386)
    # G : granularity
