#include <detpic32.h>

int main(void){

    // configure Timer 1 (Type A)
    T1CONbits.TCKPS = 3;
    PR1 = 39062;
    TMR1 = 0;
    T1CONbits.TON = 1;          

    // configure interrupts for Timer 1
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;          // reset timer 1 flag

    // configure Timer 3 (Type B)
    T3CONbits.TCKPS = 5;
    PR3 = 62499;
    TMR3 = 0;
    T3CONbits.TON = 1;

    // configure interrupts for Timer 3
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;          // reset timer 3 flag

    EnableInterrupts();

    while(1);

    return 0;
}

// isr for timer 1
void _int_(4) isr_T1(void){
    putChar('1');

    IFS0bits.T1IF = 0;          // reset Timer1 flag
}

// isr for timer 3
void _int_(12) isr_T3(void){
    putChar('3');

    IFS0bits.T3IF = 0;          // reset Timer3 flag
}
