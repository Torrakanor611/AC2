
#include <detpic32.h>


int main(void){          
    static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};          
    // configure RB0 to RB3 as inputs
    TRISB = TRISB | 0xF;          
    // configure RB8 to RB14 and RD5 to RD6 as outputs
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;

    // Select display low
    LATEbits.LATE5 = 1;
    LATEbits.LATE6 = 0;

    int index = 0;

    while(1){             
        // read dip-switch             
        // convert to 7 segments code
                    
        // send to display
        LATB = (0x80FF & LATB) | (display7Scodes[PORTB & 0xF] << 8);

    }          
    return 0;       
}
