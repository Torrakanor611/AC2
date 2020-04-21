#include<detpic32.h>

void delay(int);

void ConfigUart(unsigned int baud, char parity, unsigned int stopbits){
    // Configure UART1:       
    // 1 - Configure BaudRate Generator
    if(baud < 600 && baud > 115200){
        baud = 115200;
    }

    U1MODEbits.BRGH = (PBCLK + 8 * baud) / (16 * baud) - 1;

    // 2 – Configure number of data bits, parity and number of stop bits       
    //     (see U1MODE register)
    // data bits / parity / stop bits
    U1MODEbits.BRGH = 0;   // Divisão por 16

    if(parity == 'N'){
        U1MODEbits.PDSEL = 0;  // 8 bits sem paridade
    }else if(parity == 'E'){
        U1MODEbits.PDSEL = 1;  // 8 bits paridade ímpar
    }else if(parity == 'O'){
        U1MODEbits.PDSEL = 0x10;  // 8 bits paridade par
    }else{
        U1MODEbits.PDSEL = 0;  // 8 bits sem paridade
    }
    U1MODEbits.PDSEL = 0;  // 8 bits sem paridade
    U1MODEbits.STSEL = stopbits - 1;  // 1 stop bit

    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    // 4 – Enable UART1 (see register U1MODE)    
    U1MODEbits.ON = 1;
}

void putc(char byte2send){
    while(U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

int main(){
    ConfigUart(115200, 'N', 1);

    while(1){
        putc('p');
        delay(1000);
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
