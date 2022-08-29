
//*********************************************************************************************************************
// Change notifications
//
//*********************************************************************************************************************
//

#ifndef _SWITCH_H_
#define _SWITCH_H_ 
 

// Events
#define  BTN_RIGHT                  1
#define  BTN_LEFT                   2
#define  BTN_PUSH                   3
#define  BTN_RELEASE                4
#define  BTN_RELEASE_LONG           5

// Push actions
#define  ACT_PUSHED                 0x01
#define  ACT_RELEASED               0x02
#define  ACT_RELEASED_LONG          0x04

// Context function
#define  FUNC_SCREEN                1                             // Switching main screens
#define  FUNC_SUBSCREEN             2                             // Switching subscreens

// Menu screens
#define  NUM_SCREENS                13                            // Number of display main screens
#define  MAX_SETTING                8                             // Number of lines in the settings menu



//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//
  
void vSwitch_Initialize(void); 
  
  
  
  
#endif


