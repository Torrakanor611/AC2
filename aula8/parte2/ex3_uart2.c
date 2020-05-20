#include <detpic32.h>

volatile int voltage = 0;   // Global variable
volatile int voltMin = 33, voltMax = 0;
int value2displays = 0;

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

// admitindo que a configuração do módulo de PWM já foi feita
void setPWM(unsigned int dutyCycle){
    if(dutyCycle <= 100 && dutyCycle >= 0){
        OC1RS = (780 * dutyCycle)/100;       // OC1RS = PRx * dutyCicle / 100 
    }
}

void ConfigUart(unsigned int baud, char parity, unsigned int stopbits){
    // Configure UART1:       
    // 1 - Configure BaudRate Generator
    if(baud < 600 && baud > 115200){
        baud = 115200;
    }

    U2MODEbits.BRGH = (PBCLK + 8 * baud) / (16 * baud) - 1;

    // 2 – Configure number of data bits, parity and number of stop bits       
    //     (see U1MODE register)
    // data bits / parity / stop bits
    U2MODEbits.BRGH = 0;   // Divisão por 16

    if(parity == 'N'){
        U2MODEbits.PDSEL = 0;  // 8 bits sem paridade
    }else if(parity == 'E'){
        U2MODEbits.PDSEL = 1;  // 8 bits paridade ímpar
    }else if(parity == 'O'){
        U2MODEbits.PDSEL = 0x10;  // 8 bits paridade par
    }else{
        U2MODEbits.PDSEL = 0;  // 8 bits sem paridade
    }
    U2MODEbits.PDSEL = 0;  // 8 bits sem paridade
    U2MODEbits.STSEL = stopbits - 1;  // 1 stop bit

    // Enable UART interrupts
    IEC1bits.U2RXIE = 1;
    // Configure interrupt priority
    IPC8bits.U2IP = 2;
    // Enable UART Interruption flag
    IFS1bits.U2RXIF = 0;

    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;
    // 4 – Enable UART1 (see register U1MODE)    
    U2MODEbits.ON = 1;
}


void configureAll(){

    // configure Timer 1 (Type A)
    T1CONbits.TCKPS = 3;
    PR1 = 19530;
    TMR1 = 0;
    T1CONbits.TON = 1;          

    // configure interrupts for Timer 1
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;

    // configure Timer 3 (Type B)
    T3CONbits.TCKPS = 7;
    PR3 = 780;
    TMR3 = 0;
    T3CONbits.TON = 1;

    // configure interrupts for Timer 3
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;

    // Configure the A/D module and port RB4 as analog input // potenciometro
    TRISBbits.TRISB4 = 1;           // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;          // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 7;           // 8 amostras consecutivas CUIDADO

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;             // Enable A/D converter

    // Configure interrupt system
    IPC6bits.AD1IP = 2;             // configure priority of A/D interrupts to 2

    IEC1bits.AD1IE = 1;             // enable A/D interrupts

    // Configure display
    TRISB = TRISB & 0x00FF;                  // configure RB8-RB14 as outputs         
    TRISD = TRISD & 0xFF9F;                 // configure RD5-RD6 as outputs

    // Configure RB0 e RB1 to input
    TRISB = TRISB | 0x3;

    // configure timer T2 to 100 Hz
    // configure Timer 2 (Type B)
    T2CONbits.TCKPS = 7;
    PR2 = 780;
    TMR2 = 0;
    T2CONbits.TON = 1;          // enable timer 2

    // configure PWM
    OC1CONbits.OCM = 6;         // PWM mode on OCx; fault pin disabled    
    OC1CONbits.OCTSEL = 0;      // Use timer T2 as the time base for PWM generation    
    OC1RS = 12500;              //Ton constante defenida na função setPWM    
    OC1CONbits.ON = 1;          // Enable OC1 module 
}

void putc(char byte2send){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte2send;
}

void putString(char *str){       
    // use putc() function to send each charater ('\0' should not//  be sent)    
    int i = 0;
    while(str[i] != '\0'){
        putc(str[i]);
        i++;
    }
}

// isr for timer 1 - 4 Hz, conversion frequecy
void _int_(4) isr_T1(void){
    AD1CON1bits.ASAM = 1;       // start ADC conversion
    IFS0bits.T1IF = 0;          // reset Timer1 flag
}

// isr for timer 3 - 100 Hz, display refresh time
void _int_(12) isr_T3(void){
    static int x = 0;
    x++;
    send2displays(value2displays);

    if(x == 100){
        putString("Voltage: ");
        char dec = 0 | (voltage / 10);
        putc(dec);
        char uni = 0x30 | (voltage % 10);
        putc(uni);

        putc('\n');

        x = 0;
    }

    IFS0bits.T3IF = 0;          // reset Timer3 flag
}

void _int_(24) isr_uart1(void){

    // error detection
    if(IFS0bits.U1EIF == 1){
        if(U2STAbits.OERR == 1){
            U2STAbits.OERR = 0;
        }else{
            char c = U2RXREG;
        }

        IFS1bits.U2RXIF = 0;     // clear Uart1 Interrupt flag
    }

    if(IFS1bits.U2RXIF == 1){
        if(U2RXREG == 'L'){
            putString("maxVolt: ");
            char dec =  0 | (voltage / 10);
            putc(dec);
            char uni = 0x30 | (voltage % 10);
            putc(uni);
            putc('\n');

            putString("minVolt: ");
            dec = 0x30 | ((voltMin & 0xF0) >> 4);
            uni = 0x30 | (voltMin & 0x0F);
            putc('\n');
        }
    }

    IFS1bits.U2RXIF = 0;     // clear Uart1 Interrupt flag
}

// Interrupction Handler
void _int_(27) isr_adc(void){

    // calculate buffer average
    int* p = (int*) (&ADC1BUF0);

    int sum = 0;
    int i;
    for(i = 0; i < 8; i++){
        sum = sum + p[i*4];
    }

    int med_voltage = sum/8;
    // printInt10(med_voltage);
    // putChar('-');
    
    // printInt10(ADC1BUF0);
    // putChar('-');
    // printInt10(med_voltage / ADC1BUF0);
    // putChar('\n');

    // med_voltage = ADC1BUF0;

    voltage = (med_voltage*33 + 511) / 1023;

    value2displays = voltage;

    if(voltage < voltMin){
        voltMin = voltage;
    }

    if(voltage > voltMax){
        voltMax = voltage;
    }
    
    IFS1bits.AD1IF = 0;             // clear/reset A/D converter interrupt flag   
}

int main(void){          
    configureAll(); // Function to configure all (digital I/O, analog                           
                    // input, A/D module, timers T1 and T3, interrupts)  
    ConfigUart(115200, 'N', 1);
    
    // Reset AD1IF, T1IF and T3IF flags
    IFS1bits.AD1IF = 0;         // clear/reset A/D interrupt flag
    IFS0bits.T3IF = 0;          // reset timer 3 flag
    IFS0bits.T1IF = 0;          // reset timer 1 flag
    IFS1bits.U2RXIF = 0;        // clear Uart1 Interrupt flag

    EnableInterrupts();            // Global Interrupt Enable

    int dutyCycle;
    int portVal;

    while(1){
        portVal = PORTB & 0x3;

        // dprintInt10(portVal);

        switch(portVal){
        case 0:
            IEC0bits.T1IE = 1;      // enable timer 1 interrupts
            setPWM(0);              // LED OFF
            break;

        case 1:
            IEC0bits.T1IE = 0;      // disable timer 1 interrupts
            setPWM(100);            // LED ON (maximum bright)
            break;
        
        default:
            IEC0bits.T1IE = 1;      // enable timer 1 interrupts
            dutyCycle = value2displays * 3;
            setPWM(dutyCycle);
            break;
        }
    }
    return 0;       
}
