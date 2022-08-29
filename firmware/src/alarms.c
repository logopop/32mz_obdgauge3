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
#include "alarms.h"
 

//*********************************************************************************************************************
// Globals
// 
//*********************************************************************************************************************
//
   
extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;
 
 
//*********************************************************************************************************************
//  
//  
//*********************************************************************************************************************
// 

void vAlarmsInit(void)
{
   
   
} 

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vAlarms_State(void)
{    
   //unsigned char                 ucDebug[512]               = {0};
   unsigned long                 ulCumu                     = 0;
   uint8_t                       cChecksum                  = 0;
   uint8_t                       bNum                       = 0;
   uint8_t                       cRnd                       = 0;
   uint8_t                       bAlarm                     = 0;
   unsigned long                 ulI                        = 0;
   char                          cTemp[40]                  = {0};
   short                         iPitch                     = 0;
   short                         iRoll                      = 0;
            
//   strcpy(appData.cGpsDate, "1402");   // Test date alert

  	switch (appData.uiAlarmState)
   {
      case 0:        // Start
         if ((appData.ulLonglife > 3) && (appData.ucSdReady))      // kbdoo make longer         
            appData.uiAlarmState = 5;         
      break;
      
      case 5:        // Hold here if alarms disabled
         if (!(sConfig.cConfig1 & CFG_AL_VISUAL)) 
            appData.uiAlarmState = 5;
         else
            appData.uiAlarmState = 10;
      break;                  
      
      //********************************************
      case 10:        
         iPitch = abs(appData.iPitch);
         iRoll = abs(appData.iRoll);
         
         ulCumu = iPitch * iRoll;
         // Pitch/Roll attitude alarm
         if ((appData.ulLonglife - appData.ulLastAttAlm) > 30)
         {   
            if ((iPitch > 35) || (iRoll > 26) || (ulCumu > 600)) 
            {   
               appData.ulAlarmReturn = appData.ulLonglife;      
               appData.uiLastScreen = appData.uiDisplayScreen;
                       
               appData.ulLastAttAlm = appData.ulLonglife;               
               vSetIndicator(0, 0x12, 0x00, 0x00);      
               appData.cAlarmScreen = 1;
               if (!appData.cSdBusy)
               {
                  vPicturefromSD((uint8_t *)"roll.bmp"); 
                  appData.uiAlarmState = 11;
               }            
            }
            else
               appData.uiAlarmState = 20;
         }
         else
            appData.uiAlarmState = 20;                  
         //sprintf(cTemp, "%d%c", abs(appData.iRoll), '\0');
         //by1351_DrawText(1, 40, (char *)cTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
         
         // Set Gyro attitude LED         
         if ((iPitch > 30) || (iRoll > 22) || (ulCumu > 500))
            vSetIndicator(0, 0x06, 0x00, 0x00);                           
         else if ((iPitch > 25) || (iRoll > 18) || (ulCumu > 400))
            vSetIndicator(0, 0x06, 0x00, 0x10);
         else if ((iPitch > 20) || (iRoll > 14) || (ulCumu > 240))
            vSetIndicator(0, 0x07, 0x06, 0x00);
         else if ((iPitch > 15) || (iRoll > 10) || (ulCumu > 130))
            vSetIndicator(0, 0x00, 0x06, 0x00);
         else if ((iPitch > 13) || (iRoll > 8) || (ulCumu > 110))
            vSetIndicator(0, 0x00, 0x05, 0x00);
         else if ((iPitch > 11) || (iRoll > 7) || (ulCumu > 100))
            vSetIndicator(0, 0x00, 0x03, 0x00);
         else if ((iPitch > 9) || (iRoll > 6) || (ulCumu > 90))
            vSetIndicator(0, 0x00, 0x01, 0x00);
         else
            vSetIndicator(0, 0x00, 0x00, 0x00);                       
      break;       
      case 11:
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"5.wav");   
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 12;
         }           
      break;   
      case 12:          
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            // Go to associated data screen            
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_GYRO;
            appData.uiAlarmState = 20;
         }           
      break;   
      
      //********************************************
      case 20:       // 
         // Incoming SMS
         if (strlen((char *)appData.ucGsmMessage))
         {   
            for (ulI = 0; ulI < 160; ulI++)
               cChecksum ^= appData.ucGsmMessage[ulI];      
            if (cChecksum != appData.cSmsChecksum)
            {   
               appData.ulAlarmReturn = appData.ulLonglife;      
               appData.uiLastScreen = appData.uiDisplayScreen;
               appData.cSmsChecksum = cChecksum;               
               vSetIndicator(0, 0x00, 0x06, 0x00);      
               appData.cAlarmScreen = 1;
               if (!appData.cSdBusy)
               {
                  vPicturefromSD((uint8_t *)"sms.bmp"); 
                  appData.uiAlarmState = 21;
               }    
               appData.cSmsConfig = 0;    // New SMS, reset config flag
            }
            else
               appData.uiAlarmState = 30;         
         }   
         else            
            appData.uiAlarmState = 30;         
      break;
      case 21:       // 
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"45.wav");        
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 22;
         }                             
      break; 
      case 22:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_GSM;
            appData.uiAlarmState = 30;
         }                            
      break;
      
      //********************************************
      case 30:       //
         // System alarm messages
         if (strlen(appData.sError) > 2)
         {        
            appData.ulAlarmReturn = appData.ulLonglife;      
            appData.uiLastScreen = appData.uiDisplayScreen;

            appData.cAlarmScreen = 1;
            if (!appData.cSdBusy)
            {
               vPicturefromSD((uint8_t *)"system.bmp"); 
               appData.uiAlarmState = 31;
            }            
         }
         else
            appData.uiAlarmState = 40;         
      break;
      case 31:       // 
         if (!appData.cSdBusy)
         {                    
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"39.wav");        
            appData.ulAlarmDelay = appData.ulLonglife;
            
            strncpy(cTemp, appData.sError, 16);
            by1351_DrawRectangle(0, 56, 127, 74, 0x00, 0x00, 0x00, ATTR_FILLED);
            by1351_DrawRectangle(0, 57, 127, 73, 0x1f, 0x00, 0x00, ATTR_NONE);
            by1351_DrawRectangle(1, 58, 126, 72, 0x1f, 0x00, 0x00, ATTR_NONE);
            by1351_DrawText(4, 60, cTemp, 0x1f, 0x3f, 0x10, 0x04, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
                
            memset(appData.sError, 0x00, 16);
            appData.uiAlarmState = 32;
         }                             
      break; 
      case 32:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 10)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0;             
            vClearIndicator(1);
            appData.uiAlarmState = 40;
         }                            
      break;                                    

      //********************************************
      case 40:       //
         // Temperature alarm
         if (  (((appData.iGyroTemp > 7000) && (appData.iGyroTemp < 9900)) 
            || ((appData.fTempResult > 70.0) && (appData.fTempResult < 99.0)))                    
            && ((appData.ulLonglife - appData.ulLastTempAlm) > 120))  
         {   
            appData.ulAlarmReturn = appData.ulLonglife;      
            appData.uiLastScreen = appData.uiDisplayScreen;

            appData.ulLastTempAlm = appData.ulLonglife;  
            appData.cAlarmScreen = 1;
            if (!appData.cSdBusy)
            {
               vPicturefromSD((uint8_t *)"temp.bmp"); 
               appData.uiAlarmState = 41;
            }                             
         }   
         else            
            appData.uiAlarmState = 50;         
      break;
      case 41:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"47.wav"); 
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 42;
         }                             
      break; 
      case 42:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_SYSTEM;
            appData.uiAlarmState = 50;
         }                            
      break;                           

      //********************************************
      case 50:       //
         // APRS proximity alarm         
         bAlarm = 0;
         for (bNum = 0; bNum < APRS_ARRAYSIZE; bNum++)
         {                  
            if ((appData.tAprsEntry[bNum].dDistance <= .4) && (appData.tAprsEntry[bNum].cCallsign[0])
                 && (appData.tAprsEntry[bNum].dDistance > .1))
              bAlarm = 1;   
         }         
         if ((bAlarm) && (appData.ulLonglife - appData.ulLastAprsAlm) > 120)  
         {   
            appData.ulAlarmReturn = appData.ulLonglife;      
            appData.uiLastScreen = appData.uiDisplayScreen;

            appData.ulLastAprsAlm = appData.ulLonglife;  
            appData.cAlarmScreen = 1;
            if (!appData.cSdBusy)
            {
               vPicturefromSD((uint8_t *)"proxi.bmp"); 
               appData.uiAlarmState = 51;
            }                             
         }   
         else            
            appData.uiAlarmState = 60;         
      break;
      case 51:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"33.wav");     
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 52;
         }                             
      break; 
      case 52:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_APRSRADAR;
            appData.uiAlarmState = 60;
         }                            
      break;                           

      //********************************************
      case 60:       //
         // WiFi connect/command alert                         
         if ((appData.cWifiConn) && (appData.ulLonglife - appData.ulLastWifiAlm) > 120)  
         {   
            appData.ulAlarmReturn = appData.ulLonglife;      
            appData.uiLastScreen = appData.uiDisplayScreen;

            appData.cWifiConn = 0;
            appData.ulLastWifiAlm = appData.ulLonglife;              
            appData.cAlarmScreen = 1;
            if (!appData.cSdBusy)
            {
               vPicturefromSD((uint8_t *)"wifi.bmp"); 
               appData.uiAlarmState = 61;
            }                             
         }   
         else 
         {
            appData.cWifiConn = 0;     
            appData.uiAlarmState = 70;
         }   
      break;
      case 61:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"45.wav"); 
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 62;
         }                             
      break; 
      case 62:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_WIFI;
            appData.uiAlarmState = 70;
         }                            
      break;                           
 
      //********************************************
      case 70:       //
         
         // Geiger alarm
         if ((appData.fRadLevel > 220) && (appData.ulLonglife - appData.ulLastRadAlm) > 180)  
         {              
            appData.ulAlarmReturn = appData.ulLonglife;      
            appData.uiLastScreen = appData.uiDisplayScreen;

            appData.ulLastRadAlm = appData.ulLonglife;              
            appData.cAlarmScreen = 1;
            if (!appData.cSdBusy)
            {
               vPicturefromSD((uint8_t *)"radiat.bmp"); 
               appData.uiAlarmState = 71;
            }                             
         }   
         else            
            appData.uiAlarmState = 80;         
      break;
      case 71:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"12.wav");      
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 72;
         }                             
      break; 
      case 72:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();             
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_GEIGER;
            appData.uiAlarmState = 80;
         }                            
      break;                           
   
      //********************************************
      case 80:       //
         // Speed alarm
         if ((appData.fGpsSpeed > 111.0) && (appData.ulLonglife - appData.ulLastSpdAlm) > 120)  
         {   
            appData.ulAlarmReturn = appData.ulLonglife;      
            appData.uiLastScreen = appData.uiDisplayScreen;

            appData.ulLastSpdAlm = appData.ulLonglife;              
            appData.cAlarmScreen = 1;
            if (!appData.cSdBusy)
            {
               vPicturefromSD((uint8_t *)"speed.bmp"); 
               appData.uiAlarmState = 81;
            }                             
         }   
         else            
            appData.uiAlarmState = 90;         
      break;
      case 81:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"31.wav");  
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 82;
         }                             
      break; 
      case 82:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_SPEED;
            appData.uiAlarmState = 90;
         }                            
      break;         
        
      //********************************************
      case 90:       //
         // Elevation alert
         if ((appData.ulLonglife - appData.ulLastAltAlm) > 60)  
         {   
            if (    ((appData.dGpsAlt >= 500.0) && (appData.dPrevalt < 500.0))
                 || ((appData.dGpsAlt >= 750.0) && (appData.dPrevalt < 750.0))  
                 || ((appData.dGpsAlt >= 1000.0) && (appData.dPrevalt < 1000.0))  
                 || ((appData.dGpsAlt >= 1100.0) && (appData.dPrevalt < 1100.0))  
                 || ((appData.dGpsAlt >= 1200.0) && (appData.dPrevalt < 1200.0))  
                 || ((appData.dGpsAlt >= 1300.0) && (appData.dPrevalt < 1300.0))  
                 || ((appData.dGpsAlt >= 1400.0) && (appData.dPrevalt < 1400.0))  
                 || ((appData.dGpsAlt >= 1500.0) && (appData.dPrevalt < 1500.0))  
                 || ((appData.dGpsAlt >= 1600.0) && (appData.dPrevalt < 1600.0))  
                 || ((appData.dGpsAlt >= 1700.0) && (appData.dPrevalt < 1700.0))  
                 || ((appData.dGpsAlt >= 1800.0) && (appData.dPrevalt < 1800.0))  
                 || ((appData.dGpsAlt >= 1900.0) && (appData.dPrevalt < 1900.0))  
                 || ((appData.dGpsAlt >= 2000.0) && (appData.dPrevalt < 2000.0))  
                 || ((appData.dGpsAlt >= 2100.0) && (appData.dPrevalt < 2100.0))  
                 || ((appData.dGpsAlt >= 2200.0) && (appData.dPrevalt < 2200.0))  
                 || ((appData.dGpsAlt >= 2300.0) && (appData.dPrevalt < 2300.0))  
                 || ((appData.dGpsAlt >= 2400.0) && (appData.dPrevalt < 2400.0))  
                 || ((appData.dGpsAlt >= 2500.0) && (appData.dPrevalt < 2500.0))  
                 || ((appData.dGpsAlt >= 2600.0) && (appData.dPrevalt < 2600.0))  )                    
            {    
               appData.ulAlarmReturn = appData.ulLonglife;      
               appData.uiLastScreen = appData.uiDisplayScreen;

               appData.ulLastAltAlm = appData.ulLonglife;          
               appData.cAlarmScreen = 1;
               if (!appData.cSdBusy)
               {
                  vPicturefromSD((uint8_t *)"alt.bmp"); 
                  appData.uiAlarmState = 91;
               }                             
            }   
            else 
            {   
               appData.dPrevalt = appData.dGpsAlt;
               appData.uiAlarmState = 100;         
            }
         }
         else 
         {   
            appData.dPrevalt = appData.dGpsAlt;
            appData.uiAlarmState = 100;         
         }               
      break;
      case 91:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"6.wav");   
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 92;
         }                             
      break; 
      case 92:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_POSITION;
            appData.uiAlarmState = 100;
         }                            
      break;                            

      //********************************************
      case 100:       //
         // Extract degrees                  
         appData.cLatdeg = (int)(abs(appData.dGpsLatDec));
         appData.cLondeg = (int)(abs(appData.dGpsLonDec));
          
         // Confluence alert
         if ((appData.ulLonglife > 30) && ((appData.ulLonglife - appData.ulLastConfAlm) > 60)
            && (appData.cPrevLatdeg) && (appData.cPrevLondeg) && (appData.dGpsLatDec) && (appData.dGpsLonDec) )  
         {                             
            if ((appData.cPrevLatdeg != appData.cLatdeg) || (appData.cPrevLondeg != appData.cLondeg) )
            {               
               appData.ulAlarmReturn = appData.ulLonglife;      
               appData.uiLastScreen = appData.uiDisplayScreen;
               
               appData.ulLastConfAlm = appData.ulLonglife;                
               appData.cAlarmScreen = 1;
               if (!appData.cSdBusy)
               {
                  vPicturefromSD((uint8_t *)"confl.bmp"); 
                  appData.uiAlarmState = 101;
               }                             
               appData.cPrevLatdeg = appData.cLatdeg;
               appData.cPrevLondeg = appData.cLondeg;            
            }   
            else            
               appData.uiAlarmState = 110;         
         }   
         else  
         {
            appData.cPrevLatdeg = appData.cLatdeg;
            appData.cPrevLondeg = appData.cLondeg;            
            appData.uiAlarmState = 110;  
         }   
      break;
      case 101:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"7.wav");  
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 102;
         }                             
      break; 
      case 102:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);\
            appData.uiDisplayScreen = SCREEN_POSITION;
            appData.uiAlarmState = 110;
         }                            
      break;         
               
      //********************************************
      case 110:       //                                
         // Date alert Christmas and Valentines
         if ((appData.ulLonglife > 30) && (!appData.cDateAlert))  
         {                                   
            appData.cDateAlert = 1;
            
            // Christmas
            if ((!memcmp(appData.cGpsDate, "2412", 4)) 
               || (!memcmp(appData.cGpsDate, "2512", 4))
               || (!memcmp(appData.cGpsDate, "2612", 4)))
            {                                 
               appData.cAlarmScreen = 1;
               if (!appData.cSdBusy)
               {
                  appData.ulAlarmReturn = appData.ulLonglife;      
                  appData.uiLastScreen = appData.uiDisplayScreen;

                  sprintf(cTemp, "%ul", RNGNUMGEN1);
                  cRnd = cTemp[1];
                  cRnd -= 0x30;
                  if (!cRnd)
                     cRnd = 10;
                  sprintf(cTemp, "xmas%d.bmp%c", cRnd, '\0');                        
                  vPicturefromSD((uint8_t *)cTemp);                                                       
                  appData.uiAlarmState = 111;
               }                                                     
            }   
            // Valentines
            else if (!memcmp(appData.cGpsDate, "1402", 4)) 
            {                                                                                          
               appData.cAlarmScreen = 1;
               if (!appData.cSdBusy)
               {
                  appData.ulAlarmReturn = appData.ulLonglife;      
                  appData.uiLastScreen = appData.uiDisplayScreen;

                  sprintf(cTemp, "%ul", RNGNUMGEN1);
                  cRnd = cTemp[1];
                  cRnd -= 0x30;
                  if (cRnd > 4)
                     vPicturefromSD((uint8_t *)"valentine.bmp");
                  else
                     vPicturefromSD((uint8_t *)"valentine2.bmp");                                                    
                  appData.uiAlarmState = 111;
               }                                                     
            }   
            else            
               appData.uiAlarmState = 120;         
         }   
         else  
            appData.uiAlarmState = 120;  
      break;
      case 111:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"46.wav");  
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 112;
         }                             
      break; 
      case 112:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cDateAlert = 1;
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);\
            appData.uiDisplayScreen = SCREEN_POSITION;
            appData.uiAlarmState = 120;
         }                            
      break;                       
      
      //********************************************
      case 120:       //   time 120
         // Voltage, current
         if (
              (appData.dVoltage > 1.0) 
                 && ((appData.ulLonglife - appData.ulLastPwrAlm) > 20)                
                 && ((appData.dVoltage < 11.4) || (appData.dVoltage > 14.5) || (appData.dCurrent > 400.0)) 
                 
            )  
         {         
            appData.ulAlarmReturn = appData.ulLonglife;      
            appData.uiLastScreen = appData.uiDisplayScreen;           
            appData.ulLastPwrAlm = appData.ulLonglife;              
            appData.cAlarmScreen = 1;
            if (!appData.cSdBusy)
            {
               vPicturefromSD((uint8_t *)"warning.bmp"); 
               appData.uiAlarmState = 121;
            }                             
         }   
         else            
            appData.uiAlarmState = 200;         
      break;
      case 121:       //           
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"28.wav");  
            appData.ulAlarmDelay = appData.ulLonglife;
            appData.uiAlarmState = 122;
         }                             
        //   sprintf(appData.uiDebugbuffer3, "%f  %f", appData.dVoltage, appData.dCurrent);
          
      break; 
      case 122:       // 
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            appData.cAlarmScreen = 0; 
            appData.uiDispFunc = FUNC_SCREEN;
            vClearIndicator(1);
            appData.uiDisplayScreen = SCREEN_SENSORS;
            appData.uiAlarmState = 200;
         }                            
      break;         
                          
      //********************************************
      case 200:       //
         // Test alarm         
         if (0)         // Alarm condition
         {
            appData.ulAlarmReturn = appData.ulLonglife;      
            appData.uiLastScreen = appData.uiDisplayScreen;

            appData.cAlarmScreen = 1;
            if (!appData.cSdBusy)
            {
               vPicturefromSD((uint8_t *)"alt.bmp"); 
               appData.uiAlarmState = 201;
            }            
         }
         else
            appData.uiAlarmState = 5;
      break;
      case 201:
         appData.ulAlarmDelay = appData.ulLonglife;
         if (!appData.cSdBusy)
         {
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"ding1.wav");            
            appData.uiAlarmState = 202;
         }           
      break;   
      case 202:          
         if ((appData.ulLonglife - appData.ulAlarmDelay) > 5)
         {
            by1351_Clear();
            // Go to associated data screen            
            appData.cAlarmScreen = 0;            
            appData.uiAlarmState = 5;
         }           
      break;   
   }   
} 
 
//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//










