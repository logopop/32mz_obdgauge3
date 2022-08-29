//*********************************************************************************************************************
// OBDgauge 3
//  
//
//*********************************************************************************************************************
//  
//     
//
//
//*********************************************************************************************************************
//

//*********************************************************************************************************************
// Includes
//
 
#include "app.h"
 

//*********************************************************************************************************************
// Globals
// 
//*********************************************************************************************************************
//

#include       <xc.h>
#include       <p32xxxx.h>
#include	      <ctype.h>


uint8_t                          cCanMsg[10]                = {0};                  // CAN receive buffer  
unsigned long                    ulRGB[LED_SERIAL_NUM]      = {0};                  // RGB LED array values

NVM_CONFIG                       sConfig;                      // Stored in non-volatile memory
APP_DATA                         appData;


//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
 
int main ( void )
{
   TRISBbits.TRISB10 = 1;
    
   // Initialize all MPLAB Harmony modules, including application(s).
   SYS_Initialize ( NULL );
   
   // Initialize everything before enabling turn-switch change interrupt
   PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_CHANGE_NOTICE_B);
   
   //*******
    
   while ( true )
   {
      // Maintain state machines of all polled MPLAB Harmony modules.
      SYS_Tasks ( );    
   }

   // Execution should not come here during normal operation
   return ( EXIT_FAILURE );
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//





