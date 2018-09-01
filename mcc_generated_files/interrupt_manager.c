
#include <xc.h>

/**
    void INTERRUPT_Initialize (void)
*/
void INTERRUPT_Initialize (void)
{
    //    IOCI: IOC -  Interrupt On Change Interrupt
    //    Priority: 1
        IPC4bits.IOCIP = 1;
    //    UERI: U1E - UART1 Error
    //    Priority: 1
        IPC16bits.U1ERIP = 1;
    //    UTXI: U1TX - UART1 Transmitter
    //    Priority: 1
       // IPC3bits.U1TXIP = 1;
    //    URXI: U1RX - UART1 Receiver
    //    Priority: 1
        IPC2bits.U1RXIP = 1;
    //    TI: T1 - Timer1
    //    Priority: 1
        IPC0bits.T1IP = 1;
        
}
