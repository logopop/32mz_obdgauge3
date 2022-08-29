
//*********************************************************************************************************************
// Common library  PIC32MX
//
//*********************************************************************************************************************
//

#ifndef _WIFI_H_
#define _WIFI_H_ 
 
  
 
//*********************************************************************************************************************
// 
//
//*********************************************************************************************************************
//

// Status flags
#define                          WIFI_OK                    0b0000000000000001           // Ok return
#define                          WIFI_ERROR                 0b0000000000000010           // Error return
#define                          WIFI_CONNECTED             0b0000000000000100           // Connected to AP
#define                          WIFI_GOTIP                 0b0000000000001000           // Got IP address
#define                          WIFI_DISCONN               0b0000000000010000           // Disconnected from AP
#define                          WIFI_STACONN               0b0000000000100000           // Someone connected to us
#define                          WIFI_STAADDR               0b0000000001000000           // Giving station an address
#define                          WIFI_STADISC               0b0000000010000000           // Someone disconnected from us
#define                          WIFI_ALREADYC              0b0000000100000000           // Already connected
#define                          WIFI_RESTART               0b0000001000000000           // Restarted
#define                          WIFI_RDYSEND               0b0000010000000000           // Ready to accept stream
#define                          WIFI_NETCONN               0b0000100000000000           // Connected to server
#define                          WIFI_SENTOK                0b0001000000000000           // Buffer was sent
#define                          WIFI_STATION               0b0010000000000000           // Someone connected to us

  
  
#endif







