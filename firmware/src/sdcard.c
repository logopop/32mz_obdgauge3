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

//*********************************************************************************************************************
// Globals
// 
//*********************************************************************************************************************
//

extern void                      vBlink(char cB);
 
extern unsigned long             ulRGB[LED_SERIAL_NUM]; 
  
extern volatile uint8_t          cPwmReady;

extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;


//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//


//*********************************************************************************************************************
//
//
//*********************************************************************************************************************
//

unsigned char rcMountCard(void)
{
   unsigned char                 rcRc                       = 0;

   rcRc = SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive1", FAT, 0, NULL);      

   return rcRc;
}



//*********************************************************************************************************************
// Read file, perform action
// Type  1: BMP image to display
//       2: WAV sound to PWM out
// 
//*********************************************************************************************************************
//

int SD_State()
{
   /*
   SYS_FS_FSTAT                  sStat;
   SYS_FS_RESULT                 sResult;
   char                          ucBuff[128]                = {0}; 
   */
   unsigned char                 ucRc                       = 0;
    
   // Get started
   if (!appData.uiSdState)
      appData.uiSdState = 10;
      
   switch(appData.uiSdState)
   {
      case 10:                
         ucRc = rcMountCard();         
         if (ucRc == SYS_FS_RES_SUCCESS)
         {  
            appData.ucSdReady = 1;            
            appData.uiSdState = 100;       // Stay idle
            appData.usStatus &= ~(STAT_SD);                                             
         }   
         else
         {   
            appData.cCount++;
            if (appData.cCount > 1000)
            {
               appData.cCount = 0;
               strncpy(appData.sError, "No SD card!    ", 16);
               by1351_DrawText(4, 60, appData.sError, 0x1f, 0x3f, 0x10, 0x04, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);            
            }   
         }   
      break;
   /*
      case 20:    
         appData.cSdBusy = 1;
         
         // Stat
         memset(&sStat, 0x00, sizeof(sStat));
         sprintf((char *)ucBuff, (char *)"/mnt/myDrive1/%s%c", appData.ucFile, '\0');
         
         sResult = SYS_FS_FileStat(ucBuff, (SYS_FS_FSTAT *)&sStat);
         if (sResult == SYS_FS_RES_FAILURE)        
         {            
            strncpy(appData.sError, "SD Stat err!", 16);            
            appData.uiSdState = 100; 
         }   
         else
         {   
            appData.iFilesize = sStat.fsize;
            // Open
            appData.fileHandle = SYS_FS_FileOpen((char *)appData.ucFile, (SYS_FS_FILE_OPEN_READ));
            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
               strncpy(appData.sError, "SD open fail!", 16);
               appData.uiSdState = 100;
            }   
            else
               appData.uiSdState = 30; 
         }   
      break;   
      
      case 30:                                  
         //sprintf(appData.uiDebugbuffer4, "Size: %d\0", appData.iFilesize);          
         
         appData.pbBuffer = malloc(appData.iFilesize + 100);
         if (!appData.pbBuffer)
         {   
            strncpy(appData.sError, "Malloc fail!", 16);            
            SYS_FS_FileClose(appData.fileHandle); 
            appData.uiSdState = 200;
         }   
         else
         {   
            if (appData.ucSdType == 1)    // Image  
               ucRc = ulGetBmpImage(appData.fileHandle, appData.pbBuffer);
            else if (appData.ucSdType == 2)    // PWM sound  
            {
               // Set filepointer to beginning
               if (SYS_FS_FileSeek(appData.fileHandle, 0, SYS_FS_SEEK_SET) == -1) 
               {                  
                  strncpy(appData.sError, "F seek fail!", 16);
                  appData.uiSdState = 100;
               }   
               // Read file
               if (SYS_FS_FileRead(appData.fileHandle, (void *)appData.pbBuffer, appData.iFilesize) == -1)
               {   
                  strncpy(appData.sError, "F read fail!", 16);
                  appData.uiSdState = 100;
               }   
               else
               {   
                  appData.bSndReady = 1;  // We have a buffer            
                  //strcpy((char *)appData.uiDebugbuffer1, "Buff OK\0");
               }   
            }            
            SYS_FS_FileClose(appData.fileHandle);            
            appData.uiSdState = 40;                      
         }   
      break;   

      case 40:                      
         if (appData.ucSdType == 1)          // Image
         {                   
            by1351_PrepareFrame(0, 127);
            by1351_WriteFrame(appData.pbBuffer, 32768);
            free(appData.pbBuffer);
            appData.uiSdState = 100;     
         }   
         else if (appData.ucSdType == 2)     // Sound
         {            
            appData.cPwmDone = 0;
            appData.uiSdState = 50;   
         }
         
      break;
      
      case 50:
         // Start async sound play   
         // We have buffer, initialize OC timer
          
         appData.ulSptr = 0;
         T4CON   = 0x0;                // Disable timer 4 when setting it up
         TMR4    = 0;                  // Set timer 4 counter to 0
         IEC0bits.T4IE = 0;            // Disable Timer 4 Interrupt

         // Set up the period. Period = PBCLK3 frequency, which is SYS_FREQ / 2, divided by the frequency we want.
         PR4 = 200000000 / 4 / 11025;
         
         CFGCONbits.OCACLK = 1;
         
         // Set up the pre-scaler
         T4CONbits.TCKPS = 0;          // No pre-scale

         IFS0bits.T4IF = 0;            // Clear interrupt flag for timer 4
         IPC4bits.T4IP = 5;            // Interrupt priority
         IPC4bits.T4IS = 1;            // Sub-priority 1
         IEC0bits.T4IE = 1;            // Enable Timer 4 Interrupt

         OC1CON = 0;                   // Turn off Output Compare module 1
         OC1CONbits.OCTSEL = 0;        // Interrupt source for this module is Timerx (Which is Timer 4 for OC1)
         OC1CONbits.OCM = 0b110;       // 110 Output Compare Mode (OCM) is 1, which is PWM mode
         OC1RS = 0;                    // Initially keep the signal low for the entire duration
         OC1CONbits.ON = 1;            // Turn on Output Compare module 1 (OC1)    
                  
         // Turn on timer 4
         T4CONbits.TON   = 1;
 
         appData.uiSdState = 60;         
      break;
      
      case 60:         
         // Wait until finished
         if (appData.cPwmDone)
         {
            free(appData.pbBuffer);                            
            T4CONbits.TON   = 0;            
            appData.uiSdState = 100; 
            OC1RS = 0;
             
            OC1CONbits.ON = 0;     
         }
      break;
    */           
    
    
      case 100:
          // Normal end after mount here           
          appData.cSdBusy = 0;
          
      break;   

      case 200:         
         //strcpy((char *)appData.uiDebugbuffer1, "P Error\0");
         appData.cSdBusy = 0;
      break;    
      
   }         
    
   return 0;
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

uint8_t vPicturefromSD(uint8_t * ucFile)
{
   unsigned char                 ucTemp[HALF_BUF]           = {0};

   if ((!appData.cSdBusy) && (appData.ucSdReady))
   {
      // Test buffered image
      appData.cSdBusy = 1;
      sprintf((char *)ucTemp, "pic/%s%c", ucFile, '\0');
      ucDisplayImage(ucTemp);     
      appData.cSdBusy = 0;      
   }
   else
      return 1;
   return RC_OK;
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//


//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

uint8_t vMoviefromSD(uint8_t * ucFile, unsigned short usFrames, unsigned short usDelay)
{
   unsigned char                 ucTemp[HALF_BUF]           = {0};
   short                         sCount                     = 0;   
   
   if ((!appData.cSdBusy) && (appData.ucSdReady))
   {
      // Test buffered image
      
      while (sCount < usFrames)
      {   
         appData.cSdBusy = 1;
         sprintf((char *)ucTemp, "mov/%s%06d.bmp%c", ucFile, sCount, '\0');
         //strcpy(appData.uiDebugbuffer3, ucTemp);
         ucDisplayImage(ucTemp);     
         appData.cSdBusy = 0;      
         sCount++;
         vLongDelay(usDelay);
         if (appData.ucAbortShow)
         {   
            appData.uiDisplayScreen = SCREEN_SPEED;
            sCount = usFrames;
            break;
         }   
      }   
   }
   else
      return 1;
   return RC_OK;
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

uint8_t vSoundfromSD(uint8_t * ucFile)
{
   unsigned char                 ucTemp[HALF_BUF]           = {0};

   if ((!appData.cSdBusy) && (appData.ucSdReady))
   { 
      appData.cSdBusy = 1; 
      appData.cPwmDone = 0;
      appData.ulSptr = 0;
      sprintf((char *)ucTemp, "snd/%s%c", ucFile, '\0');
      ucPlaySound(ucTemp);
      
   }
   else
      return 1;
   return RC_OK;
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//


