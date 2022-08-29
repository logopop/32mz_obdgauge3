//*********************************************************************************************************************
//  
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

#include "app.h"
#include "gps.h"
 

//*********************************************************************************************************************
// Globals
// 
//*********************************************************************************************************************
//
   
extern APP_DATA                        appData;
extern unsigned long                   ulRGB[LED_SERIAL_NUM];    // RGB LED array values


//*********************************************************************************************************************
// Convert position to Maidenhead
//  
//*********************************************************************************************************************
// 

/*
First your longitude:

1. Convert the minutes portion of the longitude from minutes to
   decimal by dividing by 60.

2. For North America and locations of West longitude, subtract
   your longitude from 180 degrees.  For location of East
   longitude, add 180 degrees.

3. Next divide this value by 20.  The whole number result will be
   used to determine the first character fo your grid, as follows:
   0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, 7=H, 8=I, 9=J, 10=K,
  11=L, 12=M, 13=N, 14=O, 15=P, 16=Q, 17=R, 18=S.

4. For the third digit, multiply this last number by 10.
   The digit immediately before the decimal point is the third
   digit of your grid.

Now use your latitude.

1. If your latitude is north, add 90.  If your latitude is South,
   subtract your latitude from 90.

2. Divide this number by 10. The whole number result will be
   used to determine the second character of your grid as follows:
   0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, 7=H, 8=I, 9=J, 10=K,
  11=L, 12=M, 13=N, 14=0, 15=P, 16=Q, 17=R, 18=S.

3. Now, multiply this number by 10.  The digit immediately before
   before the decimal point is the fourth digit of your grid.

*/

 
//*********************************************************************************************************************
// Interpret GPS sentences, populate data fields.
//  
//*********************************************************************************************************************
// P: GPS, SBAS   L: GLONASS   A: Galileo   B: Beidou   N: Combinations
//
// GPGSV
// GLGSV
// GNGGA
// GNGSA
// GNRMC
// GNGLL
//
//

void vInterpretGps(uint8_t * ucBuff)
{
   unsigned char ucTemp[20] = {0}; 
   
   memcpy(ucTemp, ucBuff + 3, 3);
   ucTemp[3] = 0; 

   if (!strcmp(ucTemp, "GSV")) 
   {
     // do something
   } 
   else if (!strcmp(ucTemp, "GGA"))
   {
     // do something else
   }
   else if (!strcmp(ucTemp, "GSA"))
   {
     // do something else
   }
   else if (!strcmp(ucTemp, "RMC"))
   {
     // do something else
   }
   else if (!strcmp(ucTemp, "GLL"))
   {
      
   }
   else if (!strcmp(ucTemp, "VTG"))
   {
      
   }
   else // Unknown sentence
   {
        
      vDebug(ucTemp, 0);
      
      
   }


   
}

//*********************************************************************************************************************
// Receive a character from the GPS. Call to interpret if complete sentence. 
// 
//*********************************************************************************************************************
//

void APP_GpsReceiveEventHandler(SYS_MODULE_INDEX index)    // GPS USART 6
{
   unsigned char ucTemp[20] = {0}; 
   unsigned char ucChar;
     
   ucChar = DRV_USART_ReadByte(appData.drvGpsHandle);    
   if (!appData.uiGpsSync)       // Waiting for sync 0x0a
   {
      if (ucChar == 0x0a)
      {
         memset(appData.ucGpsRxbuff, 0x00, 256);
         appData.uiGpsSync = 1;
         appData.uiGpsRxptr = 0;
         appData.uiUbxSync = 0;
      }   
      else   
      {
         if (ucChar == 0xb5)
            appData.uiUbxSync = 1;
         else if ((ucChar == 0x62) && (appData.uiUbxSync == 1))
         {   
            appData.uiUbxSync = 2;
            appData.uiGpsRxptr = 0;
         
            
            vDebug(" UBX Sync!  \0", D_PURPLE); 
            vLongDelay(1000);
         }
      }   
   }
   else
   { 
      if (appData.uiGpsRxptr < 256)
      {   
         appData.ucGpsRxbuff[appData.uiGpsRxptr] = ucChar;      
        
         if (appData.ucGpsRxbuff[appData.uiGpsRxptr] == 0x0d)  // End of sentence
         {
            appData.ucGpsRxbuff[appData.uiGpsRxptr] = 0x00;
            vInterpretGps(appData.ucGpsRxbuff);
            appData.uiGpsSync = 0;
         }
         else
            appData.uiGpsRxptr++;
      }
      else
         appData.uiGpsSync = 0;
   }   
}

//*********************************************************************************************************************
//  
//*********************************************************************************************************************
//
 
void vGps_Initialize (void)
{     
   appData.drvGpsHandle = DRV_USART_Open(DRV_USART_INDEX_1, DRV_IO_INTENT_READWRITE);
   if(DRV_HANDLE_INVALID == appData.drvGpsHandle) 
   {
      vDebug("Inval GPS h\0", 0);
      while (1);
   }
    
   
   // Register UART callback
   DRV_USART_ByteReceiveCallbackSet(DRV_USART_INDEX_1, APP_GpsReceiveEventHandler);
   
   appData.uiGpsRxptr = 0;  
   appData.uiGpsSync = 0;     // Wait for sync
}

//*********************************************************************************************************************
//  Send command buffer 
//*********************************************************************************************************************
//

void vGps_sendcommand(char * ucBuff)
{
   unsigned int                  uiLength                   = 0;
   unsigned int                  i                          = 0;
      
   sprintf(appData.ucGpsTxbuff, "AT%s\r\n", ucBuff);
   uiLength = strlen(ucBuff);
   uiLength += 4;
   
  /*
   if (appData.drvUsartHandle == DRV_HANDLE_INVALID)
   {   
      vDebug("---Oh no!", 2);
      vLongDelay(200);
      vLongDelay(200);
   }
   */
   
   //uiLength--;
   while (uiLength > 0)
   {   
      if(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART_TransferStatus(appData.drvGpsHandle)))
      {           
         //DRV_USART_WriteByte(appData.drvUsartHandle, appData.ucWtxbuff[i]);         
         PLIB_USART_TransmitterByteSend(USART_ID_6, appData.ucGpsTxbuff[i]);
         
         i++;
         uiLength--;
      }
   }         
}

//*********************************************************************************************************************
//  Send uBlox message
//*********************************************************************************************************************
//

void vGps_ubxcommand(unsigned char ucClass, unsigned char ucId, unsigned short usLength, unsigned char * ucBuff)
{
   unsigned int                  i                          = 0;
   unsigned int                  uiLength                   = 0;
   unsigned char                 ucSendbuff[256]            = {0};
   unsigned char                 ucA                        = 0;
   unsigned char                 ucB                        = 0;
          
   // Preamble
   ucSendbuff[0] = 0xB5;
   ucSendbuff[1] = 0x62;
   uiLength = 2;
   // Class and Id
   ucSendbuff[2] = ucClass;
   ucSendbuff[3] = ucId;
   uiLength += 2;
   // Payload length
   memcpy(ucSendbuff + 4, (unsigned char *)&usLength, 2);
   uiLength += 2;
   // Payload
   memcpy(ucSendbuff + 6, ucBuff, usLength);
   uiLength += usLength;
   // Calculate and insert checksum   
   for(i = 0; i < (usLength + 4); i++)
   {
      ucA = ucA + ucSendbuff[i + 2];
      ucB = ucB + ucA;
   }
   ucSendbuff[uiLength] = ucA;
   ucSendbuff[uiLength+1] = ucB;
   uiLength += 2;
   
   i = 0;
   while (uiLength > 0)
   {   
      if(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART_TransferStatus(appData.drvGpsHandle)))
      {           
         //DRV_USART_WriteByte(appData.drvUsartHandle, appData.ucWtxbuff[i]);         
         PLIB_USART_TransmitterByteSend(USART_ID_6, ucSendbuff[i]);         
         i++;
         uiLength--;
      }
   }         
}

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

int Gps_State()
{    
   unsigned char                 ucBuff[]                   = {"Abcd!\0"};
   static unsigned char          i                          = 0; 
   unsigned char                 ucDebug[512]               = {0};
   
   //sprintf(ucDebug, "Gps: %lu %c", ulWseq, '\0');
   //vDebug(ucDebug, 1);
   //vLongDelay(1000);
  
    
   // Gps state machine goes here   
  /*
   ulWseq++;
   
   if (ulWseq > 5)
   {   
      //vGps_sendcommand("+UART_DEF=115200,8,1,0,0");
          
      
      
      vGps_sendcommand("+GMR");
      vGps_sendcommand("+CIUPDATE");
                    
      ulWseq = 0;
       
   }
   
   // Display incoming test
   */
   /*
   if (appData.uiUrxptr > 10)
   {
      strncpy(ucDebug, appData.ucWrxbuff, 500);
       
      //by1351_Dump(ucDebug);
      appData.uiUrxptr = 0;
      
   }   
   
   
  */
   return 0;
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//










