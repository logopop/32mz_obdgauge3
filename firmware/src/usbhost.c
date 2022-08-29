//*********************************************************************************************************************
// USB host
//  
//
//*********************************************************************************************************************
//
//  
//      
//
//
//*********************************************************************************************************************
//
  
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>

#include "ssd1351_drv.h"
#include "app.h"
#include "og_hardware.h"
#include "common_library_32x.h"


//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//
 
extern APP_DATA                  appData;
extern unsigned long             ulRGB[LED_SERIAL_NUM];  

const uint8_t writeData[12]  __attribute__((aligned(16))) = "Hello World ";

//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
                    
    }
    
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

//*********************************************************************************************************************
// Interrupt handler
// 
//*********************************************************************************************************************
//
  
void APP_SYSFSEventHandler(SYS_FS_EVENT event, void * eventData, uintptr_t context)
{
   switch(event)
   {
      case SYS_FS_EVENT_MOUNT:
         appData.deviceIsConnected = true;
      break;
          
      case SYS_FS_EVENT_UNMOUNT:
         appData.deviceIsConnected = false;
          
      break;
          
      default:
   
      break;
   }
} 
 
//*********************************************************************************************************************
//
// 
//*********************************************************************************************************************
//
  
int USB_State()
{ 
   unsigned char                 ucBuff[128]                = {0};
   unsigned long                 ucX                        = 0;
   unsigned long                 ucY                        = 0;
   unsigned long                 ulRet                      = 0; 
   
   // Get started
   if (!appData.uiUsbState)
      appData.uiUsbState = 1;
   
   switch(appData.uiUsbState)
   {
      case 1:                
         /* Set the event handler and enable the bus */
         //  k SYS_FS_EventHandlerSet(APP_SYSFSEventHandler, (uintptr_t)NULL);
         USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
         USB_HOST_BusEnable(0);
         //appData.uiUsbState = 2; 
         appData.uiUsbState = 10; // Temporary disable

      break;
   
      case 2:       
         if(USB_HOST_BusIsEnabled(0))
         {
            appData.uiUsbState = 3; //APP_STATE_WAIT_FOR_DEVICE_ATTACH;
         }
         
      break;   


      case 3:// APP_STATE_WAIT_FOR_DEVICE_ATTACH:

         /* Wait for device attach. The state machine will move
          * to the next state when the attach event
          * is received.  */
         if(appData.deviceIsConnected)
         {
            //BSP_LEDOn( APP_USB_LED_3 );
            //BSP_LEDOff( APP_USB_LED_2 );
            appData.uiUsbState = 4;//APP_STATE_DEVICE_CONNECTED;
         }

      break;

      case 4: //APP_STATE_DEVICE_CONNECTED:

          /* Device was connected. We can try mounting the disk */
          appData.uiUsbState = 5;  //APP_STATE_OPEN_FILE;
      break;

      case 5:

          /* Try opening the file for append */
          appData.fileHandle = SYS_FS_FileOpen("/mnt/myUDrive1/file.txt", (SYS_FS_FILE_OPEN_APPEND_PLUS));
          if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
          {
              /* Could not open the file. Error out*/
              appData.uiUsbState = 20;

          }
          else
          {
              /* File opened successfully. Write to file */
              appData.uiUsbState = 6;   //APP_STATE_WRITE_TO_FILE;

          }
      break;

      case 6:    //APP_STATE_WRITE_TO_FILE:

          /* Try writing to the file */
          if (SYS_FS_FileWrite( appData.fileHandle, (const void *) writeData, 12 ) == -1)
          {
              /* Write was not successful. Close the file
               * and error out.*/
              SYS_FS_FileClose(appData.fileHandle);
              appData.uiUsbState = 20;

          }
          else
          {
              /* We are done writing. Close the file */
              appData.uiUsbState = 7;
          }

      break;

      case 7:   //APP_STATE_CLOSE_FILE:

          /* Close the file */
          SYS_FS_FileClose(appData.fileHandle);

          /* The test was successful. Lets idle. */
          appData.uiUsbState = 8;   //APP_STATE_IDLE;
      break;

      case 8:   //APP_STATE_IDLE:

          /* The application comes here when the demo has completed
           * successfully. Provide LED indication. Wait for device detach
           * and if detached, wait for attach. */

          //BSP_LEDOff( APP_USB_LED_3);
          //BSP_LEDOn( APP_USB_LED_2 );
          if(appData.deviceIsConnected == false)
          {
              appData.uiUsbState = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
              //BSP_LEDOff(APP_USB_LED_2);
          }
      break;
      
      case 10:
         // Normal end here
         
      break;   
      
      case 20:

          /* The application comes here when the demo
           * has failed. Provide LED indication .*/

          //BSP_LEDOn( APP_USB_LED_1 );
          if(SYS_FS_Unmount("/mnt/myUDrive1") != 0)
          {
              /* The disk could not be un mounted. Try
               * un mounting again untill success. */

              appData.uiUsbState = APP_STATE_ERROR;
          }
          else
          {
              /* UnMount was successful. End */
              appData.uiUsbState = 10;

          }
      break;
 

       
   }         
    
   return 0;
}
 
 
 //*********************************************************************************************************************
//
// 
//*********************************************************************************************************************
//
  
 
 
 
  
 
 
 