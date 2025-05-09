.section .text
.global load_gdt

load_gdt:
    # Charger l'adresse de la GDT (argument passé via la pile) dans %eax
    movl 4(%esp), %eax       # 4(%esp) est l'emplacement du 1er argument en C
    lgdt (%eax)              # Charger la GDT depuis l'adresse pointée par %eax

    # Recharger les registres de segment avec les sélecteurs appropriés
    movw $0x10, %ax          # Sélecteur de segment de données (2e entrée dans GDT)
    movw %ax, %ds            # Charger dans DS
    movw %ax, %es            # Charger dans ES
    movw %ax, %fs            # Charger dans FS
    movw %ax, %gs            # Charger dans GS
    movw %ax, %ss            # Charger dans SS

    # Effectuer un saut loin pour recharger CS avec le sélecteur de code (1re entrée)
    ljmp $0x08, $flush       # Far jump vers le sélecteur 0x08 (code segment) et l'étiquette "flush"

flush:
    ret                      # Retour à l'appelant

