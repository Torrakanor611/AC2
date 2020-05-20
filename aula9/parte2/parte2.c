#include <detpic32.h>

#define DisableUart1RxInterrupt()   IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()   IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt()   IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt()   IEC0bits.U1TXIE = 1

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE – 1)

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

void comDrv_config(unsigned int baud, char parity, unsigned int stopbits){
    ConfigUart(baud, parity, stopbits);

    IEC0bits.U1RXIE = 1;
    IPC6bits.U1IP = 2;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
}

typedef struct{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
    unsigned int overrun;
} cicularBuffer;

volatile cicularBuffer rxb;
volatile cicularBuffer txb;

void comDrv_flushRx(void){

    rxb.count = 0;
    rxb.head = 0;
    rxb.tail = 0;
    int i;
    for(i = 0; i < BUF_SIZE; i++){
        rxb.data[i] = 0;
    }
    rxb.overrun = 0;
}

void comDrv_flushTx(void){

    txb.count = 0;
    txb.head = 0;
    txb.tail = 0;
    int i;
    for(i = 0; i < BUF_SIZE; i++){
        txb.data[i] = 0;
    }
    rxb.overrun = 0;
}

void comDrv_putc(char c){
    while(txb.count == BUF_SIZE);
    txb.data[txb.tail] = c;

    txb.tail = txb.tail++;
    if(txb.tail == BUF_SIZE){
        txb.tail = 0;
    }

    DisableUart1TxInterrupt();
    txb.count++;
    EnableUart1TxInterrupt();
}

void comDrv_puts(char* str){
    while((*str) != '\0'){
        comDrv_putc(*str);
        str++;
    }
}

int comDrv_getc(char *pchar){
    if(rxb.count == 0) return 0;
    DisableUart1RxInterrupt();
    *pchar = rxb.data[rxb.head];
    rxb.count--;
    rxb.head = rxb.head++;
    if(rxb.head == BUF_SIZE){
        rxb.head = 0;
    }

    EnableUart1RxInterrupt();
    return 1;
}

void _int_(24) isr_UART1(){
  if(IFS0bits.U1TXIF)
  {
    while (txb.count > 0 && U1STAbits.UTXBF == 0)
    {
      U1TXREG = txb.data[txb.head];
      txb.head = (txb.head + 1) & 7;
      txb.count--;
    }
    if (txb.count == 0)
      DisableUart1TxInterrupt();
    IFS0bits.U1TXIF = 0;
  }

  if (IFS0bits.U1RXIF)
  {
    while (rxb.count < BUF_SIZE && U1STAbits.URXDA != 0)
    {
      rxb.data[rxb.tail] = U1RXREG;
      rxb.tail = (rxb.tail + 1) & 7;
      rxb.count++;
      IFS0bits.U1RXIF = 0;
    }
  }

  if (IFS0bits.U1EIF)
  {
    char fg;
    if (U1STAbits.OERR)
    {
      U1STAbits.OERR = 0;
      rxb.overrun = 1;
    }

    if (U1STAbits.FERR || U1STAbits.PERR)
      fg = U1RXREG;

    IFS0bits.U1EIF = 0;
  }
}

int main(){
  char buf;
  comDrv_config(115200, 'N', 1);
  comDrv_flushTx();
  comDrv_flushRx();
  EnableInterrupts();
  comDrv_puts("PIC32 UART Device-driver\n");
  while(1)
  {
    if (comDrv_getc(&buf))
    {
      if (buf == 'S')
        comDrv_puts("Uma string com 30 caracteress.");
      comDrv_putc(buf);
    }
  }
  return 0;
}

