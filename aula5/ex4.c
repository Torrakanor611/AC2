
#include <detpic32.h>

 int main(void){       
    // Configure the A/D module and port RB4 as analog input
    TRISBbits.TRISB4 = 1;           // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;          // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 4;           // numero de conversões consecutivas

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;             // Enable A/D converter

    while(1){          
        // Start conversion
        AD1CON1bits.ASAM = 1;       // start conversion

        // Wait while conversion not done (AD1IF == 0)     
        while(IFS1bits.AD1IF == 0); // wait while conversion not done

        int* p = (int*) (&ADC1BUF0);
        int i;
        int sum = 0;
        for(i = 0; i < 4; i++){
            sum += p[i*4];
        }

        int med = sum/4;

        int v = (med*33 + 511)/1023;

        printInt(v, 10);
        putChar('\n');

        // Reset AD1IF
        IFS1bits.AD1IF = 0;       
    }       
    
    return 0;       
}
