        .equ printStr, 8
        .equ printInt, 6
        .equ printInt10, 7
        .equ readInt10, 5
        .data
value:  .space 4
msg1:   .asciiz "\nIntroduza um número(sinal e módulo): "
msg2:   .asciiz "\nValor lido, em base 2: "
msg3:   .asciiz "\nValor lido, em base 16: "
msg4:   .asciiz "\nValor lido, em base 10(unsigned): "
msg5:   .asciiz "\nValor lido, em base 10(signed): "
        .text
        .globl main
main:   nop
while:  la $a0, msg1
        li $v0, printStr
        syscall             #printStr("\nIntroduza um número(sinal e módulo): ")

        li $v0, readInt10
        syscall
        move $s0, $v0       # readInt10();

        la $a0, msg2
        li $v0, printStr
        syscall             #printStr("\nValor lido, em base 2: ")

        move $a0, $s0
        li $a1, 2
        li $v0, printInt
        syscall             # printInt(value, 2);

        la $a0, msg3
        li $v0, printStr
        syscall             # printStr("\nValor lido, em base 16: ");

        move $a0, $s0
        li $a1, 16
        li $v0, printInt
        syscall             # printInt(value, 16);

        la $a0, msg4
        li $v0, printStr
        syscall             # printStr("\nValor lido, em base 10(unsigned): ");

        move $a0, $s0
        li $a1, 10
        li $v0, printInt
        syscall             # printInt(value, 10);

        la $a0, msg5
        li $v0, printStr    
        syscall             # printStr("\nValor lido, em base 10 (signed): ");

        move $a0, $s0
        li $v0, printInt10
        syscall

j       while

        li $v0, 0
        jr $ra

        