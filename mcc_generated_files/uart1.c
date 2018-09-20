

/**
  Section: Included Files
*/
#include "uart1.h"

extern bool IFlag;
extern uint16_t timeout;

/**
  Section: UART1 APIs
*/

void UART1_Initialize(void)
{
/**    
     Set the UART1 module to the options selected in the user interface.
     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
*/
    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U1MODE = (0x8008 & ~(1<<15));  // disabling UARTEN bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; URXEN disabled; OERR NO_ERROR_cleared; URXISEL RX_3_4_BUF_FULL; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U1STA = 0x80;
    // BaudRate = 14400; Frequency = 16000000 Hz; U1BRG 277; 
    U1BRG = 0x115;
    // ADMADDR 0; ADMMASK 0; 
    U1ADMD = 0x00;
    
    U1MODEbits.UARTEN = 1;  // enabling UARTEN bit
    U1STAbits.UTXEN = 1; 
}

uint8_t UART1_Read(void)
{
    while(!(U1STAbits.URXDA == 1))
    {
        if(IFlag == 1)
        {
            TMR1_Counter16BitSet(0);
          goto exit;
        }
        
    }
    exit:
    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }
    
    return U1RXREG;
}

void UART1_Write(uint8_t txData)
{
    while(U1STAbits.UTXBF == 1)
    {
        
    }

    U1TXREG = txData;    // Write the data byte to the USART.
}

uint16_t UART1_StatusGet (void)
{
    return U1STA;
}

int __attribute__((__section__(".libc.write"))) write(int handle, void *buffer, unsigned int len) {
    int i;
    while(U1STAbits.TRMT == 0);  
    for (i = len; i; --i)
    {
        while(U1STAbits.TRMT == 0);
        U1TXREG = *(char*)buffer++;        
    }
    return(len);
}

