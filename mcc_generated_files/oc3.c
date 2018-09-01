
#include <xc.h>
#include "oc3.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gOC3Mode;

/**
  Section: Driver Interface
*/


void OC3_Initialize (void)
{
    // ENFLT0 disabled; ENFLT1 disabled; OCFLT2 disabled; ENFLT2 disabled; OCSIDL disabled; OCM Off; OCFLT1 disabled; OCFLT0 disabled; OCTSEL TMR2; TRIGMODE Only Software; 
    OC3CON1 = 0x0000;
    // SYNCSEL OCTRIG2; TRIGSTAT disabled; DCB Start of instruction cycle; OCINV disabled; OCTRIG Trigger; OC32 disabled; FLTOUT disabled; OCTRIS disabled; FLTMD Cycle; FLTTRIEN disabled; 
    OC3CON2 = 0x009D;
    // OC3RS 20; 
    OC3RS = 0x0014;
    // OC3R 10; 
    OC3R = 0x000A;
	
    gOC3Mode = OC3CON1bits.OCM;
}



void OC3_Tasks( void )
{
    if(IFS1bits.OC3IF)
    {
        IFS1bits.OC3IF = 0;
    }
}



void OC3_Start( void )
{
    OC3CON1bits.OCM = gOC3Mode;
}


void OC3_Stop( void )
{
    OC3CON1bits.OCM = 0;
}


void OC3_SingleCompareValueSet( uint16_t value )
{
    OC3R = value;
}


void OC3_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{
    OC3R = priVal;
	
    OC3RS = secVal;
}


void OC3_CentreAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC3R = priVal;
	
    OC3RS = secVal;
}

void OC3_EdgeAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC3R = priVal;
	
    OC3RS = secVal;
}

void OC3_SecondaryValueSet( uint16_t secVal )
{
   
    OC3RS = secVal;
}


void OC3_PrimaryValueSet( uint16_t priVal )
{
   
    OC3R = priVal;
}

bool OC3_IsCompareCycleComplete( void )
{
    return(IFS1bits.OC3IF);
}


bool OC3_FaultStatusGet( OC3_FAULTS faultNum )
{
    bool status;
    /* Return the status of the fault condition */
   
    switch(faultNum)
    { 
        case OC3_FAULT0:status = OC3CON1bits.OCFLT0;
            break;
        case OC3_FAULT1:status = OC3CON1bits.OCFLT1;
            break;
        case OC3_FAULT2:status = OC3CON1bits.OCFLT2;
            break;
        default :
            break;

    }
    return(status);
}


void OC3_FaultStatusClear( OC3_FAULTS faultNum )
{
    
    switch(faultNum)
    { 
        case OC3_FAULT0:OC3CON1bits.OCFLT0 = 0;
                break;
        case OC3_FAULT1:OC3CON1bits.OCFLT1 = 0;
                break;
        case OC3_FAULT2:OC3CON1bits.OCFLT2 = 0;
                break;
        default :
                break;
    }    
}


void OC3_ManualTriggerSet( void )
{
    OC3CON2bits.TRIGSTAT= true; 
}

bool OC3_TriggerStatusGet( void )
{
    return( OC3CON2bits.TRIGSTAT );
}


void OC3_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    OC3CON2bits.TRIGSTAT = 0;
}

/**
 End of File
*/
