//*********************************************************************************************************************
//  OBDGAUGE3
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
#include "can.h"

extern APP_DATA                   appData;
extern NVM_CONFIG                 sConfig;

void vCan_State(void);
void vCanTransmit(short, uint8_t *, uint8_t);

//APP_DRV_OBJECTS appDrvObject;


//*********************************************************************************************************************  


//*********************************************************************************************************************
// Swap endian 4-byte longs
//
//*********************************************************************************************************************
//

unsigned long ulSwapLendian(unsigned long ulValue)
{
   unsigned long                 ulTemp                     = 0;
   unsigned char               * pbP                        = NULL;
   unsigned char               * pbQ                        = NULL;
//   char                          cI                         = 0;

   pbP = (unsigned char *)&ulValue;
   pbQ = (unsigned char *)&ulTemp;
   
   *(pbQ+1) = *(pbP+2);
   *(pbQ) = *(pbP+3);
   *(pbQ+3) = *pbP;
   *(pbQ+2) = *(pbP+1);

   return ulTemp;
}

//*********************************************************************************************************************
// Software reset
//
//*********************************************************************************************************************
//

void vResetDevice(void)
{ 
   appData.cResetFlag = 1;
   by1351_Clear();
   by1351_DrawText(1, 30, (char *)"RESET!", 31, 61, 31, 0, 0, 0, FONT_VGA, 2, 0);
   vBeep(4);
   
   SYS_FS_Unmount("/mnt/myDrive1");
   //SYS_INT_StatusGetAndDisable();
   //WIFI_RESET = 0;
   appData.uiDispFunc = FUNC_SCREEN;
   appData.uiDisplayScreen = 0;
   appData.uiDisplaySubscreen = 0;    
   vClearIndicator(1);                            
   appData.cEditFreq = 0;      
   vLongDelay(1000);
   by1351_Initialize(); 
   appData.ulResetCounter = 0;
   appData.uiPressState = 0;
   
   by1351_Initialize();  
   by1351_SetContrast(6);  
   appData.ulLife = 0;
   appData.ulLonglife = 0;
   appData.deviceIsConnected = false;
   appData.uiAppState = 0;  
   appData.uiNvramState = 0;
   appData.uiSdState = 0;
   appData.uiDisplayState = 0;
   appData.uiUsbState = APP_STATE_BUS_ENABLE; 
     appData.uiWifiState = 0; 
   appData.uiCanState = 0; 
   appData.uiDmenu = 0;
   appData.cCount = 0;                    // SD mount counter
   appData.ucSdReady = 0;                 // 1 when volume mounted
   appData.uiMiscState = 0;
   appData.uiNvramState = 0;
   appData.ulMiscCount = 0;
   appData.ucCalDone = 0;
   appData.usStatus = 0b0000011111111111; // Status all errors until confirmed ok
   appData.cSmsConfig = 0;
   // Initialize display           
   appData.uiDisplayPrev = 0;
   appData.uiDisplayScreen = 0;
   appData.uiDisplayState = 0;
   memset(appData.ucSmsSetting, 0x00, 20);
   
   // APRS scale
   appData.fScale = 10;                   // Initial scale
   appData.dAprsStations = 4;             // Number of stations on screen
   appData.uiAprsAge = 180;               // Recently seen    
}

//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//

void vPress_State(void)
{
   double                        dTemp                      = 0.0;
   long                          ulTemp                     = 0;
   long                          ulTimeMult                 = 0;

   switch (appData.uiPressState)
   {
      case 0:
         appData.uiPressState = 10;
      break;
      
      case 10:
         // Start-up GPS altitude averaging, for pressure calibration
         if (!appData.ucCalDone)
         {   
            // Average GPPS altitude for pressure calibration
            if ((appData.sGpsMode > 1) && (!appData.ucCalStarted))
            {   
               vSetIndicator(0, 0x01, 0x00, 0x00);
               vLongDelay(5);
               vClearIndicator(0);                              
               appData.uiCalStart = appData.ulLonglife;
               appData.ucCalStarted = 1; 
               appData.ulAltNum = 0;
               appData.dAltAccu = 0;                     
               appData.uiPressState = 20;
            }   
         }  
      break;
      
      case 20:
         // Wait 10 seconds after GPS has acquired altitude
         if ((appData.ucCalStarted) && ((appData.ulLonglife - appData.uiCalStart) > 10))
         {               
            appData.dAltAccu += appData.dGpsAlt;
            appData.ulAltNum++;
            appData.uiPressState = 30;
         }             
      break; 
      
      case 30:        
         if (((appData.ulLonglife - appData.uiCalStart) > 20) || (appData.fGpsSpeed > 5.0))
         {
            vSetIndicator(0, 0x00, 0x00, 0x01);
            vLongDelay(5);
            vClearIndicator(0);
            appData.ucCalDone = 1;
            appData.dCalAltitude = appData.dAltAccu / appData.ulAltNum;  
            appData.dCalPressure = appData.dPressure;
            // Find normalized pressure at sealevel
            appData.dSealevel = (2.62057 * pow(10, 24) * (appData.dPressure)) 
                    / (pow((44330 - appData.dCalAltitude), 5.255));        
            appData.uiPressState = 50;
         }   
      break;
      
      case 50:       // Will remain here     
         
         // Calculate altitude by pressure
         // H = 44330 * [1 - (P/p0)^(1/5.255) ]        
         if (appData.ucCalDone)
         {   
            // Altitude by pressure
            appData.dPressAlt = 44330 * (1 - pow((appData.dCalPressure/appData.dSealevel), (1/5.255)));
            if (appData.dCalAltitude <= 0.2)    // No calibration
            {
               appData.ucCalDone = 0;
               appData.ucCalStarted = 0;               
            }  
            else
            {   
               //sprintf(appData.uiDebugbuffer2, "%f\0", appData.dPressAlt);                             
               // Rate of change
               //appData.fAngle += .01;
               //appData.dAltChange = 120.0 * sin(appData.fAngle);   // Test
               if ((appData.ulAltChangeTime - appData.ulLastAltChange) > 5000)   // 5 seconds intervals
               {
                  ulTemp = appData.ulAltChangeTime - appData.ulLastAltChange;    // Timestamps
                  dTemp = appData.dPressAlt - appData.dPrevPressAlt;             // Altitude change
                  ulTimeMult = 60000 / ulTemp;
                  appData.dAltChange = dTemp * ulTimeMult;                       // Multiply up to a minute
                  // Save timestamp and current pressure altitude
                  appData.ulLastAltChange = appData.ulAltChangeTime;
                  appData.dPrevPressAlt = appData.dPressAlt;

                  // Create scale value
                  dTemp = appData.dAltChange;
                  if (dTemp > 90.0)
                     dTemp = 90.0;
                  else if (dTemp < -90.0)
                     dTemp = -90.0;              
                  appData.iAltScale = (int)(sin(dTemp * 0.01745329) * 37.0);  
               }   
                                    
               //sprintf(appData.uiDebugbuffer2, "%.1f %d\0", dTemp, appData.iAltScale);
               //vLongDelay(1000);
            }   
         }
      break;
      
   }   
}

//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//

// Workaround for 64-bit atof linking
long double _dstrtod(const char * s, char ** endptr)
{
    return(strtod(s, endptr));
}

void vMisc_State(void)
{    
   uint8_t                       ucBytes[8]                 = {0};
   uint8_t                       ucTemp[50]                 = {0};
   uint8_t                     * pP                         = NULL;
    
  	switch (appData.uiMiscState)
   {
      case 0:                                // Start                           
         appData.uiMiscState = 10; 
      break;
      case 10:
         // Fetch temperature
         appData.uiTempState = 1;                         
         appData.uiMiscState = 20;         
      break;                         
      case 20:
         // Check for reset
         if (!BUTTON)
         {
            if (!appData.cEditFreq)
            {   
               appData.ulResetCounter++;
               if (appData.ulResetCounter > 60)  // Push length
               {    
                  // Send reset message to sensors
                  ucBytes[0] = appData.cCanSendScreen;
                  vCanTransmit(CID_RESET, ucBytes, 1);
                  // Reset this device
                  vResetDevice();
                  appData.ulResetCounter = 0;                
               }   
            }   
         }
         else
            appData.ulResetCounter = 0;                            
         appData.uiMiscState = 30;         
      break;            
      case 30:         
         // Set OLED contrast
         if (appData.ulMiscCount > 5)
         {              
            Auto_Contrast(); 
            appData.ulMiscCount = 0;
         }   
         else
            appData.ulMiscCount++;
         appData.uiMiscState = 40;  
      break;           
      case 40:
         // Check for incoming config values in SMS message, set config value
         // B: Set lower battery limit " !+B11.5 "
                           
         pP = strstr(appData.ucGsmMessage, "!+"); 
         if ((!appData.cSmsConfig) && pP)
         {   
            switch(*(pP + 2))
            {        
               // Set battery limit
               case 'B':
               case 'b':   
                  vBeep(4);
                  strncpy(ucTemp, pP + 3, 4);                                  
                  sConfig.fBatteryLimit = atof(ucTemp);                                                   
                  sprintf(appData.ucSmsSetting, "Batt lim %2.1fv%c", sConfig.fBatteryLimit, '\0'); 
                  appData.cCfgChanged = 1;
                  
                  /*
                  sprintf(ucTemp, "%s %f  %c", ucTemp, sConfig.fBatteryLimit, '\0');
                  by1351_DrawText(0, 30, (char *)ucTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
                  vLongDelay(2000);
                  */            
               break;   
               
               // Set approved phone number, format "+4712345678"
               case 'T':
               case 't':
                  memset(ucTemp, 0x00, 20);
                  strcpy(ucTemp, pP + 3);                  
                  strcpy(sConfig.bGsmPrimary, ucTemp);
                  sprintf(appData.ucSmsSetting, "Phone: %s%c", ucTemp, '\0'); 
                  appData.cCfgChanged = 1;
                  
               break;   
               //sConfig.bGsmPrimary
               
                  
                  
            }
            appData.cSmsConfig = 1;
         }
         appData.uiMiscState = 50;  
      break;           
      case 50:         
         // Test exception handler
         /*
         if (appData.ulLonglife > 10)
         {   
            pCrashme = (uint32_t*)0x80001001; 
            *pCrashme = 12;
         }
         */ 
                  
         // Set time on local CPU
         if ((!appData.ucTimeSet) && (appData.sGpsMode > 1) && (appData.cGpsTime[1]))
         {                        
            appData.ucTimeSet = 1;            
          //  sprintf(appData.uiDebugbuffer3, "%02x %02x %02x %02x %c", appData.cGpsTime[2], appData.cGpsTime[3], appData.cGpsTime[4], appData.cGpsTime[5], '\0'); 
            RTCTIMEbits.HR10 = appData.cGpsTime[0] - 0x30;
            RTCTIMEbits.HR01 = appData.cGpsTime[1] - 0x30;
            RTCTIMEbits.MIN10 = appData.cGpsTime[2] - 0x30;
            RTCTIMEbits.MIN01 = appData.cGpsTime[3] - 0x30;
            RTCTIMEbits.SEC10 = appData.cGpsTime[4] - 0x30;
            if (appData.cGpsTime[5] < 0x39)
               appData.cGpsTime[5]++;                    
            RTCTIMEbits.SEC01 = appData.cGpsTime[5] - 0x30;

            RTCDATEbits.YEAR10 = appData.cGpsDate[0] - 0x30;
            RTCDATEbits.YEAR01 = appData.cGpsDate[1] - 0x30;
            RTCDATEbits.MONTH10 = appData.cGpsDate[2] - 0x30;
            RTCDATEbits.MONTH01 = appData.cGpsDate[3] - 0x30;
            RTCDATEbits.DAY10 = appData.cGpsDate[4] - 0x30;
            RTCDATEbits.DAY01 = appData.cGpsDate[5] - 0x30;                       
         }         
         appData.uiMiscState = 10;         
      break;                       
   }   
}

//*********************************************************************************************************************
// Go to sleep
//
//*********************************************************************************************************************
//

void vInitSleep(void)
{
   
   /*
   int i = 0;

   // CFG-PMS 
   unsigned char ucData[] = {0x00, 0x02, 0x00, 0x15, 0x00, 0x05, 0x00, 0x00};    
   vDebug("Sleeping...\0", 0);   
   

   vClearRGBled(ulRGB, 0);
    
   vLongDelay(50);         
   LED_GREEN = 0;
   LED_BLUE = 0;
   LED_RED = 0;
   
   // Timers off
   TMR_Stop_Default(TMR_ID_2);
   
   // UART off
   
   // SPI off
   
   // ADC off
   AD1CON1bits.ADCEN = 0; 
   
   // Display sleep
   vCommand(0xAE);  
   OLED_PWR = 0;
   
   // OC off
   DRV_OC0_Disable();

   
   // CPU
   PLIB_OSC_PBOutputClockDisable(OSC_ID_0, OSC_PERIPHERAL_BUS_1);
   PLIB_OSC_PBOutputClockDisable(OSC_ID_0, OSC_PERIPHERAL_BUS_2);
   PLIB_OSC_PBOutputClockDisable(OSC_ID_0, OSC_PERIPHERAL_BUS_3);
   PLIB_OSC_PBOutputClockDisable(OSC_ID_0, OSC_PERIPHERAL_BUS_4);
   PLIB_OSC_PBOutputClockDisable(OSC_ID_0, OSC_PERIPHERAL_BUS_5);
   PLIB_OSC_PBOutputClockDisable(OSC_ID_0, OSC_PERIPHERAL_BUS_7);
   PLIB_OSC_PBOutputClockDisable(OSC_ID_0, OSC_PERIPHERAL_BUS_8);

   SYS_INT_StatusGetAndDisable();
 
   SYS_DEVCON_SystemUnlock ( );
   RCONbits.SLEEP = 0;     // Clear flag that indicates that device woke up from sleep
   OSCCONbits.SLPEN = 1;     // Tell device to sleep when "WAIT" command is issued  PLIB_OSC_OnWaitActionSet(OSC_ID_0, OSC_ON_WAIT_SLEEP);
   SYS_DEVCON_SystemLock ( );
   asm("WAIT");
   */
   while (1);
}
 
//*********************************************************************************************************************
// Called if sound buffer must be read
//  
//*********************************************************************************************************************
//

uint8_t ucReadSndBuff(void)
{            
   if (appData.ucReadSndBuff == 1)    
      appData.ulSndBytes = SYS_FS_FileRead(appData.fileHandle, (void *)appData.pbSoundbuff1, 4096);            
   else if (appData.ucReadSndBuff == 2)
      appData.ulSndBytes = SYS_FS_FileRead(appData.fileHandle, (void *)appData.pbSoundbuff2, 4096);          
   appData.ucReadSndBuff = 0;
   
   return RC_OK;
}

//*********************************************************************************************************************
// Timer 5 life callback, 1ms
//  
//*********************************************************************************************************************
//

void vTmr5_event(void)
{
   appData.ulLife++;
   if (appData.ulLife >= 1800000000)     // Rollover after 30 minutes
      appData.ulLife = 0;
   // Count seconds
   if (!(appData.ulLife % 1000))
      appData.ulLonglife++;
    
}   
 
//*********************************************************************************************************************
// Timers init
//  
//*********************************************************************************************************************
//

void vTimersInit(void)
{   
   TMR_Start_Default(TMR_ID_2);
   TMR_Start_Default(TMR_ID_1);
     
   // Fast pace timer init (Timer 5)
   // Timer clock is 100 MHz
   T5CON = 0x0000;
   TMR5 = 0;
   T5CONbits.TCKPS = 0b101;         // Prescaler /32
   
   IFS0bits.T5IF = 0;               // Clear interrupt flag
   IPC6bits.T5IP = 1;               // Interrupt priority
   IPC6bits.T5IS = 0;               // Sub-priority
   IEC0bits.T5IE = 1;               // Enable Timer
   PR5 = 0x0c35;                    // 3125   1ms   
   T5CONbits.ON = 1;                // Module enabled
  
}

//*********************************************************************************************************************
// Timers tick for PWM
//  
//*********************************************************************************************************************
//


// Not in use
void APP_OC_timer_tick(uintptr_t pT, uint32_t uiAlarmcount)
{
   /*
   
   if (appData.ulSptr > 4095)      // Switch buffer, flag for next read 
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
      
         
      //appData.ulSptr = 0;
      //DRV_TMR_Stop(appData.handleTmr); 
      //free(appData.pbBuffer);
   }
   
   if (appData.bSndReady)     // We have buffer
   {   
      
      
      if (appData.bSndReady == 1)
         OC1RS = appData.bVolume * appData.pbSoundbuff1[appData.ulSptr];    
      else
         OC1RS = appData.bVolume * appData.pbSoundbuff2[appData.ulSptr];    
       
       
      //sprintf(cTemp, " - %d\0", appData.ulSptr);
      //vDebug(cTemp, D_PURPLE); 
        
      appData.ulSptr++;
   }
   */
   
   /*
   if (appData.ulSptr > appData.iFilesize) 
   {   
      //appData.ulSptr = 0;
      DRV_TMR_Stop(appData.handleTmr); 
      free(appData.pbBuffer);
   }
   
   if (appData.bSndReady)     // We have buffer
   {   
      OC1RS = appData.bVolume * appData.pbBuffer[appData.ulSptr];    
      
      //sprintf(cTemp, " - %d\0", appData.ulSptr);
      //vDebug(cTemp, D_PURPLE); 
        
      appData.ulSptr++;
   } 
    */
}

//*********************************************************************************************************************
// OC1 PWM Init
//  
//*********************************************************************************************************************
//

void vPwmInit(void)
{
   DRV_OC0_Enable();  
   appData.bSndReady = 0;  // No file yet
   appData.ulSptr = 0;  // Buffer read pointer 
   appData.handleTmr = DRV_TMR_Open(DRV_TMR_INDEX_0, DRV_IO_INTENT_EXCLUSIVE); 
   if(DRV_HANDLE_INVALID == appData.handleTmr)
      strncpy(appData.sError, "No TMR1 handle!", 16);
   else
   {   
      // Enable the OCMP module and start the timer
      DRV_OC0_Start();
      DRV_TMR_AlarmRegister(appData.handleTmr, 35, true, 0, APP_OC_timer_tick);
      DRV_TMR_AlarmEnable(appData.handleTmr, true);            
      DRV_TMR_Start(appData.handleTmr);   
      PR2 = 9060;          // 11025 kHz sampling
      appData.bVolume = 20;
   }
}   
  
//*********************************************************************************************************************
// Initialize App
//
//*********************************************************************************************************************
//

void APP_Initialize(void)
{
   #define                       MY_SPI_ID                  SPI_ID_2
   #define                       SPI_BAUD_RATE              10000000
   
   unsigned char                 ucRet                      = 0;
    
   // Trace debugging
   //appData.cDebug = 1;
   
   // Turn off LEDs
   LED_RED     = LED_OFF;
   LED_GREEN   = LED_OFF;
   LED_BLUE    = LED_OFF;
  
   // Display module on
   OLED_PWR = 1;   
   WIFI_ENABLE = 1;        // Wifi module on
   OLED_RESET = 1;
     
   // Initialize timers
   vTimersInit();
       
   // Bareboard test
   while (0)
   {
      LED_GREEN = LED_ON;
      vLongDelay(1000);
      LED_RED = LED_ON;
      vLongDelay(1000);
      LED_BLUE = LED_ON;
      vLongDelay(1000);
      LED_RED     = LED_OFF;
      LED_GREEN   = LED_OFF;
      LED_BLUE    = LED_OFF;
      vLongDelay(1000);
   }
      
   // Initialize OC1 PWM output
   vPwmInit();
   
   // Sense test mode
   if (!BUTTON)      // Hold button while starting
   {
      appData.uiTest = 1;  
   }   
  
   vSetRGBled(appData.ulRGB, 0, 0x00, 0x01, 0x00);
   vSetRGBled(appData.ulRGB, 1, 0x00, 0x00, 0x00);
   vSetRGBled(appData.ulRGB, 2, 0x00, 0x00, 0x00);
   
   // Place the App in its initial state.
   appData.ulLife = 0;
   appData.deviceIsConnected = false;
   appData.uiAppState = 0;                      
   appData.uiSdState = 0; 
   appData.uiUsbState = APP_STATE_BUS_ENABLE; 
   appData.uiWifiState = 0; 
   appData.uiCanState = 0; 
   appData.uiDmenu = 0;
   appData.cCount = 0;                    // SD mount counter
   appData.ucSdReady = 0;                 // 1 when volume mounted
   appData.uiMiscState = 0;
   appData.uiNvramState = 0;
   appData.ulMiscCount = 0;
   appData.ucCalDone = 0;
   appData.usStatus = 0b0000011111111111; // Status all errors until confirmed ok
   appData.cSmsConfig = 0;
   // Initialize display           
   appData.uiDisplayPrev = 0;
   appData.uiDisplayScreen = 0;
   appData.uiDisplayState = 0;
   memset(appData.ucSmsSetting, 0x00, 20);
   
   // APRS scale
   appData.fScale = 10;                   // Initial scale
   appData.dAprsStations = 4;             // Number of stations on screen
   appData.uiAprsAge = 180;               // Recently seen
 
   // Cause initial draw of BT screen
   appData.cBchanged = 12; // Any value
   
   // Initialize RNG
   RNGCONbits.CONT = 1;
   RNGCONbits.PLEN = 0xd4;   
   RNGCONbits.PRNGEN = 1;
   RNGCONbits.TRNGEN = 1;
        
   vSetRGBled(appData.ulRGB, 0, 0x00, 0x00, 0x01);   
   
   // Initialize rotary switch
   vSwitch_Initialize();

   vSetRGBled(appData.ulRGB, 0, 0x00, 0x01, 0x01);   
   
   // Initialize light sensor
   vAD_Initialize(); 

   vSetRGBled(appData.ulRGB, 0, 0x01, 0x01, 0x01);   
   LED_GREEN = LED_ON;
   
   // Initialize WiFi
   vWifi_Initialize(); 

   vSetRGBled(appData.ulRGB, 0, 0x00, 0x00, 0x04);   
   //LED_RED = LED_ON;
   
   // Initialize alarms
   vAlarmsInit();
   
   // Initialize CAN
   vCan_Initialize();
     
   // Init menu system
   appData.uiDispFunc = FUNC_SCREEN;      // Start navigating main screens
   
   // Initialize Real-Time clock
   RTCCONbits.ON = 1;  
   
   LED_BLUE = LED_ON;
   
   // Init OLED
   vSetRGBled(appData.ulRGB, 0, 0x01, 0x00, 0x00);   
   by1351_Initialize();  
   vSetRGBled(appData.ulRGB, 0, 0x01, 0x00, 0x01);   
   by1351_SetContrast(6);  
   
   LED_RED     = LED_OFF;
   LED_GREEN   = LED_OFF;
   LED_BLUE    = LED_OFF;
     
   // Initialize board temperature sensor
   ucRet = ucInitOwTemp();
   if (ucRet)        
      strncpy(appData.sError, "No gauge Temp!", 16);    
   
   // Clear panel LED 0
   vSetRGBled(appData.ulRGB, 0, 0x00, 0x00, 0x00);   
   
   //vBeep(7);

}

//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//
 
void vSndRd(void)
{
   if (appData.ucReadSndBuff)
      ucReadSndBuff();   
}   

//*********************************************************************************************************************
// App tasks called from Sys tasks
//
//*********************************************************************************************************************
//
  
void APP_Tasks ( void )
{
   vSndRd();
   
   //static int iTesta = 0;
   vTrace("A");   
   // NVram configuration data
   vNvram_State();
   
   vSndRd();
   
   vTrace("B");   
   // CAN
   vCan_State();
   
   vSndRd();
   
   vTrace("C");   
   // USB 
   //USB_State();
   
   vTrace("D");   
   // SD card state
   SD_State();
   
   vSndRd();
     
   vTrace("E");   
   // Temperature sensor state
   Temperature_State(); 
    
   vSndRd();
   
   vTrace("F");   
   // Display state
   vDisplay_State();

   vSndRd();   
   
   vTrace("G");   
   // WiFi
   vWifi_State();
            
   vSndRd();
  
   vTrace("H");   
   // Alarms state
   vAlarms_State();
  
   vSndRd();
   
   vTrace("I");   
   // Miscellaneous state
   vMisc_State();
   
   vSndRd(); 
   
   // Check for pressure variometer
   vPress_State();
        
   // Dump debug if any
   if (strlen((char *)appData.uiDebugbuffer0))
      by1351_DrawText(0, 1 * 9, (char *)appData.uiDebugbuffer0, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   if (strlen((char *)appData.uiDebugbuffer1))
      by1351_DrawText(0, 2 * 9, (char *)appData.uiDebugbuffer1, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   if (strlen((char *)appData.uiDebugbuffer2))
      by1351_DrawText(0, 3 * 9, (char *)appData.uiDebugbuffer2, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   if (strlen((char *)appData.uiDebugbuffer3))
      by1351_DrawText(0, 4 * 9, (char *)appData.uiDebugbuffer3, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   if (strlen((char *)appData.uiDebugbuffer4))
      by1351_DrawText(0, 5 * 9, (char *)appData.uiDebugbuffer4, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   if (strlen((char *)appData.uiDebugbuffer5))
      by1351_DrawText(0, 6 * 9, (char *)appData.uiDebugbuffer5, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   if (strlen((char *)appData.uiDebugbuffer6))
      by1351_DrawText(0, 7 * 9, (char *)appData.uiDebugbuffer6, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   if (strlen((char *)appData.uiDebugbuffer7))
      by1351_DrawText(0, 8 * 9, (char *)appData.uiDebugbuffer7, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);   
    
   
  
   
   
//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//
   // Test
  // iTesta++;
  // if (iTesta == 500)
  //    appData.cPrevLatdeg = 34;
   
   // Reset watchdog
   WDTCONbits.WDTCLRKEY = 0x5743;  
      
   // Calculate random number   
   appData.llRandom2 = RNGNUMGEN1 * RNGNUMGEN2;
      
}


//*********************************************************************************************************************
// 
 





