//*********************************************************************************************************************
// ssd1351 driver
//
//
//*********************************************************************************************************************
//  PIC32MZ
//  128x128 OLED - SSD1351 Controller
//
//*********************************************************************************************************************
//  All source licensed under the MIT license unless 3rd party modules, see 'license.txt'
//
//  Color depth: 
//    R: Max 0x1F
//    G: Max 0x3F
//    B: Max 0x1F
//
//*********************************************************************************************************************
//

#ifndef SSD1351_DRV_H
   #define	SSD1351_DRV_H
#endif

//*********************************************************************************************************************
//  Argument defines
//
//*********************************************************************************************************************
//

// Types
#define                          BYTE                       unsigned char

// Attributes
#define                          ATTR_NONE                  0b00000000
#define                          ATTR_FILLED                0b00000001
#define                          ATTR_PAD                   0b00000010

// Fonts and sizes
#define                          FONT_VGA                   0x00
#define                          FONT_ALPS                  0x01
#define                          FONT_ITAL                  0x02

// Returns
#define                          RC_OK                      0x00
#define                          RC_ERROR                   0x01
#define                          RC_INVALID_PARAMETER       0x02


//*********************************************************************************************************************
//  Prototypes
//
//*********************************************************************************************************************
//

void vData(char);

void vDat(char);

void vCommand(char);

uint8_t cDataRead(void);

BYTE by1351_Initialize(void);

BYTE by1351_Clear(void);

BYTE by1351_Scroll(BYTE byLines);

BYTE by1351_SetContrast(BYTE byContrast);

BYTE by1351_DrawLine(BYTE byX1, BYTE byY1, BYTE byX2, BYTE byY2, BYTE byR, BYTE byG, BYTE byB, BYTE byAttrib);

BYTE by1351_DrawPixel(BYTE byX1, BYTE byY1, BYTE byR, BYTE byG, BYTE byB, BYTE byAttrib);

BYTE by1351_DrawRectangle(BYTE byX1, BYTE byY1, BYTE byX2, BYTE byY2, BYTE byR, BYTE byG, BYTE byB, BYTE byAttrib);

BYTE by1351_DrawCircle(BYTE byX, BYTE byY, BYTE byRadius, BYTE byR, BYTE byG, BYTE byB, BYTE byAttrib);

BYTE by1351_DrawText(BYTE byX, BYTE byY, char * pcString, BYTE byR, BYTE byG, BYTE byB, BYTE byRb, 
        BYTE byGb, BYTE byBb, BYTE byFont, BYTE bySize, BYTE byAttrib);

BYTE by1351_DrawLargeText(uint8_t byX, uint8_t byY, char * pcString,
        uint8_t byR, uint8_t byG, uint8_t byB, uint8_t byRb, uint8_t byGb, uint8_t byBb, uint8_t byAttrib); 

BYTE by1351_FadeIn(BYTE bySpeed);

BYTE by1351_FadeOut(BYTE bySpeed);

unsigned int by1351_WriteSprite(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t *); 

BYTE by1351_Gradient(BYTE byX1, BYTE byY1, BYTE byX2, BYTE byY2, BYTE byR, BYTE byG, BYTE byB, BYTE byAttrib);

BYTE by1351_WriteColors(BYTE iR, BYTE iG, BYTE iB);

BYTE by1351_WriteString(char * pcString, BYTE iR, BYTE iG, BYTE iB);

unsigned int ui1351_GetColor(BYTE byR, BYTE byG, BYTE byB);

BYTE by1351_PrepareFrame(uint8_t, uint8_t);

BYTE by1351_WriteFrame(BYTE * byBuff, unsigned int uiSize);

BYTE by1351_DumpText(char * pcString);




