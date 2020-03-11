
# include <detpic32.h>
# include "functions.h"

int main(void){ 

    TRISE = TRISE & 0xFFF0;

    int counter = 0;

    while(1){
        LATE = counter;
        counter++;

        if(counter == 16){
            counter = 0;
        }

        // ou fazer o resto da divisao inteira por 16, para obrigar a ser entre 0-16

        delay(500);
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
