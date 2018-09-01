

/**
  Section: Included Files
*/
#include "uart1.h"

extern bool IFlag;

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
    
    IEC0bits.U1RXIE = 1;
    
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    
    U1MODEbits.UARTEN = 1;  // enabling UARTEN bit
    U1STAbits.UTXEN = 1; 

    uart1_obj.txHead = uart1_txByteQ;
    uart1_obj.txTail = uart1_txByteQ;
    uart1_obj.rxHead = uart1_rxByteQ;
    uart1_obj.rxTail = uart1_rxByteQ;
    uart1_obj.rxStatus.s.empty = true;
    uart1_obj.txStatus.s.empty = true;
    uart1_obj.txStatus.s.full = false;
    uart1_obj.rxStatus.s.full = false;
}

uint8_t UART1_Read(void)
{
    while(!(U1STAbits.URXDA == 1))
    {
        if(IFlag == 1)
        {
            IFlag = 0;
            printf("%c", 0x06 );
            break;
        }
        
    }

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


