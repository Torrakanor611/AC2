#include <detpic32.h>

// admitindo que a configuração do módulo de PWM já foi feita
void setPWM(unsigned int dutyCycle){
    if(dutyCycle < 100 && dutyCycle > 0){
        OC1RS = (780 * dutyCycle)/100;       // OC1RS = PRx * dutyCicle / 100 
    }
}

