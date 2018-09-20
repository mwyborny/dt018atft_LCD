
#include <xc.h>

/**
    void INTERRUPT_Initialize (void)
*/
void INTERRUPT_Initialize (void)
{
    //    IOCI: IOC -  Interrupt On Change Interrupt
    //    Priority: 1
        IPC4bits.IOCIP = 1;
        
}
