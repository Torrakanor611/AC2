        .equ SFR_BASE_HI, 0xBF88      # 16 MSbits of SFR area       
        .equ TRISE, 0x6100            # TRISE address is 0xBF886100      
        .equ PORTE, 0x6110            # PORTE address is 0xBF886110      
        .equ LATE,  0x6120            # LATE  address is 0xBF886120

        .equ TRISB, 0x6040
        .equ PORTB, 0x6050
        .equ LATB,  0x6060

        .equ    READ_CORE_TIMER,11        
        .equ    RESET_CORE_TIMER, 12 

        .data
        .text
        .globl main

# $s0 -> v


# configuracao dos portos

main:   li $s0, 0

        lui $t1, SFR_BASE_HI
        lw $t2, TRISE($t1)
        andi $t2, $t2, 0xFFFE       # bit0 = 0
        sw $t2, TRISE($t1)

while:  sw $s0, LATE($t1)           # LATE0 = v;

        li $a0, 500
        jal delay                   # delay(500);

        xori $s0, $s0, 1            # v = (v xor 1);

        j while

        jr $ra


##--------------------

delay:   

for:    ble $a0, 0, endfor

        li $v0, RESET_CORE_TIMER
        syscall

while2: li $v0, READ_CORE_TIMER 
        syscall
        blt $v0, 20000, while2

        addi $a0, $a0, -1

        j for
endfor:
    jr $ra
