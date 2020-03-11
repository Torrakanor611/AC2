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

# $s0 -> escrita
# $s1 -> aux

# configuracao dos portos

main:   lui $t1, SFR_BASE_HI

        lw $t2, TRISE($t1)
        andi $t2, $t2, 0xFFF0       # bit3_0 forced to 0
        sw $t2, TRISE($t1)

        li $s0, 0

while:  sw $s0, LATE($t1)

        andi $s1, $s0, 0x8          # get bit3 of #s0

        sll $s0, $s0, 1

if:     bne $s1, 8, else

        andi $s0, $s0, 0xFFFE       # force bit0 of $s0 to 0

        j endif
else:
        ori $s0, $s0, 1             # force bit0 of $s0 to 1

endif:  li $a0, 750
        jal delay

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
    