
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

main:   lui $t1, SFR_BASE_HI

        lw $t2, TRISB($t1)          # get resgister
        ori $t2, $t2, 0xF           # RB3_0 input        # modify
        sw $t2, TRISB($t1)              # write register

        lw $t2, TRISE($t1)
        andi $t2, $t2, 0xFFF0       # RE3_0 output
        sw $t2, TRISE($t1)

while:  lw $t2, PORTB($t1)          # read

        andi $t2, $t2, 0xF          # get 4 right bits

if:     bne $t2, 0x8, else
        sw $t2, LATE($t1)
        j end
else:   bne $t2, 0x4, else1
        sw $t2, LATE($t1)
        j end
else1:  bne $t2, 0x2, else2
        sw $t2, LATE($t1)
        j end
else2:  bne $t2, 0x1, else3
        sw $t2, LATE($t1)
        j end

else3:  ori $t2, $t2, 0xF           # all 4 bits to 1
        sw $t2, LATE($t1)           # write turn on

        # li $a0, 1000;               
        # jal delay

        # andi $t2, $t2, 0x0
        # sw $t2, LATE($t1)           # write turn off during 1 sec

end:

        j while

        j $ra



# função delay

# delay:   

# for:    ble $a0, 0, endfor

#         li $v0, RESET_CORE_TIMER
#         syscall

# while2: li $v0, READ_CORE_TIMER 
#         syscall
#         blt $v0, 20000, while2

#         addi $a0, $a0, -1

#         j for
# endfor:
#     jr $ra
