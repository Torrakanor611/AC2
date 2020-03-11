        .equ    READ_CORE_TIMER,11        
        .equ    RESET_CORE_TIMER, 12       
        .equ    PUT_CHAR, 3      
        .equ    PRINT_INT, 6
        .data
        .text
        .globl main

##------------------
# funcao de teste
main:   

while1: li $a0, 'b'
        li $v0, PUT_CHAR
        syscall 

        li $a0, 1000
        jal delay

        j while1

        jr $ra
##------------------
        # $S0 -> ms (numero de milisegundos a dar delay)

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
