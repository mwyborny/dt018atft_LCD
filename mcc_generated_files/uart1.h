

#ifndef UART1_H
#define UART1_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: Data Types
*/

/** UART1 Driver Hardware Flags

  @Summary
    Specifies the status of the hardware receive or transmit

  @Description
    This type specifies the status of the hardware receive or transmit.
    More than one of these values may be OR'd together to create a complete
    status value.  To test a value of this type, the bit of interest must be
    AND'ed with value and checked to see if the result is non-zero.
*/
typedef enum
{
    /* Indicates that Receive buffer has data, at least one more character can be read */
    UART1_RX_DATA_AVAILABLE
        /*DOM-IGNORE-BEGIN*/  = (1 << 0) /*DOM-IGNORE-END*/,
    
    /* Indicates that Receive buffer has overflowed */
    UART1_RX_OVERRUN_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 1) /*DOM-IGNORE-END*/,

    /* Indicates that Framing error has been detected for the current character */
    UART1_FRAMING_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 2) /*DOM-IGNORE-END*/,

    /* Indicates that Parity error has been detected for the current character */
    UART1_PARITY_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 3) /*DOM-IGNORE-END*/,

    /* Indicates that Receiver is Idle */
    UART1_RECEIVER_IDLE
        /*DOM-IGNORE-BEGIN*/  = (1 << 4) /*DOM-IGNORE-END*/,

    /* Indicates that the last transmission has completed */
    UART1_TX_COMPLETE
        /*DOM-IGNORE-BEGIN*/  = (1 << 8) /*DOM-IGNORE-END*/,

    /* Indicates that Transmit buffer is full */
    UART1_TX_FULL
        /*DOM-IGNORE-BEGIN*/  = (1 << 9) /*DOM-IGNORE-END*/

}UART1_STATUS;

/**
  Section: Macro Declarations
*/

#define UART1_DataReady  (U1STAbits.URXDA == 1)

/**
  Section: UART1 APIs
*/

/**
  @Summary
    Initialization routine that takes inputs from the UART1 GUI.

  @Description
    This routine initializes the UART1 driver.
    This routine must be called before any other UART1 routine is called.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    
*/

void UART1_Initialize(void);

/**
  @Summary
    Read a byte of data from the UART1.

  @Description
    This routine reads a byte of data from the UART1.

  @Preconditions
    UART1_Initialize() function should have been called
    before calling this function. This is a blocking function.
    So the recieve status should be checked to see
    if the receiver is not empty before calling this function.

  @Param
    None

  @Returns
    A data byte received by the driver.
*/

uint8_t UART1_Read(void);

/**
  @Summary
    Writes a byte of data to the UART1.

  @Description
    This routine writes a byte of data to the UART1.

  @Preconditions
    UART1_Initialize() function should have been called
    before calling this function. The transfer status should be checked to see
    if transmitter is not busy before calling this function.

  @Param
    txData  - Data byte to write to the UART1

  @Returns
    None
*/

void UART1_Write(uint8_t txData);

/**
  @Summary
    Returns the transmitter and receiver status

  @Description
    This returns the transmitter and receiver status. The returned status 
    contains a 16 bit value. 
    The caller should perform an "AND" with the bit of interest and verify if the
    result is non-zero (as shown in the example) to verify the desired status
    bit.

  @Preconditions
    UART1_Initializer function should have been called 
    before calling this function

  @Param
    None.

  @Returns
    16 bit value describing the current status of the transfer.

  @Example
    <code>
        while(!(UART1_StatusGet() & UART1_TX_COMPLETE ))
        {
           // Wait for the tranmission to complete
        }
    </code>
*/

uint16_t UART1_StatusGet (void );

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  // UART1_H