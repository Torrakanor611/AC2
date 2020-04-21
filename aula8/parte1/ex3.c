#include<detpic32.h>

void delay(int ms)
{
	int K = 20000;
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < K);
	}
}

void putc(char byte2send){
    while(U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

void putString(char *str){       
    // use putc() function to send each charater ('\0' should not//  be sent)    
    int i = 0;
    while(str[i] != '\0'){
        putc(str[i]);
        i++;
    }
}

int main(){
    // Configure UART1 (115200, N, 8, 1)
    // Configure UART1:       
    // 1 - Configure BaudRate Generator
    U1MODEbits.BRGH = (PBCLK + 8 * 115200) / (16 * 115200) - 1; 
    // 2 – Configure number of data bits, parity and number of stop bits       
    //     (see U1MODE register)
    // data bits / parity / stop bits
    U1MODEbits.BRGH = 0;   // Divisão por 16
    U1MODEbits.PDSEL = 0;  // 8 bits sem paridade
    U1MODEbits.STSEL = 0;  // 1 stop bit
    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    // 4 – Enable UART1 (see register U1MODE)    
    U1MODEbits.ON = 1;

    while(1){          
        putString("String de teste\n");          
        // wait 1 s
        delay(1000);       
    } 

    return 0;
}
// Configure UART1:       
    // 1 - Configure BaudRate Generator
    U1MODEbits.BRGH = (PBCLK + 8 * 115200) / (16 * 115200) - 1; 
    // 2 – Configure number of data bits, parity and number of stop bits       
    //     (see U1MODE register)
    // data bits / parity / stop bits
    U1MODEbits.BRGH = 0;   // Divisão por 16
    U1MODEbits.PDSEL = 0;  // 8 bits sem paridade
    U1MODEbits.STSEL = 0;  // 1 stop bit
    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    // 4 – Enable UART1 (see register U1MODE)    
    U1MODEbits.ON = 1;