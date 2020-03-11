
#include <detpic32.h>

int main(){

    // defenir os pinos dos displays como saídas
    TRISB = TRISB & 0x00FF;

    // defenir ambos os displays como saídas
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    // enable o display high como saída ativa e o disable o display low
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;

    while(1){
        char c = getChar();

        LATB = LATB & 0x00FF;

        if(c == 'a' || c == 'A'){
            LATBbits.LATB8 = 1;

        }else if(c == 'b' || c == 'B'){
            LATBbits.LATB9 = 1;

        }else if(c == 'c' || c == 'C'){
            LATBbits.LATB10 = 1;

        }else if(c == 'd' || c == 'D'){
            LATBbits.LATB11 = 1;

        }else if(c == 'e' || c == 'E'){
            LATBbits.LATB12 = 1;

        }else if(c == 'f' || c == 'F'){
            LATBbits.LATB13 = 1;

        }else if(c == 'g' || c == 'G'){
            LATBbits.LATB14 = 1;
        }
    }

    return 0;
}
