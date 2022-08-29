//*********************************************************************************************************************
//  
//  
//
//*********************************************************************************************************************
//
//               APRS        Geiger  
//               
//               GPS         GSM 
//
//*********************************************************************************************************************
//
  
#include "app.h"
#include "display.h"

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;

//*********************************************************************************************************************
// Symbols BMP
//*********************************************************************************************************************
//

// APRS
unsigned char bLogoAprs[] =
{
   0xaf, 0x7b, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x30, 0x84, 
   0xff, 0xff, 0x9a, 0xd6, 0x5d, 0xef, 0x04, 0x21, 0x00, 0x00, 0x5d, 0xef, 0x38, 0xc6, 0xff, 0xff, 
   0xff, 0xff, 0x14, 0xa5, 0xff, 0xff, 0xb6, 0xb5, 0xd7, 0xbd, 0xff, 0xff, 0x96, 0xb5, 0xff, 0xff, 
   0xff, 0xff, 0x59, 0xce, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0xff, 0xff, 
   0xff, 0xff, 0x92, 0x94, 0xff, 0xff, 0x1c, 0xe7, 0x5d, 0xef, 0xff, 0xff, 0x72, 0x94, 0xff, 0xff, 
   0x9a, 0xd6, 0xff, 0xff, 0x00, 0x00, 0xdb, 0xde, 0x3d, 0xef, 0x00, 0x00, 0xff, 0xff, 0xdb, 0xde, 
   0x00, 0x00, 0x6e, 0x73, 0x00, 0x00, 0xdb, 0xde, 0x3d, 0xef, 0x00, 0x00, 0x6e, 0x73, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdb, 0xde, 0x3d, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};

unsigned char bLogoGeiger[] =
{
   0x00, 0x00, 0xd3, 0x9c, 0xc7, 0x39, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x52, 0xd3, 0x9c, 0x00, 0x00, 
   0x4d, 0x6b, 0xff, 0xff, 0x59, 0xce, 0x00, 0x00, 0x21, 0x08, 0xdb, 0xde, 0xff, 0xff, 0x4d, 0x6b, 
   0xba, 0xd6, 0xff, 0xff, 0xff, 0xff, 0x28, 0x42, 0xaa, 0x52, 0xff, 0xff, 0xff, 0xff, 0x9a, 0xd6, 
   0xd7, 0xbd, 0xf8, 0xc5, 0x14, 0xa5, 0x59, 0xce, 0x18, 0xc6, 0x55, 0xad, 0xf8, 0xc5, 0xd7, 0xbd, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x73, 0x4d, 0x6b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x82, 0x10, 0x7d, 0xef, 0x5d, 0xef, 0x82, 0x10, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x51, 0x8c, 0xff, 0xff, 0xff, 0xff, 0x31, 0x8c, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x34, 0xa5, 0xbf, 0xff, 0xbf, 0xff, 0x55, 0xad, 0x00, 0x00, 0x00, 0x00
};

unsigned char bLogoGps[] =
{
   0x00, 0x00, 0x72, 0x94, 0xcb, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x71, 0x8c, 0x49, 0x4a, 0xf0, 0x83, 0xcb, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x8e, 0x73, 0x2c, 0x63, 0x00, 0x00, 0xf4, 0xa4, 0xd7, 0xbd, 0x0c, 0x63, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x8e, 0x73, 0x51, 0x8c, 0x1c, 0xe7, 0xff, 0xff, 0xb7, 0xbd, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x79, 0xce, 0xff, 0xff, 0x1c, 0xe7, 0x14, 0xa5, 0x8a, 0x52, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0xf0, 0x83, 0x9a, 0xd6, 0x31, 0x8c, 0x00, 0x00, 0x30, 0x84, 0x8a, 0x52, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0x7b, 0xeb, 0x5a, 0x49, 0x4a, 0x71, 0x8c, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0x7b, 0x72, 0x94, 0x00, 0x00
};

unsigned char bLogoGsm[] =
{
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x96, 0xb5, 0x00, 0x00, 0x00, 0x00, 
   0xff, 0xff, 0x61, 0x08, 0xfb, 0xde, 0xc3, 0x18, 0x38, 0xc6, 0xf4, 0xa4, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0xff, 0xff, 0xdf, 0xff, 0x76, 0xb5, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
   0x00, 0x00, 0x30, 0x84, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x8a, 0x52, 0xc7, 0x39, 0xff, 0xff, 
   0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x2d, 0x6b, 0x00, 0x00, 0x51, 0x8c, 0x6e, 0x73, 0xff, 0xff, 
   0x00, 0x00, 0x00, 0x00, 0xfb, 0xde, 0xe4, 0x20, 0xff, 0xff, 0x51, 0x8c, 0x6e, 0x73, 0xff, 0xff, 
   0x00, 0x00, 0x00, 0x00, 0xfb, 0xde, 0x04, 0x21, 0xff, 0xff, 0x51, 0x8c, 0x6e, 0x73, 0xff, 0xff, 
   0x00, 0x00, 0x00, 0x00, 0x79, 0xce, 0x82, 0x10, 0xff, 0xff, 0xf0, 0x83, 0x2c, 0x63, 0xff, 0xff
};   
   
unsigned char bLogoSquare[] =    // For test
{
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
 
long lPressure[] =               // Pressure/altitude
{
   -30.48,	1016.92,
   -15.24,	1015.08,
   0.0,     1013.25,
   15.24,	1011.42,
   30.48,	1009.59,
   60.96,	1005.95,
   91.44,	1002.31,
   121.92,	998.689,
   152.4,	995.075,
   182.88,	991.472,
   213.36,	987.88,
   243.84,	984.298,
   274.32,	980.727,
   304.8,	977.166,
   609.6,	942.129,
   914.4,	908.117,
   1219.2,	875.105,
   1524.0,	843.073,
   1828.8,	811.996,
   2133.6,	781.854,
   2438.4,	752.624,
   2743.2,	724.285,
   3048,    696.817,
   4572,    571.82,
   6096,    465.633   
};

//*********************************************************************************************************************
// Supporting display functions

float fDegtoRad(float fDeg)
{
   return(fDeg * PI / 180.000);
}

float fRadtoDeg(float fRad)
{
   return(180.000 * fRad / PI);
}

//*********************************************************************************************************************
// vSetIndicator
//  Set RGB front LED, relative to lighting conditions
//*********************************************************************************************************************
//
 
void vSetIndicator(uint8_t cLed, uint8_t cR, uint8_t cG, uint8_t cB)
{
//   int                           iLength                    = 0;
   uint8_t                       uI                         = 0;
   
   if (!appData.uiContrast)
      uI = 1;
   else
      uI = appData.uiContrast;
   
   vSetRGBled(appData.ulRGB, cLed, cR * uI, cG * uI, cB * uI);  
}   

//*********************************************************************************************************************
// vClearIndicator
//  Clear RGB front LED
//*********************************************************************************************************************
//
 
void vClearIndicator(uint8_t cLed)
{
   vSetRGBled(appData.ulRGB, cLed, 0x00, 0x00, 0x00);  
}   

//*********************************************************************************************************************
// Cut number of decimals in a number
//
//*********************************************************************************************************************
//
//

void vCutDec(char * cString, unsigned char bNum)
{
   unsigned char                 bTemp                      = 0;
   int                           iLength                    = 0;

   iLength = strlen(cString);
   do
   {
      if (cString[bTemp] == '.')
      {
         cString[bTemp + bNum + 1] = 0x00;
         break;
      }

      bTemp++;
   } while (bTemp < iLength);   
}


//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//


//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

 

//*********************************************************************************************************************
// Main display updating functions

//*********************************************************************************************************************
// Draw single scale
//  
//*********************************************************************************************************************
//

void vDrawScale(uint8_t x, uint8_t y, uint8_t cLength, uint8_t cVal, uint8_t cT1, uint8_t cT2, uint8_t cReverse)  
{
   uint8_t                                i                          = 0;
       
   // If cT2 = 0, no yellow.
   // Loop all segments
   for (i = 0; i < cLength; i++)
   {
      if (cReverse)  // G Y R
      {
         if ((i < cT1) & (i< cVal))
            by1351_DrawRectangle(x + (i*4), y, x + 2 + (i*4), y + 7, 0x00, 0x3f, 0x00, ATTR_FILLED);
         else if ((cT2) & (i < cT2) & (i< cVal))
            by1351_DrawRectangle(x + (i*4), y, x + 2 + (i*4), y + 7, 0x19, 0x36, 0x00, ATTR_FILLED);
         else if (i < cVal)
            by1351_DrawRectangle(x + (i*4), y, x + 2 + (i*4), y + 7, 0x1f, 0x00, 0x00, ATTR_FILLED);
         else
            by1351_DrawRectangle(x + (i*4), y, x + 2 + (i*4), y + 7, 0x03, 0x03, 0x03, ATTR_FILLED);   
      }
      else           // R Y G
      {
         if ((i < cT1) & (i< cVal))
            by1351_DrawRectangle(x + (i*4), y, x + 2 + (i*4), y + 7, 0x1f, 0x00, 0x00, ATTR_FILLED);
         else if ((cT2) & (i < cT2) & (i< cVal))
            by1351_DrawRectangle(x + (i*4), y, x + 2 + (i*4), y + 7, 0x19, 0x36, 0x00, ATTR_FILLED);
         else if (i < cVal)
            by1351_DrawRectangle(x + (i*4), y, x + 2 + (i*4), y + 7, 0x00, 0x3f, 0x00, ATTR_FILLED);
         else
            by1351_DrawRectangle(x + (i*4), y, x + 2 + (i*4), y + 7, 0x03, 0x03, 0x03, ATTR_FILLED);     
      }       
      if (i < 12)
      {   
         by1351_DrawLine(x + 3 + (i*4), y, x + 3 + (i*4), y + 7, 0x00, 0x00, 0x00, ATTR_FILLED); 
         by1351_DrawPixel(x + 3 + (i*4), y + 7, 0x00, 0x00, 0x00, ATTR_FILLED); 
      }   
   }       
}

//*********************************************************************************************************************
// Update 4 scales
//
//*********************************************************************************************************************
//

void vUpdateScales(void)
{
   const uint8_t                          bGlevel[]                  = {15, 36, 42, 48, 60, 76, 96, 120, 144, 168, 200, 240};    // Geiger curve
   const uint8_t                          bMlevel[]                  = {0, 2, 4, 6, 8, 11, 14, 16, 19, 22, 25, 28};              // GSM signal curve
   uint8_t                                bTemp1                     = 0;
   uint8_t                                bTemp2                     = 0;
   //unsigned int                           uiTime                     = 0;
   
   uint8_t                                bAprsScale                 = 0;
   uint8_t                                bGeigerScale               = 0;
   uint8_t                                bGpsScale                  = 0;
   uint8_t                                bGsmScale                  = 0;
   
   unsigned long ulTemp = 0;
   
   // Calculate scale values
   
   // Geiger
   for (bTemp1 = 0; bTemp1 < 12; bTemp1++)
   {
      if (bGlevel[bTemp1] > appData.iCountsMin)
         break;
   }
   bGeigerScale = bTemp1;
   
   // GPS
   //bGpsScale = appData.sGpsSatsUsed;
   bTemp2 = 0;
   for (bTemp1 = 0; bTemp1 < 32; bTemp1++)   // Only GPS sats            
      ulTemp+= appData.sData[bTemp1].iSignal;   
   bGpsScale = (int)(ulTemp / 100.0);
  
 //  sprintf((char *)appData.uiDebugbuffer1, "  S: %d  \0", bGpsScale);   
   if (bGpsScale > 12)
      bGpsScale = 12;
   
//   sprintf(appData.uiDebugbuffer4, "S: %d \0", bGpsScale);
   
   // GSM
   for (bTemp1 = 0; bTemp1 < 12; bTemp1++)
   {
      if (bMlevel[bTemp1] > appData.iGsmSignal)  
         break;
   }
   if ((appData.iGsmSignal == 99) || (!appData.iGsmSignal))
      bGsmScale = 0;
   else if (appData.iGsmSignal)
      bGsmScale = bTemp1;
 
   // APRS
   bTemp2 = 0;   
   for (bTemp1 = 0; bTemp1 < APRS_ARRAYSIZE; bTemp1++)
   {      
      if (((appData.uiGpsTime - appData.tAprsEntry[bTemp1].uiTime) < APRS_STALE) & (strlen((char *)appData.tAprsEntry[bTemp1].cCallsign)))
         bTemp2++;
   }
   bAprsScale = bTemp2;
   if (bAprsScale > 12)
      bAprsScale = 12;
   
   // APRS     Geiger
   // GPS      GSM      
    
   // APRS
   vDrawScale(14, 0, 12, bAprsScale, 3, 5, 0);  
   by1351_WriteSprite(0, 0, 8, 8, bLogoAprs);

   // Geiger
   vDrawScale(81, 0, 12, bGeigerScale, 3, 7, 1); //Rev
   by1351_WriteSprite(68, 0, 8, 8, bLogoGeiger);

   // GPS
   vDrawScale(14, 120, 12, bGpsScale, 3, 5, 0); 
   by1351_WriteSprite(0, 120, 8, 8, bLogoGps);

   // GSM
   vDrawScale(81, 120, 12, bGsmScale, 4, 7, 0); 
   by1351_WriteSprite(68, 120, 8, 8, bLogoGsm);
   
   // Lines
   by1351_DrawLine(0, 8, 127, 8, 0x10, 0x20, 0x10, 0);
   by1351_DrawLine(0, 119, 127, 119, 0x10, 0x20, 0x10, 0);
     
   // Reset color scan
   vCommand(0xA0);
   vData(0x70);    
}

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplayIntro(void)
{      
   char                          ucBuff[20]                 = {0};
   
   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 0, 127, 127, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }

   if (appData.uiDispFunc == FUNC_SUBSCREEN)   
   {   
      if (!appData.cIntroDone)
      {   
         appData.cIntroDone = 1;         
         sprintf(ucBuff, "pic%02d.bmp%c", appData.iIntroPic, '\0');         
         vPicturefromSD((uint8_t *)ucBuff);   
      }   
   }
   else
   {  
      if (!appData.cIntroDone)
      {   
         appData.cIntroDone = 1;
         vPicturefromSD((uint8_t *)"logo4.bmp");      
      }   
   }   
}
   
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplayShow(void)
{
   char                          ucBuff[20]                 = {0};
   char                          cTemp[20]                  = {0};
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;

   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }

   appData.ucAbortShow = 0;   
   appData.ucShowNext = 0;   
   
   vSetIndicator(2, 0x00, 0x02, 0x02);
   vLongDelay(200);
   vSetIndicator(2, 0x00, 0x00, 0x00);

   // Video 1
   if (!appData.ucAbortShow) 
      vVidfromSD("mov/jeep_1.vid");                  // Test 
   
   vSetIndicator(2, 0x02, 0x00, 0x02);
   vLongDelay(200);
   vSetIndicator(2, 0x00, 0x00, 0x00);
   
   if (appData.ucShowNext) 
   {
      appData.ucAbortShow = 0;   
      appData.ucShowNext = 0;  
   }
         
   // Video 2
   if (!appData.ucAbortShow) 
      vVidfromSD("mov/jeep_2.vid");                  // Test 
   
   vSetIndicator(2, 0x02, 0x00, 0x02);
   vLongDelay(200);
   vSetIndicator(2, 0x00, 0x00, 0x00);
   
   if (appData.ucShowNext) 
   {
      appData.ucAbortShow = 0;   
      appData.ucShowNext = 0;  
   }

   // Video 3
   if (!appData.ucAbortShow) 
      vVidfromSD("mov/jeep_3.vid");                  // Test 
   
   vSetIndicator(2, 0x02, 0x00, 0x02);
   vLongDelay(200);
   vSetIndicator(2, 0x00, 0x00, 0x00);
   
   if (appData.ucShowNext) 
   {
      appData.ucAbortShow = 0;   
      appData.ucShowNext = 0;  
   }
   
   // Video 4
   if (!appData.ucAbortShow) 
      vVidfromSD("mov/mario.vid");                  // Test 
   
   vSetIndicator(2, 0x02, 0x00, 0x02);
   vLongDelay(200);
   vSetIndicator(2, 0x00, 0x00, 0x00);
   
   if (appData.ucShowNext) 
   {
      appData.ucAbortShow = 0;   
      appData.ucShowNext = 0;  
   }
   
   if (appData.ucAbortShow) 
   {   
      if (appData.ucAbortShow == 2)
         appData.uiDisplayScreen = SCREEN_INTRO;
      else
         appData.uiDisplayScreen = SCREEN_SETTINGS;
      by1351_DrawRectangle(0, 0, 127, 127, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear text window  
      appData.ucAbortShow = 0;
   }   
}

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplaySpeed(void)
{
   char                          ucBuff[20]                 = {0};
   char                          cTemp[20]                  = {0};
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;
   
   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }
   
   // Speed  
   sprintf((char *)ucBuff, "%.1f km/h%c", appData.fGpsSpeed, '\0');
   if (appData.fGpsSpeed >= 110.0)
      by1351_DrawLargeText(0, 25, ucBuff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);    
   else if (appData.fGpsSpeed >= 90.0)
      by1351_DrawLargeText(0, 25, ucBuff, 0x1f, 0x3f, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);    
   else
      by1351_DrawLargeText(0, 25, ucBuff, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);    
   
   sprintf((char *)ucBuff, "%.1f deg%c", appData.fGpsBearing, '\0');
   if ((appData.fGpsBearing == 0.0) || (appData.fGpsBearing == 90.0) || (appData.fGpsBearing == 180.0) || (appData.fGpsBearing == 270.0))
      by1351_DrawLargeText(0, 55, ucBuff, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, ATTR_PAD);    
   else
      by1351_DrawLargeText(0, 55, ucBuff, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);    
   
   if ((appData.fGpsBearing < 11.25) || (appData.fGpsBearing >= 348.75)) {cTemp[0] = 'N';  cR = 0x07; cG = 0x3f; cB = 0x07;}
      else if (appData.fGpsBearing < 33.75)    {strncpy(cTemp, "NNE", 3);               cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 56.25)    {strncpy(cTemp, "NE", 2);                cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 78.75)    {strncpy(cTemp, "ENE", 3);               cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 101.25)   {cTemp[0] = 'E';                         cR = 0x07; cG = 0x3f; cB = 0x07;}
      else if (appData.fGpsBearing < 123.75)   {strncpy(cTemp, "ESE", 3);               cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 146.25)   {strncpy(cTemp, "SE", 2);                cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 168.75)   {strncpy(cTemp, "SSE", 3);               cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 191.25)   {cTemp[0] = 'S';                         cR = 0x07; cG = 0x3f; cB = 0x07;}
      else if (appData.fGpsBearing < 213.75)   {strncpy(cTemp, "SSW", 3);               cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 236.25)   {strncpy(cTemp, "SW", 2);                cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 258.75)   {strncpy(cTemp, "WSW", 3);               cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 281.25)   {cTemp[0] = 'W';                         cR = 0x07; cG = 0x3f; cB = 0x07;}
      else if (appData.fGpsBearing < 303.75)   {strncpy(cTemp, "WNW", 3);               cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 326.25)   {strncpy(cTemp, "NW", 2);                cR = 0x17; cG = 0x30; cB = 0x1c;}
      else if (appData.fGpsBearing < 348.75)   {strncpy(cTemp, "NNW", 3);               cR = 0x17; cG = 0x30; cB = 0x1c;}
   if (appData.fGpsSpeed > 2.0)
      by1351_DrawLargeText(40, 85, cTemp, cR, cG, cB, 0x00, 0x00, 0x00, ATTR_PAD);       
   else   
      by1351_DrawLargeText(40, 85, "--  ", cR, cG, cB, 0x00, 0x00, 0x00, ATTR_PAD);       
}

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplayPosition(void)
{
   char                          ucBuff[20]                 = {0};
   double                        dTemp                      = 0.0;
   double                        dAngle                     = 0.0;    
   
   #define                       XPOS                       56
   #define                       YPOS                       48
   
   uint8_t                       ucStartx                   = 0;
   uint8_t                       ucEndx                     = 0;
   uint8_t                       ucStarty                   = 0;
   uint8_t                       ucEndy                     = 0;
   
   uint8_t                       ucR                        = 0;
   uint8_t                       ucG                        = 0;
   uint8_t                       ucB                        = 0;
   
   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }

   if (appData.uiDispFunc == FUNC_SUBSCREEN)    // Details and graph
   {   
      // If first update after change, clear screen
      if (appData.cChangedScreen)
      {   
         by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
         appData.cChangedScreen = 0;
      }
      // Altitude from pressure
      by1351_DrawLargeText(0, 93, "A\0", 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, ATTR_NONE);    
      sprintf((char *)ucBuff, "%.1f m%c", appData.dPressAlt, '\0');   
      by1351_DrawLargeText(16, 93, ucBuff, 0x00, 0x3f, 0x1f, 0x00, 0x00, 0x00, ATTR_PAD);     
           
      // Calculated sea level pressure
      by1351_DrawText(47, 70, "Sea:", 0x1f, 0x20, 0x1f, 0, 0, 0, FONT_VGA, 1, 0);
      sprintf((char *)ucBuff, "%.1fmb%c", appData.dSealevel, '\0');   // 
      by1351_DrawText(47, 80, ucBuff, 0x1f, 0x20, 0x1f, 0, 0, 0, FONT_VGA, 1, 0);
      
      //sprintf((char *)ucBuff, "%lu%c", appData.ulLife, '\0');   
      //by1351_DrawText(60, 80, (char *)ucBuff, 0x1f, 0x00, 0x1f, 0, 0, 0, FONT_VGA, 1, 0);
                       
      if (appData.iAltScale != appData.iPrevAltScale)
      {        
         by1351_DrawRectangle(0, 12, 40, 88, 0x0e, 0x1e, 0x0e, ATTR_NONE);  
         by1351_DrawRectangle(1, 13, 39, 87, 0x1f, 0x3f, 0x1f, ATTR_NONE);         
         by1351_DrawRectangle(2, 14, 38, 86, 0x05, 0x08, 0x05, ATTR_FILLED);
         if (appData.iAltScale < 0)
         {
            by1351_DrawRectangle(2, 50, 38, 50 + abs(appData.iAltScale), 0x1f, 0x00, 0x00, ATTR_FILLED);                                                
         }
         else if (appData.iAltScale > 0)
         {
            by1351_DrawRectangle(2, 50 - appData.iAltScale, 38, 50, 0x00, 0x3f, 0x00, ATTR_FILLED);                                                
         }        
         by1351_DrawLine(2, 50, 38, 50, 0x1f, 0x3f, 0x1f, 0);            
         appData.iPrevAltScale = appData.iAltScale;
      }              
      if (appData.dAltChange > 80)                            
         vSetIndicator(0, 0x00, 0x03, 0x00);
      else if (appData.dAltChange > 60)                            
         vSetIndicator(0, 0x00, 0x02, 0x00);
      else if (appData.dAltChange > 40)                            
         vSetIndicator(0, 0x00, 0x01, 0x00);
      else if (appData.dAltChange < -80)                            
         vSetIndicator(0, 0x03, 0x00, 0x00);
      else if (appData.dAltChange < -60)                            
         vSetIndicator(0, 0x02, 0x00, 0x00);
      else if (appData.dAltChange < -40)                            
         vSetIndicator(0, 0x01, 0x00, 0x00);
      else        
         vSetIndicator(0, 0x00, 0x00, 0x00);
      
      // Variometer value      
      ucR = 0x1f; ucG = 0x3f; ucB = 0x1f;
      if (appData.dAltChange > 1.0)
      {
         ucR = (0x1f - (int)(appData.dAltChange / 2.0)); 
         ucG = 0x3f; 
         ucB = (0x1f - (int)(appData.dAltChange / 2.0));         
      }   
      else if (appData.dAltChange < -1.0)
      {
         ucR = 0x1f; 
         ucG = (0x3f + (int)(appData.dAltChange)); 
         ucB = (0x1f + (int)(appData.dAltChange / 2));         
      }               
      if (ucR > 0x1f) ucR = 0x1f;
      if (ucG > 0x3f) ucG = 0x3f;
      if (ucB > 0x1f) ucB = 0x1f;
      
      sprintf((char *)ucBuff, "%.1f%c", appData.dAltChange, '\0');   
      by1351_DrawLargeText(54, 22, ucBuff, ucR, ucG, ucB, 0x00, 0x00, 0x00, ATTR_PAD);     
      by1351_DrawText(58, 46, (char *)"m/min\0", 0x1f, 0x00, 0x1f, 0, 0, 0, FONT_VGA, 1, 0);            
   }
   else
   {   
      if (appData.cChangedScreen)
      {
         appData.cChangedScreen = 0;
         by1351_DrawRectangle(0, 10, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear text window  
      }
      
      // Set color
      if (appData.sGpsMode < 2)
      {
         ucR = 0x1f; ucG = 0x00; ucB = 0x00;
      }
      else if (appData.sGpsMode == 2)
      {
         ucR = 0x1f; ucG = 0x3f; ucB = 0x00;
      }
      else  
      {
         ucR = 0x00; ucG = 0x3f; ucB = 0x1f;
      }
     
      // Latitude  
      dTemp = appData.dGpsLatDec;
      
      if (appData.dGpsLatDec < 0.0)
      { 
         dTemp = -dTemp;
         sprintf((char *)ucBuff, "%3.4f%c", dTemp, '\0');   
         by1351_DrawLargeText(0, 16, "S\0", 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, ATTR_NONE);    
      }
      else
      {
         sprintf((char *)ucBuff, "%3.4f%c", dTemp, '\0');   
         by1351_DrawLargeText(0, 16, "N\0", 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, ATTR_NONE);             
      }   
      if (appData.sGpsMode < 2)
         by1351_DrawLargeText(20, 16, "--.---\0", ucR, ucG, ucB, 0x00, 0x00, 0x00, ATTR_PAD);    
      else
         by1351_DrawLargeText(20, 16, ucBuff, ucR, ucG, ucB, 0x00, 0x00, 0x00, ATTR_PAD);    
         
      // Longitude  
      dTemp = appData.dGpsLonDec;
      if (appData.dGpsLonDec < 0.0)
      { 
         dTemp = -dTemp;
         sprintf((char *)ucBuff, "%3.4f%c", dTemp, '\0');   
         by1351_DrawLargeText(0, 40, "W\0", 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, ATTR_NONE);    
      }
      else
      {
         sprintf((char *)ucBuff, "%3.4f%c", dTemp, '\0');   
         by1351_DrawLargeText(0, 40, "E\0", 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, ATTR_NONE);             
      }   
      if (appData.sGpsMode < 2)
         by1351_DrawLargeText(20, 40, "--.---\0", ucR, ucG, ucB, 0x00, 0x00, 0x00, ATTR_PAD);    
      else
         by1351_DrawLargeText(20, 40, ucBuff, ucR, ucG, ucB, 0x00, 0x00, 0x00, ATTR_PAD);    

      // Altitude
      by1351_DrawLargeText(0, 64, "A\0", 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, ATTR_NONE);
      if (appData.sGpsMode < 3)
      {   
         ucR = 0x1f; ucG = 0x00; ucB = 0x00;
         sprintf((char *)ucBuff, " -- m%c", '\0');   
      }
      else
      {
         ucR = 0x00; ucG = 0x3f; ucB = 0x1f;
         sprintf((char *)ucBuff, "%.1f m%c", appData.dGpsAlt, '\0');         
      }   
      by1351_DrawLargeText(20, 64, ucBuff, ucR, ucG, ucB, 0x00, 0x00, 0x00, ATTR_PAD);             

      // Maiden
      sprintf((char *)ucBuff, "%s%c", appData.cMaidenhead, '\0');   
      by1351_DrawLargeText(20, 88, ucBuff, 0x1f, 0x3f, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);                
   }
}   
   
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vClearGyroCircle(void)
{
   by1351_DrawCircle(64, 64, 61, 0x00, 0x00, 0x00, ATTR_FILLED);
   //by1351_DrawCircle(64, 64, 63, 0x00, 0x10, 0x1f, ATTR_NONE);   
}   

void vDisplayGyro(void)
{
   char                          ucBuff[24]                 = {0};
   uint8_t                       cRoadlx                    = 0;
   uint8_t                       cRoadly                    = 0;
   uint8_t                       cRoadrx                    = 0;
   uint8_t                       cRoadry                    = 0;
                    
   by1351_DrawCircle(64, 64, 63, 0x00, 0x10, 0x1f, ATTR_NONE);
   by1351_DrawLine(3, 64, 125, 64, 0x1f, 0x1f, 0x00, ATTR_NONE); 
   by1351_DrawLine(3, 63, 125, 63, 0x1f, 0x1f, 0x00, ATTR_NONE); 
   by1351_DrawLine(3, 65, 125, 65, 0x1f, 0x1f, 0x00, ATTR_NONE); 
   
   by1351_DrawText(0, 109, "P\0", 0x1f, 0x2f, 0x10, 0, 0, 0, FONT_VGA, 1, ATTR_NONE);   
   sprintf((char *)ucBuff, "%03d%c", abs(appData.iPitch), '\0');
   by1351_DrawText(0, 120, ucBuff, 0x00, 0x3f, 0x1f, 0, 0, 0, FONT_VGA, 1, ATTR_NONE);   

   by1351_DrawText(120, 109, "R\0", 0x1f, 0x2f, 0x10, 0, 0, 0, FONT_VGA, 1, ATTR_NONE);   
   sprintf((char *)ucBuff, "%03d%c", abs(appData.iRoll), '\0');
   by1351_DrawText(104, 120, ucBuff, 0x00, 0x3f, 0x1f, 0, 0, 0, FONT_VGA, 1, ATTR_NONE);   
 
   // Calculate road line
   cRoadlx = (unsigned char)(X_CENTER + (int)(sin(fDegtoRad(270 - appData.iPitch - appData.iRoll)) * (1 * X_CENTER)));
   cRoadly = (unsigned char)(Y_CENTER - (int)(cos(fDegtoRad(270 - appData.iPitch - appData.iRoll)) * (1 * Y_CENTER)));
   
   cRoadrx = (unsigned char)(X_CENTER + (int)(sin(fDegtoRad(90 + appData.iPitch - appData.iRoll)) * (1 * X_CENTER)));
   cRoadry = (unsigned char)(Y_CENTER - (int)(cos(fDegtoRad(90 + appData.iPitch - appData.iRoll)) * (1 * Y_CENTER)));
      
   if ((cRoadly != appData.iPrevly) || (cRoadry != appData.iPrevry))
   {
      appData.iPrevly = cRoadly;
      appData.iPrevry = cRoadry;
      vClearGyroCircle();
      
      by1351_DrawLine(3, 64, 125, 64, 0x1f, 0x1f, 0x00, ATTR_NONE); 
      by1351_DrawLine(3, 63, 125, 63, 0x1f, 0x1f, 0x00, ATTR_NONE); 
      by1351_DrawLine(3, 65, 125, 65, 0x1f, 0x1f, 0x00, ATTR_NONE);    
   }   
   if ((cRoadly == 64) && (cRoadry == 64))
   {         
      by1351_DrawLine(cRoadlx + 4, cRoadly, cRoadrx - 3, cRoadry, 0x00, 0x3f, 0x00, ATTR_NONE); 
      by1351_DrawLine(cRoadlx + 4, cRoadly + 1, cRoadrx - 3, cRoadry + 1, 0x1f, 0x00, 0x1f, ATTR_NONE);    
   }
   else
   {   
      by1351_DrawLine(cRoadlx + 4, cRoadly, cRoadrx - 3, cRoadry, 0x1f, 0x0f, 0x1f, ATTR_NONE); 
      by1351_DrawLine(cRoadlx + 4, cRoadly + 1, cRoadrx - 3, cRoadry + 1, 0x1f, 0x0f, 0x1f, ATTR_NONE); 
      by1351_DrawLine(cRoadlx + 4, cRoadly - 1, cRoadrx - 3, cRoadry - 1, 0x1f, 0x0f, 0x1f, ATTR_NONE);             
   }      
   // sprintf((char *)ucBuff, "%d %d\0", cRoadlx, cRoadly);
   // by1351_DrawText(40, 80, ucBuff, 0x1f, 0x3f, 0x1f, 0, 0, 0, FONT_VGA, 1, ATTR_NONE);
      
}
      
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplayGeiger(void)
{
   char                          ucBuff[24]                 = {0};
//   double                        dTemp                      = 0.0;
   uint8_t                       i                          = 0;
   uint8_t                       j                          = 0;
   int                           iMaxMin                    = 0;
   int                           iMaxAve                    = 0;
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;
   
   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }

   if (appData.uiDispFunc == FUNC_SUBSCREEN)    // Graphs
   {   
      if (appData.cChangedScreen)
      {
         appData.cChangedScreen = 0;
         by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear text window  
      }
      if (appData.cGotGeiger)   
      {
         appData.cGotGeiger = 0;       
       
         // Find scale
         iMaxMin = 0;
         iMaxAve = 0;         
         for (i = 0; i < 128; i++)
         {
            if (appData.sGraph[i].iCountsMin > iMaxMin)
               iMaxMin = appData.sGraph[i].iCountsMin;
            if (appData.sGraph[i].iAve10 > iMaxAve)
               iMaxAve = appData.sGraph[i].iAve10;         
         }
          
         // Draw curves
         j = appData.cGraphPtr;
                                   
         for (i = 0; i <= 127; i++)
         {
            // CPM
            // Erase 
            by1351_DrawLine(127 - i, 11, 127 - i, 117 /*- appData.sGraph[j].iCountsMin*/, 0x00, 0x00, 0x00, ATTR_NONE); 
            
            // 10 min average curve
            if (appData.sGraph[j].iAve10)    
            {
               if (appData.sGraph[j].iAve10 < 106)
               {   
                  by1351_DrawLine(127 - i, 117 - appData.sGraph[j].iAve10, 127 - i, 117, 0x00, 0x00, 0x12, ATTR_NONE); 
               }   
            }   
            // Minute curve
//            by1351_DrawLine(127 - i, 114 - appData.sGraph[j].iCountsMin, 127 - i, 116 - appData.sGraph[j].iCountsMin, 0x1f, 0x1f, 0x00, ATTR_NONE); 
            if (appData.sGraph[j].iCountsMin)
            {   
               if (appData.sGraph[j].iCountsMin < 106)
               {   
                  by1351_DrawPixel(127 - i, 116 - appData.sGraph[j].iCountsMin, 0x1f, 0x3f, 0x00, ATTR_NONE); 
                  by1351_DrawPixel(127 - i, 117 - appData.sGraph[j].iCountsMin, 0x1f, 0x1f, 0x00, ATTR_NONE); 
               }      
            }
            
            if (j > 0)
               j--;
            else
               j = 127;
         }
         // Write values
         if (appData.fRadLevel < 120)
            { cR = 0x00, cG = 0x3f; cB = 0x00; }
         else if ((appData.fRadLevel >= 120) & (appData.fRadLevel < 180))
            { cR = 0x00, cG = 0x3f; cB = 0x0f; }
         else if ((appData.fRadLevel >= 180) & (appData.fRadLevel < 260))
            { cR = 0x1f, cG = 0x3f; cB = 0x00; }
         else if ((appData.fRadLevel >= 260) & (appData.fRadLevel < 350))
            { cR = 0x1f, cG = 0x10; cB = 0x08; }
         else
            { cR = 0x1f, cG = 0x00; cB = 0x00; }  
         sprintf(ucBuff, "CPM: %d %c", appData.iCountsMin, '\0');
         by1351_DrawText(0, 12, ucBuff, cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
 
         if (appData.fRad10 < 120)
            { cR = 0x00, cG = 0x3f; cB = 0x00; }
         else if ((appData.fRad10 >= 120) & (appData.fRad10 < 180))
            { cR = 0x00, cG = 0x3f; cB = 0x0f; }
         else if ((appData.fRad10 >= 180) & (appData.fRad10 < 260))
            { cR = 0x1f, cG = 0x3f; cB = 0x00; }
         else if ((appData.fRad10 >= 260) & (appData.fRad10 < 350))
            { cR = 0x1f, cG = 0x10; cB = 0x08; }
         else
            { cR = 0x1f, cG = 0x00; cB = 0x00; }  
         sprintf(ucBuff, "Ave: %d %c", appData.iAve10, '\0');
         by1351_DrawText(0, 23, ucBuff, cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
         
      }
   }
   else
   {   
      if (appData.cChangedScreen)
      {
         appData.cChangedScreen = 0;
         by1351_DrawRectangle(0, 10, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear text window  
      }

      sprintf(ucBuff, "%d /5 %c", appData.iCounts5, '\0');  
      if (appData.iCounts5 > 30)
         by1351_DrawLargeText(0, 16, ucBuff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);             
      else if (appData.iCounts5 > 20)
         by1351_DrawLargeText(0, 16, ucBuff, 0x1f, 0x3f, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);             
      else 
         by1351_DrawLargeText(0, 16, ucBuff, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);             

      sprintf(ucBuff, "%d cpm %c", appData.iCountsMin, '\0');   
      if (appData.iCountsMin > 80)
         by1351_DrawLargeText(0, 40, ucBuff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);             
      else if (appData.iCountsMin > 50)
         by1351_DrawLargeText(0, 40, ucBuff, 0x1f, 0x3f, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);             
      else 
         by1351_DrawLargeText(0, 40, ucBuff, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, ATTR_PAD);             
      
      sprintf(ucBuff, "%.1f nSh%c", appData.fRadLevel, '\0');
      if (appData.fRadLevel < 100)
         { cR = 0x00, cG = 0x3f; cB = 0x00; }
      else if ((appData.fRadLevel >= 100) & (appData.fRadLevel < 150))
         { cR = 0x00, cG = 0x3f; cB = 0x0f; }
      else if ((appData.fRadLevel >= 150) & (appData.fRadLevel < 180))
         { cR = 0x1f, cG = 0x3f; cB = 0x00; }
      else if ((appData.fRadLevel >= 180) & (appData.fRadLevel < 220))
         { cR = 0x1f, cG = 0x10; cB = 0x08; }
      else
         { cR = 0x1f, cG = 0x00; cB = 0x00; }                  
      by1351_DrawLargeText(0, 66, ucBuff, cR, cG, cB, 0x00, 0x00, 0x00, ATTR_PAD);

      if (appData.iAve10 > 0)
         sprintf(ucBuff, "%.1f ave %c", appData.fRad10, '\0');
      else   
         sprintf(ucBuff, "No ave %c", '\0');
      if (appData.fRad10 < 100)
         { cR = 0x00, cG = 0x3f; cB = 0x00; }
      else if ((appData.fRad10 >= 100) & (appData.fRad10 < 150))
         { cR = 0x00, cG = 0x3f; cB = 0x0f; }
      else if ((appData.fRad10 >= 150) & (appData.fRad10 < 180))
         { cR = 0x1f, cG = 0x3f; cB = 0x00; }
      else if ((appData.fRad10 >= 180) & (appData.fRad10 < 220))
         { cR = 0x1f, cG = 0x10; cB = 0x08; }
      else
         { cR = 0x1f, cG = 0x00; cB = 0x00; }                  
      by1351_DrawLargeText(0, 90, ucBuff, cR, cG, cB, 0x00, 0x00, 0x00, ATTR_PAD);                    
   }         
} 
      
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

//*********************************************************************************************************************
// APRS Radar: calculate horizontal and vertical screen position based on bearing and distance data,
//        draw the station on the radar
//*********************************************************************************************************************
//

void vClearCircles(void)
{
   by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window      
   by1351_DrawPixel(64, 64, 0x1f, 0x3f, 0x1f, 0); 
   by1351_DrawCircle(64, 64, 32, 0x1f, 0x3f, 0x1f, 0);
   by1351_DrawCircle(64, 64, 63, 0x1f, 0x3f, 0x1f, 0);
}

void vDisplayAprsRadar(void)
{
   unsigned char                 bNum                       = 0;
   float                         fBearing                   = 0;
   float                         fDistance                  = 0;
   unsigned char                 bXpos                      = 0;
   unsigned char                 bYpos                      = 0;
   char                          cTemp[10]                  = {0};
   float                         fMinscale                  = 100;      // Minimum scale
   float                         fMaxscale                  = 0;        // Maximum scale
   unsigned char                 bVisible                   = 0;
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;
   unsigned int                  uiCount                    = 0;
   uint8_t                       cChecksum                  = 0;
   uint8_t                     * pC                         = NULL;  
   
     
   // Determine scale, see if checksum has changed   
   bVisible = 0;
   fMaxscale = 0;
   for (bNum = 0; bNum < APRS_ARRAYSIZE; bNum++)
   {                  
      if ((appData.tAprsEntry[bNum].dDistance <= appData.fScale) && (appData.tAprsEntry[bNum].cCallsign[0])
              && (appData.tAprsEntry[bNum].dDistance > .2))
         bVisible++;
      
      if (fMaxscale < appData.tAprsEntry[bNum].dDistance)
         fMaxscale = appData.tAprsEntry[bNum].dDistance;
   }
   
   if ((appData.ulLife - appData.ulLastzoom) > 150)  // 150 ms
   {         
      appData.ulLastzoom = appData.ulLife;
      
      if (appData.uiDispFunc != FUNC_SUBSCREEN) // Not manual zoom
      {   
         if ((bVisible < appData.dAprsStations - 1) && (appData.fScale < 80.0))
         {
            vClearCircles();
            appData.fScale += 1.0;
            appData.cPrevChecksum = 0;
         }
         else if ((bVisible > appData.dAprsStations + 1) && (appData.fScale > 2.0))
         {
            vClearCircles();
            appData.fScale -= 1.0;
            appData.cPrevChecksum = 0;
         }      
      }   
   }

   // Find chacksum, compare   
   pC = (uint8_t *)appData.tAprsEntry;
   for (uiCount = 0; uiCount < (sizeof(APRS_DATA) * APRS_ARRAYSIZE); uiCount++)
   {   
      cChecksum ^= *pC;
      pC++;
   }   
   if ((cChecksum != appData.cPrevChecksum) || (appData.cAprsRefresh))
   {
      appData.cAprsRefresh = 0;
      appData.cPrevChecksum = cChecksum;   
      vClearCircles();
      
      //vSetRGBled(appData.ulRGB, 1, 0x02, 0x00, 0x00);
  
      // Draw callsigns
      for (bNum = 0; bNum < APRS_ARRAYSIZE; bNum++)
      {
         if (!strncmp((char *)appData.tAprsEntry[bNum].cCallsign, APRS_CALLSIGN, strlen(APRS_CALLSIGN)))
         {        
            // Own callsign, do nothing

         }
         else
         {                      
            fBearing = appData.tAprsEntry[bNum].dBearing;
            fDistance = appData.tAprsEntry[bNum].dDistance;
            if ((fDistance <= appData.fScale) && (fDistance))
            {
               if (fDistance < fMinscale)
                  fMinscale = fDistance;
               bXpos = (unsigned char)(X_CENTER + (int)(sin(fDegtoRad(fBearing)) * (fDistance * X_CENTER / appData.fScale)));
               bYpos = (unsigned char)(Y_CENTER - (int)(cos(fDegtoRad(fBearing)) * (fDistance * Y_CENTER / appData.fScale)));
               if ((bXpos < 128) && (bYpos > 11) && (bYpos < 117))
               {
                  by1351_DrawRectangle(bXpos - 1, bYpos - 1, bXpos, bYpos, 0x1f, 0x3f, 0, 0); 
                  if ((appData.tAprsEntry[bNum].cCallsign[0] == 'L') && (appData.tAprsEntry[bNum].cCallsign[1] == 'A'))
                  {   
                     strncpy(cTemp, (char *)appData.tAprsEntry[bNum].cCallsign + 2, 4);
                     cTemp[4] = 0;
                  }                 
                  else
                     strncpy(cTemp, (char *)appData.tAprsEntry[bNum].cCallsign, 6);
                  if (bXpos < 122)
                  {                      
                     if ((appData.uiGpsTime - appData.tAprsEntry[bNum].uiTime) < 30)   // New
                     {
                        cR = 0x1f; cG = 0x3f; cB = 0x1f;
                     }
                     else if ((appData.uiGpsTime - appData.tAprsEntry[bNum].uiTime) < 60)
                     {
                        cR = 0x0a; cG = 0x3f; cB = 0x0f;                     
                     }   
                     else if ((appData.uiGpsTime - appData.tAprsEntry[bNum].uiTime) < 120)
                     {
                        cR = 0x1a; cG = 0x0e; cB = 0x1f;                     
                     }   
                     else if ((appData.uiGpsTime - appData.tAprsEntry[bNum].uiTime) < 180)
                     {
                        cR = 0x1a; cG = 0x2f; cB = 0x0d;                     
                     }   
                     else
                     {
                        cR = 0x10; cG = 0x1a; cB = 0x1f;                     
                     }   
                        
                     if (bYpos > 108)
                     {
                        by1351_DrawRectangle(bXpos +2, bYpos - 8, bXpos + 2 + (strlen(cTemp)* 8), bYpos - 1, 0x00, 0x00, 0x00, 0); 
                        by1351_DrawText(bXpos + 2, bYpos - 8, cTemp, cR, cG, cB, 0, 0, 0, FONT_VGA, 1, 0);
                     }
                     else
                     {
                        by1351_DrawRectangle(bXpos + 2, bYpos, bXpos + 2 + (strlen(cTemp)* 8), bYpos + 7, 0x00, 0x00, 0x00, 0);
                        by1351_DrawText(bXpos + 2, bYpos, cTemp, cR, cG, cB, 0, 0, 0, FONT_VGA, 1, 0);
                     }
                  }
               }
            }
         }   
      }
   }   
   // Scale number
   sprintf((char *)cTemp, "%d %c", (int)appData.fScale, '\0');     
   by1351_DrawText(0, 118, cTemp, 0x1f, 0x3f, 0x00, 0, 0, 0, FONT_VGA, 1, ATTR_NONE);   
     
}
  
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplayAprsList(void)
{    
   int                           i                          = 0;
   int                           j                          = 0;
   int                           k                          = 0;
   char                          cTemp[150]                 = {0};     
   char                          cTemp2[150]                = {0};     
   char                        * pP;
   char                        * pQ;
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;         
   uint8_t                       cEmpty                     = 0;   
   uint8_t                       rcRc                       = RC_OK;
   unsigned long                 ulTime                     = 0;
   unsigned long                 ulTime2                    = 0;
     
   if (appData.uiDispFunc == FUNC_SUBSCREEN)    // Subscreen, single stations
   {
      if (appData.cChangedScreen)
      {           
         by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
         appData.cChangedScreen = 0;

         // Top bar      
         sprintf((char *)cTemp, "%d %c", appData.uiStation, '\0');    
         by1351_DrawText(0, 12, "                ", 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x0f, FONT_VGA, 1, ATTR_PAD);  
         by1351_DrawText(0, 12, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x0f, FONT_VGA, 1, ATTR_PAD);  
         sprintf((char *)cTemp, "%s    %c", appData.tAprsEntry[appData.uiStation].cCallsign, '\0');
         if (strlen((char *)appData.tAprsEntry[appData.uiStation].cCallsign) > 2)      
            by1351_DrawText(23, 12, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x0f, FONT_VGA, 1, ATTR_PAD);
         else
            by1351_DrawText(23, 12, "         \0", 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x0f, FONT_VGA, 1, ATTR_PAD);
         sprintf((char *)cTemp, "%d%c", appData.tAprsEntry[appData.uiStation].uiTime, '\0');
         if (appData.tAprsEntry[appData.uiStation].uiTime > 0)
            by1351_DrawText(96, 12, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x0f, FONT_VGA, 1, ATTR_PAD); 

         // Text
         i = strlen((char *)appData.tAprsEntry[appData.uiStation].cComment);
         strcpy(cTemp2, (char *)appData.tAprsEntry[appData.uiStation].cComment);
         pP = cTemp2;
         pQ = pP;

         for (j = 0; j < i; j++)
         {   
            pP++;
            if (((pP - pQ) > 11) && (*pP == 0x20))          // LF at space
            {
               *pP = 0;
               if (*pQ == 0x20)
                  pQ++;
               by1351_DrawText(0, 24 + (k * 9), pQ, 0x1f, 0x3f, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD); 
               k++;
               pQ = pP + 1;
            }   
            else if (((pP - pQ) > 14) || (j == (i - 1)))    // Full line
            {
               strncpy(cTemp, pQ, 16);
               by1351_DrawText(0, 24 + (k * 9), cTemp, 0x1f, 0x3f, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD); 
               k++;
               pQ = pP + 1;
            }                     
         }

         // Bottom bar
         if (appData.tAprsEntry[i].dDistance < 100.0)
            sprintf((char *)cTemp, "%.1fkm %c", appData.tAprsEntry[i].dDistance, '\0');
         else
            sprintf((char *)cTemp, "%.0fkm %c", appData.tAprsEntry[i].dDistance, '\0');         
         by1351_DrawText(0, 105, "                ", 0x1f, 0x3f, 0x1f, 0x00, 0x1f, 0x00, FONT_VGA, 1, ATTR_PAD);  
         by1351_DrawText(0, 105, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x1f, 0x00, FONT_VGA, 1, ATTR_PAD);  

         sprintf((char *)cTemp, "%.0f %c", appData.tAprsEntry[appData.uiStation].dBearing, '\0');
         by1351_DrawText(56, 105, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x1f, 0x00, FONT_VGA, 1, ATTR_PAD);      

         if (appData.tAprsEntry[appData.uiStation].sAltitude != 0)         
            sprintf((char *)cTemp, "A:%d %c", appData.tAprsEntry[appData.uiStation].sAltitude, '\0');
         else
            strcpy(cTemp, "      \0");
         by1351_DrawText(96, 105, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x1f, 0x00, FONT_VGA, 1, ATTR_PAD);   
                   
         // Look up callsign database                  
         if (strlen(appData.tAprsEntry[appData.uiStation].cCallsign) > 3)
         {  
            //ulTime = appData.ulLife;
            rcRc = ucHamLookup(appData.tAprsEntry[appData.uiStation].cCallsign);
            if (rcRc == RC_OK)
            {                 
               if (k > 8)
               {
               }   
               else if (k > 7)                 // One line
               {
                  by1351_DrawRectangle(0, 95, 127, 102, 0x10, 0x00, 0x00, ATTR_FILLED);
                  sprintf((char *)cTemp, "%s %s%c", appData.sHamRecord.ucSurname, appData.sHamRecord.ucName, '\0');
                  cTemp[16] = 0;
                  by1351_DrawText(0, 77, cTemp, 0x1f, 0x3f, 0x1f, 0x10, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);                  
               }
               else if (k > 6)            // Two lines
               {
                  by1351_DrawRectangle(0, 86, 127, 102, 0x10, 0x00, 0x00, ATTR_FILLED);
                  sprintf((char *)cTemp, "%s %s%c", appData.sHamRecord.ucSurname, appData.sHamRecord.ucName, '\0');
                  cTemp[16] = 0;
                  by1351_DrawText(0, 77, cTemp, 0x1f, 0x3f, 0x1f, 0x10, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);      
                  sprintf((char *)cTemp, "%s%c", appData.sHamRecord.ucAddress, '\0');
                  cTemp[16] = 0;
                  by1351_DrawText(0, 86, cTemp, 0x1f, 0x3f, 0x1f, 0x10, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);                                                   
               }   
               else
               {   
                  by1351_DrawRectangle(0, 77, 127, 102, 0x10, 0x00, 0x00, ATTR_FILLED);
                  sprintf((char *)cTemp, "%s %s%c", appData.sHamRecord.ucSurname, appData.sHamRecord.ucName, '\0');
                  cTemp[16] = 0;
                  by1351_DrawText(0, 77, cTemp, 0x1f, 0x3f, 0x1f, 0x10, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);      
                  sprintf((char *)cTemp, "%s%c", appData.sHamRecord.ucAddress, '\0');
                  cTemp[16] = 0;
                  by1351_DrawText(0, 86, cTemp, 0x1f, 0x3f, 0x1f, 0x10, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);      
                  sprintf((char *)cTemp, "%s %s%c", appData.sHamRecord.ucCity, appData.sHamRecord.ucCountry, '\0');
                  cTemp[16] = 0;
                  by1351_DrawText(0, 95, cTemp, 0x1f, 0x3f, 0x1f, 0x10, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);              
               }      
            }   
         }
      }   
   }
   else     // Main list
   {        
      if (appData.cChangedScreen)
      {   
         by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
         appData.cChangedScreen = 0;
      }
      j = 0;
      for (i = 0; i < APRS_ARRAYSIZE; i++)
      {
         if ((strlen((char *)appData.tAprsEntry[i].cCallsign)) && (j < 8) && appData.tAprsEntry[i].dDistance > 0.02) 
         {
            cEmpty = 1;
            sprintf((char *)cTemp, "%s  %c", appData.tAprsEntry[i].cCallsign, '\0');

            if ((appData.uiGpsTime - appData.tAprsEntry[i].uiTime) < 30)   // New
            {
               cR = 0x1f; cG = 0x3f; cB = 0x1f;
            }
            else if ((appData.uiGpsTime - appData.tAprsEntry[i].uiTime) < 60)
            {
               cR = 0x0a; cG = 0x3f; cB = 0x0f;                     
            }   
            else if ((appData.uiGpsTime - appData.tAprsEntry[i].uiTime) < 120)
            {
               cR = 0x1a; cG = 0x0e; cB = 0x1f;                     
            }   
            else if ((appData.uiGpsTime - appData.tAprsEntry[i].uiTime) < 180)
            {
               cR = 0x1a; cG = 0x2f; cB = 0x0d;                     
            }   
            else
            {
               cR = 0x10; cG = 0x1a; cB = 0x1f;                     
            }   
                      
            by1351_DrawText(0, 18 + (j*12), cTemp, cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);  

            if (appData.tAprsEntry[i].dDistance < 100.0)
               sprintf((char *)cTemp, "%.1f%c", appData.tAprsEntry[i].dDistance, '\0');
            else
               sprintf((char *)cTemp, "%.0f%c", appData.tAprsEntry[i].dDistance, '\0');         
            by1351_DrawText(67, 18 + (j*12), cTemp, 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);  

            sprintf((char *)cTemp, "%.0f%c", appData.tAprsEntry[i].dBearing, '\0');
            by1351_DrawText(104, 18 + (j*12), cTemp, 0x00, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);  

            j++;
         }      
      }  
      if (!cEmpty) // No stations
         by1351_DrawText(0, 18, (char *)"APRS \0", 0x1f, 0, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);       

      // Dividers
      by1351_DrawLine(65, 17, 65, 110, 0x00, 0x00, 0x1f, 0);
      by1351_DrawLine(102, 17, 102, 110, 0x00, 0x00, 0x1f, 0);
   }   
}
      
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplayWifi(void)
{
   int                           i                          = 0;
   int                           j                          = 0;
   int                           k                          = 0;
   int                           l                          = 0;
   uint8_t                       cConnecting                = 0;
   char                          cTemp[150]                 = {0};     
   char                          cTemp2[50]                 = {0};  
   
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;    
   uint8_t                       cTmp                       = 0;
   uint8_t                       cEmpty                     = 0;         
   uint8_t                       ucBusy                     = 0;         
 
  
   i = 0;
   ucBusy = 1;
   
   // If first update after change, clear screen
   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
      appData.cWchanged = 1;
   }
 
   if (appData.cWchanged)
   {   
      while (ucBusy)
      {  
         k++;
         if (*appData.sWifiAp[i].cSsid)
         {   
            if (!memcmp(appData.cLastSsid,appData.sWifiAp[i].cSsid, strlen(appData.sWifiAp[i].cSsid))) 
               cConnecting = 1;
            else
               cConnecting = 0;
            cEmpty = 1;
            memset(cTemp, 50, 0x00);
            memset(cTemp2, 50, 0x00);
            if (strlen(appData.sWifiAp[i].cSsid) > 13)
            {   
               strncpy(cTemp2, appData.sWifiAp[i].cSsid, 13);
               //strncat(cTemp2, "..", 1);
            }           
            else
               strcpy(cTemp2, appData.sWifiAp[i].cSsid); 
            sprintf(cTemp, "%s%c", cTemp2, '\0');

            by1351_DrawText(0, 14 + (l * 9), "                ", cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);    
            
            for (j = 0; j < strlen(cTemp2); j++)
            {
               if ((cTemp[j] < 0x20) || (cTemp[j] > 0x7a))
                  cTemp[j] = '.';               
            }
            cTmp = appData.sWifiAp[i].cRssi;

            if (cTmp > 32)
               {cR = 0x00; cG = 0x3f; cB = 0x00;}
            else if (cTmp > 29)
               {cR = 0x05; cG = 0x32; cB = 0x06;}
            else if (cTmp > 25)
               {cR = 0x07; cG = 0x24; cB = 0x04;}
            else if (cTmp > 22)
               {cR = 0x10; cG = 0x18; cB = 0x00;}
            else if (cTmp > 19)
               {cR = 0x14; cG = 0x12; cB = 0x00;}
            else if (cTmp > 16)
               {cR = 0x1a; cG = 0x08; cB = 0x00;}
            else  
               {cR = 0x1f; cG = 0x00; cB = 0x00;}
            if (cConnecting)   //(appData.sWifiAp[i].cEncryption == 10)    // Connecting...            
               by1351_DrawText(0, 14 + (l * 9), cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x12, FONT_VGA, 1, ATTR_PAD);          
            else
               by1351_DrawText(0, 14 + (l * 9), cTemp, cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);          

            cTmp = appData.sWifiAp[i].cEncryption;                        
            if ((cTmp == 0) || (cTmp == 10))          // Open
               {cR = 0x00; cG = 0x00; cB = 0x00;}
            else if (cTmp == 1)     // WEP
               {cR = 0x10; cG = 0x00; cB = 0x00;}
            else if (cTmp == 2)     // WPA PSK
               {cR = 0x0b; cG = 0x00; cB = 0x08;}
            else if (cTmp == 3)     // WPA2 PSK
               {cR = 0x00; cG = 0x00; cB = 0x09;}
            else if (cTmp == 4)     // WPA WPA2 PSK
               {cR = 0x00; cG = 0x00; cB = 0x18;}
            else if (cTmp == 5)     // WPA2 Enterprise (Not in use
               {cR = 0x1f; cG = 0x00; cB = 0x05;}

            sprintf(cTemp, " %d%c", appData.sWifiAp[i].cRssi, '\0');            
            by1351_DrawText(103, 14 + (l * 9), cTemp, 0x1f, 0x3f, 0x1f, cR, cG, cB, FONT_VGA, 1, ATTR_NONE);          
            l++;              
         } 
         i++;
         if ((i > 10) || ((k - i) > 3))
            ucBusy = 0;
      }
      if (l)
         l--;
      while (l < 11)
      {   
         by1351_DrawText(0, 14 + (l * 9), "                ", cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);    
         l++;
      }     
      appData.cWchanged = 0;
      if (!cEmpty) // No APs
         by1351_DrawText(0, 18, (char *)"WiFi  \0", 0x1f, 0, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);       

   }   
}
      
//*********************************************************************************************************************
// Not used
//*********************************************************************************************************************
//

void vDisplayBt(void)
{          
   int                           i                          = 0;
   int                           j                          = 0;
   char                          cTemp[150]                 = {0};     
   char                          cTemp2[50]                 = {0};  
   
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;    
   uint8_t                       cTmp                       = 0;
   int                           iSig                       = 0;
   uint8_t                       cEmpty                     = 0;         

   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }

   // Generate checksum         
   cTmp = 0;
   for (i = 0; i < BT_ARRAYSIZE; i++)
      cTmp ^= (uint8_t)appData.sBtData[i].cRssi[2];
   
   if (appData.cBchanged != cTmp)
   {   
      appData.cBchanged = cTmp;
   
      for (i = 0; i < 10; i++)
      {  
         by1351_DrawText(0, 14 + (i * 9), "                ", cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);    
         if (*appData.sBtData[i].cRssi)
         {   
            cEmpty = 1;
            memset(cTemp, 50, 0x00);
            memset(cTemp2, 50, 0x00);
            if (strlen((char *)appData.sBtData[i].cName) > 13)
            {   
               strncpy(cTemp2, (char *)appData.sBtData[i].cName, 13);
               //strncat(cTemp2, "..", 1);
            }           
            else
               strcpy(cTemp2, (char *)appData.sBtData[i].cName); 
            sprintf(cTemp, "%s%c", cTemp2, '\0');

            for (j = 0; j < strlen(cTemp2); j++)
            {
               if ((cTemp[j] < 0x20) || (cTemp[j] > 0x7a))
                  cTemp[j] = '.';               
            }
        
            sscanf((char *)appData.sBtData[i].cRssi, "%d", &iSig);
                        
            if (iSig < -80)        
               {cR = 0x1f; cG = 0x00; cB = 0x00;}
            else if (iSig < -70)     
               {cR = 0x1f; cG = 0x00; cB = 0x10;}
            else if (iSig < -60)     
               {cR = 0x1f; cG = 0x00; cB = 0x1f;}
            else if (iSig < -50)     
               {cR = 0x10; cG = 0x00; cB = 0x1f;}
            else if (iSig < -40)     
               {cR = 0x00; cG = 0x1f; cB = 0x1f;}
            else if (iSig < -30)    
               {cR = 0x00; cG = 0x3f; cB = 0x1f;}
            else    
               {cR = 0x00; cG = 0x3f; cB = 0x00;}
            
            by1351_DrawText(0, 14 + (i * 9), cTemp, cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD);          
            
            sprintf(cTemp, " %s%c", appData.sBtData[i].cRssi, '\0');            
            by1351_DrawText(103, 14 + (i * 9), cTemp, 0x1f, 0x3f, 0x1f, cR, cG, cB, FONT_VGA, 1, ATTR_NONE);          
         }   
      } 
      if (!cEmpty)
         by1351_DrawText(0, 18, (char *)"BlueTooth\0", 0x1f, 0, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);       

   }   
}

//*********************************************************************************************************************
// Sensors
//*********************************************************************************************************************
//

void vDisplaySensors(void)
{          
   int                           i                          = 0;
   int                           j                          = 0;
   char                          ucBuff[12]                 = {0};     
   
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;    

   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
      appData.dOldTemperature = -100;
      appData.dOldPressure = -100;
      appData.dOldVoltage = -100;
      appData.dOldCurrent = -100;
      appData.ulSensLife = 0;      
   }
   
   if ((appData.ulLife - appData.ulSensLife) > 300)
   {   
      appData.ulSensLife = appData.ulLife;
      
      // Temperature
      if (appData.dTemperature != appData.dOldTemperature)
      {   
         appData.dOldTemperature = appData.dTemperature;

         if (appData.dTemperature < -10.0)        
            {cR = 0x00; cG = 0x00; cB = 0x1f;}
         else if (appData.dTemperature < 0.0)     
            {cR = 0x00; cG = 0x05; cB = 0x1f;}
         else if (appData.dTemperature < 10.0)     
            {cR = 0x00; cG = 0x10; cB = 0x1f;}
         else if (appData.dTemperature < 20.0)     
            {cR = 0x00; cG = 0x2f; cB = 0x1f;}
         else if (appData.dTemperature < 40.0)     
            {cR = 0x00; cG = 0x3f; cB = 0x00;}
         else if (appData.dTemperature < 50.0)    
            {cR = 0x1f; cG = 0x1f; cB = 0x00;}
         else    
            {cR = 0x1f; cG = 0x00; cB = 0x00;}

         sprintf(ucBuff, "T: %.2f%c", appData.dTemperature, '\0');     
         by1351_DrawLargeText(0, 16, ucBuff, cR, cG, cB, 0x00, 0x00, 0x00, ATTR_PAD);  
      }   

      // Pressure
      if (appData.dPressure != appData.dOldPressure)
      {   
         appData.dOldPressure = appData.dPressure;

         if (appData.dPressure < 900.0)        
            {cR = 0x00; cG = 0x00; cB = 0x1f;}
         else if (appData.dPressure < 935.0)     
            {cR = 0x00; cG = 0x05; cB = 0x1f;}
         else if (appData.dPressure < 960.0)     
            {cR = 0x00; cG = 0x10; cB = 0x1f;}
         else if (appData.dPressure < 1000.0)     
            {cR = 0x00; cG = 0x2f; cB = 0x1f;}
         else if (appData.dPressure < 1025.0)     
            {cR = 0x00; cG = 0x3f; cB = 0x00;}
         else if (appData.dPressure < 1040)    
            {cR = 0x1f; cG = 0x1f; cB = 0x00;}
         else    
            {cR = 0x1f; cG = 0x00; cB = 0x00;}

         sprintf(ucBuff, "P: %.2f%c", appData.dPressure, '\0');     
         by1351_DrawLargeText(0, 40, ucBuff, cR, cG, cB, 0x00, 0x00, 0x00, ATTR_PAD);  
      }   

      // Voltage
      if (appData.dVoltage != appData.dOldVoltage)
      {   
         appData.dOldVoltage = appData.dVoltage;

         if (appData.dVoltage < 11.0)        
            {cR = 0x1f; cG = 0x00; cB = 0x00;}
         else if (appData.dVoltage < 11.5)     
            {cR = 0x15; cG = 0x05; cB = 0x00;}
         else if (appData.dVoltage < 12.0)     
            {cR = 0x1f; cG = 0x1f; cB = 0x00;}
         else if (appData.dVoltage < 13.0)     
            {cR = 0x00; cG = 0x2f; cB = 0x1f;}
         else if (appData.dVoltage < 14.0)     
            {cR = 0x00; cG = 0x3f; cB = 0x00;}
         else if (appData.dVoltage < 15.0)    
            {cR = 0x1f; cG = 0x1f; cB = 0x00;}
         else    
            {cR = 0x1f; cG = 0x00; cB = 0x00;}

         sprintf(ucBuff, "U: %.2f%c", appData.dVoltage, '\0');     
         by1351_DrawLargeText(0, 64, ucBuff, cR, cG, cB, 0x00, 0x00, 0x00, ATTR_PAD);  
      }   

      // Current
      if (appData.dCurrent != appData.dOldCurrent)
      {   
         appData.dOldCurrent = appData.dCurrent;

         if (appData.dCurrent < 100.0)        
            {cR = 0x00; cG = 0x00; cB = 0x1f;}
         else if (appData.dCurrent < 150.0)     
            {cR = 0x00; cG = 0x2f; cB = 0x0f;}
         else if (appData.dCurrent < 200.0)     
            {cR = 0x00; cG = 0x3f; cB = 0x00;}
         else if (appData.dCurrent < 250.0)     
            {cR = 0x00; cG = 0x3f; cB = 0x10;}
         else if (appData.dCurrent < 300.0)     
            {cR = 0x10; cG = 0x3f; cB = 0x00;}
         else if (appData.dCurrent < 350.0)    
            {cR = 0x1f; cG = 0x00; cB = 0x00;}
         else    
            {cR = 0x1f; cG = 0x00; cB = 0x1f;}

         sprintf(ucBuff, "I: 0.%.0f%c", appData.dCurrent, '\0');     
         by1351_DrawLargeText(0, 88, ucBuff, cR, cG, cB, 0x00, 0x00, 0x00, ATTR_PAD);  
      }      
   }   
}

//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplaySats(void)
{    
//   double                        dTemp                      = 0.0;
   int                           iDist                      = 0;
   int                           iVpos                      = 0;
   int                           iHpos                      = 0;
   int                           i                          = 0;
   char                          cTemp[30]                  = {0};      
   uint8_t                       iSig                       = 0;      
   uint8_t                       iI                         = 0;     
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;
   uint8_t                       cC                         = 0;

   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }

   by1351_DrawCircle(64, 64, 53, 0x10, 0x30, 0x10, 0);  
   by1351_DrawPixel(64, 64, 0x00, 0x3f, 0x1f, 0); 

   // Number of sats used 
   iI = 0;
   for (i = 0; i < 32; i++)
   {
      if ((appData.sData[i].iSignal > 2) && (appData.sData[i].iElevation > 5)) 
         iI++;
   }   
   sprintf((char *)cTemp, "%d %c", iI, '\0');
   by1351_DrawText(0, 13, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   sprintf((char *)cTemp, "%d %c", appData.sGpsSatsSeen, '\0');
   by1351_DrawText(0, 23, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   iI = 0;
  
   
   /*
   appData.sGlonassSatsSeen = 0;
   for (i = 0; i < 32; i++)
   {
      if ((appData.sData[i + 32].iSignal > 3) && (appData.sData[i + 32].iSat)) 
         iI++;
      if (appData.sData[i + 32].iSat > 0)
         appData.sGlonassSatsSeen++;
   }   
   sprintf((char *)cTemp, "%d %c", iI, '\0');
   by1351_DrawText(110, 13, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   sprintf((char *)cTemp, "%d %c", appData.sGlonassSatsSeen, '\0');
   by1351_DrawText(110, 23, cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
   */
              
   for (i = 0; i < SAT_ARRAYSIZE; i++)
   {   
      if (appData.sData[i].iElevation > 5)
      {              
         //if (appData.sData[i].iElevation > 95)
         //   vSetRGBled(appData.ulRGB, 1, 0x02, 0x00, 0x00);         
         iDist = (int)((cos(((float)appData.sData[i].iElevation/180.0)* PI)) * 50.0);   
         iVpos = 64 + ((int)((float)iDist * sin(((float)appData.sData[i].iAzimuth/180.0)*PI)));
         iHpos = 64 - ((int)((float)iDist * cos(((float)appData.sData[i].iAzimuth/180.0)*PI)));         
         /*
         if (i < 8)
         {   
            sprintf(cTemp, "%d %d %c", appData.sData[i].iAzimuth, appData.sData[i].iElevation, '\0');
            by1351_DrawText(0, 10 * (i + 1), (char *)cTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
         }
         */
         if (appData.sData[i].iSat)
         {
            iSig = appData.sData[i].iSignal;
           
            cB = 0x05;
            cC = appData.sData[i].iSignal;
            if (cC > 0x1f)
               cC = 0x1f;
            cR = 0x1f - cC;
            cG = cC * 3;
            if (cG > 0x3f)
               cG = 0x3f;
            by1351_DrawPixel(iVpos, iHpos, cR, cG, cB, 0); 
            by1351_DrawPixel(iVpos + 1, iHpos, cR, cG, cB, 0); 
            by1351_DrawPixel(iVpos + 1, iHpos + 1, cR, cG, cB, 0); 
            by1351_DrawPixel(iVpos, iHpos + 1, cR, cG, cB, 0);                                                  
         }   
      }   
   }    
}
      
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplayGsm(void)
{        
   char                          cTemp[200]                  = {0};  
   char                          cTemp2[200]                 = {0};  
   int                           i                          = 0;
   int                           j                          = 0;
   int                           k                          = 0;  
   char                        * pP;
   char                        * pQ;
   
   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }

   sprintf((char *)cTemp, "Signal: %d %c", appData.iGsmSignal, '\0');  
   if (appData.cGsmRegistered)   // Green
      by1351_DrawText(0, 13, cTemp, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);       
   else
      by1351_DrawText(0, 13, cTemp, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);       
   
   if ((appData.ulLonglife - appData.ulButtontime) < 10)
   {
      sprintf((char *)cTemp, "'%s'%c", sConfig.bGsmPrimary , '\0');  
      by1351_DrawText(0, 22, cTemp, 0x1f, 0x0c, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);              
   }
   else
   {               
      sprintf((char *)cTemp, "%s %c", appData.ucGsmOperator, '\0');  
      by1351_DrawText(0, 22, cTemp, 0x1f, 0x0c, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   }
   
   // Write module mode, only overwrite if jamming.
   switch(appData.iGsmMode)
   {        
      case 0:
         strcpy(cTemp, "No service      \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 1:
         strcpy(cTemp, "GSM             \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 2:
         strcpy(cTemp, "GPRS            \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 3:
         strcpy(cTemp, "EGPRS (Edge)    \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 4:
         strcpy(cTemp, "WCDMA           \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 5:
         strcpy(cTemp, "HSDPA only      \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 6:
         strcpy(cTemp, "HSUPA only      \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 7:
         strcpy(cTemp, "HSDPA/HSUPA     \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 8:
         strcpy(cTemp, "LTE             \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 9:
         strcpy(cTemp, "TDS-CDMA        \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 10:
         strcpy(cTemp, "TDS-HSDPA only  \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 11:
         strcpy(cTemp, "TDS-HSUPA only  \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 12:
         strcpy(cTemp, "TDS HSDPA/HSUPA \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 13:
         strcpy(cTemp, "CDMA            \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 14:
         strcpy(cTemp, "EVDO            \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 15:
         strcpy(cTemp, "Hybrid CDMA/EVDO\0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 16:
         strcpy(cTemp, "1XLTE (CDMA/LTE)\0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 23:
         strcpy(cTemp, "eHRPD           \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      case 24:
         strcpy(cTemp, "Hybr. CDMA/eHRPD\0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x2f, 0x10, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      break;
      
   }
   
   switch(appData.cJamming)
   {
      case 1:
         //strcpy(cTemp, "No jam      \0");
         //by1351_DrawText(0, 31, cTemp, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
      break;
      case 2:
         strcpy(cTemp, "Jamming     \0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
      break;
      case 3:
         strcpy(cTemp, "Interference\0");
         by1351_DrawText(0, 31, cTemp, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
      break;
      default:
         //strcpy(cTemp, "Jam unknown \0");
         //by1351_DrawText(0, 31, cTemp, 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
      break;
   }   
        
   by1351_DrawLine(0, 42, 127, 42, 0x00, 0x20, 0x1f, 0);
   // Last SMS message
   if (strlen((char *)appData.ucGsmMessage))
   {
      strncpy(cTemp2, (char *)appData.ucGsmMessage, 160); 
      sprintf((char *)cTemp, "%s %c", appData.ucGsmSender, '\0');
      by1351_DrawText(0, 44, cTemp, 0x04, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);     
      
      i = strlen((char *)appData.ucGsmMessage);
      pP = cTemp2;
      pQ = pP;
      
      for (j = 0; j < i; j++)
      {   
         pP++;
         if (((pP - pQ) > 11) && (*pP == 0x20))          // LF at space
         {
            *pP = 0;
            if (*pQ == 0x20)
               pQ++;
            by1351_DrawText(0, 54 + (k * 9), pQ, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD); 
            k++;
            pQ = pP + 1;                         
         }   
         else if (((pP - pQ) > 14) || (j == (i - 1)))    // Full line
         {
            strncpy(cTemp, pQ, 16);
            by1351_DrawText(0, 54 + (k * 9), (char *)cTemp, 0x1f, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_PAD); 
            k++;
            pQ = pP + 1;
         }  
         if (k > 6)
            break;
      }                  
   }
}
      
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplaySystem(void)
{  
   char                          cTemp[30]                  = {0};  
   
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;
     
   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }
   
   if (appData.iGyroTemp < 1000) {cR = 0x00; cG = 0x00; cB = 0x1f;}
   else if (appData.iGyroTemp < 4500) {cR = 0x00; cG = 0x3f; cB = 0x00;}
   else if (appData.iGyroTemp < 5500) {cR = 0x1f; cG = 0x3f; cB = 0x00;}
   else if (appData.iGyroTemp < 6500) {cR = 0x1f; cG = 0x00; cB = 0x1f;}
   else {cR = 0x1f; cG = 0x00; cB = 0x00;}         
   sprintf((char *)cTemp, "GyrTmp: %.1f c %c ", (double)(appData.iGyroTemp / 100.0), '\0');  
   by1351_DrawText(0, 13, cTemp, cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   
   if (appData.fTempResult < 10) {cR = 0x00; cG = 0x00; cB = 0x1f;}
   else if (appData.fTempResult < 45) {cR = 0x00; cG = 0x3f; cB = 0x00;}
   else if (appData.fTempResult < 55) {cR = 0x1f; cG = 0x3f; cB = 0x00;}
   else if (appData.fTempResult < 65) {cR = 0x1f; cG = 0x00; cB = 0x1f;}
   else {cR = 0x1f; cG = 0x00; cB = 0x00;}         
   sprintf((char *)cTemp, "MtrTmp: %.1f c %c ", (double)appData.fTempResult, '\0');  
   by1351_DrawText(0, 23, cTemp, cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
      
   sprintf((char *)cTemp, "CAN Tx: %lu %c", appData.ulTxCount, '\0');  
   by1351_DrawText(0, 35, cTemp, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   sprintf((char *)cTemp, "CAN Rx: %lu %c", appData.ulRxCount, '\0');  
   by1351_DrawText(0, 45, cTemp, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   
   sprintf((char *)cTemp, "Sensor: %.2f v%c", appData.dVoltage, '\0');    
   if (appData.fVoltage < 10.9) {cR = 0x1f; cG = 0x00; cB = 0x00;}
   else if (appData.fVoltage < 11.3) {cR = 0x1f; cG = 0x3f; cB = 0x00;}
   else if (appData.fVoltage < 13.9) {cR = 0x00; cG = 0x3f; cB = 0x00;}
   else if (appData.fVoltage < 14.45) {cR = 0x1f; cG = 0x3f; cB = 0x00;}
   else {cR = 0x1f; cG = 0x00; cB = 0x00;}         
   by1351_DrawText(0, 55, cTemp, cR, cG, cB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   
   if (sConfig.cConfig1 & CFG_GEIGER)
      by1351_DrawText(0, 65, "Geiger snd ON  ", 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   else
      by1351_DrawText(0, 65, "Geiger snd OFF ", 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   
   if (sConfig.cConfig1 & CFG_AL_VISUAL)
   {   
      if (sConfig.cConfig1 & CFG_AL_AUDIO)      
         by1351_DrawText(0, 75, "Alarms ON    ", 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
      else
         by1351_DrawText(0, 75, "Alarms Quiet ", 0x00, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);            
   }
   else
      by1351_DrawText(0, 75, "Alarms OFF   ", 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
      
   // Contrast AD and step
   sprintf((char *)cTemp, "Contr: %lu %1x %c", appData.ulADHvalue, appData.uiContrast, '\0');     
   if (OPTO_IN)
      by1351_DrawText(0, 85, cTemp, 0x1f, 0x3f, 0x1f, 0x1f, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   else   
      by1351_DrawText(0, 85, cTemp, 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   
   // Clock and date
   sprintf((char *)cTemp, "Time: %d%d:%d%d:%d%d%c", RTCTIMEbits.HR10, RTCTIMEbits.HR01, RTCTIMEbits.MIN10, RTCTIMEbits.MIN01, RTCTIMEbits.SEC10, RTCTIMEbits.SEC01, '\0');  
   by1351_DrawText(0, 95, cTemp, 0x08, 0x3f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   sprintf((char *)cTemp, "Date: %d%d.%d%d.%d%d%c", RTCDATEbits.YEAR10, RTCDATEbits.YEAR01, RTCDATEbits.MONTH10, RTCDATEbits.MONTH01, RTCDATEbits.DAY10, RTCDATEbits.DAY01, '\0');  
   by1351_DrawText(0, 105, cTemp, 0x0e, 0x22, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   
}   
//*********************************************************************************************************************
// 
//*********************************************************************************************************************
//

void vDisplaySettings(void)
{  
   char                          cTemp[30]                  = {0};  

/*   
   uint8_t                       cR                         = 0;
   uint8_t                       cG                         = 0;
   uint8_t                       cB                         = 0;
*/
   
   if (appData.cChangedScreen)
   {   
      by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window  
      appData.cChangedScreen = 0;
   }

   // Set status
   if (appData.usStatus & STAT_SENS)            // No sensor module
      strcpy(cTemp, "No module       ");
   else if (appData.usStatus & STAT_CURR)       // Current out of spec
      strcpy(cTemp, "Current error   ");
   else if (appData.usStatus & STAT_VOLT)       // Voltage out of spec
      strcpy(cTemp, "Voltage error   ");
   else if (appData.usStatus & STAT_GPS)        // No GPS data
      strcpy(cTemp, "No GPS data     ");
   else if (appData.usStatus & STAT_GEIG)       // No Geiger data
      strcpy(cTemp, "No Geiger data  ");
   else if (appData.usStatus & STAT_GYRO)       // No Gyro data
      strcpy(cTemp, "No Gyro data    ");
   else if (appData.usStatus & STAT_APRS)       // No GPRS data
      strcpy(cTemp, "No APRS data    ");
   else if (appData.usStatus & STAT_WIFI)       // No WiFi data
      strcpy(cTemp, "No WiFi data    ");
   else if (appData.usStatus & STAT_CONT)       // No Contrast AD data
      strcpy(cTemp, "No Contrast data");
   else if (appData.usStatus & STAT_GSM)        // No GSM data
      strcpy(cTemp, "No GSM data     ");
   else if (appData.usStatus & STAT_SD)         // No SD card
      strcpy(cTemp, "No SD card      ");
   else
      strcpy(cTemp, "Status OK       ");
   
   //sprintf(cTemp, "%04x%c", appData.usStatus, '\0');   
   if (strlen(appData.ucSmsSetting) > 3)
       by1351_DrawText(0, 13, appData.ucSmsSetting, 0x1f, 0x3f, 0x0f, 0x00, 0x00, 0x1f, FONT_VGA, 1, ATTR_NONE);                   
   else if (appData.usStatus)
      by1351_DrawText(0, 13, cTemp, 0x1f, 0x3f, 0x0f, 0x1f, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   else
      by1351_DrawText(0, 13, cTemp, 0x1f, 0x3f, 0x0f, 0x00, 0x3f, 0x00, FONT_VGA, 1, ATTR_NONE);  
   
   // 
   sprintf((char *)cTemp, "Tx power: %d%c", sConfig.bPower, '\0');  
   by1351_DrawText(0, 25, cTemp, 0x1f, 0x1f, 0x0f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   sprintf((char *)cTemp, "Path: %s%c", sConfig.bAprsPath, '\0');  
   by1351_DrawText(0, 35, cTemp, 0x1f, 0x3f, 0x0f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   sprintf((char *)cTemp, "Period: %d   %c", sConfig.bPeriod, '\0');  
   by1351_DrawText(0, 45, cTemp, 0x1f, 0x3f, 0x0f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   sprintf((char *)cTemp, "Freq TX: %s%c", sConfig.bTXfreq, '\0');  
   by1351_DrawText(0, 55, cTemp, 0x1f, 0x2f, 0x09, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   sprintf((char *)cTemp, "Freq RX: %s%c", sConfig.bRXfreq, '\0');  
   by1351_DrawText(0, 65, cTemp, 0x1f, 0x2f, 0x09, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   sprintf((char *)cTemp, "Disp bias: %d  %c", appData.iDispBias, '\0');  
   by1351_DrawText(0, 75, cTemp, 0x0f, 0x3f, 0x03, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   sprintf((char *)cTemp, "Geig. cal: %d  %c", appData.iGeigerCal, '\0');  
   by1351_DrawText(0, 85, cTemp, 0x0f, 0x3f, 0x03, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  
   if (!appData.cReturnAlarm)
      sprintf((char *)cTemp, "Ret.scrn: off %c", '\0');  
   else         
      sprintf((char *)cTemp, "Ret.scrn: %d  %c", appData.cReturnAlarm, '\0');  
   by1351_DrawText(0, 95, cTemp, 0x13, 0x1f, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);  

   by1351_DrawText(0, 105, (char *)"Done      \0", 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);        
      
   if (appData.uiDispFunc == FUNC_SUBSCREEN)    // Subscreen, scroll and select settings
   {    
      switch (appData.cSetting)
      {
         case 0:              // Done
            by1351_DrawText(0, 105, (char *)"Done      \0", 0x1f, 0x00, 0x1f, 0x00, 0x30, 0x00, FONT_VGA, 1, ATTR_NONE);        
      
         break;
         case 1:              // Tx power
            sprintf((char *)cTemp, "Tx power: %d%c", sConfig.bPower, '\0');  
            by1351_DrawText(0, 25, cTemp, 0x1f, 0x1f, 0x0f, 0x00, 0x20, 0x00, FONT_VGA, 1, ATTR_NONE);  
         
         break;
         case 2:              // Path
            sprintf((char *)cTemp, "Path: %s%c", sConfig.bAprsPath, '\0');  
            by1351_DrawText(0, 35, cTemp, 0x1f, 0x3f, 0x0f, 0x00, 0x20, 0x00, FONT_VGA, 1, ATTR_NONE);  
         
         break;
         case 3:              // Period
            sprintf((char *)cTemp, "Period: %d   %c", sConfig.bPeriod, '\0');  
            by1351_DrawText(0, 45, cTemp, 0x1f, 0x3f, 0x0f, 0x00, 0x20, 0x00, FONT_VGA, 1, ATTR_NONE);  
          
         break;
         case 4:              // Freq TX
            sprintf((char *)cTemp, "Freq TX: %s%c", sConfig.bTXfreq, '\0');  
            by1351_DrawText(0, 55, cTemp, 0x1f, 0x2f, 0x09, 0x00, 0x20, 0x00, FONT_VGA, 1, ATTR_NONE);  
         
         break;
         case 5:              // Freq RX
            sprintf((char *)cTemp, "Freq RX: %s%c", sConfig.bRXfreq, '\0');  
            by1351_DrawText(0, 65, cTemp, 0x1f, 0x2f, 0x09, 0x00, 0x20, 0x00, FONT_VGA, 1, ATTR_NONE);  
         
         break;
         case 6:              // Display contrast bias
            sprintf((char *)cTemp, "Disp bias: %d  %c", appData.iDispBias, '\0');  
            by1351_DrawText(0, 75, cTemp, 0x0f, 0x3f, 0x03, 0x00, 0x20, 0x00, FONT_VGA, 1, ATTR_NONE);  
         
         break;
         case 7:              // Geiger calibration
            sprintf((char *)cTemp, "Geig. cal: %d  %c", appData.iGeigerCal, '\0');  
            by1351_DrawText(0, 85, cTemp, 0x0f, 0x3f, 0x03, 0x00, 0x20, 0x00, FONT_VGA, 1, ATTR_NONE);  
         
         break;
         case 8:              // Alarm return screen, 0 is off
            if (!appData.cReturnAlarm)
               sprintf((char *)cTemp, "Ret.scrn: off %c", '\0');  
            else         
               sprintf((char *)cTemp, "Ret.scrn: %d  %c", appData.cReturnAlarm, '\0');  
            by1351_DrawText(0, 95, cTemp, 0x13, 0x1f, 0x1f, 0x00, 0x20, 0x00, FONT_VGA, 1, ATTR_NONE);  
         
         break;
      }
   }
   
}   

//*********************************************************************************************************************
// Screens 
// 
//*********************************************************************************************************************
//

void vUpdateScreen()
{
   switch(appData.uiDisplayScreen)
   {
      case SCREEN_INTRO:
         vDisplayIntro();
      break;    
      case SCREEN_SPEED:      
         vDisplaySpeed();
      break;
      case SCREEN_POSITION:   
         vDisplayPosition();
      break;
      case SCREEN_GYRO:       
         vDisplayGyro();
      break;
      case SCREEN_GEIGER:     
         vDisplayGeiger();
      break;
      case SCREEN_APRSRADAR:  
         vDisplayAprsRadar();
      break;
      case SCREEN_APRSLIST:   
         vDisplayAprsList();
      break;
      case SCREEN_WIFI:       
         vDisplayWifi();
      break;
      case SCREEN_SENSORS:         
         vDisplaySensors();
//         vDisplayBt();
      break;
      case SCREEN_SATS:       
         vDisplaySats();
      break;
      case SCREEN_GSM:        
         vDisplayGsm();
      break;  
      case SCREEN_SYSTEM:        
         vDisplaySystem();
      break;  
      case SCREEN_SETTINGS:        
         vDisplaySettings();
      break;  
      case SCREEN_SHOW:
         vDisplayShow();
      break;   
   }   
}
   
//*********************************************************************************************************************
// Set screen, menus, etc.
// 
//*********************************************************************************************************************
//

void vDisplay_State(void)
{   
   char                          cTemp[30]                  = {0};  
          
   //sprintf((char *)cTemp, " %d %c", appData.uiDisplayState, '\0');
   //by1351_DrawText(0, 50, cTemp, 0x1f, 0x3f, 0x1f, 0, 0, 0, FONT_VGA, 1, ATTR_NONE);   
       
  	switch (appData.uiDisplayState)
   {     
      case 0:        // Start
         if (appData.ucSdReady)
         {
            sprintf(cTemp, "%d", RNGNUMGEN1);
                        
            if (cTemp[4] == '0')                            
               vPicturefromSD((uint8_t *)"logo4.bmp");
            else if (cTemp[4] == '1')                            
               vPicturefromSD((uint8_t *)"logo1.bmp");
            else if ((cTemp[4] == '2') || (cTemp[4] == '5'))                            
               vPicturefromSD((uint8_t *)"logo4.bmp");
            else if (cTemp[4] == '3')                            
               vPicturefromSD((uint8_t *)"pic64.bmp");
            else if (cTemp[4] == '4')                            
               vPicturefromSD((uint8_t *)"logo4.bmp");
            else                            
               vPicturefromSD((uint8_t *)"logo4.bmp");
                                                              
            appData.ulDispDelay = appData.ulLife;
            appData.uiDisplayState = 10; 
            if (sConfig.cConfig1 & CFG_AL_AUDIO) 
               vSoundfromSD((uint8_t *)"3.wav");                    
         }   
      break;
      case 10:   
         if ((appData.ulLife - appData.ulDispDelay) > 3000)  
            appData.uiDisplayState = 14;         
      break;                               
      case 14:      
         if (appData.ucSdReady)
         {
            vPicturefromSD((uint8_t *)"pic62.bmp");
                                                              
            appData.ulDispDelay = appData.ulLife;
            appData.uiDisplayState = 15; 
         }   
      break;
      case 15:
         if ((appData.ulLife - appData.ulDispDelay) > 3000)  
            appData.uiDisplayState = 20;         
      break;                         
      
      case 20:
         by1351_Clear();
         appData.uiDisplayState = 30;         
      break;           
      //****** Check changes to screen and update
      case 30:
         appData.uiDisplayScreen = SCREEN_GYRO;
         appData.uiDisplayPrev = 0;         
         appData.uiDisplayState = 40;  
      break;           
      case 40:
         if ((appData.uiDisplayScreen != appData.uiDisplayPrev) && (!appData.cAlarmScreen))   // Something changed, clear and update
         {
            appData.uiDisplayLock = 1;
            if ((appData.uiDisplayScreen == SCREEN_APRSRADAR) || (appData.uiDisplayScreen == SCREEN_GYRO))
               by1351_Clear();
            else if ((appData.uiDisplayPrev == SCREEN_APRSRADAR) || (appData.uiDisplayPrev == SCREEN_GYRO))
               by1351_Clear();
            else
            {
               appData.cChangedScreen = 1;  
               by1351_DrawRectangle(0, 9, 127, 118, 0x00, 0x00, 0x00, ATTR_FILLED);  // Clear window
            }   
            vUpdateScreen();
            appData.uiDisplayOld = appData.uiDisplayPrev;
            appData.uiDisplayPrev = appData.uiDisplayScreen;
            // Send change notification
            if (appData.uiDispFunc == FUNC_SCREEN)
               appData.cCanSendScreen = appData.uiDisplayScreen;   
            appData.uiDisplayLock = 0;
         }
         appData.uiDisplayState = 50;  
      break;           
      case 50:         
         if ((appData.uiDisplayScreen) && (!appData.cAlarmScreen))
         {   
            if ((appData.uiDisplayScreen != SCREEN_APRSRADAR) && (appData.uiDisplayScreen != SCREEN_GYRO) && (!appData.cAlarmScreen))
               vUpdateScales();                    // Update signal scales         
         }   
         appData.uiDisplayState = 60;         
      break;           
      case 60:         
         if (!appData.cAlarmScreen)
         {   
            vUpdateScreen();                       // Update current screen data         
         }  
         appData.cRefresh = 0;
         appData.uiDisplayState = 70;         
      break;     
      case 70:         
         if (!appData.cAlarmScreen)
         {   
            // Return to pre-alarm screen if applicable         
            if ((appData.uiLastScreen) && (appData.cReturnAlarm))
            {              
               if ((appData.ulLonglife - appData.ulAlarmReturn) > (appData.cReturnAlarm + 5))
               {
                  appData.uiDisplayScreen = appData.uiLastScreen;
                  appData.uiLastScreen = 0;         
               }   
            } 
         }  
         appData.cRefresh = 0;
         appData.uiDisplayState = 40;         
      break;          
   }   
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//










