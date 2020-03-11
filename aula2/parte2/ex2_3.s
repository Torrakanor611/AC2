        .equ    READ_CORE_TIMER,11        
        .equ    RESET_CORE_TIMER, 12       
        .equ    PUT_CHAR, 3      
        .equ    PRINT_INT, 6
        .data
        .text
        .globl main

main:   

while:  lui $t1, 0xBF88
        lw $t2, 0x6050($t1)

        #andi $t2, $t2, 0x000F 

        move $a0, $t2
        li $a1, 2
        li $v0, PRINT_INT
        syscall

        li      $a0,'\n'            #        
        li      $v0,PUT_CHAR        #        
        syscall                     #  putChar('\n')  

        li $a0, 500
        jal delay

        j while

        jr $ra

##--------------------

delay:   

for:    ble $a0, 0, endfor

        li $v0, RESET_CORE_TIMER
        syscall

while2:  li $v0, READ_CORE_TIMER 
        syscall
        blt $v0, 20000, while2

        addi $a0, $a0, -1

        j for
endfor:
    jr $ra
