//*********************************************************************************************************************
// ssd1351 driver
//
//
//*********************************************************************************************************************
//   
//  PIC32MZ
//  128x128 OLED
//
//*********************************************************************************************************************
//  
//   
//
//*********************************************************************************************************************
//

#include "app.h"

// Fonts
#include       "ssd1351_vga_font.h"                         // VGA
#include       "ssd1351_alps_font.h"                        // Alps
#include       "ssd1351_ital_font.h"                        // Italics
#include       "ssd1351_hack_font.h"                        // 20 high 4-bit antialiased, normal and bold


//*********************************************************************************************************************
//
//
//*********************************************************************************************************************
//

extern void                      vShortDelay(unsigned long);
extern void                      vLongDelay(unsigned long);
extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;


#define BMP_WIDTH 1298

//*********************************************************************************************************************
// Initialize SSD1351 chip. 
//
//*********************************************************************************************************************
//

uint8_t by1351_Initialize(void) 
{
   // Reset
   OLED_RESET = 0;
   vLongDelay(100);
   OLED_RESET = 1;
   vLongDelay(200);
   
     
   vCommand(0xFD); // Unlock (just in case)
   vData(0xB1);
   vCommand(0xAD); // Config
   vData(0x8E);
 
   //vSetRGBled(appData.ulRGB, 2, 0x00, 0x02, 0x00);

   vCommand(0xA0); // Color depth
   vData(0x70);    // 65k colors
//   vData(0x74);    // 65k colors
   
   vCommand(0xB0); // Power saving off
   vData(0x00);
   vCommand(0xA1); // Startline
   vData(0x00);
   vCommand(0xA2); // Start
   vData(0x00);
   vCommand(0xB9); // Default LUT
   vCommand(0xA6); // Normal mode
      
   //vCommand(0xBB); //
   //vData(0x1b);
    
   vCommand(0xC1);         //  Default contrast current
   vData(0b10001010);      
   vData(0b01010001);
   vData(0b10001010);
      
   by1351_Clear();
   vCommand(0xAF); // Sleep off

   return RC_OK;
}

//*********************************************************************************************************************
// Reset geometry
//
//*********************************************************************************************************************
//

uint8_t by1351_Reset(void) 
{
   // Reset row and column
   vCommand(0x15);
   vData(0x00);
   vData(0x7F);
   vCommand(0x75);
   vData(0x00);
   vData(0x7F);

   return RC_OK;
}

//*********************************************************************************************************************
// Clear panel.
//
//*********************************************************************************************************************
//

uint8_t by1351_Clear(void) 
{
   unsigned int i = 0;

   vCommand(0x15); // Column address
   vData(0x00); // Start
   vData(0x7F); // End
   vCommand(0x75); // Row address
   vData(0x00); // Start
   vData(0x7F); // End
   vCommand(0x5C); // Write to RAM
   for (i = 0; i < 16384; i++) 
   {
      vData(0x0000);
      vData(0x0000);
   }

   return RC_OK;
}

//*********************************************************************************************************************
// Prepare memory frame write
//
//*********************************************************************************************************************
//

uint8_t by1351_PrepareFrame(uint8_t iLine, uint8_t iEnd)
{
   vCommand(0x15);   // Column address
   vData(0x00);      // Start
   vData(0x7F);      // End
   vCommand(0x75);   // Row address
   vData(iLine);     // Start
   vData(iEnd);      // End
   vCommand(0x5C);   // Write to RAM

   return RC_OK;
}

//*********************************************************************************************************************
// Write memory frame
//
//*********************************************************************************************************************
//

uint8_t by1351_WriteFrame(uint8_t * byBuff, unsigned int uiSize) 
{      
   unsigned int                           i                          = 0;

   if (uiSize > 32768) // Too big
      return RC_INVALID_PARAMETER;

   OLED_RW = 0;
   OLED_DATA = 1; // D/(C)
   PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, (PORTS_DATA_MASK) 0x00FF);
   
   for (i = 0; i < uiSize; i++)
   {
      PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, (PORTS_DATA_TYPE) byBuff[i]);
      OLED_ENABLE = 1;
      //vShortDelay(1); //  
      OLED_ENABLE = 0;  
   }    
   return RC_OK;
}

//*********************************************************************************************************************
// Scroll display section up by n lines
//
//*********************************************************************************************************************
//

uint8_t by1351_Scroll(uint8_t byLines) 
{
   unsigned int                           uiAddr                     = 0;
   unsigned int                           uiRaddr                    = 0;
   uint8_t                                byMem[0x7F * 0x7F * 2]     = {0};

   uiRaddr = byLines * 18 * 0x7F;

   // Set column address read
   vCommand(0x15);
   vData(0x00); // Start
   vData(0x7F); // End

   // Set row address read
   vCommand(0x75);
   vData(0x09 * byLines); // Start
   vData(0x7F - 2); // End

   // Read RAM
   vCommand(0x5D);
   for (uiAddr = 0; uiAddr <= ((0x7F * 0x7F * 2) - uiRaddr); uiAddr++)
      byMem[uiAddr] = cDataRead();
   vCommand(0xAD); // noop

   // Write RAM   
   // Set column address write
   vCommand(0x15);
   vData(0x00); // Start
   vData(0x7F); // End

   // Set row address write
   vCommand(0x75);
   vData(0x00); // Start
   vData(0x7F - (byLines * 9)); // End

   vCommand(0x5C);
   for (uiAddr = 2; uiAddr <= ((0x7F * 0x7F * 2) - uiRaddr); uiAddr++)
      vData(byMem[uiAddr] & 0xFD); // Mask is hack for read error
   vCommand(0xAD); // noop

   return RC_OK;
}

//*********************************************************************************************************************
// Set panel contrast, value 0-16.
//
//*********************************************************************************************************************
//

uint8_t by1351_SetContrast(uint8_t byContrast) 
{
   if (byContrast > 0x0f)
      return RC_INVALID_PARAMETER;
   vCommand(0xC7); // Set master contrast
   vData(byContrast);

   return RC_OK;
}


//*********************************************************************************************************************
// Writes a sprite array to a defined screen area
//
//*********************************************************************************************************************
//

unsigned int by1351_WriteSprite(uint8_t byX, uint8_t byY, uint8_t byXsize, uint8_t byYsize, uint8_t * pucBitmap) 
{
   unsigned int                           uiI                        = 0;
   uint8_t                              * pucTemp;
   
   // Parameter check
   if (((byX + byXsize) > 128) | ((byY + byYsize) > 128))
      return RC_INVALID_PARAMETER;
   if (!pucBitmap)
      return RC_INVALID_PARAMETER;

   pucTemp = pucBitmap;
   // Set column
   vCommand(0x15);
   vData(byX);
   vData(byX + byXsize - 1);
   // Set row
   vCommand(0x75);
   vData(byY);
   vData(byY + byYsize - 1);
   // Write to RAM
   vCommand(0x5C);
   for (uiI = 0; uiI < (byXsize * byYsize); uiI++) {
      vData(*pucTemp++);
      vData(*pucTemp++);
   }
   by1351_Reset();

   return RC_OK;
}

//*********************************************************************************************************************
// Maps RGB to two-byte LCD format. Returns resulting integer.
// R < 32, G < 62, B < 32
//
//*********************************************************************************************************************
//

unsigned int ui1351_GetColor(uint8_t byR, uint8_t byG, uint8_t byB) 
{
   uint8_t                                byByte1                    = 0;
   uint8_t                                byByte2                    = 0;
   uint8_t                                byTemp                     = 0;
   unsigned int                           uiColor                    = 0;

   byR &= 0x1f;
   byG &= 0x3f;
   byB &= 0x1f;
   byByte1 = (byR << 3);
   byTemp = byG;
   byTemp = (byTemp >> 3);
   byByte1 |= byTemp;
   byByte2 = (byG << 5);
   byByte2 |= (byB & 0x1f);
   uiColor = byByte2 | byByte1 << 8;

   return uiColor;
}

//*********************************************************************************************************************
// Maps RGB to two-byte LCD format. Writes data bytes to LCD.
//
//  Color depth 16 bits: 
//    R: Max 0x1F
//    G: Max 0x3F
//    B: Max 0x1F
//
//
//*********************************************************************************************************************
//

uint8_t by1351_WriteColors(uint8_t byR, uint8_t byG, uint8_t byB) 
{
   uint8_t                                byByte1                    = 0;
   uint8_t                                byByte2                    = 0;
   uint8_t                                byTemp                     = 0;
 
   byR &= 0x1f;
   byG &= 0x3f;
   byB &= 0x1f;
   byByte1 = (byB << 3);
   byTemp = byG;
   byTemp = (byTemp >> 3);
   byByte1 |= byTemp;
   byByte2 = (byG << 5);
   byByte2 |= (byR & 0x1f);
   vData(byByte1);
   vData(byByte2);

   return RC_OK;
}

//*********************************************************************************************************************
// Draw a pixel
// Arguments:
//  byX              : X position
//  byY              : Y position
//  byR, byG, byB    : RGB color
//  byAttrib         : Not used
//
//*********************************************************************************************************************
//

uint8_t by1351_DrawPixel(uint8_t byX, uint8_t byY, uint8_t byR, uint8_t byG, uint8_t byB, uint8_t byAttrib) 
{
   // Set column
   vCommand(0x15);
   vData(byX);
   vData(0x7F);
   // Set row
   vCommand(0x75);
   vData(byY);
   vData(0x7F);
   // Write to RAM
   vCommand(0x5C);
   by1351_WriteColors(byR, byG, byB);
   by1351_Reset();

   return RC_OK;
}

//*********************************************************************************************************************
// Draw a line
// Generic Bresenham line implementation, or straight H or V loops
// Arguments:
//  byX1             : X position left
//  byY1             : Y position upper
//  byX2             : X position right
//  byY2             : Y position lower
//  byR, byG, byB    : RGB color
//  byAttrib         : Not used
//
//*********************************************************************************************************************
//

uint8_t by1351_DrawLine(uint8_t byX1, uint8_t byY1, uint8_t byX2, uint8_t byY2, uint8_t byR, uint8_t byG, uint8_t byB, uint8_t byAttrib) 
{
   int                                    iDx                        = 0;
   int                                    iSx                        = 0;
   int                                    iDy                        = 0;
   int                                    iSy                        = 0;
   int                                    err                        = 0;
   int                                    iE2                        = 0;

   if ((byX1 > 127) || (byX2 > 127) || (byY1 > 127) || (byY2 > 127))
      return RC_INVALID_PARAMETER;

   if (byX1 == byX2) // Vertical line
   {
      // Set vertical scan
      vCommand(0xA0);
      vData(0x71);

      // Set coloumn
      vCommand(0x15);
      vData(byX1);
      vData(byX1);
      // Set row
      vCommand(0x75);
      vData(byY1);
      vData(byY2);
      // Write buffer data
      vCommand(0x5C);
      for (iSx = 0; iSx < (byY2 - byY1); iSx++)
         by1351_WriteColors(byR, byG, byB);

      // Reset scan
      vCommand(0xA0);
      vData(0x70);
   } 
   else if (byY1 == byY2) // Horizontal line
   {
      // Set column
      vCommand(0x15);
      vData(byX1);
      vData(byX2);
      // Set row
      vCommand(0x75);
      vData(byY1);
      vData(byY2);
      vCommand(0x5C);

      // Fetch color bytes once only, for speed
      iSy = ui1351_GetColor(byR, byG, byB);
      for (iSx = byX1; iSx <= byX2; iSx++)
         by1351_WriteColors(byR, byG, byB);
   } 
   else // Slanted - Bresenham
   {
      iDx = abs(byX2 - byX1);
      iSx = byX1 < byX2 ? 1 : -1;
      iDy = abs(byY2 - byY1);
      iSy = byY1 < byY2 ? 1 : -1;
      err = (iDx > iDy ? iDx : -iDy) / 2;

      for (;;) {
         by1351_DrawPixel(byX1, byY1, byR, byG, byB, byAttrib);
         if (byX1 == byX2 && byY1 == byY2)
            break;
         iE2 = err;
         if (iE2 >-iDx) {
            err -= iDy;
            byX1 += iSx;
         }
         if (iE2 < iDy) {
            err += iDx;
            byY1 += iSy;
         }
      }
      by1351_Reset();
   }
   return RC_OK;
}

//*********************************************************************************************************************
// Rectangle, filles and outlined
// Arguments:
//  byX1             : X position left
//  byY1             : Y position upper
//  byX2             : X position right
//  byY2             : Y position lower
//  byR, byG, byB    : RGB color
//  byAttrib:
//    ATTR_FILLED    : Filled shape   
//
//*********************************************************************************************************************
//

uint8_t by1351_DrawRectangle(uint8_t byX1, uint8_t byY1, uint8_t byX2, uint8_t byY2, uint8_t byR, uint8_t byG, uint8_t byB, uint8_t byAttrib) 
{
   int                                    i                          = 0;
   int                                    j                          = 0;

   if ((byX1 > 127) || (byX2 > 127) || (byY1 > 127) || (byY2 > 127))
      return RC_INVALID_PARAMETER;
   if ((byX1 > byX2) || (byY1 > byY2))
      return RC_INVALID_PARAMETER;

   if (byAttrib & ATTR_FILLED) // Filled
   {
      // Set coloumn
      vCommand(0x15);
      vData(byX1);
      vData(byX2);
      // Set row
      vCommand(0x75);
      vData(byY1);
      vData(byY2);
      // Write to RAM
      i = (byX2 - byX1 + 1) * (byY2 - byY1 + 1);
      vCommand(0x5C);
      for (j = 0; j <= i; j++)
         by1351_WriteColors(byR, byG, byB);
   } 
   else // Outlinesd
   {
      by1351_DrawLine(byX1, byY1, byX1, byY2, byR, byG, byB, byAttrib);
      by1351_DrawLine(byX1, byY1, byX2, byY1, byR, byG, byB, byAttrib);
      by1351_DrawLine(byX2, byY1, byX2, byY2, byR, byG, byB, byAttrib);
      by1351_DrawLine(byX1, byY2, byX2, byY2, byR, byG, byB, byAttrib);
   }
   return RC_OK;
}

//*********************************************************************************************************************
// Circle
// Generic Bresenham line implementation
// Arguments:
//  byX              : X position
//  byY              : Y position
//  byRadius         : Radius
//  byR, byG, byB    : RGB color
//  byAttrib:
//    ATTR_FILLED    : Filled shape   
//
//*********************************************************************************************************************
//

uint8_t by1351_DrawCircle(uint8_t byX, uint8_t byY, uint8_t byRadius, uint8_t byR, uint8_t byG, uint8_t byB, uint8_t byAttrib) 
{
   int                                    x                          = 0;
   int                                    y                          = 0;
   int                                    iToggle                    = 0;

   x = byRadius;
   y = 0;
   iToggle = 1 - x;

   while (x >= y) 
   {
      if (byAttrib & ATTR_FILLED) 
      {
         by1351_DrawLine(-y + byX, -x + byY, y + byX, -x + byY, byR, byG, byB, byAttrib);
         by1351_DrawLine(-x + byX, -y + byY, x + byX, -y + byY, byR, byG, byB, byAttrib);
         by1351_DrawLine(-x + byX, y + byY, x + byX, y + byY, byR, byG, byB, byAttrib);
         by1351_DrawLine(-y + byX, x + byY, y + byX, x + byY, byR, byG, byB, byAttrib);
      } 
      else 
      {
         by1351_DrawPixel(y + byX, -x + byY, byR, byG, byB, byAttrib); // 1
         by1351_DrawPixel(-y + byX, -x + byY, byR, byG, byB, byAttrib); // 8
         by1351_DrawPixel(x + byX, -y + byY, byR, byG, byB, byAttrib); // 2
         by1351_DrawPixel(-x + byX, -y + byY, byR, byG, byB, byAttrib); // 7
         by1351_DrawPixel(x + byX, y + byY, byR, byG, byB, byAttrib); // 3
         by1351_DrawPixel(-x + byX, y + byY, byR, byG, byB, byAttrib); // 6
         by1351_DrawPixel(y + byX, x + byY, byR, byG, byB, byAttrib); // 4
         by1351_DrawPixel(-y + byX, x + byY, byR, byG, byB, byAttrib); // 5
      }
      y++;
      if (iToggle <= 0)
         iToggle += 2 * y + 1;
      else 
      {
         x--;
         iToggle += 2 * (y - x) + 1;
      }
   }
   by1351_Reset();

   return RC_OK;
}
 
//*********************************************************************************************************************
// Draw text string.
// Arguments:
//  byX              : X position (upper left of character)
//  byY              : Y position (upper left of character) 
//  pcString         : Zero terminated string
//  byR, byG, byB    : RGB character color
//  byRb, byGb, byBb : RGB background color
//  byFont:          : Font name define
//  bySize           : Letter size (1 is smallest)
//  byAttrib         : Attributes (not used)      
//
//*********************************************************************************************************************
//

uint8_t by1351_DrawText(uint8_t byX, uint8_t byY, char * pcString,
        uint8_t byR, uint8_t byG, uint8_t byB, uint8_t byRb, uint8_t byGb, uint8_t byBb, uint8_t byFont, uint8_t bySize, uint8_t byAttrib) 
{
   int                                    iLen                       = 0;
   int                                    i                          = 0;
   int                                    j                          = 0;
   int                                    k                          = 0;
   int                                    l                          = 0;
   int                                    byWidth                    = 8; // Character font width
   uint8_t                              * pFont                      = NULL;
   uint8_t                              * pP                         = NULL;
   uint8_t                                bChar                      = 0;
   uint8_t                                m                          = 0;
   uint8_t                                n                          = 0;

 
   if (!bySize)
      bySize = 1;
   // bAttrib selects direction, shade
   // Set font pointer
   if (byFont == FONT_VGA)
      pFont = font_vga;
   else if (byFont == FONT_ALPS)
      pFont = font_alps;
   else if (byFont == FONT_ITAL)
      pFont = font_ital;
   else
      return RC_INVALID_PARAMETER;

   iLen = strlen((char *)pcString);

   // Set vertical scan
   vCommand(0xA0);
   vData(0x71);

   // Set coloumn
   vCommand(0x15);
   vData(byX);
   if ((byX + (iLen * byWidth * bySize)) > 127) // Only map what there is room for
      vData(0x7f);
   else
      vData(byX + (iLen * byWidth * bySize));

   // Set row
   vCommand(0x75);
   vData(byY);
   vData(byY + 7 + (8 * (bySize - 1))); // Font always 8 pixels tall

   // Stop counter
   l = byX;

   // Write data
   vCommand(0x5C);

   // Traverse string
   for (i = 0; i < iLen; i++) 
   {
      if (l > (127 - byWidth + 1))
         break;
      // Set pointer to desired font character position
      pP = pFont + (pcString[i] * 8);

      //Traverse 8 bytes of character data
      for (j = 0; j < 8; j++) 
      {
         for (n = 0; n < bySize; n++) 
         {
            bChar = *pP;
            for (k = 0; k < 8; k++) 
            {
               for (m = 0; m < bySize; m++) 
               {
                  if (bChar & 0x01) 
                  {
                     by1351_WriteColors(byR, byG, byB);

                  } 
                  else
                     by1351_WriteColors(byRb, byGb, byBb);
               }
               bChar = bChar >> 1;
            }
         }
         pP++;
      }
      l += byWidth;
   }
   // Reset scan
   vCommand(0xA0);
   vData(0x70);

   return RC_OK;
}

//*********************************************************************************************************************
// Space columns

void vInsertBlankCol(uint8_t uiCol)
{
   unsigned int                           i                          = 0;
   
   for (i = 0; i < (uiCol * 24); i++)    
      by1351_WriteColors(0x00, 0x00, 0x00);                         
}

//*********************************************************************************************************************
// Draw large text string.
// Arguments:
//  byX              : X position (upper left of character)
//  byY              : Y position (upper left of character) 
//  pcString         : Zero terminated string
//  byR, byG, byB    : RGB character color
//  byRb, byGb, byBb : RGB background color
//  byAttrib         : Attributes       
//
//  Using the 'hack' font, 4-bit colour, antialiased
//
//*********************************************************************************************************************
//

BYTE by1351_DrawLargeText(uint8_t byX, uint8_t byY, char * pcString,
        uint8_t byR, uint8_t byG, uint8_t byB, uint8_t byRb, uint8_t byGb, uint8_t byBb, uint8_t byAttrib) 
{
   int                                    iLen                       = 0;  
//   uint8_t                              * pP                         = NULL;
   unsigned int                           l                          = 0;
   unsigned int                           uiIndex                    = 0;
   unsigned int                           uiOffset                   = 0;
   unsigned int                           uiWidth                    = 0;
   unsigned int                           i                          = 0;
   unsigned int                           j                          = 0;
   unsigned int                           uiBindex                   = 0;    
   uint8_t                                uiCol                      = 0;
   uint8_t                                ucPixels                   = 0;
   uint8_t                                cR                         = 0;
   uint8_t                                cG                         = 0;
   uint8_t                                cB                         = 0;
   
   
   iLen = strlen((char *)pcString);
   
   // Set vertical scan
   vCommand(0xA0);
   vData(0x71);

   // Set column
   vCommand(0x15);
   vData(byX);
   vData(0x7f);
    
   // Set row
   vCommand(0x75);
   vData(byY);
   vData(byY + 23);                       // Font always 22 pixels tall
   
   uiCol = byX;                           // Init colomn counter for padding 
   
   // Write data
   vCommand(0x5C);
   
   // Traverse string
   for (l = 0; l < iLen; l++) 
   {   
      if (pcString[l] == 32)        // Space
      {   
         vInsertBlankCol(10);
         uiCol += 10;
      }   
      else
      {   
         uiIndex = (pcString[l] - 33) * 3;

         // Look up offset and width
         uiOffset = hack_lookup[uiIndex + 1];
         uiWidth = hack_lookup[uiIndex + 2];

         uiWidth += 1;
         uiBindex = uiOffset;

         for (i = 0; i < uiWidth; i++)    // Column
         {
            for (j = 0; j < 24; j++)      
            {                                                      
               // B: 2  G: 3  R: 3 
               ucPixels = font_hack[uiBindex];                                                                       
               cR = (ucPixels & 0x07) << 2;            
               ucPixels = font_hack[uiBindex];            
               cG = (ucPixels & 0x38);            
               ucPixels = font_hack[uiBindex];
               cB = (ucPixels & 0xc0) >> 3;    
               
               if ((!cR) && (!cG) && (!cB))
                  by1351_WriteColors(byRb, byGb, byBb);    
               else
               {
                  //Add colors
                  cR &= byR;
                  cG &= byG;
                  cB &= byB;                  
                  by1351_WriteColors(cR, cG, cB);    
               }      
               uiBindex += BMP_WIDTH;        
            }           
            uiBindex = uiOffset + i;               // Next column
            uiCol++;
         }        
         uiCol++;
         vInsertBlankCol(1);                      // Space between characters
      }   
   }   
   // Pad if wanted
   if (byAttrib & ATTR_PAD)
   {
      if (uiCol < 128)
         vInsertBlankCol(128 - uiCol);           
   }   
            
   // Reset scan
   vCommand(0xA0);
   vData(0x70);

   return RC_OK;
}

//*********************************************************************************************************************
// Fade in.
// Arguments:
//  bySpeed:   Fading speed (0:instant  10:slow)
//
//*********************************************************************************************************************
//

uint8_t by1351_FadeIn(uint8_t bySpeed) 
{
   int                                    i                          = 0;

   if (!bySpeed)
      by1351_SetContrast(0x0f);
   else {
      for (i = 0; i <= 0x0f; i++) 
      {
         by1351_SetContrast(i);
         vLongDelay(20 * bySpeed);
      }
   }
   return RC_OK;
}

//*********************************************************************************************************************
// Fade out.
// Arguments:
//  bySpeed:   Fading speed (0:instant  10:slow)
//
//*********************************************************************************************************************
//

uint8_t by1351_FadeOut(uint8_t bySpeed) 
{
   int                                    i                          = 0;

   if (!bySpeed)
      by1351_SetContrast(0x00);
   else {
      for (i = 0x0f; i >= 0; i--) 
      {
         by1351_SetContrast(i);
         vLongDelay(20 * bySpeed);
      }
   }
   return RC_OK;
}

//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//

uint8_t by1351_Gradient(uint8_t byX1, uint8_t byY1, uint8_t byX2, uint8_t byY2, uint8_t byR, uint8_t byG, uint8_t byB, uint8_t byAttrib) 
{


   return RC_OK;
}

//*********************************************************************************************************************
// Shift screen up one line and write new line at bottom. Emulates terminal window.
// Use 'DrawText' for regular text writing.
//*********************************************************************************************************************
//

uint8_t by1351_WriteString(char * pcString, uint8_t iR, uint8_t iG, uint8_t iB) 
{
   by1351_Scroll(1); // Scroll up one line
   by1351_DrawLine(0, 125, 127, 125, 0, 0, 0, ATTR_NONE);
   by1351_DrawLine(0, 126, 127, 126, 0, 0, 0, ATTR_NONE);
   by1351_DrawLine(0, 127, 127, 127, 0, 0, 0, ATTR_NONE);
   by1351_DrawText(0, 117, (char *) "                             \0", 0x1f, 0x00, 0x0d, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);
   by1351_DrawText(0, 117, (char *) pcString, iR, iG, iB, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);
   return RC_OK;
}

//*********************************************************************************************************************
// Dump debug buffer to screen, zero terminated
//
//*********************************************************************************************************************
//

uint8_t by1351_DumpText(char * pcString) 
{
//   int                                    x                          = 0;
   int                                    y                          = 0;
   int                                    i                          = 0;
   int                                    j                          = 0;
   int                                    k                          = 0;
   uint8_t                                ucLine[20]                 = {0};

   i = strlen(pcString);
   if (i > 240)
      i = 240;
   if (i) {
      by1351_Clear();
      j = (i / 16); // Number of lines

      for (y = 0; y < j; y++) {
         strncpy((char *)ucLine, pcString + (y * 16), 16);
         for (k = 0; k < 16; k++) // Filter
         {
            if ((*(ucLine + k)) < 0x20)
               *(ucLine + k) = 0x20;
         }
         by1351_DrawText(0, y * 9, (char *)ucLine, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE);
      }
   }
   return RC_OK;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
// Hardware callbacks
//*********************************************************************************************************************
// Command to ssd1351 driver
//
//*********************************************************************************************************************
//

void vCommand(char cCommand) 
{
   OLED_RW = 0;
   OLED_DATA = 0; // D/(C)
   PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, (PORTS_DATA_MASK) 0x00FF);
   PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, (PORTS_DATA_TYPE) cCommand);
   OLED_ENABLE = 1;
   vShortDelay(1); //  
   OLED_ENABLE = 0;
}

//*********************************************************************************************************************
// Data to ssd1351 driver
//
//*********************************************************************************************************************
//
//      OLED_RESET    LATFbits.LATF0    // !Reset 
//      OLED_DATA     LATFbits.LATF1    // Data/!Command
//      OLED_RW       LATDbits.LATD5    // Read/!Write
//      OLED_ENABLE   LATDbits.LATD4    // Enable
//

void vData(char cData) 
{
   OLED_RW = 0;
   OLED_DATA = 1; // D/(C)
   PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, (PORTS_DATA_MASK) 0x00FF);
   PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, (PORTS_DATA_TYPE) cData);
   OLED_ENABLE = 1;
   //vShortDelay(1); //  
   OLED_ENABLE = 0;
}

// Quick version
void vDat(char cData) 
{
   PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, (PORTS_DATA_TYPE) cData);
   OLED_ENABLE = 1;
   //vShortDelay(1); //  
   OLED_ENABLE = 0;
}

//*********************************************************************************************************************
// Data from ssd1351 driver
//
//*********************************************************************************************************************
//
//      OLED_RESET    LATFbits.LATF0    // !Reset 
//      OLED_DATA     LATFbits.LATF1    // Data/!Command
//      OLED_RW       LATDbits.LATD5    // Read/!Write
//      OLED_ENABLE   LATDbits.LATD4    // Enable
//

uint8_t cDataRead(void) 
{
   uint8_t                                cData;

   OLED_RW = 1;
   OLED_DATA = 1; // D/(C)
   PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, (PORTS_DATA_MASK) 0x00FF);
   cData = (uint8_t) PLIB_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_E);
   OLED_ENABLE = 1;
   vShortDelay(1); //  
   OLED_ENABLE = 0;
   OLED_RW = 0;

   return cData;
}

//*********************************************************************************************************************
//
//
//*********************************************************************************************************************
//
