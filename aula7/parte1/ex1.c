#include <detpic32.h>

int main(void){
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;
    T3CONbits.TON = 1;

    int counter = 0;

    while(1){
        while(IFS0bits.T3IF == 0);
        IFS0bits.T3IF = 0;

        printInt10(counter);
        putChar('\n');
        counter++;
    }

    return 0;
}
