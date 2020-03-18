
#include <detpic32.h>

void delay(int ms)
{
	int K = 20000;
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < K);
	}
}

// --------------------------------

void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

    // display low active
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;

    LATB = (0x80FF & LATB) | (display7Scodes[value >> 4] << 8);  // value / 16 = digito das dezenas

    // como fazer?

    // display high active
    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 1;

    LATB = (0x80FF & LATB) | (display7Scodes[value & 0x0F] << 8);  // value & 0x0F = digito das unidades
}

// --------------------------------

int main(void){
    // configure displays
    TRISB = TRISB & 0x00FF;                 // configure RB8-RB14 as outputs         
    TRISD = TRISD & 0xFF9F;                 // configure RD5-RD6 as outputs

    int counter = 0;

    while(1){
        send2displays((unsigned char)counter);

        delay(200);

        counter++;
        //putChar((char)counter);

    }

    return 0;
}
