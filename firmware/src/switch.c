//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//
//  
//
//*********************************************************************************************************************
//
 
#include "app.h"
#include "switch.h"
 

//*********************************************************************************************************************
// Globals
// 
//*********************************************************************************************************************
//

extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;

#define     TUNE                 10



//*********************************************************************************************************************
// Change frequency up or down by 5 kHz
//
//*********************************************************************************************************************
//

void vChangeFrequency(unsigned char * cFrequ, uint8_t cDir)
{
   unsigned long                          ulFreq                     = 0;
   char                                   cFreq[10];
   //char                                   cTemp[20];
  
   memcpy(cFreq, cFrequ, 3);
   memcpy(cFreq + 3, cFrequ + 4, 3);
   cFreq[7] = 0;    
   sscanf(cFreq, "%lu", &ulFreq);
   if (cDir)
      ulFreq += 5;
   else
      ulFreq -= 5;
   // Check band limits
   if (ulFreq < 144000)
      ulFreq = 144000;
   if (ulFreq > 147995)
      ulFreq = 147995;
   // Write back to variables
   memset(cFrequ, 0x00, 8);
   sprintf((char *)cFrequ, "%lu", ulFreq);
   cFrequ[6] = cFrequ[5];
   cFrequ[5] = cFrequ[4];
   cFrequ[4] = cFrequ[3];
   cFrequ[3] = '.';
}
   
//*********************************************************************************************************************
// Track screens and menus
//
//*********************************************************************************************************************
//
 
void vNavigateMenus(uint8_t uiAction)
{ 
   // Make sure screens are redrawn
   appData.cWchanged = 42;
   appData.cBchanged = 42;
   
   // Edit setting  RX = 1, TX = 2
   if ((appData.cEditFreq) && (appData.uiDispFunc == FUNC_SUBSCREEN))
   {            
      switch(uiAction)
      {
         case BTN_RIGHT:
            if (appData.cEditFreq == 1)
            {
               vChangeFrequency(sConfig.bRXfreq, 1);
            }   
            else if (appData.cEditFreq == 2)
            {   
               vChangeFrequency(sConfig.bTXfreq, 1);                                                       
            }   
         break;       
         case BTN_LEFT:
            if (appData.cEditFreq == 1)
            {
               vChangeFrequency(sConfig.bRXfreq, 0);
            }   
            else if (appData.cEditFreq == 2)
            {   
               vChangeFrequency(sConfig.bTXfreq, 0);                                                       
            }                   
         break;
         case BTN_RELEASE_LONG:
            strcpy((char *)sConfig.bTXfreq, APRS_TXFREQ);
            strcpy((char *)sConfig.bRXfreq, APRS_RXFREQ);                                  
         break;
         case BTN_RELEASE:
            
            
         break;                  
         case BTN_PUSH:
            vSetIndicator(1, 0x00, 0x00, 0x02);
            appData.cEditFreq = 0;
         break; 
      }
   } 
   else if ((appData.cEditBias) && (appData.uiDispFunc == FUNC_SUBSCREEN))
   {            
      switch(uiAction)
      {
         case BTN_RIGHT:
            appData.iDispBias++;
            
         break;       
         case BTN_LEFT:
            appData.iDispBias--;     
         break;
         case BTN_RELEASE_LONG:
                                     
         break;
         case BTN_RELEASE:
                        
         break;                  
         case BTN_PUSH:
            sConfig.iDispBias = appData.iDispBias;
            vSetIndicator(1, 0x00, 0x00, 0x02);
            appData.cEditBias = 0;
         break;                          
      }
   }
   else if ((appData.cEditGeiger) && (appData.uiDispFunc == FUNC_SUBSCREEN))
   {            
      switch(uiAction)
      {
         case BTN_RIGHT:
            appData.iGeigerCal++;
            
         break;       
         case BTN_LEFT:
            appData.iGeigerCal--;     
         break;
         case BTN_RELEASE_LONG:
                                     
         break;
         case BTN_RELEASE:
                        
         break;                  
         case BTN_PUSH:
            sConfig.iGeigerCal = appData.iGeigerCal;
            vSetIndicator(1, 0x00, 0x00, 0x02);
            appData.cEditGeiger = 0;
         break;                          
      }
   }
   else if ((appData.cEditAlmDelay) && (appData.uiDispFunc == FUNC_SUBSCREEN))
   {            
      switch(uiAction)
      {
         case BTN_RIGHT:
            appData.cReturnAlarm++;
            if (appData.cReturnAlarm > 180)
               appData.cReturnAlarm = 0;
         break;       
         case BTN_LEFT:
            if (!appData.cReturnAlarm)
               appData.cReturnAlarm = 180;
            else
               appData.cReturnAlarm--;     
         break;
         case BTN_RELEASE_LONG:
                                     
         break;
         case BTN_RELEASE:
                        
         break;                  
         case BTN_PUSH:
            sConfig.cConfig2 = appData.cReturnAlarm;
            vSetIndicator(1, 0x00, 0x00, 0x02);
            appData.cEditAlmDelay = 0;
         break;                          
      }
   }
   else
   {   
      switch(uiAction)
      {
         case BTN_RIGHT:
            if (appData.uiDispFunc == FUNC_SCREEN)
            {   
               appData.cChangedScreen = 1;
               appData.uiDisplayScreen++;
               if (appData.uiDisplayScreen > NUM_SCREENS)
               {
                  appData.cIntroDone = 0;
                  appData.uiDisplayScreen = 0;
               }                  
               // Refresh radar   
               if (appData.uiDisplayScreen == SCREEN_APRSRADAR)
                  appData.cAprsRefresh = 1;
               // Refresh sensors
               appData.dOldTemperature = -999;
               appData.dOldPressure = -999;
               appData.dOldVoltage = -999;
               appData.dOldCurrent = -999;
               appData.uiDisplayState = 40;
            } 
            else if (appData.uiDispFunc == FUNC_SUBSCREEN)
            {
               if (appData.uiDisplaySubscreen == SCREEN_APRSLIST_SUB)
               {   
                   while (strlen((char *)appData.tAprsEntry[appData.uiStation].cCallsign) < 2)
                  {
                     appData.uiStation++;   
                     if (appData.uiStation >= APRS_ARRAYSIZE)
                        break;
                  }   

                  appData.cChangedScreen = 1;
                  appData.uiStation++;
                  if (appData.uiStation >= APRS_ARRAYSIZE)
                     appData.uiStation = 0; 
               }   
               else if (appData.uiDisplaySubscreen == SCREEN_APRSRADAR_SUB)
               {
                  if (appData.fScale < 76.0)
                  {   
                     appData.fScale += 5.0;
                     appData.cAprsRefresh = 1;
                  }   
               }   
               else if (appData.uiDisplaySubscreen == SCREEN_INTRO_SUB)
               {
                  appData.iIntroPic++;
                  if (appData.iIntroPic > INTROPIC_MAX)
                     appData.iIntroPic = INTROPIC_MIN;
                  appData.cIntroDone = 0;
               }   
               else if (appData.uiDisplaySubscreen == SCREEN_SETTINGS_SUB)
               {                        
                  appData.cSetting++;
                  if (appData.cSetting > MAX_SETTING) // Max setting line
                     appData.cSetting = 0;
               }
               else if (appData.uiDisplaySubscreen == SCREEN_POSITION_SUB)
               {   
                  appData.dSealevel += 0.1;
               }
            }   
         break;   
         case BTN_LEFT:            
            if (appData.uiDispFunc == FUNC_SCREEN)
            {   
               appData.cChangedScreen = 1;
               if (!appData.uiDisplayScreen)
                  appData.uiDisplayScreen = NUM_SCREENS;
               else
                  appData.uiDisplayScreen--; 
               if (!appData.uiDisplayScreen)
                  appData.cIntroDone = 0;

               // Refresh radar   
               if (appData.uiDisplayScreen == SCREEN_APRSRADAR)
                  appData.cAprsRefresh = 1;
               // Refresh sensors
               appData.dOldTemperature = -999;
               appData.dOldPressure = -999;
               appData.dOldVoltage = -999;
               appData.dOldCurrent = -999;
               appData.cWchanged = 42;               
               appData.uiDisplayState = 40;
            }   
            else if (appData.uiDispFunc == FUNC_SUBSCREEN)
            {
               if (appData.uiDisplaySubscreen == SCREEN_APRSLIST_SUB)
               {   
                  appData.cChangedScreen = 1;
                  if (appData.uiStation > 0)
                     appData.uiStation--;
                  else
                     appData.uiStation = APRS_ARRAYSIZE - 1; 

                  while (strlen((char *)appData.tAprsEntry[appData.uiStation].cCallsign) < 2)
                  {
                     appData.uiStation--;   
                     if (!appData.uiStation)
                        break;
                  }    
               }   
               else if (appData.uiDisplaySubscreen == SCREEN_APRSRADAR_SUB)
               {
                  if (appData.fScale > 6.0)
                  {   
                     appData.fScale -= 5.0;
                     appData.cAprsRefresh = 1;
                  }   
               } 
               else if (appData.uiDisplaySubscreen == SCREEN_INTRO_SUB)
               {
                  appData.iIntroPic--;
                  if (appData.iIntroPic < INTROPIC_MIN)
                     appData.iIntroPic = INTROPIC_MAX;
                  appData.cIntroDone = 0;
               }   
               else if (appData.uiDisplaySubscreen == SCREEN_SETTINGS_SUB)
               {                                       
                  if (appData.cSetting == 0) // Max setting line
                     appData.cSetting = MAX_SETTING;
                  else
                     appData.cSetting--;
               }
               else if (appData.uiDisplaySubscreen == SCREEN_POSITION_SUB)
               {   
                  appData.dSealevel -= 0.1;
               }
            }   
         break;   
         case BTN_PUSH:
            appData.uiPushed |= ACT_PUSHED;      
            appData.cCanSendScreen = appData.uiDisplayScreen;   
            if (appData.uiDisplayScreen == SCREEN_SYSTEM)      // Cycle settings
            {
               if (!sConfig.cConfig1)                          // Geiger off, alarms off
               {
                  sConfig.cConfig1 = CFG_AL_VISUAL | CFG_AL_AUDIO;               
               }
               else if ((sConfig.cConfig1 & CFG_AL_VISUAL) && (sConfig.cConfig1 & CFG_AL_AUDIO) && (!(sConfig.cConfig1 & CFG_GEIGER))) // Geiger off, alarms on
               {
                  sConfig.cConfig1 |= CFG_GEIGER;               
               }
               else if ((sConfig.cConfig1 & CFG_AL_VISUAL) && (sConfig.cConfig1 & CFG_AL_AUDIO) && (sConfig.cConfig1 & CFG_GEIGER))    // Geiger on, alarms on
               {
                  sConfig.cConfig1 = CFG_AL_VISUAL;               
               }
               else
                  sConfig.cConfig1 = 0;
               appData.ulLastCfgChange = appData.ulLonglife;
            }           
            else if (appData.uiDisplayScreen == SCREEN_SHOW)   // Push cycles videos
            {
               appData.ucAbortShow = 1;   
               appData.ucShowNext = 1;  
            }                                                                                  
            else if (appData.uiDisplayScreen == SCREEN_GYRO)   // Push is resetting zero
            {
               sConfig.iXoff = -appData.iX;
               sConfig.iYoff = -appData.iY;                        
            }                                   
            // Subscreen, reset to normal
            else if (appData.uiDispFunc == FUNC_SUBSCREEN)
            {                            
               //by1351_DrawRectangle(0, 11, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window      
               if (appData.uiDisplaySubscreen == SCREEN_APRSRADAR_SUB)
               {  
                  vClearIndicator(1);
                  appData.uiDispFunc = FUNC_SCREEN;   
               }
               else if (appData.uiDisplaySubscreen == SCREEN_APRSLIST_SUB)
               {  
                  appData.cChangedScreen = 1;
                  vClearIndicator(1);
                  appData.uiDispFunc = FUNC_SCREEN;   
               }
               else if (appData.uiDisplaySubscreen == SCREEN_INTRO_SUB)
               {               
                  by1351_Clear();
                  vClearIndicator(1);
                  appData.uiDispFunc = FUNC_SCREEN;   
                  appData.uiDisplayScreen = SCREEN_SPEED;               
               }            
               else if (appData.uiDisplaySubscreen == SCREEN_SETTINGS_SUB)   
               {                
                  if (appData.cSetting)       // Not on 'done', change setting
                  {  
                     if (appData.cSetting == 1)       // TX power
                     {
                        sConfig.bPower++;
                        if (sConfig.bPower > 5)
                           sConfig.bPower = 1;
                     }   
                     else if (appData.cSetting == 2)  // Path
                     {   
                        if (!strncmp((char *)sConfig.bAprsPath, "WIDE1-1", 7))
                           strncpy((char *)sConfig.bAprsPath, "WIDE2-2", 7);
                        else if (!strncmp((char *)sConfig.bAprsPath, "WIDE2-2", 7))
                           strncpy((char *)sConfig.bAprsPath, "WIDE3-3", 7);
                        else if (!strncmp((char *)sConfig.bAprsPath, "WIDE3-3", 7))
                           strncpy((char *)sConfig.bAprsPath, "WIDE4-4", 7);
                        else if (!strncmp ((char *)sConfig.bAprsPath, "WIDE4-4", 7))
                           strncpy((char *)sConfig.bAprsPath, "WIDE1-1", 7);                                                
                     }   
                     else if (appData.cSetting == 3)  // Period
                     {   
                        if (sConfig.bPeriod == 30)
                           sConfig.bPeriod = 60;
                        else if (sConfig.bPeriod == 60)
                           sConfig.bPeriod = 120;
                        else if (sConfig.bPeriod == 120)
                           sConfig.bPeriod = 300;
                        else if (sConfig.bPeriod == 300)
                           sConfig.bPeriod = 600;
                        else if (sConfig.bPeriod == 600)
                           sConfig.bPeriod = 1200;
                        else if (sConfig.bPeriod == 1200)
                           sConfig.bPeriod = 30;
                        else
                           sConfig.bPeriod = 60;                                                
                     }      
                     else if (appData.cSetting == 4)  // Freq TX
                     {   
                        appData.cEditFreq = 2;
                        vSetIndicator(1, 0x00, 0x05, 0x03);
                                               
                     }
                     else if (appData.cSetting == 5)  // Freq RX
                     {   
                        appData.cEditFreq = 1;
                        vSetIndicator(1, 0x00, 0x05, 0x03);
                                                                                                                 
                     } 
                     else if (appData.cSetting == 6)  // Display contrast bias
                     {
                        appData.cEditBias = 1;
                        vSetIndicator(1, 0x00, 0x05, 0x03);
                     }   
                     else if (appData.cSetting == 7)  // Display Geiger calibration
                     {
                        appData.cEditGeiger = 1;
                        vSetIndicator(1, 0x00, 0x05, 0x03);
                     }
                     else if (appData.cSetting == 8)  // Display alarm return delay value, seconds, 0 is off
                     {
                        appData.cEditAlmDelay = 1;
                        vSetIndicator(1, 0x00, 0x05, 0x03);
                     }   
                  }   
                  else
                  {                                    
                     appData.uiDispFunc = FUNC_SCREEN;              
                     vClearIndicator(1);
                  }   
               }               
               else if (appData.uiDisplaySubscreen == SCREEN_GEIGER_SUB)
               {   
                  appData.uiDispFunc = FUNC_SCREEN;   
                  vClearIndicator(1);
                  appData.cChangedScreen = 1;
               }
               else if (appData.uiDisplaySubscreen == SCREEN_POSITION_SUB)
               {   
                  appData.uiDispFunc = FUNC_SCREEN;   
                  vClearIndicator(1);
                  vClearIndicator(0);
                  appData.cChangedScreen = 1;
               }
               else
               {                  
                  vClearIndicator(1);
                  appData.uiDispFunc = FUNC_SCREEN;   
               }                          
            }   
            else  // Enter subscreen if it exists
            {
               appData.cCanSendScreen = appData.uiDisplayScreen;   
               if (appData.uiDisplayScreen == SCREEN_INTRO)
               {   
                  vSetIndicator(1, 0x00, 0x00, 0x02);
                  by1351_DrawRectangle(0, 0, 127, 127, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window
                  appData.uiDisplaySubscreen = SCREEN_INTRO_SUB;
                  appData.cIntroDone = 0;
                  appData.uiDispFunc = FUNC_SUBSCREEN;  
                  appData.iIntroPic = INTROPIC_MIN;
               }   
               else if (appData.uiDisplayScreen == SCREEN_GEIGER)
               {   
                  vSetIndicator(1, 0x00, 0x00, 0x02);
                  by1351_DrawRectangle(0, 11, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window
                  appData.uiDisplaySubscreen = SCREEN_GEIGER_SUB;
                  appData.uiDispFunc = FUNC_SUBSCREEN;
                  appData.cChangedScreen = 1;
                  appData.cGotGeiger = 1;    
               }   
               else if (appData.uiDisplayScreen == SCREEN_APRSLIST)
               {  
                  vSetIndicator(1, 0x00, 0x00, 0x02);
                  appData.cChangedScreen = 1;
                  /*
                  by1351_DrawRectangle(0, 11, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window
                  by1351_DrawText(0, 15,  "                \0", 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x0f, FONT_VGA, 1, ATTR_PAD);  // Top bar
                  by1351_DrawText(0, 100, "                \0", 0x1f, 0x3f, 0x1f, 0x00, 0x1f, 0x00, FONT_VGA, 1, ATTR_PAD);  // Bottom bar
                   */
                  
                  appData.uiDisplaySubscreen = SCREEN_APRSLIST_SUB;
                  appData.uiDispFunc = FUNC_SUBSCREEN;
                  appData.uiStation = 0;
               }   
               else if (appData.uiDisplayScreen == SCREEN_APRSRADAR)
               {  
                  vSetIndicator(1, 0x00, 0x00, 0x02);
                  appData.uiDisplaySubscreen = SCREEN_APRSRADAR_SUB;
                  appData.uiDispFunc = FUNC_SUBSCREEN;              
               }       
               else if (appData.uiDisplayScreen == SCREEN_SETTINGS)
               {  
                  vSetIndicator(1, 0x00, 0x00, 0x02);
                  appData.uiDisplaySubscreen = SCREEN_SETTINGS_SUB;
                  appData.uiDispFunc = FUNC_SUBSCREEN;              
               }       
               else if (appData.uiDisplayScreen == SCREEN_POSITION)
               {  
                  if (appData.dCalAltitude > 0.1)  // We have valid data
                  {                        
                     appData.cChangedScreen = 1;
                     appData.iPrevAltScale = 1;    // Assure refresh
                     vSetIndicator(1, 0x00, 0x00, 0x02);
                     appData.uiDisplaySubscreen = SCREEN_POSITION_SUB;
                     appData.uiDispFunc = FUNC_SUBSCREEN;              
                  }   
                  else
                     vBeep(2);
               }       
            }         
         break;   
         case BTN_RELEASE:
            appData.uiPushed |= ACT_RELEASED;          
         break;   
         case BTN_RELEASE_LONG:           
            appData.uiPushed |= ACT_RELEASED_LONG; 
         break;   
      }   
   }   
}

//*********************************************************************************************************************
// Callback from change notification  
//
//*********************************************************************************************************************
//

void APP_SwitchReceiveEventHandler(void)   
{ 
   uint8_t                                a                          = 0;
   uint8_t                                b                          = 0;
   static uint8_t                         a0                         = 0;
   static uint8_t                         c0                         = 0;

   if ((appData.ulLonglife > 2) || (appData.cStarted))
   {
      appData.cStarted = 1;            
      a = TURN_L;
      b = TURN_R;
      if (a != a0) 
      {              // A changed
         a0 = a;
         if (b != c0) 
         {
            c0 = b;
            if (a == b)
            {
               if ((appData.ulLife - appData.ulLastRight) > 50)
               {   
                  vBeep(5);
                  appData.ucAbortShow = 2;
                  if (!appData.uiDisplayLock)
                  {
                     vNavigateMenus(BTN_RIGHT);
                  }   
                  appData.ulLastRight = appData.ulLife;
                  appData.ulButtontime = appData.ulLonglife;
               }   
            }
            else
            {
               if ((appData.ulLife - appData.ulLastLeft) > 50)
               {   
                  vBeep(6);
                  appData.ucAbortShow = 1;
                  if (!appData.uiDisplayLock)
                  {
                     vNavigateMenus(BTN_LEFT);
                  }   
                  appData.ulLastLeft = appData.ulLife;  
                  appData.ulButtontime = appData.ulLonglife;
               }
            }   
         }   
      }

      // Pushbutton
      if (!BUTTON && !appData.uiButtonState) 
      {
         vBeep(7);
         if (((appData.ulLife - appData.ulLastLeft) < 300) || ((appData.ulLife - appData.ulLastRight) < 300)) // Revert screen
            appData.uiDisplayScreen = appData.uiDisplayOld;
         appData.uiButtonState = 1;
         if ((appData.ulLife - appData.ulPushlife) > 100)
         {
            appData.ulPushlife = appData.ulLife;         
            vNavigateMenus(BTN_PUSH);
         }
      }   
      else if (BUTTON && appData.uiButtonState) 
      {
         appData.uiButtonState = 0;         
         if ((appData.ulLife < appData.ulPushlife) || ((appData.ulLife - appData.ulPushlife) > 500))
         {   
            vBeep(3);   
            vNavigateMenus(BTN_RELEASE_LONG);                 
            vBeep(3);
         }   
         else   
            vNavigateMenus(BTN_RELEASE);                 
      }                                    
   }     
}

//*********************************************************************************************************************
//  
//*********************************************************************************************************************
//
 
void vSwitch_Initialize (void)
{       
   appData.uiButtonState = 0;
   appData.ulLastChange = appData.ulLife;
}

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//
 



//*********************************************************************************************************************
// 
 




