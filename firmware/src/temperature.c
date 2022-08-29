//*********************************************************************************************************************
// Maxim DS18B20 handling
//
//*********************************************************************************************************************
//

//#define _DISABLE_OPENADC10_CONFIGPORT_WARNING 
 
#include "app.h"
#include "temperature.h"
 
extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;  


//*********************************************************************************************************************
// Reset one-wire bus, check for presence, return presence status
//
//*********************************************************************************************************************
//
    
char cResetOw(void)
{
   char                          cData                      = 0;

   OW_TRIS = 0;      // Output              
   OW_PIN = 1;
   vLongDelay(1);
   OW_PIN = 0;       // First pulldown
   vDelayUs(600);    // Pulldown minimum 480 uS
   OW_PIN = 1;
   OW_TRIS = 1;      // Input
   vDelayUs(70);     // Pulse after 15 - 60 uS 
   cData = OW_PIN;   // Should be 0 if device present    
   vDelayUs(410);
      
   return cData;
}     

//*********************************************************************************************************************
// Write bit
//
//*********************************************************************************************************************
//

void vWriteOwBit(char cBit)
{
   OW_TRIS = 0;         // Output
   if (cBit)
   { 
      OW_PIN = 0;              
      vDelayUs(6);      // was 6 
      OW_PIN = 1;
      vDelayUs(64); 
   }
   else
   {
      OW_PIN = 0;              
      vDelayUs(60);     // was 60
      OW_PIN = 1;
      vDelayUs(10);      
   }
}

//*********************************************************************************************************************
// Read bit
//
//*********************************************************************************************************************
//

char cReadOwBit(void)
{
   char cData = 0;
   
   OW_TRIS = 0;         // Output
   OW_PIN = 0;              
   vDelayUs(6);         // was 6 
   OW_PIN = 1;
   vDelayUs(9);         // was 9       
   OW_TRIS = 1;      
   cData = OW_PIN; 
   vDelayUs(55);    
   
   return cData;  
}

//*********************************************************************************************************************
// Write a byte
//
//*********************************************************************************************************************
//
  
void vWriteOwByte(char cByte)
{   
   unsigned char cLoop = 0;
   
   for (cLoop = 0; cLoop < 8; cLoop++)
   {
      vWriteOwBit(cByte & 0x01);    //Sending LS-bit first
      cByte >>= 1;                  // shift the data byte for the next bit to send 
   }     
}   

//*********************************************************************************************************************
// Read a byte
//
//*********************************************************************************************************************
//
  
unsigned char ucReadOwByte(void)
{
   unsigned char cData = 0;
   unsigned char cLoop = 0;
   
   for (cLoop = 0; cLoop < 8; cLoop++)
   {
      cData >>= 1;         // shift the result to get it ready for the next bit to receive
      if (cReadOwBit())
         cData |= 0x80;    // if result is one, then set MS-bit
   }
   return (cData);
}

//*********************************************************************************************************************
// Initialize Dallas sensor device
//
//*********************************************************************************************************************
//
  
unsigned char ucInitOwTemp(void)
{    
   if (cResetOw())
      return (RC_ERROR);    
 
   appData.uiTempState = 0;
   
   // Skip need for ROM address
   vWriteOwByte(0xcc);  
   // Set configuration register etc.     
   vWriteOwByte(0x4e);  // Write 3 bytes to scratchpad
   vWriteOwByte(0xff);  // T-high
   vWriteOwByte(0x00);  // T-low
   vWriteOwByte(0x7f);  // Configuration Tconv/4 187mS
   
   // Get unique serial number
   cResetOw();
   vWriteOwByte(0x33);
   appData.ucTserial[0] = ucReadOwByte();
   appData.ucTserial[1] = ucReadOwByte();
   appData.ucTserial[2] = ucReadOwByte();
   appData.ucTserial[3] = ucReadOwByte();
   appData.ucTserial[4] = ucReadOwByte();
   appData.ucTserial[5] = ucReadOwByte();
   appData.ucTserial[6] = ucReadOwByte();
   appData.ucTserial[7] = ucReadOwByte();
   
   return (RC_OK);
}   
   
//*********************************************************************************************************************
// Start sampling
//
//*********************************************************************************************************************
//
  
unsigned char ucStartSampling(void)
{
   // Start with reset
   cResetOw();
   
   // Skip ROM
   vWriteOwByte(0xcc);
 
   // Convert temperature
   vWriteOwByte(0x44);

   return (RC_OK);
}   
   
//*********************************************************************************************************************
// Read temperature from Dallas sensor device
//
//*********************************************************************************************************************
//
  
float fReadOwTemp()
{
   float          fResult  = 0;   
   unsigned char  cBuff[9] = {0};
   unsigned char  ucI = 0;
   unsigned char  ucTemp = 0;
    
   // Start with reset
   cResetOw();
   // Skip ROM
   vWriteOwByte(0xcc);
   // Read scratchpad
   vWriteOwByte(0xbe);    
   do
   {
      cBuff[ucI] = ucReadOwByte();      
      ucI++;    
   } while (ucI < 9);    

   if (cBuff[1] & 0x80)    // Negative
   {
      cBuff[0] ^= 0xff;                
      cBuff[1] ^= 0xff;                
      cBuff[0]++; 
      fResult = ((cBuff[0] & 0x0f) * 0.0625);   
      ucTemp = cBuff[0] >> 4;
      ucTemp |= (cBuff[1] << 4);
      fResult += ucTemp;
      fResult = 0 - fResult;                  
   }
   else                    // Positive
   {
      fResult = ((cBuff[0] & 0x0f) * 0.0625);
      ucTemp = cBuff[0] >> 4;
      ucTemp |= (cBuff[1] << 4);  
      fResult += ucTemp;            
   }      
   return fResult;
}   
  
//*********************************************************************************************************************
// Temperature state
//
//*********************************************************************************************************************
//
  
int Temperature_State()
{    
//   unsigned char                 ucBuff[]                   = {"Abcd!\0"};
//   static unsigned char          i                          = 0; 
//   unsigned char                 ucDebug[128]               = {0};
   
   switch (appData.uiTempState)
   {
      case 0:        // Idle
         // Idle, wait for start
                   
      break;
      case 1:        // Start sampling
         ucStartSampling();
         OW_TRIS = 1;                
         appData.uiTempState = 2;
      break;
      case 2:        // Busy, wait for finish
         if (OW_PIN)     
            appData.uiTempState = 3;         
      break;  
      case 3:        // Result ready
         appData.fTempResult = fReadOwTemp();
         appData.uiTempState = 4;
         
         // Test
         //sprintf((char *)ucDebug, "%.2f C%c", appData.fTempResult, '\0');
         //vDebug(ucDebug, D_GREEN);
         //by1351_DrawText(0, 115, ucDebug, 0x1f, 0x15, 0x1f, 0x00, 0x00, 0x00, FONT_ALPS, 1, ATTR_NONE);
      break;  
      case 4:       
         if (appData.uiTempCount > 50)
         {
            appData.uiTempCount = 0;
            appData.uiTempState = 5;
         }
         else
            appData.uiTempCount++;
      break;        
      case 5:       
         appData.uiTempState = 1;
      break;  
      default:
         appData.uiTempState = 1;
   }  
   return (RC_OK);
}   
   
//*********************************************************************************************************************
//   
 
