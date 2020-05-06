#include <detpic32.h>

void delay(int ms){
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void _int_(12)	isr_timer3(void)
{
	U1TXREG = 'U';
	IFS0bits.T3IF=0;
}

int main (void)
{
	int Tbit, i, baudrate, mbaud;

	delay(1000); 		// espera um segundo até envio da mensagem inicial	
	T3CONbits.TCKPS=7;	// Prescaler = 256
	PR3=39064; 			// Freq = 2Hz
	TMR3=0;
	T3CONbits.TON=1;
	IPC3bits.T3IP=2;
	IEC0bits.T3IE=1;
	IFS0bits.T3IF=0;

	baudrate = 4800;
	//baudrate = 57600;
	U1BRG = ((20000000 + 8 * baudrate) / (16 * baudrate)) - 1;
	while(1)
	{
		EnableInterrupts();
		while(PORTDbits.RD3 == 1);	// espera que deixe de ser 1 para sincronizar
		while(PORTDbits.RD3 == 0);	// espera pela transição de 0 --> 1
		resetCoreTimer();
		while(PORTDbits.RD3 == 1);	//espera pela transição de 1 --> 0
		Tbit = readCoreTimer()*50; 	//ms
		//while(1);			// comentar para fazer medida
		DisableInterrupts();
		printf("Tempo de bit (nseg): %d\n",Tbit);
		mbaud = (int) (1000000000/Tbit);
		printf("Baudrate: %d\n", mbaud);
		putChar('\n');
		getChar();
	}
}


