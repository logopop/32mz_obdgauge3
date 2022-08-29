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
 
#include "wifi.h"
 

//*********************************************************************************************************************
// Globals
// 
//*********************************************************************************************************************
//
 
extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;

void APP_WifiReceiveEventHandler(SYS_MODULE_INDEX);  
 
//*********************************************************************************************************************
//  
//*********************************************************************************************************************
//
 
void vWifi_Initialize (void)
{          
   // Hardware   
   WIFI_RESET = 0;         //  Reset
   vShortDelay(300);
   WIFI_RESET = 1;         //  
       
   appData.drvWifiHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE);
   if(DRV_HANDLE_INVALID == appData.drvWifiHandle) 
   {
      strncpy(appData.sError, "WiFi IO err!", 16);
   }
    
   // Register UART callback for wifi
   DRV_USART_ByteReceiveCallbackSet(DRV_USART_INDEX_0, APP_WifiReceiveEventHandler);   
   appData.uiWifiRxptr = 0;  
}

//*********************************************************************************************************************
//  Send command buffer 
//*********************************************************************************************************************
//

void vWifi_sendcommand(char * ucBuff, uint8_t cStream)
{
   unsigned int                  uiLength                   = 0;
   unsigned int                  i                          = 0;
 
   
   if (cStream)
   {
      uiLength = strlen(ucBuff);
      strncpy((char *)appData.ucWifiTxbuff, ucBuff, uiLength);
   }
   else
   {   
      sprintf((char *)appData.ucWifiTxbuff, "AT%s%c%c", ucBuff, '\r', '\n');
//      sprintf((char *)appData.ucWifiTxbuff, "AT%s\r\n", ucBuff);
      uiLength = strlen(ucBuff);
      uiLength += 4;
   }
  
   if (appData.drvWifiHandle == DRV_HANDLE_INVALID)
   {   
      strncpy(appData.sError, "WiFi hndl err!", 16);
   } 
 
   //sprintf(ucTemp, "%02x %02x %02x %02x %02x \0",appData.ucWifiTxbuff[0], appData.ucWifiTxbuff[1], appData.ucWifiTxbuff[2], appData.ucWifiTxbuff[3], appData.ucWifiTxbuff[4]);
   //vDebug(ucTemp, 1);
   
   // Prepare RX buffer for answer   
   appData.uiWifiRxptr = 0;  
   memset(appData.ucWifiRxbuff, 0x00, 4096);
   
   while (uiLength > 0)
   {   
      if(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART_TransferStatus(appData.drvWifiHandle)))
      {                    
         DRV_USART_WriteByte(appData.drvWifiHandle, appData.ucWifiTxbuff[i]);         
         //PLIB_USART_TransmitterByteSend(USART_ID_4, appData.ucWifiTxbuff[i]);
         
         i++;
         uiLength--;
      }
   }         
}

//*********************************************************************************************************************
//  Parse AP data into struct
//*********************************************************************************************************************
//

void vParseApentry(char *cEntry)
{
   char                          cTemp[200]                 = {0};
   char                        * pP                         = NULL;
   char                        * pQ                         = NULL;
   int                           iTmp                       = 0;
   uint8_t                       cTmp                       = 0;
   
   // Encryption
   sscanf(cEntry + 1, "%d", &iTmp);
   appData.sWifiAp[appData.cApPtr].cEncryption = (uint8_t)iTmp;
   // AP name
   memset(cTemp, 0x00, 200);
   pP = strchr(cEntry + 4, '"');
   if (pP)
   {   
      // kbdoo test
      sprintf(appData.sTrapDebug, "A%lu%c", pP - cEntry - 4, '\0');

      strncpy(cTemp, cEntry + 4, pP - cEntry - 4);
      cTemp[pP - cEntry - 4] = 0;
   }
   else
      return;
   memset(appData.sWifiAp[appData.cApPtr].cSsid, 0x00, 32);
   strcpy(appData.sWifiAp[appData.cApPtr].cSsid, cTemp);
   // Signal 
   sscanf(pP + 2, "%d", &iTmp);
   cTmp = 100 - abs(iTmp);
   appData.sWifiAp[appData.cApPtr].cRssi = cTmp;
   // MAC
   pP += 7;
   pQ = strchr(pP, '"');
   if (pQ)
   {   
      strncpy(cTemp, pP, pQ - pP);
      cTemp[pQ - pP] = 0;
   }
   else
      return;
   strcpy(appData.sWifiAp[appData.cApPtr].cMac, cTemp);
   // Channel
   sscanf(pQ + 2, "%d", &iTmp);
   cTmp = iTmp;
   appData.sWifiAp[appData.cApPtr].cChannel = cTmp;
             
   if (appData.cApPtr < 31)
      appData.cApPtr++; 
}   

//*********************************************************************************************************************
//  
//*********************************************************************************************************************
//
 
void vParseStream(void)
{    
   char                          cTemp[300]                 = {0};
   char                        * pP                         = NULL;
   char                        * pQ                         = NULL;
   char                          cDone                      = 0;
   
   // Categorize stream
   strncpy(cTemp, appData.ucWifiMsg, 20);
   
   //********** List of access points
   if (strstr(cTemp, "+CWLAP:"))                // 
   {
      appData.cApPtr = 0;                                      // Array loc
      memset(appData.sWifiAp, 0x00, sizeof(WIFI_DATA) * 64);   // Clear array
      
      pP = strstr(appData.ucWifiMsg, "+CWLAP:");
      if (pP)
      {
         do
         {   
            pQ = strstr(pP + 1, "+CWLAP:");
            
            // kbdoo test
            sprintf(appData.sTrapDebug, "P%lu%c", pQ - pP - 1, '\0');

            memset(cTemp, 0x00, 300);
            
            if (!pQ)
            {                              
               pQ = strstr(pP + 1, "\r\n");   // Last entry
               cDone = 1;            
            }                        
            else if ((pQ - pP) < 200)
            {   
               strncpy(cTemp, pP + 7, pQ - pP - 1);
               cTemp[pQ - pP + 7] = 0;            
               vParseApentry(cTemp);               
               pP = pQ;
            }
            else
            {
               
               cDone = 1;
               
               /*
               sprintf(cTemp, "%s%c", pP + 4, '\0');
               by1351_DrawText(0, 20, (char *)cTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
               sprintf(cTemp, "%s%c", pP + 20, '\0');
               by1351_DrawText(0, 29, (char *)cTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
               sprintf(cTemp, "%s%c", pP + 36, '\0');
               by1351_DrawText(0, 38, (char *)cTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
               sprintf(cTemp, "%s%c", pP + 52, '\0');
               by1351_DrawText(0, 47, (char *)cTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);                     
               */
            }               
         } while ((pQ) && (!cDone));                   
      } 
      appData.cWchanged = 1;
   }   
   appData.uiStreamlength = 0;
}   
   
//*********************************************************************************************************************
//  
//*********************************************************************************************************************
//
 
void vInterpretWifi(void)
{
   if (strstr(appData.ucWifiMsg, "OK"))
      appData.iWifiStatus |= WIFI_OK;   
   if (strstr(appData.ucWifiMsg, "ERROR"))
      appData.iWifiStatus |= WIFI_ERROR;   
   if (strstr(appData.ucWifiMsg, "WIFI CONNECTED"))
      appData.iWifiStatus |= WIFI_CONNECTED;   
   if (strstr(appData.ucWifiMsg, "WIFI DISCONNECT"))
      appData.iWifiStatus |= WIFI_DISCONN;   
   if (strstr(appData.ucWifiMsg, "WIFI GOT IP"))
      appData.iWifiStatus |= WIFI_GOTIP;   
   if (strstr(appData.ucWifiMsg, "SEND OK"))
      appData.iWifiStatus |= WIFI_SENTOK;   
   if (strstr(appData.ucWifiMsg, "ALREADY CONNECTED"))
      appData.iWifiStatus |= WIFI_ALREADYC;   
   if (strstr(appData.ucWifiMsg, "CONNECT"))
      appData.iWifiStatus |= WIFI_NETCONN;   
   if (strstr(appData.ucWifiMsg, "STA_CONNECTED"))
      appData.iWifiStatus |= WIFI_STATION;   
   if (strchr(appData.ucWifiMsg, '>'))
      appData.iWifiStatus |= WIFI_RDYSEND;   
   if (strstr(appData.ucWifiMsg, "ready"))      // Device restarted
   {
      appData.iWifiStatus |= WIFI_RESTART;   
      appData.uiWifiRxptr = 0;
      appData.iWifiStatus = 0x00;
      appData.uiStreamlength = 0;
      appData.uiWifiState = 10;
   }  
   
   appData.usStatus &= ~(STAT_WIFI);   
                 
   /*
   strncpy(ucTemp, appData.ucWifiMsg, 15);
   ucTemp[15] = 0;
   vDebug(ucTemp, D_GREEN, 1);
     */
}   

//*********************************************************************************************************************
//  
//*********************************************************************************************************************
//

void APP_WifiReceiveEventHandler(SYS_MODULE_INDEX index)    //void vWifi_MsgRecv()
{
   unsigned char                 ucChar                     = 0x00;
//   char                          ucTemp[200];
   
   ucChar = DRV_USART_ReadByte(appData.drvWifiHandle);
   appData.cHdr[0] = appData.cHdr[1]; 
   appData.cHdr[1] = appData.cHdr[2]; 
   appData.cHdr[2] = appData.cHdr[3]; 
   appData.cHdr[3] = ucChar; 
   
   memset(appData.ucWifiMsg, 0x00, 256);
   
   // Check for stream input
   if (!memcmp(appData.cHdr, "+CWL", 4))
      appData.cStream = 1;
   
   if (appData.uiWifiRxptr < 4096)
   {   
      appData.ucWifiRxbuff[appData.uiWifiRxptr] = ucChar;
      appData.uiWifiRxptr++;
            
      if (ucChar == '>') 
      {             
         appData.iWifiStatus |= WIFI_RDYSEND;   
         
      }
      
      // Done?
      if (appData.cStream)
      {          
         if (!memcmp(appData.cHdr, "OK\r\n", 4))
         {                
            appData.cStream = 0;
            appData.uiStreamlength = appData.uiWifiRxptr;
            memcpy(appData.ucWifiMsg, appData.ucWifiRxbuff, appData.uiWifiRxptr); 
            appData.iWifiStatus |= WIFI_OK;             
         }  
      }
      else
      {   
         if ((appData.ucWifiRxbuff[appData.uiWifiRxptr - 1] == 0x0a) && (appData.ucWifiRxbuff[appData.uiWifiRxptr - 2] == 0x0d))
         {
            // Interpret
            if (appData.uiWifiRxptr < 256)
            {   
               memcpy(appData.ucWifiMsg, appData.ucWifiRxbuff, appData.uiWifiRxptr); 

               //sprintf(ucTemp, " \0",appData.ucWifiTxbuff[0], appData.ucWifiTxbuff[1], appData.ucWifiTxbuff[2], appData.ucWifiTxbuff[3], appData.ucWifiTxbuff[4]);

               appData.ucWifiMsg[appData.uiWifiRxptr - 2] = 0x00;

               /*
               if (appData.uiWifiState == 115) 
               {   
                  vDebug(appData.ucWifiMsg, D_BLUE, 1);
                  vLongDelay(2000);
               } 
                */
               appData.uiStreamlength = appData.uiWifiRxptr;            
               vInterpretWifi();               
            }
            appData.uiWifiRxptr = 0;
         }
      }   
   }
   else
   {
      appData.uiWifiRxptr = 0;       
   }   
}

//*********************************************************************************************************************
// Connect to open AP
//*********************************************************************************************************************
//
  
uint8_t cConnectAp(void)
{
   char                          cTemp[150]                 = {0};
   uint8_t                       cResult                    = 0;        // 1 if open AP found
//   char                        * pP                         = NULL;
   uint8_t                       i                          = 0;        //

   for (i = 0; i < 32; i++)
   {
      if ((*appData.sWifiAp[i].cSsid) && (!appData.sWifiAp[i].cEncryption))
      {         
         appData.sWifiAp[i].cEncryption = 10;
         strncpy(appData.cLastSsid, appData.sWifiAp[i].cSsid, 32);
         sprintf(cTemp, "+CWJAP_CUR=\"%s\",\"\"", appData.sWifiAp[i].cSsid); 
         vWifi_sendcommand(cTemp, 0);
         cResult = i + 1;
         i = 32;
         break;
      }   
   }
   return cResult;
}   

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//
  
void vPrepareRequest(void)
{
   char                          ucTemp[255]             = {0};   
    
   char                          ucRequest[]             = "GET /jp/jpsmart.php?";
   char                          ucRequestType[50]       = {0};
   char                          ucRequestTail[100]      = {0};

   sprintf(ucRequestTail, "Host: %s%c%cConnection: close%c%c%c%c", sConfig.bCloudServer, '\r', '\n', '\r', '\n', '\r', '\n');
   sprintf(ucRequestType, "HTTP/1.0%c%c", '\r', '\n');
   
   // Populate empty members
   sprintf((char *)appData.cGpsBearing, "%.0f", appData.fGpsBearing);
   sprintf((char *)appData.cGpsSpeed, "%.1f", appData.fGpsSpeed);
   
   // Build argument string
   memset(appData.ucWifiTempbuff, 0x00, 256);
   sprintf(appData.ucWifiTempbuff, "%sdate=%s&time=%s&lat=%.4f&lon=%.4f%c", 
         ucRequest, appData.cGpsDate, appData.cGpsTime, appData.dGpsLatDec, appData.dGpsLonDec, '\0');
   sprintf(ucTemp, "&alt=%.1f&spd=%s&dir=%s&rad=%.2f&tmp=%.1f%c", 
         appData.dGpsAlt, appData.cGpsSpeed, appData.cGpsBearing, appData.fRadLevel, appData.fTempResult, '\0');
   strcat(appData.ucWifiTempbuff, ucTemp);
   sprintf(ucTemp, "&usd=%d&sn=%d&sm=%d&rand=%x%x&ssid=%s %s%c",
         appData.sGpsSatsUsed, appData.sGpsSatsSeen, appData.sGpsMode, (unsigned int)appData.llRandom, (unsigned int)appData.llRandom2, appData.cLastSsid, ucRequestType, '\0');
   strcat(appData.ucWifiTempbuff, ucTemp);
   strcat(appData.ucWifiTempbuff, ucRequestTail);
   appData.iTxLen = strlen(appData.ucWifiTempbuff);    
     
}   

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//
  
void vWifi_State(void)
{
   char                          ucTemp[100]                = {0};   
   uint8_t                       cTmp                       = 0;
   
   //sprintf(ucTemp, "%d %c", appData.uiWifiState, '\0');
   //by1351_DrawText(100, 100, ucTemp, 0x00, 0x3f, 0x1f, 0, 0, 0, FONT_VGA, 1, 0);
   
   sprintf(appData.sTrapDebug, "A%c", '\0');
   
   // Timeout check
   if (appData.uiWifiState == appData.uiWifiPrevstate)
   {
      appData.uiWifiTimeout++;
      if (appData.uiWifiTimeout > 5000)
      {
         appData.uiWifiState = 0;
         appData.uiWifiTimeout = 0;
      }   
   }
   else
   {
      appData.uiWifiPrevstate = appData.uiWifiState;   
      appData.uiWifiTimeout = 0;
   }   
  
   sprintf(appData.sTrapDebug, "W%d%c", appData.uiWifiState, '\0');

   // Check for incoming connection
   if (appData.iWifiStatus & WIFI_STATION)
   {          
      vSetIndicator(2, 0x01, 0x01, 0x01);
      
   }                                   
     
   // State machine
  	switch (appData.uiWifiState)
   {
      case 0:        // Start, wait          
         if (appData.ulLife > 100) 
         {
            vSetIndicator(2, 0x00, 0x00, 0x00);
            appData.iWifiStatus = 0x00;
            vWifi_sendcommand("+RST", 0); // Restart     
            
            appData.uiWifiState = 5;
         }   
      break;
      case 5:
         if (appData.iWifiStatus & WIFI_RESTART)
             appData.uiWifiState = 10;         
      break;                 
      case 10:                          
         // No echo
         appData.iWifiStatus = 0x00;
         if (appData.ulLife > 11000)      // Indicate reset
         {   
             
         }   
         vWifi_sendcommand("E1", 0);                       
//         vWifi_sendcommand("+CWAUTOCONN=0");                       
         appData.uiWifiState = 20;      
      break;                         
      case 20:
         if (appData.iWifiStatus & WIFI_OK)
             appData.uiWifiState = 30;
      break;           
      case 30:
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CWMODE_CUR=3", 0);                          
         appData.uiWifiState = 32;            
      break;                                  
      case 32:
         if (appData.iWifiStatus & WIFI_OK)
         {          
            appData.uiWifiState = 34;
         }                                   
      break;          
      case 34:
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CIPDNS_CUR=1,\"8.8.8.8\"", 0);                          
         appData.uiWifiState = 35;            
      break;                                  
      case 35:
         if (appData.iWifiStatus & WIFI_OK)
         {          
            appData.uiWifiState = 41;
         }                                   
      break;          
      case 41:                    
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CIPMUX=0", 0);
         appData.uiWifiState = 42;         
      break;           
      case 42:     
         // Wait for OK
         if (appData.iWifiStatus & WIFI_OK)
         {
            appData.uiWifiState = 45;  
         }
      break;     
                        
      case 45:                    
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CWSAP_CUR=\"Jeep_TJ\",\"\",4,0", 0);
         appData.uiWifiState = 50;         
      break;           
      case 50:     
         // Wait for OK
         if (appData.iWifiStatus & WIFI_OK)
         {
            appData.uiWifiState = 52;  
         }
      break;
      case 52:                    
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CWHOSTNAME=\"Jeep_TJ\"", 0);
         appData.uiWifiState = 54;         
      break;           
      case 54:     
         // Wait for OK
         if ((appData.iWifiStatus & WIFI_OK) || (appData.iWifiStatus & WIFI_ERROR))
         {
            appData.uiWifiState = 56;  
         }
      break;     
      case 56:                    
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CWLAPOPT=1,2047", 0);
         appData.uiWifiState = 60;         
      break;           
      case 60:     
         // Wait for OK
         if (appData.iWifiStatus & WIFI_OK)
         {
            appData.uiWifiState = 70;  // 220 for config
         }
      break;     
      // Regular loop
      case 70:         
         vSetIndicator(2, 0x00, 0x00, 0x00);
         appData.cApPtr = 0;                     
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CWLAP", 0);   
         appData.uiWifiState = 80;         
      break;           
      case 80:         
         // Wait for OK
         if (appData.iWifiStatus & WIFI_OK)
         {
            vParseStream();            
            appData.uiWifiState = 90;              
         }
         else if (appData.iWifiStatus & WIFI_ERROR)   // Reset
            appData.uiWifiState = 0;    
      break;           
      case 90:
         // Check for open AP and connect
         appData.iWifiStatus = 0x00;
         cTmp = cConnectAp();
         
         if (cTmp)  // Found open
            appData.uiWifiState = 110;         
         else
            appData.uiWifiState = 100;         
      break;             
      case 100:          
        
         appData.uiWifiState = 70;         
      break;           
      
      // Waiting for connect
      case 110:                
         if (appData.iWifiStatus & WIFI_OK)
            appData.uiWifiState = 115;              
         if (appData.iWifiStatus & WIFI_ALREADYC)
            appData.uiWifiState = 115;             
      break;           
      case 115:               
         vSetIndicator(2, 0x00, 0x00, 0x01);
         appData.ulConntime = appData.ulLonglife;
         
         // Connect to cloud    198.54.115.18
         appData.iWifiStatus = 0x00;
         sprintf(ucTemp, "+CIPSTART=\"TCP\",\"%s\",80", sConfig.bCloudServer);
         vWifi_sendcommand(ucTemp, 0);     
         appData.uiWifiState = 120;                           
      break;   
      case 120:            
         if ((appData.iWifiStatus & WIFI_OK) ) // && (appData.iWifiStatus & WIFI_NETCONN))            
            appData.uiWifiState = 125;              
         else if ((appData.iWifiStatus & WIFI_ERROR) && (!(appData.iWifiStatus & WIFI_ALREADYC)))
            appData.uiWifiState = 70; 
         else if (appData.iWifiStatus & WIFI_ALREADYC)
            appData.uiWifiState = 125; 
         else
         {
            if ((appData.ulLonglife - appData.ulConntime) > 2)    // Connection timeout
            {
               vSetIndicator(2, 0x00, 0x00, 0x00);
               appData.uiWifiState = 70;
            }   
         }   
        
      break;           
      case 125:                           
         vSetIndicator(2, 0x00, 0x01, 0x00);
         // Prepare request                  
         vPrepareRequest();         
         appData.uiWifiState = 127;                  
      break;
      
      case 127:         
         appData.iWifiStatus = 0x00;
       
         sprintf(ucTemp, "+CIPSENDEX=%d", appData.iTxLen);
         vWifi_sendcommand(ucTemp, 0);
          
         appData.uiWifiState = 129;                  
      break;
      
      case 129:   
         // Wait for prompt                                  
         if ((appData.iWifiStatus & WIFI_OK) && (appData.iWifiStatus & WIFI_RDYSEND))
         {                
            appData.uiWifiState = 132;
         }                  
      break;
      
      case 132:            
         appData.iWifiStatus = 0x00;
         // Send request                            
         vWifi_sendcommand(appData.ucWifiTempbuff, 1);                           
         appData.uiWifiState = 134;                  
      break;      
      case 134:                        
         if (appData.iWifiStatus & WIFI_SENTOK)
         {  
            vSetIndicator(2, 0x01, 0x01, 0x01);
            appData.cWifiConn = 1;
            appData.uiWifiState = 136;
         }                                                      
      break;
      
      case 136:         
                           
         appData.uiWifiState = 138;                  
      break;
     
      case 138:         
         // Disconnect
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CIPCLOSE", 0);
         for (cTmp = 0; cTmp < 32; cTmp++)
         {   
            if (appData.sWifiAp[cTmp].cEncryption == 10)
               appData.sWifiAp[cTmp].cEncryption = 0;         
         }         
         appData.uiWifiState = 140;                  
      break;
      case 140:                   
         if ((appData.iWifiStatus & WIFI_OK) || (appData.iWifiStatus & WIFI_ERROR))
         {
            vSetIndicator(2, 0x00, 0x00, 0x00);
            appData.uiWifiState = 70;           // 70
         }       
      break;
           
      
      // One-time configuration section
      case 220:
         appData.iWifiStatus = 0x00;
         vWifi_sendcommand("+CIPSTAMAC_DEF=\"00:1d:9c:ba:be:69\"", 0);
         vLongDelay(2000);
         vWifi_sendcommand("+CIPAPMAC_DEF=\"00:1d:9c:ba:be:ee\"", 0);
         
         //         vWifi_sendcommand("+CWJAP_DEF=\"Plan9_3\",\"78babecafe\"");
         //vWifi_sendcommand("+CWJAP_DEF=\"Plan9_4\",\"klodrik73\"");

                  
         //vWifi_sendcommand("+UART_DEF=115200,8,1,0,0");          
         // vWifi_sendcommand("+CIUPDATE");
         
         appData.uiWifiState = 70;              
      default:
         appData.uiWifiState = 0;
   }   
   sprintf(appData.sTrapDebug, "B%c", '\0');

}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//










