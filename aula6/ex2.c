
#include <detpic32.h>









int main(void){
    // Configure the A/D module and port RB4 as analog input
    TRISBbits.TRISB4 = 1;           // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;          // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 0;

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;             // Enable A/D converter

    // Configure interrupt system
    IPC6bits.AD1IP = 2;             // configure priority of A/D interrupts to 2

    IEC1bits.AD1IE = 1;             // enable A/D interrupts

    IFS1bits.AD1IF = 0;             // clear/reset A/D interrupt flag

    EnableInterrupts();

    // start A/C conversion
    AD1CON1bits.ASAM = 1;

    while(1){ }
    // a rotina de serviço à interrupção foi programada para responder ao fim de uma conversão pelo que a função é chamada automaticamente
    return 0;
}