.section .text
.global gdt_verify 

gdt_verify:
    # Réserve un espace sur la pile pour stocker la valeur du GDTR
    subl $10, %esp           # Réserver 10 octets (6 octets suffisent pour le mode 32 bits)

    # Stocke le registre GDTR dans la mémoire pointée par %esp
    sgdt (%esp)              # Sauvegarde GDTR dans l'espace réservé

    # Charger les composants de GDTR dans les registres
    movw (%esp), %ax         # Charger la limite (16 bits) dans %ax
    movl 2(%esp), %ebx       # Charger la base (32 bits) dans %ebx

    # Nettoyer la pile en restaurant l'espace réservé
    addl $10, %esp           # Libérer les 10 octets réservés

    # Retourner à l'appelant
    ret

