
#include <detpic32.h>


unsigned int voltage = 0;

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
    static char display_flag = 0; // variavel estática: não altera o seu valor em chamadas consecutivas a uma função

    unsigned char digit_low = value % 10;           // cuidado
    unsigned char digit_high = (int) (value/10);

    if(display_flag){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;

        LATB = (LATB & 0x80FF) | display7Scodes[digit_low] << 8;
    }else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;

        LATB = (LATB & 0x80FF) | display7Scodes[digit_high] << 8;
    }

    display_flag = !display_flag;

}

// Interrupction Handler
void _int_(27) isr_adc(void){

    // calculate buffer average
    int* p = (int*) (&ADC1BUF0);

    int sum = 0;
    int i;
    for(i = 0; i < 8; i++){
        sum += p[i*4];
    }

    int med_voltage; // = sum/8;

    med_voltage = ADC1BUF0;

    voltage = (med_voltage*33 + 511) / 1023;

    
    //putChar('\n');
    //putChar('i');
    //printInt((int) voltage , 10);



    IFS1bits.AD1IF = 0;             // clear/reset A/D interrupt flag   

}


int main(void){
    // Configure the A/D module and port RB4 as analog input // potenciometro
    TRISBbits.TRISB4 = 1;           // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;          // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 0;           // 8 amostras consecutivas

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;             // Enable A/D converter

    // Configure interrupt system
    IPC6bits.AD1IP = 2;             // configure priority of A/D interrupts to 2

    IEC1bits.AD1IE = 1;             // enable A/D interrupts

    // Configure display
    TRISB = TRISB & 0x00FF;                     // configure RB8-RB14 as outputs         
    TRISD = TRISD & 0xFF9F;                 // configure RD5-RD6 as outputs

    IFS1bits.AD1IF = 0;                 // clear/reset A/D interrupt flag

    EnableInterrupts();

    int i = 0;

    while(1){ 
        delay(10);                  // refresh time = 10ms (100Hz)

        if(i++ >= 25){
            i = 0;

            // start A/C conversion
            AD1CON1bits.ASAM = 1;

        }
        printInt((int) voltage , 10);
        putChar('\n');
        send2displays(voltage);
    }
    
    return 0;
}
