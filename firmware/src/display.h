
//*********************************************************************************************************************
//  
//
//*********************************************************************************************************************
//

#ifndef _DISPLAY_H_
#define _DISPLAY_H_ 
 
  
 
//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//
  

//*********************************************************************************************************************
// Screens

#define                          SCREEN_INTRO               0x00                 // Intro picture
#define                          SCREEN_SPEED               0x01                 // Speed, direction, maidenhead
#define                          SCREEN_POSITION            0x02                 // Position, altitude
#define                          SCREEN_GYRO                0x03                 // Gyro attitude
#define                          SCREEN_GEIGER              0x04                 // Geiger data, graph   
#define                          SCREEN_APRSRADAR           0x05                 // APRS radar
#define                          SCREEN_APRSLIST            0x06                 // APRS station list    
#define                          SCREEN_WIFI                0x07                 // WiFi data
#define                          SCREEN_SENSORS             0x08                 // Sensor data
#define                          SCREEN_SATS                0x09                 // Sats in view
#define                          SCREEN_GSM                 0x0a                 // GSM data
#define                          SCREEN_SYSTEM              0x0b                 // System data
#define                          SCREEN_SETTINGS            0x0c                 // Settings
#define                          SCREEN_SHOW                0x0d                 // Demo show

                                                                                  
// Geiger subscreen
#define                          SCREEN_GEIGER_SUB          0x20                 // Geiger graph

// APRS list subscreen
#define                          SCREEN_APRSLIST_SUB        0x30                 // Station details

// APRS radar subscreen
#define                          SCREEN_APRSRADAR_SUB       0x40                 // Manual zoom

// Intro subscreen
#define                          SCREEN_INTRO_SUB           0x50                 // Show images

// Settings subscreen
#define                          SCREEN_SETTINGS_SUB        0x60                 // Edit settings

// Position subscreen
#define                          SCREEN_POSITION_SUB        0x70                 // Altitude detail and graph

//*********************************************************************************************************************
//  
//                                                                                  
  
 





#endif


