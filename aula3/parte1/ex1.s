        .equ SFR_BASE_HI, 0xBF88      # 16 MSbits of SFR area       
        .equ TRISE, 0x6100            # TRISE address is 0xBF886100      
        .equ PORTE, 0x6110            # PORTE address is 0xBF886110      
        .equ LATE,  0x6120            # LATE  address is 0xBF886120

        .equ TRISB, 0x6040
        .equ PORTB, 0x6050
        .equ LATB,  0x6060

        .data
        .text
        .globl main

# configuracao dos portos

main:   lui $t1, SFR_BASE_HI        
        lw $t2, TRISE($t1)
        andi $t2, $t2, 0xFFFE       # bit0 = 0
        sw $t2, TRISE($t1)

        lw $t2, TRISB($t1)
        ori $t2, $t2, 1             # bit0 = 1
        sw $t2, TRISB($t1)

# ciclo infinito

while:  lw $t2, PORTB($t1)          # READ
        andi $t2, $t2, 0x0001       # reset all bits except bit0

        #xori $t2, $t2, 1            # xori para inverter o sinal do bit

        sw $t2, LATE($t1)           # WRITE

        j while

        jr $ra