#include <detpic32.h>


int main(void){

    // configure Timer
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;
    T3CONbits.TON = 1;


    // Configure Timer Interrupts
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

    EnableInterrupts();

    while(1);

    return 0;
}

void _int_(12) isr_T3(void){
    static int counter = 0;
    static int print_flag = 0;

    if(print_flag){
        printInt10(counter);
        putChar('\n');
        counter++;
    }

    print_flag = !print_flag;

    IFS0bits.T3IF = 0;          // reset Timer3 flag
}
