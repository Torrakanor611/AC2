
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

main:   lui $t1, SFR_BASE_HI

        lw $t2, TRISB($t1)          # get resgister
        ori $t2, $t2, 0xF           # R3_0 input        # modify
        sw $t2, TRISB($t1)              # write register

        lw $t2, TRISE($t1)
        andi $t2, $t2, 0xFFF0       # RD3_0 output
        sw $t2, TRISE($t1)

while:  lw $t2, PORTB($t1)          # read

        li $t3, 0

        andi $t3, $t2, 0x1            # retrieve bit 0
                                        # atenção é 0x1 e não 0x0 !
        sll $t4, $t3, 3

        andi $t3, $t2, 0x2            # retrieve bit 1
        sll $t5, $t3, 1

        andi $t3, $t2, 0x4            # bit 3
        srl $t6, $t3, 1

        andi $t3, $t2, 0x8              #bit 4
        srl $t7, $t3, 3

        li $t3, 0

        or $t3, $t3, $t4
        or $t3, $t3, $t5
        or $t3, $t3, $t6
        or $t3, $t3, $t7 


        sw $t3, LATE($t1)           # write

        j while

        j $ra
