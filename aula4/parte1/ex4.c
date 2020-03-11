
#include <detpic32.h>
#include "functions.h"

int main(void){
    int i;          
    unsigned char segment;          
    LATDbits.LATD6 = 1;                     // display high active          
    LATDbits.LATD5 = 0;                     // display low inactive          
    TRISB = TRISB & 0x00FF;                 // configure RB8-RB14 as outputs         
    TRISD = TRISD & 0xFF9F;                 // configure RD5-RD6 as outputs          
    while(1){             
        LATDbits.LATD6 = !LATDbits.LATD6;   //             
        LATDbits.LATD5 = !LATDbits.LATD5;   // toggle display selection             
        segment = 1;           

        for(i=0; i < 7; i++){  

            LATB = (LATB & 0x00FF) | (segment << 8);                 // send "segment" value to display               
            delay(500);                     // wait 0.5 second          
            segment = segment << 1;             
        }          
    }          
    
    return 0;
}

void delay(int ms)
{
	int K = 20000;
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < K);
	}
}
