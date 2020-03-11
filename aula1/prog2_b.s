        .data
c:      .space 1
        .text
        .globl main
main:   
do: 
        li $v0, 2
        syscall             # getChar();

        move $s0, $v0       # $s0 = c;

while1:  li $v0, 1
        syscall
        move $s1, $v0

        bne $s1, '0', endwhl
        j while1
endwhl:

 if:    beq $s0, '\n' , endif

        move $a0, $s0
        li $v0, 3
        syscall             # putChar();
endif:

while:  bne $s0, '\n', do

        li $v0, 0
        jr $ra              # return 0;    
