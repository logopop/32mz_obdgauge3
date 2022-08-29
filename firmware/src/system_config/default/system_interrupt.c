/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system/common/sys_common.h"
#include "app.h"
#include "system_definitions.h"

extern APP_DATA                  appData;
//char cTemp[255];

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************
 
void __ISR(_UART4_TX_VECTOR, ipl2AUTO) _IntHandlerDrvUsartTransmitInstance0(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart0);
}
void __ISR(_UART4_RX_VECTOR, ipl2AUTO) _IntHandlerDrvUsartReceiveInstance0(void)
{
    DRV_USART_TasksReceive(sysObj.drvUsart0);
}
void __ISR(_UART4_FAULT_VECTOR, ipl2AUTO) _IntHandlerDrvUsartErrorInstance0(void)
{
    DRV_USART_TasksError(sysObj.drvUsart0);
}
 
  
 
void __ISR(_CHANGE_NOTICE_B_VECTOR, ipl1AUTO) _IntHandlerChangeNotification_PortB(void)
{

    APP_SwitchReceiveEventHandler();   // kbdoo keep
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CHANGE_NOTICE_B);
}

void __ISR(_RTCC_VECTOR, ipl1AUTO) _IntHandlerSysRtcc (void)
{
    SYS_RTCC_Tasks(sysObj.sysRtcc);
}
 
 
void __ISR(_TIMER_1_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_1);
}
 
void __ISR(_TIMER_5_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance1(void)
{
   vTmr5_event();          // kbdoo keep
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_5);
}
 

void __ISR(_SPI1_RX_VECTOR, ipl3AUTO) _IntHandlerSPIRxInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void __ISR(_SPI1_TX_VECTOR, ipl3AUTO) _IntHandlerSPITxInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void __ISR(_SPI1_FAULT_VECTOR, ipl3AUTO) _IntHandlerSPIFaultInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}

void __ISR(_OUTPUT_COMPARE_1_VECTOR, ipl7AUTO) _IntHandlerDrvOCInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_OUTPUT_COMPARE_1);
}

void __ISR(_CAN2_VECTOR, IPL3AUTO) _IntHandlerDrvCANInstance0(void)
{
    vCAN_MsgRecv();  // kbdoo keep
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CAN_2);
}  

// kbdoo keep
void __ISR(_TIMER_4_VECTOR, ipl5auto) timer4_handler(void)
{
   uint8_t                       cVal        = 0;
           
//   char                          ucTemp[30];
    
   IFS0bits.T4IF = 0;               // Clear interrupt flag for timer 

   // Check buffers
   if (appData.ulSptr > 4095)       // Switch buffer, flag for next read 
   {           
      if (appData.bSndReady == 1)
      {   
         appData.bSndReady = 2;
         appData.ucReadSndBuff = 1;
      }   
      else
      {   
         appData.bSndReady = 1;
         appData.ucReadSndBuff = 2;
      }   
      appData.ulSptr = 0;    
   }
  
   // Read first second buffer
   if (appData.ucSoundFirst == 1)
   {   
      appData.ucReadSndBuff = 2;
      appData.ucSoundFirst = 0;
   }   
   
   // Play
   if (!appData.cPwmDone)
   {          
      if (appData.bSndReady == 1)
         memcpy(&cVal, appData.pbSoundbuff1 + appData.ulSptr, 2);       // Output next byte to OC1
      else if (appData.bSndReady == 2)
         memcpy(&cVal, appData.pbSoundbuff2 + appData.ulSptr, 2);                       
      
      appData.ulSptr ++;         
           
      // 8_44      
      OC1RS = (cVal * 10) + 0x06;     
      
      // Check for end of file
      if ((appData.ulSptr >= appData.ulSndBytes) && (appData.ulSndBytes < 4096))
      {         
         //sprintf(ucTemp, " %d %c", appData.uiWifiState, '\0');
         //by1351_DrawText(1, 40, (char *)ucTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
         
         appData.cPwmDone = 1;
         DRV_TMR_Stop(appData.handleTmr); 
         free(appData.pbSoundbuff1);
         free(appData.pbSoundbuff2);
         appData.ucReadSndBuff = 0;
         appData.cSdBusy = 0;
         OC1RS = 0;
         OC1CON = 0;  
         SYS_FS_FileClose(appData.fileHandle); 
      }      
   }            
}

/*******************************************************************************
 End of File
*/
