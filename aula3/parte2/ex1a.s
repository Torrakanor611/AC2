        .equ SFR_BASE_HI, 0xBF88      # 16 MSbits of SFR area       
        .equ TRISE, 0x6100            # TRISE address is 0xBF886100      
        .equ PORTE, 0x6110            # PORTE address is 0xBF886110      
        .equ LATE,  0x6120            # LATE  address is 0xBF886120

        .equ    READ_CORE_TIMER,11        
        .equ    RESET_CORE_TIMER, 12

        .data
        .text
        .globl main

# $s0 -> counter

# configuracao dos portos

main:   lui $t1, SFR_BASE_HI

        lw $t2, TRISE($t1)
        andi $t2, $t2, 0xFFF0       # bit3_0 = 0000
        sw $t2, TRISE($t1)

        li $s0, 0                   # counter = 0;

while:  sw $s0, LATE($t1)

        addi $s0, $s0, 1

        li $a0, 1000
        jal delay                   # delay(500);

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
