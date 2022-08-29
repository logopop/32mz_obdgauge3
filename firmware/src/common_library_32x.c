
//*********************************************************************************************************************
// Common library
//
//*********************************************************************************************************************
//
//     
//*********************************************************************************************************************
//
  

#include "app.h"
   
extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;


//*********************************************************************************************************************
// Routine tracing
//
//*********************************************************************************************************************
//

void vTrace(char * cBuff)
{
   char                          cTemp[10]                   = {0};
   
   if (appData.cDebug)
   {
      sprintf(cTemp, " %s %c", cBuff, '\0');
      by1351_DrawText(60, 118, (char *)cTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   }
}

//*********************************************************************************************************************
// Convert two characters to one-byte numerical
//
//*********************************************************************************************************************
//

char cGetTupple(char * cBuff, int iOffset)
{
   int                           iVal                       = 0;
   char                          cTemp[3]                   = {0};

   strncpy(cTemp, cBuff + iOffset, 2);
   if (cTemp[0] == '0')
   {
      cTemp[0] = 0x20;
      if (cTemp[1] == '0')
         return 0;
   }
   sscanf(cTemp, "%d", &iVal);
   return (char)iVal;
}

//*********************************************************************************************************************
// Byte checksum routine
// 
//*********************************************************************************************************************
//

unsigned char ucChecksum(char * psString)
{
   unsigned char                 ucCheck                    = 0;    
   int                           i                          = 0;
   
   for (i = 0; i < strlen(psString); i++)
      ucCheck ^= psString[i];   
   return ucCheck;
}      


//*********************************************************************************************************************
// Parse a database record
// 
//*********************************************************************************************************************
//

void vParseRecord(uint8_t * ulBuff, uint8_t ucField, uint8_t * ucDest, uint8_t ucSize) 
{
   uint8_t                       ucTemp[50]                 = {0};
   uint8_t                       ucOffset                   = 0;
   uint8_t                       ucFld                      = 0;
   uint8_t                       ucEnd                      = 0;
   uint8_t                       ucBusy                     = 0;

   ucBusy = 1;
   do
   {
      if (ulBuff[ucOffset] == ';')
         ucFld++;        
      if (ucFld >= ucField)
      {         
         ucOffset++;                  
         for (ucEnd = 0; ucEnd < ucSize; ucEnd++)
         {
            if (ulBuff[ucOffset + ucEnd] == ';')
            {      
              
               break;
            }   
         }   
            
         //strncpy(ucTemp, ulBuff + ucOffset, 15);
         //by1351_DrawText(0, 65, ucTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);  
         //vLongDelay(5000);                    
         strncpy(ucDest, ulBuff + ucOffset, ucEnd);
         ucBusy = 0;                             
      }      
      ucOffset++;
   } while ((ucOffset < 255) && (ucBusy));
}

//*********************************************************************************************************************
// ucHamLookup
//    Fetch data about callsign from 'db/amator.csv' on the SD card.  
//    Return in struct
//*********************************************************************************************************************
// Kallesignal;Organisasjon;Fornavn;Etternavn;Adresse;Adresse_2;Postnr;Poststed;Land;Type;Gyldigdato;Utløpsdato;Oppdatertdato;Kommentar
//
//

uint8_t ucHamLookup(uint8_t * ucCallsign)
{
#define                          BUF_SIZE                   5000
   
   SYS_FS_FSTAT                  sStat;
   SYS_FS_RESULT                 sResult;
   uint8_t                       rcRc                       = RC_OK;
   uint8_t                       ucPath[HALF_BUF]           = {0};
   uint8_t                       ucPath2[HALF_BUF]          = {0};
   uint8_t                       ucCall[10]                 = {0};
   uint8_t                       ucDone                     = 0;
   unsigned long                 ulPtr                      = 0;
   unsigned long                 ulOffset                   = 0;
   uint8_t                     * ucP                        = NULL;  
   long                          lRead                      = 0;
   uint8_t                       ucTemp[20]                 = {0};
   uint8_t                       ucTemp2[30]                = {0};
   uint8_t                       ucRepeater                 = 0;     // True when in repeater database
   
   ucP = strchr(ucCallsign, '-');
   if (ucP)
      *ucP = 0x00;
   
   // Clear result struct
   memset(appData.sHamRecord.ucName, 0x00, 20);
   memset(appData.sHamRecord.ucSurname, 0x00, 20);
   memset(appData.sHamRecord.ucAddress, 0x00, 20);
   memset(appData.sHamRecord.ucAddress2, 0x00, 20);
   memset(appData.sHamRecord.ucZip, 0x00, 6);
   memset(appData.sHamRecord.ucCity, 0x00, 20);
   memset(appData.sHamRecord.ucCountry, 0x00, 10);

   sprintf(ucCall, "%s%c%c", ucCallsign, ';', '\0');   
   if ((!appData.cSdBusy) && (appData.ucSdReady))
   {
      appData.cSdBusy = 1;
      sprintf((char *)ucPath, "db/%s%c", CALLSIGN_DB, '\0');            
      
      // Malloc source buffer
      appData.pbBuffer = malloc(BUF_SIZE);
      if (appData.pbBuffer)
      {                          
         // Stat file
         memset(&sStat, 0x00, sizeof(sStat));
         sprintf((char *)ucPath2, (char *)"/mnt/myDrive1/%s%c", ucPath, '\0');
         sResult = SYS_FS_FileStat(ucPath2, (SYS_FS_FSTAT *)&sStat);
         if (sResult != SYS_FS_RES_FAILURE)        
         {             
            // Get size and open file
            appData.iFilesize = sStat.fsize;
            appData.fileHandle = SYS_FS_FileOpen((char *)ucPath, (SYS_FS_FILE_OPEN_READ));
            if(appData.fileHandle != SYS_FS_HANDLE_INVALID)
            {       
               ucDone = 0;
               do
               {                                   
                  lRead = SYS_FS_FileRead(appData.fileHandle, (void *)appData.pbBuffer, BUF_SIZE);
                  if (lRead != -1)
                  {                                            
                     ulPtr += (lRead - 200);
                     if (lRead < MAX_BUF)
                        ucDone = 1;
                     
                     for (ulOffset = 0; ulOffset < lRead; ulOffset++)
                     {               
                        if (appData.cChangedScreen)
                           ucDone = 1;
                        if (!memcmp(appData.pbBuffer + ulOffset, ucCall, strlen(ucCall)))
                        {                                           
                           // by1351_DrawText(0, 65, appData.pbBuffer + ulOffset, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);   
                           // vLongDelay(2000);  
                           if (ucRepeater)
                           {
                              vParseRecord(appData.pbBuffer + ulOffset, 1, appData.sHamRecord.ucSurname, 20);
                              vParseRecord(appData.pbBuffer + ulOffset, 5, appData.sHamRecord.ucName, 20);
                              
                              vParseRecord(appData.pbBuffer + ulOffset, 2, ucTemp, 20);
                              ucTemp[7] = 0;
                              vParseRecord(appData.pbBuffer + ulOffset, 3, ucTemp2, 20);
                              ucTemp2[7] = 0;
                              sprintf(appData.sHamRecord.ucAddress, "%s %s", ucTemp, ucTemp2); 

                              vParseRecord(appData.pbBuffer + ulOffset, 6, appData.sHamRecord.ucCity, 20);
                              *appData.sHamRecord.ucCountry = 0;
                           }
                           else
                           {   
                              vParseRecord(appData.pbBuffer + ulOffset, 2, appData.sHamRecord.ucName, 20);
                              vParseRecord(appData.pbBuffer + ulOffset, 3, appData.sHamRecord.ucSurname, 20);
                              vParseRecord(appData.pbBuffer + ulOffset, 4, appData.sHamRecord.ucAddress, 20);
                              vParseRecord(appData.pbBuffer + ulOffset, 5, appData.sHamRecord.ucAddress2, 20);
                              vParseRecord(appData.pbBuffer + ulOffset, 6, appData.sHamRecord.ucZip, 6);
                              vParseRecord(appData.pbBuffer + ulOffset, 7, appData.sHamRecord.ucCity, 20);
                              vParseRecord(appData.pbBuffer + ulOffset, 8, appData.sHamRecord.ucCountry, 10);
                           }      
                           ucDone = 1;                                                     
                        } 
                        else if (!ucRepeater)
                        {
                           if (!memcmp(appData.pbBuffer + ulOffset, "---Rep", 6))
                              ucRepeater = 1;                           
                        }   
                     }  
                     SYS_FS_FileSeek(appData.fileHandle, -200, SYS_FS_SEEK_CUR); // Step back a bit to cover splits
                  }
                  else
                  {
                     vBeep(4);
                     ucDone = 1;
                  }
               }      
               while ((ulPtr <= appData.iFilesize) && (!ucDone));                                 
               SYS_FS_FileClose(appData.fileHandle);                  
            }  
         } 
         free(appData.pbBuffer);
      }     
      appData.cSdBusy = 0;      

      if ((strlen(appData.sHamRecord.ucName) < 2) && (strlen(appData.sHamRecord.ucCountry) < 2))
         strcpy(appData.sHamRecord.ucAddress, "No record!");
      else
      {
         vSetIndicator(2, 0x00, 0x02, 0x00);
         vLongDelay(100);
         vSetIndicator(2, 0x00, 0x00, 0x00);                     
      }   
      
      //strcpy(appData.sHamRecord.ucSurname, "Testing123");  
   }
   else
      rcRc = RC_ERROR;
      
   return rcRc;
}

//*********************************************************************************************************************
// ucPlaySound
// Start buffered play from SD
// Buffers are freed when timer callback detects end of file
//*********************************************************************************************************************
//

uint8_t ucPlaySound(uint8_t * ucPath)
{
   SYS_FS_FSTAT                  sStat;
   SYS_FS_RESULT                 sResult;
   unsigned char                 ucRc                       = 0;
   char                          ucBuff[128]                = {0}; 
   
   appData.ucReadSndBuff = 0;
   
   // Malloc sound buffers
   appData.pbSoundbuff1 = malloc(4096);
   if (appData.pbSoundbuff1)
   {      
      appData.pbSoundbuff2 = malloc(4096);
      if (appData.pbSoundbuff2)
      {   
         // Stat file
         memset(&sStat, 0x00, sizeof(sStat));
         sprintf((char *)ucBuff, (char *)"/mnt/myDrive1/%s%c", ucPath, '\0');
         
         sResult = SYS_FS_FileStat(ucBuff, (SYS_FS_FSTAT *)&sStat);
         if (sResult != SYS_FS_RES_FAILURE)        
         {            
            // Get size and open file
            appData.iFilesize = sStat.fsize;
            appData.fileHandle = SYS_FS_FileOpen((char *)ucPath, (SYS_FS_FILE_OPEN_READ));
            if((appData.fileHandle != SYS_FS_HANDLE_INVALID) && (appData.iFilesize > 8192))  // Smallest file is 8k
            {
               if (SYS_FS_FileSeek(appData.fileHandle, 64, SYS_FS_SEEK_SET) != -1)           // Skipping header
               {                                        
                  if (SYS_FS_FileRead(appData.fileHandle, (void *)appData.pbSoundbuff1, 4096) != -1)
                  {                                            
                     // We have the first buffer
                     appData.ulSndBytes = 4096;    // Used for stopping   
                     appData.ucSoundFirst = 1;     // We are starting

                     appData.ulSptr = 0;
                     T4CON   = 0x0;                // Disable timer 4 when setting it up
                     TMR4    = 0;                  // Set timer 4 counter to 0
                     IEC0bits.T4IE = 0;            // Disable Timer 4 Interrupt

                     // Set up the period. Period = PBCLK3 frequency, which is SYS_FREQ / 2, divided by the frequency we want.
                     PR4 = 200000000 / 2 / 24000;

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

                     // Turn on timer 4, start playback
                     T4CONbits.TON   = 1;
                     appData.bSndReady = 1;        // We have buffers  
                  }
                  else
                     ucRc = 5;                                                      
               }
               else
                  ucRc = 4;                                                  
            }
            else
               ucRc = 3;                                       
         }
         else
            ucRc = 2;                     
      }
      else
         ucRc = 1;      
   }
   else
      ucRc = 1;   

   return ucRc;;
}

//*********************************************************************************************************************
// BMP header structure
// 
//*********************************************************************************************************************
//

#pragma pack(push, 1)
typedef struct _BMHDR
{
   // File Header
   short                         sType;                                             // "BM"
   unsigned long                 ulFsize;                                           // File size
   unsigned long                 ulReserve;
   unsigned long                 ulDataOffset;                                      // Data offset
   // Info Header
   unsigned long                 ulHdrsize;
   unsigned long                 ulHsize;
   unsigned long                 ulVsize;
   unsigned short                usPlanes;
   unsigned short                usBitcount;
   unsigned long                 ulCompression;
   unsigned long                 ulSizeImage;
   unsigned long                 ulXPelsPerMeter;
   unsigned long                 ulYPelsPerMeter;
   unsigned long                 ulClrUsed;
   unsigned long                 ulClrImportant;
} BMHDR;
#pragma pack(pop)
BMHDR sHeader;

BYTE bBuff[54] = {0};

//*********************************************************************************************************************
// ucDisplayVid  fromSD
// Display images from vid image sequence
// 
//*********************************************************************************************************************
//

uint8_t vVidfromSD(uint8_t * ucPath)
{
   SYS_FS_FSTAT                  sStat;
   SYS_FS_RESULT                 sResult;
   char                          ucBuff[128]                = {0}; 
   unsigned char                 ucRc                       = 0;
   int                           i                          = 0;     
   int                           j                          = 0;     
   int                           iSrc                       = 0;     
   int                           iDst                       = 0;     
   int                           iLine                      = 0;     
   unsigned char                 ucR                        = 0; 
   unsigned char                 ucG                        = 0; 
   unsigned char                 ucB                        = 0; 
   unsigned char                 ucH                        = 0; 
   unsigned char                 ucL                        = 0; 
   unsigned char                 ucTemp                     = 0; 
   unsigned long                 ulFrame                    = 0;
   uint8_t                     * ucDstBuff                  = NULL;
   HDR_STRUCT                    sFhdr;
   float                         fPace                      = 0;
   unsigned long                 ulTime                     = 0;
   
   uint8_t                       ucTest = 0; 
    
   if ((!appData.cSdBusy) && (appData.ucSdReady))
   {
      // Malloc source buffer
      appData.pbBuffer = malloc(6144);
      if (appData.pbBuffer)
      {      
         ucDstBuff = malloc(4096);
         if (ucDstBuff)
         {   
            // Stat file
            memset(&sStat, 0x00, sizeof(sStat));
            sprintf((char *)ucBuff, (char *)"/mnt/myDrive1/%s%c", ucPath, '\0');

            sResult = SYS_FS_FileStat(ucBuff, (SYS_FS_FSTAT *)&sStat);
            if (sResult != SYS_FS_RES_FAILURE)        
            {            
               // Get size and open file
               appData.iFilesize = sStat.fsize;
               appData.fileHandle = SYS_FS_FileOpen((char *)ucPath, (SYS_FS_FILE_OPEN_READ));
               if(appData.fileHandle != SYS_FS_HANDLE_INVALID)
               {
                  // Get header info
                  if (SYS_FS_FileRead(appData.fileHandle, (void *)&sFhdr, sizeof(HDR_STRUCT)) != -1) // 24 bytes highball format
                  {
                     // Parse each frame and display
                   
                     //sprintf(ucBuff, "P: %lu %lu\0", sFhdr.ulFrames, sFhdr.ulFramesize);                  
                     //by1351_DrawText(0, 54, ucBuff, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD); 
                     
                     for (ulFrame = 0; ulFrame < sFhdr.ulFrames; ulFrame++)
                     {         
                        /*
                        // FPS
                        fPace = (1000.0 / (appData.ulLife - ulTime));
                        ulTime = appData.ulLife;                        
                        sprintf(ucBuff, " %.2f fps \0", fPace);                  
                        by1351_DrawText(0, 14, ucBuff, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD); 
                        
                        */
                        if (appData.ucAbortShow)
                        {   
                           ulFrame = 999999;
                           break;
                        }   
                        // Read and populate BMP header
                        if (SYS_FS_FileRead(appData.fileHandle, (void *)&sHeader, sizeof(BMHDR)) != -1) // 36 hex long
                        {
                           if(sHeader.usBitcount == 24)        // Check image format
                           {                                                                           
                              // Read - convert - write loop
                              for (j = 0; j < 8; j++)           
                              {
                                 if (SYS_FS_FileRead(appData.fileHandle, (void *)appData.pbBuffer, 6144) != -1)
                                 {  
                                    for (iLine = 15  /*sHeader.ulVsize - 1*/ ; iLine >= 0; iLine--)
                                    {                                 
                                       iDst = iLine * sHeader.ulHsize * 2;
                                       for (i = 0; i < sHeader.ulHsize; i++)
                                       {   
                                          ucR = *(appData.pbBuffer + iSrc);
                                          ucG = *(appData.pbBuffer + iSrc + 1);
                                          ucB = *(appData.pbBuffer + iSrc + 2);

                                          // Reduce color depth by throwing away LSBs. 
                                          // R4 R3 R2 R1 R0 G5 G4 G3 G2 G1 G0 B4 B3 B2 B1 B0
                                          ucTemp = ucG;
                                          ucH = (ucR & 0xF8);        
                                          ucH |= (ucTemp >> 5); 
                                          ucL = (ucB >> 3);
                                          ucTemp = (ucG << 3);         
                                          ucTemp &= 0xE0;
                                          ucL |= ucTemp;
                                                                                                                             
                                          *(ucDstBuff + iDst + (i*2)) = ucH;
                                          *(ucDstBuff + iDst + (i*2) + 1) = ucL;  
                                          
                                          iSrc += 3;
                                       }
                                    }                                               
                                 }
                                 else
                                    ucRc = 7;        

                                 // Write image segment
                                 by1351_PrepareFrame(16 * (7 - j), (16 * (8 - j)) - 1);      // Ready display for direct writes
                                 by1351_WriteFrame(ucDstBuff, 4096);  
                                 iSrc = 0;
                              }   
                           }
                           else
                              ucRc = 6;                                               
                        }   
                        else
                           ucRc = 5;                             
                     }
                  }       
                  else
                     ucRc = 3;        
                  SYS_FS_FileClose(appData.fileHandle);    
               }   
               else
                  ucRc = 2;   
               free(ucDstBuff);
            }   
            else
               ucRc = 1;                  
            free(appData.pbBuffer);
         }
         else
            ucRc = 1;                  
      }              
      return ucRc;           
   }
}   

//*********************************************************************************************************************
// ucDisplayImage
// Buffered read BMP from SD and write to display
// 
//*********************************************************************************************************************
//

uint8_t ucDisplayImage(uint8_t * ucPath)
{
   SYS_FS_FSTAT                  sStat;
   SYS_FS_RESULT                 sResult;
   char                          ucBuff[128]                = {0}; 
   unsigned char                 ucRc                       = 0;
   int                           i                          = 0;     
   int                           j                          = 0;     
   int                           iSrc                       = 0;     
   int                           iDst                       = 0;     
   int                           iLine                      = 0;     
   unsigned char                 ucR                        = 0; 
   unsigned char                 ucG                        = 0; 
   unsigned char                 ucB                        = 0; 
   unsigned char                 ucH                        = 0; 
   unsigned char                 ucL                        = 0; 
   unsigned char                 ucTemp                     = 0; 
   uint8_t                     * ucDstBuff                  = NULL;
   
   uint8_t                       ucTest = 0; 
   
   
   // Malloc source buffer
   appData.pbBuffer = malloc(6144);
   if (appData.pbBuffer)
   {      
      ucDstBuff = malloc(4096);
      if (ucDstBuff)
      {   
         // Stat file
         memset(&sStat, 0x00, sizeof(sStat));
         sprintf((char *)ucBuff, (char *)"/mnt/myDrive1/%s%c", ucPath, '\0');

         sResult = SYS_FS_FileStat(ucBuff, (SYS_FS_FSTAT *)&sStat);
         if (sResult != SYS_FS_RES_FAILURE)        
         {            
            // Get size and open file
            appData.iFilesize = sStat.fsize;
            appData.fileHandle = SYS_FS_FileOpen((char *)ucPath, (SYS_FS_FILE_OPEN_READ));
            if(appData.fileHandle != SYS_FS_HANDLE_INVALID)
            {
               // Read and populate BMP header
               if (SYS_FS_FileSeek(appData.fileHandle, 0, SYS_FS_SEEK_SET) != -1) 
               {               
                  // Read header
                  if (SYS_FS_FileRead(appData.fileHandle, (void *)&sHeader, sizeof(BMHDR)) != -1) // 36 hex long
                  {
                     if(sHeader.usBitcount == 24)        // Check image format
                     {                                              
                        //ulImagesize = sHeader.ulHsize * sHeader.ulVsize * 2;          // Supports 16 or 24 bit pixel depth, no pallette
                        //iLine = sHeader.ulVsize - 1;
                        
                        // Read - convert - write loop
                        for (j = 0; j < 8; j++)           
                        {
                           if (SYS_FS_FileRead(appData.fileHandle, (void *)appData.pbBuffer, 6144) != -1)
                           {  
                              for (iLine = 15  /*sHeader.ulVsize - 1*/ ; iLine >= 0; iLine--)
                              {                                 
                                 iDst = iLine * sHeader.ulHsize * 2;
                                 for (i = 0; i < sHeader.ulHsize; i++)
                                 {   
                                    ucR = *(appData.pbBuffer + iSrc);
                                    ucG = *(appData.pbBuffer + iSrc + 1);
                                    ucB = *(appData.pbBuffer + iSrc + 2);
 
                                    // Reduce color depth by throwing away LSBs. 
                                    // R4 R3 R2 R1 R0 G5 G4 G3 G2 G1 G0 B4 B3 B2 B1 B0
                                    ucTemp = ucG;
                                    ucH = (ucR & 0xF8);        
                                    ucH |= (ucTemp >> 5); 
                                    ucL = (ucB >> 3);
                                    ucTemp = (ucG << 3);         
                                    ucTemp &= 0xE0;
                                    ucL |= ucTemp;
                                    
                                    *(ucDstBuff + iDst + (i*2)) = ucH;
                                    *(ucDstBuff + iDst + (i*2) + 1) = ucL;  
                                    
                                    if (ucTest)
                                    {   
                                       sprintf(ucBuff, "%02x %02x %c", ucH, ucL, '\0');                  
                                       by1351_DrawText(0, 54, ucBuff, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD); 
                                       while (1);                                                                           
                                    }                                     
                                    iSrc += 3;
                                 }
                              }                                               
                           }
                           else
                              ucRc = 7;        
                           
                           // Write image segment
                           
                           by1351_PrepareFrame(16 * (7 - j), (16 * (8 - j)) - 1);      // Ready display for direct writes
                           by1351_WriteFrame(ucDstBuff, 4096);  
                           
                           /*
                           sprintf(ucBuff, "P: %d %02x \0", 16 * (7 - j), 16 * (7 - j) );                  
                           by1351_DrawText(0, 54, ucBuff, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD); 
                           while(1);
                           */
                           
                           iSrc = 0;
                        }   
                     }
                     else
                        ucRc = 6;                                               
                  }   
                  else
                     ucRc = 5;                             
               }
               else
                  ucRc = 4;   
               SYS_FS_FileClose(appData.fileHandle);          
            }       
            else
               ucRc = 3;                     
         }   
         else
            ucRc = 2;   
          
         free(ucDstBuff);
      }   
      else
         ucRc = 1;                  
      free(appData.pbBuffer);
   }
   else
      ucRc = 1;            
 
   return ucRc;
}

//*********************************************************************************************************************
// GetBmpImage
// Reads BMP image from open file handle, return BMP in given buffer. Function returns buffer size needed, also
// if no input buffer is given. Only 16 and 24 bit color depths are supported. Compression is not supported.
//*********************************************************************************************************************
//

//*********************************************************************************************************************


unsigned long ulGetBmpImage(SYS_FS_HANDLE hBMPFile, uint8_t * pbImage)
{
//   unsigned char                 ucBuff[128]                = {0};
   unsigned long                 ulImagesize                = 0;
   unsigned char               * pucBuffer                  = NULL;     // Temporary processing buffer
   int                           i                          = 0;     
   int                           iSrc                       = 0;     
   int                           iDst                       = 0;     
   int                           iLine                      = 0;     
   unsigned char                 ucR                        = 0; 
   unsigned char                 ucG                        = 0; 
   unsigned char                 ucB                        = 0; 
   unsigned char                 ucH                        = 0; 
   unsigned char                 ucL                        = 0; 
   unsigned char                 ucTemp                     = 0; 
   
  
   if (hBMPFile == NULL)
      return RC_INVALID_PARAMETER;
       
   // Set filepointer to beginning
   if (SYS_FS_FileSeek(hBMPFile, 0, SYS_FS_SEEK_SET) == -1) 
      return RC_ERROR;
      
   // Read header
   if (SYS_FS_FileRead(hBMPFile, (void *)&sHeader, sizeof(BMHDR) ) == -1)
      return RC_ERROR;

   ulImagesize = sHeader.ulHsize * sHeader.ulVsize * 2;          // Supports 16 or 24 bit pixel depth, no pallette
   iLine = sHeader.ulVsize - 1;
   
   if ((sHeader.ulCompression) || (sHeader.usBitcount < 16))
      return RC_ERROR;   
   // Return size if needed
   if (!pbImage)        // No buffer, return resulting buffer size needed for raw image
      return (ulImagesize);
   // Read rest of input image
   pucBuffer = (unsigned char *)malloc(sHeader.ulFsize);  // Includes header size, not necessary
   if (!pucBuffer)
      return RC_ERROR;
   if (SYS_FS_FileRead(hBMPFile, (void *)pucBuffer, sHeader.ulFsize - sizeof(BMHDR)) == -1)
   {
      free(pucBuffer);
      return RC_ERROR;
   }   
   
   // Move image into provided buffer, convert as necessary
   if(sHeader.usBitcount == 24)        // Map from 3 to two bytes pr. pixel
   {
      //vDebug("Start.\0", D_GREEN);
      for (iLine = sHeader.ulVsize - 1; iLine >= 0; iLine--)
      {
         iDst = iLine * sHeader.ulHsize * 2;
         for (i = 0; i < sHeader.ulHsize; i ++)
         {   
            // Values stored 'backwards'
            ucR = *(pucBuffer + iSrc);
            ucG = *(pucBuffer + iSrc + 1);
            ucB = *(pucBuffer + iSrc + 2);
 
            // Reduce color depth by throwing away LSBs. 
            // R4 R3 R2 R1 R0 G5 G4 G3 G2 G1 G0 B4 B3 B2 B1 B0
            ucTemp = ucG;
            ucH = (ucR & 0xF8);        
            ucH |= (ucTemp >> 5); 
            ucL = (ucB >> 3);
            ucTemp = (ucG << 3);         
            ucTemp &= 0xE0;
            ucL |= ucTemp;

            *(pbImage + iDst + (i*2)) = ucH;
            *(pbImage + iDst + (i*2) + 1) = ucL;                                                                                 
             
            iSrc += 3;
         }   
      }
   }
   free(pucBuffer);   
   return (ulImagesize);
}
 
//*********************************************************************************************************************
// Debug to display 
// 117 is last line, with 2 lines to spare
// Mode 0: terminal text, mode 1: label in the middle of the screen
//*********************************************************************************************************************
//

void vDebug(char * cString, BYTE byColor, BYTE byMode)
{  
   if (byMode)
   {
      by1351_DrawText(0, 42, (char *)"                \0", 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, 0);
      by1351_DrawText(0, 50, (char *)"                \0", 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, 0);
      by1351_DrawText(0, 58, (char *)"                \0", 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, 0);
      
      switch(byColor)
      {
         case D_GREEN:
            by1351_DrawText(0, 50, (char *)cString, 0x1f, 0x3f, 0x1f, 0x00, 0x10, 0x00, FONT_VGA, 1, 0);
         break;   
         case D_YELLOW:
            by1351_DrawText(0, 50, (char *)cString, 0x1f, 0x3f, 0x1f, 0x10, 0x10, 0x00, FONT_VGA, 1, 0);
         break;   
         case D_RED:
            by1351_DrawText(0, 50,  (char *)cString, 0x1f, 0x3f, 0x1f, 0x10, 0x00, 0x00, FONT_VGA, 1, 0);
         break;   
         case D_PURPLE:
            by1351_DrawText(0, 50,  (char *)cString, 0x1f, 0x3f, 0x1f, 0x10, 0x10, 0x10, FONT_VGA, 1, 0);
         break;   
         case D_BLUE:
            by1351_DrawText(0, 50,  (char *)cString, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x10, FONT_VGA, 1, 0);
         break;      
      }       
   }
   else
   {
      switch(byColor)
      {
         case D_GREEN:
            by1351_WriteString((char *)cString, 0x00, 0x3f, 0x00);
         break;   
         case D_YELLOW:
            by1351_WriteString((char *)cString, 0x1f, 0x3f, 0x00);
         break;   
         case D_RED:
            by1351_WriteString((char *)cString, 0x1f, 0x00, 0x00);
         break;   
         case D_PURPLE:
            by1351_WriteString((char *)cString, 0x1f, 0x00, 0x1f);
         break;   
         case D_BLUE:
            by1351_WriteString((char *)cString, 0x00, 0x00, 0x1f);
         break;      
      }   
   }   
}    
 
//*********************************************************************************************************************
// Beep
//
//*********************************************************************************************************************
//

void vBeep(int iSound)
{
   int         i = 0;
   int         j = 0;
   int         k = 0;
//   int         iTemp = 0;
   
   // Map to digital IO   
   PLIB_PORTS_RemapOutput(PORTS_ID_0, 0x00, OUTPUT_PIN_RPG9);
                 
   switch (iSound)
   {
      case 0:                             // Error           
         for (i = 0; i < 50; i++)
         {
            SPEAKER = 1;   
            vLongDelay(5);
            SPEAKER = 0;   
            vLongDelay(5);
         }     
      break;
      case 1:                             // Message
         for (i = 0; i < 140; i++)
         {
            SPEAKER = 1;   
        		vShortDelay(36000);
            SPEAKER = 0;   
        		vShortDelay(36000);               
         }
         for (i = 0; i < 135; i++)
         {
            SPEAKER = 1;   
        		vShortDelay(43000);
            SPEAKER = 0;   
        		vShortDelay(43000);               
         }
      break;
      case 2:                             // Low blipp
          for (i = 0; i < 5; i++)
         {
            SPEAKER = 1;   
            vShortDelay(80000);
            SPEAKER = 0;   
            vShortDelay(83000);        		   
         }
      break;
      case 3:                             // Tick
         for (i = 0; i < 55; i++)
         {
            SPEAKER = 1;   
        		vShortDelay(4500);
            SPEAKER = 0;   
        		vShortDelay(4500);               
         }
      break;                                              
      case 4:                             // Dit-dit-dit
         for (k = 0; k < 3; k++)
         {                 
            for (i = 0; i < 60; i++)
            {
               SPEAKER = 1;   
        		   vShortDelay(15500);
               SPEAKER = 0;   
        		   vShortDelay(15500);        		   
        		}
        		vLongDelay(60);          
         }
      break;
      case 5:
         for (i = 0; i < 3; i++)
         {
            SPEAKER = 1;   
            vShortDelay(48000);
            SPEAKER = 0;   
            vShortDelay(48000);        		   
         }
      break;   
      case 6:
         for (i = 0; i < 2; i++)
         {
            SPEAKER = 1;   
            vShortDelay(51000);
            SPEAKER = 0;   
            vShortDelay(51000);        		   
         }
      break;
      case 7:
         for (i = 0; i < 2; i++)
         {
            SPEAKER = 1;   
            vShortDelay(54000);
            SPEAKER = 0;   
            vShortDelay(54000);        		   
         }
      break;  
      case 8:                             // Geiger tick
         for (i = 0; i < 10; i++)
         {
            SPEAKER = 1;   
        		vShortDelay(3000);
            SPEAKER = 0;   
        		vShortDelay(3000);               
         }
      break;                                              
           
   }   
   SPEAKER = 0;   
   // Map back to OC function
   PLIB_PORTS_RemapOutput(PORTS_ID_0, OUTPUT_FUNC_OC1, OUTPUT_PIN_RPG9);
}

//*********************************************************************************************************************
// Send RGB value to a WS2812 string
// BBRRGG
//*********************************************************************************************************************
//

void vSetRGBled(unsigned long ulRGB[LED_SERIAL_NUM], unsigned char ucNum, unsigned char ucR, unsigned char ucG, unsigned char ucB)
{
   unsigned char                 i                          = 0;
   unsigned char                 j                          = 0;
   unsigned long                 ulColor                    = 0;
   unsigned long                 ulTemp                     = 0;
   unsigned long                 k                          = 0;
//   unsigned long                 l                          = 0;
   
//   unsigned char ucTemp[20] = {0}; 
     
   // Address out-of-bounds?
   if (ucNum > LED_SERIAL_NUM)
      return;
   
   // Insert value in array   
   
   for (k = 0; k < 8; k++)
   {   
      ulColor |= (ucB & 0x01);        
      ulColor = ulColor << 1;
      ucB = ucB >> 1;
   }   
 
   for (k = 0; k < 8; k++)
   {
      ulColor |= (ucR & 0x01);
      ulColor = ulColor << 1;
      ucR = ucR >> 1;
   }   

   for (k = 0; k < 8; k++)
   {   
      ulColor |= (ucG & 0x01);
      if (k < 7)
         ulColor = ulColor << 1;
      ucG = ucG >> 1;
   }   

   ulRGB[ucNum] = ulColor; 
   
   // Update entire array  
   // Reset
 
   LED_SERIAL = 0;                             
   vShortDelay(6000);
                 
   SYS_INT_Disable();
   
   for (i = 0; i < LED_SERIAL_NUM; i++)                        // Iterate all LEDs in string
   {         
      ulTemp = ulRGB[i];         
      // Clock out 24 bits for one LED
 
      for (j = 0; j < 24; j++)
      {   
         if (ulTemp & 1)   // One
         {              
            LED_SERIAL = 1;
            vShortDelay(72);
            LED_SERIAL = 0;   
            vShortDelay(34);            
         }   
         else              // Zero
         {          
            LED_SERIAL = 1;
            vShortDelay(34);               
            LED_SERIAL = 0;
            vShortDelay(72);
         }    
         ulTemp = ulTemp >> 1;
      }    
   }      
   vShortDelay(6000);
   LED_SERIAL = 1;  
   SYS_INT_Enable();  
}   

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vClearRGBled(unsigned long ulRGB[LED_SERIAL_NUM], unsigned long ulColor)
{
   unsigned char                 i                          = 0;

   for (i = 0; i < LED_SERIAL_NUM; i++)
      vSetRGBled(ulRGB, i, 0, 0, 0);
}

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//






//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

