//*********************************************************************************************************************
// Hardware defines
//
//
//*********************************************************************************************************************
//  CAN
//  PIC32MZ
//  128x128 OLED
//
//*********************************************************************************************************************
//  All source licensed under the MIT license unless 3rd party modules, see 'license.txt'
//
//*********************************************************************************************************************
//


//*********************************************************************************************************************
// Display port defines
//
//*********************************************************************************************************************
//

#define                          OLED_RESET                 LATFbits.LATF0    // !Reset 
#define                          OLED_DATA                  LATFbits.LATF1    // Data/!Command
#define                          OLED_RW                    LATDbits.LATD5    // Read/!Write  d5
#define                          OLED_ENABLE                LATDbits.LATD4    // Enable  d4
#define                          OLED_PWR                   LATDbits.LATD9    // Oled voltage reg. disable


// Board LEDs
#define                          LED_BLUE                   LATGbits.LATG6    // Blue board LED
#define                          LED_RED                    LATGbits.LATG7    // Red board LED
#define                          LED_GREEN                  LATGbits.LATG8    // Green board LED
#define                          LED_ON                     0                 // On is low
#define                          LED_OFF                    1                 // Off is high


// Serial display LEDs
#define                          LED_SERIAL                 LATDbits.LATD10   // RGB string WS2812B
#define                          LED_SERIAL_NUM             3                 // Number of LEDs in string

#define                          WIFI_RESET                 LATBbits.LATB8    // WiFi module reset, pulled high  
#define                          WIFI_ENABLE                LATBbits.LATB9    // WiFi module enable/!Power down  

// Defines speaker pin
#define                          SPEAKER                    LATGbits.LATG9    // Not needed, using OC1 output
#define                          OPTO_IN                    PORTBbits.RB3     // Opto-couple for 12v in   

// Twist-push button
#define                          BUTTON                     PORTBbits.RB11    // Push
#define                          TURN_R                     PORTBbits.RB13    // Turn right
#define                          TURN_L                     PORTBbits.RB12    // Turn left




