
#include <detpic32.h>

 int main(void){
    static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};       
    // Configure the A/D module and port RB4 as analog input
    TRISBbits.TRISB4 = 1;           // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;          // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 3;           // numero de conversões consecutivas

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;             // Enable A/D converter


    // Configure display
    TRISB = TRISB & 0x80FF;

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    int display_flag = 0;
    int v = 0;
    int i = 0;

    while(1){
        if(i++ % 25 == 0){ // 0, 250ms, 500ms, 750ms, ... 
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

            v = (med*33+511)/1023;
            
            // Reset AD1IF
            IFS1bits.AD1IF = 0;
        }

        int dez = v / 10;
        int uni = v % 10;

        display_flag = !display_flag;

        if(display_flag){
            LATDbits.LATD6 = 0;
            LATDbits.LATD5 = 1;
            LATB = (LATB & 0x80FF) | (display7Scodes[uni] << 8);
        }else{
            LATDbits.LATD5 = 0;
            LATDbits.LATD6 = 1;
            LATB = (LATB & 0x80FF) | (display7Scodes[dez] << 8);
        }

        resetCoreTimer();
        while(readCoreTimer() < 200000);
    
    }
    return 0;
 }
