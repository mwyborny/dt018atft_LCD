
#include <xc.h>
#include "pin_manager.h"

/**
    void PIN_MANAGER_Initialize(void)
*/
void PIN_MANAGER_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0001;
    LATB = 0x0000;
    LATC = 0x0000;
    LATD = 0x1021;
    LATE = 0x0000;
    LATF = 0x0020;
    LATG = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0xC67E;
    TRISB = 0xFFDF;
    TRISC = 0xF01E;
    TRISD = 0xC64A;
    TRISE = 0x0201;
    TRISF = 0x219C;
    TRISG = 0xF28C;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    IOCPDA = 0x0000;
    IOCPDB = 0x0000;
    IOCPDC = 0x0000;
    IOCPDD = 0x0000;
    IOCPDE = 0x0000;
    IOCPDF = 0x0000;
    IOCPDG = 0x0080;
    IOCPUA = 0x0000;
    IOCPUB = 0x0000;
    IOCPUC = 0x0000;
    IOCPUD = 0x0000;
    IOCPUE = 0x0000;
    IOCPUF = 0x0000;
    IOCPUG = 0x0080;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;
    ODCE = 0x0000;
    ODCF = 0x0000;
    ODCG = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSA = 0x06C0;
    ANSB = 0xFF3F;
    ANSC = 0x6010;
    ANSD = 0x0080;
    ANSE = 0x0210;
    ANSG = 0x0340;


    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPOR5bits.RP11R = 0x000F;   //RD0->OC3:OC3;
    RPOR8bits.RP17R = 0x0003;   //RF5->UART1:U1TX;
    RPINR22bits.SDI2R = 0x001A;   //RG7->SPI2:SDI2;
    RPINR18bits.U1RXR = 0x000A;   //RF4->UART1:U1RX;
    RPOR9bits.RP19R = 0x000A;   //RG8->SPI2:SDO2;
    RPOR9bits.RP18R = 0x0008;   //RB5->SPI1:SCK1OUT;
    RPOR11bits.RP23R = 0x0009;   //RD2->SPI1:SS1OUT;
    RPOR10bits.RP21R = 0x000B;   //RG6->SPI2:SCK2OUT;
    RPOR1bits.RP2R = 0x0007;   //RD8->SPI1:SDO1;

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    /****************************************************************************
     * Interrupt On Change for group IOCND - negative
     ***************************************************************************/
	IOCNDbits.IOCND6 = 1; // Pin : RD6
	IOCNDbits.IOCND7 = 1; // Pin : RD7

    /****************************************************************************
     * Interrupt On Change for group IOCPD - positive
     ***************************************************************************/
	IOCPDbits.IOCPD6 = 0; // Pin : RD6
	IOCPDbits.IOCPD7 = 0; // Pin : RD7

    /****************************************************************************
     * Interrupt On Change for group PADCON - config
     ***************************************************************************/
	PADCONbits.IOCON = 1; 

    IEC1bits.IOCIE = 1; // Enable IOCI interrupt 
}

/* Interrupt service routine for the IOCI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _IOCInterrupt ( void )
{
    if(IFS1bits.IOCIF == 1)
    {
        // Clear the flag
        IFS1bits.IOCIF = 0;
            }
        }
