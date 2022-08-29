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
#include "contrast.h"
 

//*********************************************************************************************************************
// Globals
// 
//*********************************************************************************************************************
//
  
extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;

 
//*********************************************************************************************************************
//  
//*********************************************************************************************************************
//
 

//*********************************************************************************************************************
// Init
// 
//*********************************************************************************************************************
//

//*********************************************************************************************************************
// Initialize AD
//
//*********************************************************************************************************************
//

void vAD_Initialize(void)
{
   // Calibration registers 
   ADC0CFG = DEVADC0;
   ADC1CFG = DEVADC1;
   ADC2CFG = DEVADC2;
   ADC3CFG = DEVADC3;
   ADC4CFG = DEVADC4;
   ADC7CFG = DEVADC7;

   // Reset ADCON1
   ADCCON1 = 0;  
   ADCCON1bits.SELRES = 3;    // Resolution 
   ADCCON1bits.STRGSRC = 1;   // Global software trigger
   ADCCON2bits.SAMC = 100;      // ADC0 sampling time 5
   ADCCON2bits.ADCDIV = 8;    // ADC0 clock frequency

   ADCANCON = 0;
   ADCANCONbits.WKUPCLKCNT = 5;
   ADCCON3 = 0;
   ADCCON3bits.ADCSEL = 0;    // Input clock source
   ADCCON3bits.CONCLKDIV = 3; 
   ADCCON3bits.VREFSEL = 0;   // Reference v source

   ADC4TIMEbits.ADCDIV = 4;
   ADC4TIMEbits.SAMC = 100;  
   ADC4TIMEbits.SELRES = 3;     
 
   ADCIMCON1bits.SIGN12 = 0;  // unsigned data
   ADCIMCON1bits.DIFF12= 0;   // Single ended

   ADCGIRQEN1 = 0;            // No interrupts
   ADCGIRQEN2 = 0;

   ADCCSS1 = 0; 
   ADCCSS2 = 0; 
   ADCCSS1bits.CSS4 = 1;      // Class set for scan

   ADCCMPCON1 = 0;            // No comparators
   ADCCMPCON2 = 0; 
   ADCCMPCON3 = 0;  
   ADCCMPCON4 = 0;
   ADCCMPCON5 = 0;
   ADCCMPCON6 = 0;

   ADCFLTR1 = 0;              // No oversampling filters
   ADCFLTR2 = 0;
   ADCFLTR3 = 0;
   ADCFLTR4 = 0;
   ADCFLTR5 = 0;
   ADCFLTR6 = 0;

   // Trigger source for AN4
   ADCTRG2bits.TRGSRC4 = 1;

   ADCEIEN1 = 0; // No early interrupts
   ADCEIEN2 = 0;
   
   // ADC ON
   ADCCON1bits.ON = 1;
 
   // Wait for Vref stable
   while(!ADCCON2bits.BGVRRDY);  
   while(ADCCON2bits.REFFLT);  
   
   // Enable the clock
   ADCCON3bits.DIGEN4 = 1;    // Enable ADC4
   ADCANCONbits.ANEN4 = 1;    // Enable clock to ADC4
   while( !ADCANCONbits.WKRDY4 );   // Wait until ADC4 is ready
 
}   


//*********************************************************************************************************************
// State 
// 
//*********************************************************************************************************************
//

//*********************************************************************************************************************
// Auto_Contrast
// Read AD light level, set OLED contrast + global contrast variable
//*********************************************************************************************************************
//

void Auto_Contrast(void)
{  
   
//   uint8_t                       uiContrastState;              // 0: idle 1: Start sample 2: Wait 3: Add up result 4: Calculate contrast          
//   uint8_t                       uiContrastCount;              // State delay count
//   unsigned long                 ulADvalue;                    // Accumulated AD value
//   uint8_t                       uiContrast;                   // Current display contrast (1-10)
   
   unsigned int uiLut[16] = {3500, 3770, 3772, 3774, 3776, 3778, 3780, 3785, 3790, 3795, 3810, 3830, 3850, 3870, 3900, 3950};
   unsigned int                  i                          = 0;
//   char                          cTemp[30]                  = {0};
   
   
   switch(appData.uiContrastState)
   {
      case 0:        // Idle
         appData.uiContrastState = 1;
         appData.ulADvalue = 0;
      break; 
      case 1:        // Start sample
         // Start conversion
         ADCCON3bits.GSWTRG = 1;
         appData.uiContrastState = 2;         
      break;
      case 2:        // Wait for result 
         if (ADCDSTAT1bits.ARDY4)             
            appData.uiContrastState = 3;            
         else
         {             
            appData.uiContrastCount++;
            if (appData.uiContrastCount > 20)  
            {  
               vAD_Initialize();
               appData.uiContrastState = 0;
            }   
         }           
      break;
      case 3:        
         appData.ulADAvalue += ADCDATA4;          
         appData.ulADHvalue = ADCDATA4;          
         appData.uiContrastAve++;   
         if (appData.ulADHvalue > 3100)
            appData.usStatus &= ~(STAT_CONT);           

         if (appData.uiContrastAve > 9)
         {
            appData.ulADvalue = appData.ulADAvalue / 10;   
            appData.ulADvalue += appData.iDispBias;
            for (i = 0; i < 15; i++)
            {
               if (uiLut[i] > appData.ulADvalue)
                  break;         
            }          

            // Contrast is 0x00 to 0x0f
            by1351_SetContrast(i);
            appData.uiContrastCount = 0;        
            appData.uiContrast = i;                     
            appData.uiContrastAve = 0;
            appData.ulADAvalue = 0;             
         }   
         appData.uiContrastState = 0;
      break;
              
      default:
         appData.uiContrastState = 0;   
    }
}   
 



















