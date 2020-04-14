#include <detpic32.h>

// admitindo que a configuração do módulo de PWM já foi feita
void setPWM(unsigned int dutyCycle){
    if(dutyCycle < 100 && dutyCycle > 0){
        OC1RS = (780 * dutyCycle)/100;       // OC1RS = PRx * dutyCicle / 100 
    }
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

void configureAll(){
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

int main(void){
    configureAll();

    int i = 0;

    while(1){

        setPWM(i);

        delay(500);

        if(i == 100){
            i = -10;
        }

        i+=10;
    }

    return 0;
}
