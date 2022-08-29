//*********************************************************************************************************************
// OBDgauge 3
//  
//
//*********************************************************************************************************************
//  
//  
//*********************************************************************************************************************
// To do:
//
//   
//
//
//
//*********************************************************************************************************************
//

#ifndef _APP_H
#define _APP_H

 
//*****************************************************************************
// Section: Included Files
//
//*****************************************************************************
// 

#include       <xc.h>
#include       <p32xxxx.h>
#include	      <ctype.h>
#include       <stdio.h>
#include       <string.h>
#include       <stdlib.h>
#include       <math.h>
#include       <float.h> 
#include       <stdbool.h>
#include       "system_definitions.h"
#include       "system/common/sys_module.h"   // SYS function prototypes
#include       <peripheral/peripheral.h>
#include       <stddef.h>                     // Defines NULL
#include       <stdint.h>
#include       "system_config.h"
#include       "ssd1351_drv.h"
#include       "og_hardware.h"
#include       "peripheral/adc/plib_adc.h"    // plib ADC
#include       "peripheral/devcon/plib_devcon.h"
#include       "peripheral/reset/plib_reset.h"
#include       "switch.h"
#include       "display.h"
#include       "system_config.h"
#include       "driver/driver_common.h"
#include       "system_definitions.h"
 
 

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif 
typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT = 0,
	APP_STATE_SERVICE_TASKS,

	/* TODO: Define states used by the application state machine. */

} APP_STATES;

//*********************************************************************************************************************
// Defines
//
//*********************************************************************************************************************
//

// Version
#define                          VERSION                    "3.4.0"

// Debug screen colors
#define                          D_GREEN                    0x00
#define                          D_YELLOW                   0x01
#define                          D_RED                      0x02
#define                          D_PURPLE                   0x03
#define                          D_BLUE                     0x04

// Intro screens on-demand 
#define                          INTROPIC_MIN               1
#define                          INTROPIC_MAX               66


//*********************************************************************************************************************
//

#define                          PI                         3.14159265358979323846              // Pi
#define                          MAX_BUF                    256
#define                          HALF_BUF                   128

//*********************************************************************************************************************
// APRS

#define                          APRS_ARRAYSIZE             50                   // Number of station cells
#define                          APRS_MAXAGE                20                   // Minutes before entry is cleared
#define                          APRS_CALLSIGN              "LA1PCA\0"           // Callsign
#define                          APRS_CALLSIGN_EXT          "-9\0"               //
#define                          APRS_SYMBOL                "/j"                 // Map symbol (Jeep)
#define                          APRS_PATH                  "WIDE1-1\0"          // Routing path
#define                          APRS_POWER                 1                    // Relative output power
#define                          APRS_PERIOD                61                   // Seconds between non-moving transmissions
#define                          APRS_TXFREQ                "144.800\0"          // Transmit frequency MHz
#define                          APRS_RXFREQ                "144.800\0"          // Receive frequency MHz

#define                          APRS_STALE                 600                  // Don't count station if older than n seconds
#define                          X_CENTER                   64                   // Display center
#define                          Y_CENTER                   64                   // Display center

#define                          CALLSIGN_DB                "amator.csv"         // Callsign database

//*********************************************************************************************************************
// Bluetooth

#define                          BT_NAME                    "\"Jeep TJ\"\0"      // Device name
#define                          BT_PIN                     "2001"               // Device PIN
#define                          BT_ARRAYSIZE               20                   // Number of devices

//*********************************************************************************************************************
// Gyro

#define                          GYRO_FACTOR                260                  // Gyro calibration

//*********************************************************************************************************************
// Geiger

#define                          GEIGER_ARRAYSIZE           120                  // 10 minutes, 5 sec slots
#define                          SIEVERT_RATE               2.85714              // Nanosievert/h per pulse, 2 tubes  

//*********************************************************************************************************************
// WIFI

#define                          WIFI_DNS                   "8.8.8.8\0"          // DNS server              
#define                          WIFI_CLOUD                 "eternalbank.com\0"  // Cloud server            

//*********************************************************************************************************************
// GSM

#define                          GSM_KEYWORD                "jp \0"              // Keyword to submit commands              
#define                          GSM_PRIMARY                "+4746644580\0"      // Primary 'owner' telephone number
#define                          GSM_SERVICE                "+4792001000\0"      // Service center number
 
//*********************************************************************************************************************
// NVRAM

// 2048
//#define                          NVM_ADDRESS                0x9D1F0000           // Start address 
// 512
#define                          NVM_ADDRESS                0x9d07e000           // Start address 

#define                          NVM_MAGIC                  0xcafebabe           // Valid config header

//*********************************************************************************************************************
// DATABASES

#define                          DB_IP_LOC                  "db-ip-city-lite-2020-11.csv"
#define                          DB_MERSENNE_RND            "/mersenne_data/rnd_44_1khz"

//*********************************************************************************************************************
// Status flags

#define                          STAT_OK                    0b0000000000000000         // Component status OK
#define                          STAT_CURR                  0b0000000000000001         // Current out of spec
#define                          STAT_VOLT                  0b0000000000000010         // Voltage out of spec
#define                          STAT_SENS                  0b0000000000000100         // No Sensor module
#define                          STAT_GPS                   0b0000000000001000         // No GPS
#define                          STAT_GEIG                  0b0000000000010000         // No Geiger
#define                          STAT_GYRO                  0b0000000000100000         // No Gyro
#define                          STAT_APRS                  0b0000000001000000         // No APRS
#define                          STAT_WIFI                  0b0000000010000000         // No WiFi
#define                          STAT_CONT                  0b0000000100000000         // No Contrast AD
#define                          STAT_GSM                   0b0000001000000000         // No GSM
#define                          STAT_SD                    0b0000010000000000         // No SD card


//*********************************************************************************************************************
// Config settings

// Configuration byte 1

#define                          CFG_OFF                    0b00000000           // All off
#define                          CFG_GEIGER                 0b00000001           // Sensor board geiger sound on
#define                          CFG_AL_AUDIO               0b00000010           // All alarms audio on
#define                          CFG_AL_VISUAL              0b00000100           // All alarms visuals on

// To test exception handler
//uint32_t* pCrashme;  

#define                          BATT_LIMIT                 11.6                 // Battery alarm limit

//*********************************************************************************************************************
// NVRAM configuration structure
//
//*********************************************************************************************************************
//

typedef struct  _Configuration
{
   unsigned int                  uiMagic;                         // Magic number - config exists (unsigned int - 0xcafebabe)
   // APRS
   unsigned char                 bCallsign[8];                    // APRS callsign
   unsigned char                 bCall_ext[4];                    // APRS callsign extension
   unsigned char                 bAprsSym[4];                     // APRS symbol
   unsigned char                 bAprsPath[12];                   // APRS path
   unsigned char                 bPower;                          // Relative TX power
   unsigned int                  bPeriod;                         // Standstill transmission frequency (minutes)
   unsigned char                 bTXfreq[8];                      // TX frequency ASCII
   unsigned char                 bRXfreq[8];                      // RX frequency ASCII
   // Bluetooth
   unsigned char                 bBTname[16];                     // BT device name
   unsigned char                 bBTpin[4];                       // BT pin
   // GSM
   unsigned char                 bGsmPrimary[20];                 // Primary owner phone number
   unsigned char                 bGsmService[20];                 // Cellphone SMS service number
   // WiFi
   unsigned char                 bDnsServer[16];                  // DNS server
   unsigned char                 bCloudServer[40];                // DNS name of cloud info server
   // Gyro level
   short                         iXoff;                           // Roll offset
   short                         iYoff;                           // Pitch offset
   
   int                           iDispBias;                       // Display contrast bias
   int                           iGeigerCal;                      // Geiger calibration (uSi factor)
   
   char                          cConfig1;                        // Config bits
   char                          cConfig2;                        // Alarm screen return delay
   unsigned int                  uiConfig3;                       // Config value future use
   float                         fBatteryLimit;                   // Low level battery alarm limit   
} NVM_CONFIG;

//*********************************************************************************************************************
// SDcard image sequence file header
 
typedef struct
{
	char                          cSignature[10];                  // Header signature
	unsigned long                 ulFrames;                        // Number of frames
	unsigned long                 ulFps;                           // Frames per second
	unsigned long                 ulFramesize;                     // Size of each frame
} HDR_STRUCT;

//*********************************************************************************************************************
// APP data
//
//*********************************************************************************************************************
//

#define                          SAT_ARRAYSIZE              32                  // 

// USB state
typedef enum
{
	APP_STATE_BUS_ENABLE = 0,
   APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE,
   APP_STATE_WAIT_FOR_DEVICE_ATTACH,
   APP_STATE_DEVICE_CONNECTED,
   APP_STATE_MOUNT_DISK,
   APP_STATE_UNMOUNT_DISK,
   APP_STATE_OPEN_FILE,
   APP_STATE_WRITE_TO_FILE,
   APP_STATE_CLOSE_FILE,
   APP_STATE_IDLE,
   APP_STATE_ERROR
} USB_STATES;

// WiFi UART
#define MAX_NUM_OF_BYTES            64
#define MAX_NUM_OF_BYTES_IN_BUF     (MAX_NUM_OF_BYTES + 4)

// Ham radio callsign database record structure
typedef struct
{
   uint8_t                             ucCallsign[8];
   uint8_t                             ucOrg[40];
   uint8_t                             ucName[20];
   uint8_t                             ucSurname[20];   
   uint8_t                             ucAddress[20];   
   uint8_t                             ucAddress2[20];   
   uint8_t                             ucZip[6];        
   uint8_t                             ucCity[20];        
   uint8_t                             ucCountry[10];              
} HAM_RECORD;

// Protocol structures

// WiFi station array
typedef struct
{
   uint8_t                             cEncryption;      // 0:Open 1:WEP 2:WPA 3:WPA2 4:Wpa1/2 5:WPA Enterprise 10: Open, being contacted 
   uint8_t                             cRssi;            // Signal strength
   char                                cSsid[32];        // AP name
   char                                cMac[18];         // Mac address  
   uint8_t                             cChannel;         // AP channel
} WIFI_DATA;

// GPS sat array
typedef struct
{
   int                                 iSat;
   int                                 iElevation;
   int                                 iAzimuth;
   int                                 iSignal;
} SAT_DATA;   

// APRS station array
typedef struct 
{
   uint8_t                             cLatitude[10];                // Latitude
   uint8_t                             cLongitude[11];               // Longitude
   short                               sAltitude;                    // Altitude
   double                              dDistance;                    // Distance from here
   double                              dBearing;                     // Bearing from here
   unsigned int                        uiTime;                       // Time of entry
   uint8_t                             cSymbol[4];                   // Symbol 0-FFFF
   uint8_t                             cCallsign[12];                // Callsign
   uint8_t                             cComment[120];                // Comment
} APRS_DATA;

// Bluetooth device array
typedef struct 
{
   uint8_t                             cName[20];                    // Name of device
   uint8_t                             cAddress[18];                 // Address of device
   uint8_t                             cRssi[4];                     // Signal -127..0
   uint8_t                             cStatus;                      //      
} BT_DATA;

// 001 CID_ALIVE
typedef struct
{
   // Time, temp, random
   uint8_t                          cTime[6];
   uint8_t                          cDate[6];
   double                           dGsmTemp;
   short                            iGyroTemp;
   float                            fVoltage;                     // Sensor input 12v voltage (AD - Not used)
   long long                        llRandom;                     // Incoming random
   // Power/Pressure data (v4)
   double                           dTemperature;
   double                           dPressure;
   double                           dVoltage;
   double                           dCurrent;         
} LIFE_MSG;   
 
// 201 CID_GEIGER
typedef struct
{
   int                              iCounts;                      // Counts 
   int                              iCounts5;                     // Counts last 5 seconds 
   int                              iCountsMin;                   // Calculated counts per minute
   int                              iAve10;                       // Average CPM last 10 minutes
   float                            fRadLevel;                    // Radiation level uSi
   float                            fRad10;                       // Radiation level uSi average over 10 minutes
} GEIGER_MSG;

// Local geiger graph array
typedef struct 
{
   int                              iCountsMin;
   int                              iAve10;
} GEIGER_GRAPH;   

// 212 CID_GPS_POS
typedef struct
{
   double                           dGpsLatDec;                   // Decimal latitude
   double                           dGpsLonDec;                   // Decimal longitude   
   double                           fGpsSpeed;                    // Speed
   double                           fGpsBearing;                  // Bearing   
   short                            sGpsSatsUsed;                 // Sats used
   short                            sGpsSatsSeen;                 // GPS sats currently seen
   double                           dGpsDop;                      // Current DOP
   double                           dGpsAlt;                      // Altitude
   short                            dGpsMode;                     // Gps mode      
   uint8_t                          cMaidenhead[8];               // Maidenhead grid locator
} GPS_POS_MSG;

// 216 CID_GSMS 
typedef struct
{
   uint8_t                          cGsmRegistered;               // 1: registered to network
   short                            iGsmSignal;                   // Signal strength  
   uint8_t                          ucGsmOperator[20];            // Operator  
   uint8_t                          cJamming;                     // Jamming status
   short                            iGsmMode;                     // GSM protocol mode (0-24) 
   uint8_t                          cGsmOnline;                   // Module is responding
   short                            iMisc1;                       // Future use
   short                            iMisc2;                       // Future use
} GSMS_MSG;


// 218 CID_GSM 
typedef struct
{
   short                            iGsmMsgSize;                  // Message size
   uint8_t                          ucGsmSender[20];              // Sender phone number
   uint8_t                          ucGsmMessage[160];            // Last message     
} GSM_MSG;


//*********************************************************************************************************************
//  APP data pool
//  
//*********************************************************************************************************************
//

typedef struct
{   
   // Main 
   unsigned int                     uiAppState;                   // 0: not initiated  1: sleeping   2: running
   uint8_t                          uiTest;                       // True when testmode
   unsigned long                    ulLife;                       // Sequence timer counter 1ms
   unsigned long                    ulLonglife;                   // Sequence timer counter 1 sec  
   unsigned long                    ulResetCounter;               // Reset trigger counter
   uint8_t                          uiMiscState;                  // Misc. tasks in main state machine     
   unsigned long                    ulMiscCount;                  // Misc. tasks pacer  
   uint8_t                          cDebug;                       // Local debug flag
   uint8_t                          cStarted;                     // App started, enable switch
   char                             sError[64];                   // System error message
   unsigned long                    ulLastCfgChange;              // Time of last config change
   uint8_t                          cCfgChanged;                  // Config changed, send to sensors
   uint8_t                          cResetFlag;                   // Device has been reset
   unsigned short                   usStatus;                     // Component status flags
   
   // NVRAM data
   uint8_t                          uiNvramState;
   DRV_HANDLE                       hNvram;                       // Handle to NVRAM driver   
   DRV_HANDLE                       hNvmCmdHandle;                // NVM command handle
   unsigned long                    ulConfigSent;                 // Config sent to sensors delay
   
   // USB data
   uint8_t                          uiUsbState;           
   SYS_FS_HANDLE                    fileUsbHandle;
   USB_STATES                       usbState;
   //uint8_t                          data[1024]; kbdoo
   uint32_t                         nBytesWritten;
   uint32_t                         nBytesRead;
   bool                             deviceIsConnected;
       
   //Display
   uint8_t                          uiDisplayState;      
   uint8_t                          uiDisplayPrev;                // Previous visible screen      
   uint8_t                          uiDisplayOld;                 // Previous visible screen saved      
   uint8_t                          uiDisplayScreen;              // Current visible screen   
   uint8_t                          uiPushed;                     // Holds push status, cleared in display routine
   uint8_t                          uiDispFunc;                   // Context for buttons
   uint8_t                          uiDisplaySubscreen;           // Subscreen, 0 if not active. Toggled with push
   uint8_t                          uiStation;                    // APRS subscreen station page
   uint8_t                          cRefresh;                     // One when refreshing, no input
   uint8_t                          cIntroDone;                   // Intro screen written
   int                              iIntroPic;                    // Subscreen pic
   uint8_t                          cDateAlert;                   // 1 when date alert done
   uint8_t                          cSetting;                     // Settings screen menu line number
   uint8_t                          cEditFreq;                    // Editing frequency
   uint8_t                          cEditBias;                    // Editing display bias   
   uint8_t                          cEditGeiger;                  // Editing Geiger calibration   
   uint8_t                          cEditAlmDelay;                // Editing alarm screen return delay, seconds, 0 is off
   uint8_t                          uiDisplayLock;                // Draw spinlock
   uint8_t                          cChangedScreen;               // Switched, display clear signal
   int                              iDispBias;                    // Volatile disp bias
   int                              iGeigerCal;                   // Volatile Geiger calibration factor
   uint8_t                          ucAbortShow;                  // Stop show due to switch change   
   uint8_t                          ucShowNext;                   // Button pushed, go to next video  
   uint8_t                          ucSmsSetting[30];             // Setting text in status line
   
   // RGB array
   unsigned long                    ulRGB[LED_SERIAL_NUM];        // RGB LED array values

   // Display
   uint8_t                          uiCurrentContrast;   
   uint8_t                          uiDmenu;                      // Current menu page
   unsigned long                    ulDispDelay;                  // Delay counter  

   double                           dAngle;                       // Variometer arrow angle
   uint8_t                          ucTimeSet;                    // Flag if time set from GPS
   
   // SD card
   uint8_t                          uiSdState;
   uint8_t                          ucFile[HALF_BUF];
   uint8_t *                        pbBuffer;
   unsigned long                    cCount;
   int                              iFilesize;
   unsigned char                    cSdBusy; 
   volatile SYS_FS_HANDLE           fileHandle;
   uint8_t                          ucSdType;
   uint8_t                          ucSdReady;
    
   // WiFi   
   uint8_t                          uiWifiState;           
   uint8_t                          uiWifiPrevstate;           
   unsigned int                     uiWifiTimeout;
   DRV_HANDLE                       drvWifiHandle;
   char                             ucWifiTempbuff[1024];
   char                             ucWifiTxbuff[1024];
   char                             ucWifiRxbuff[4096];           // Should be 4096
   int                              iTxLen;                       // Tx length
   char                             ucWifiMsg[2048];
   unsigned int                     uiWifiRxptr;
   unsigned int                     uiStreamlength;               // 
   uint8_t                          cStream;                      // Stream flag
   char                             cHdr[5];                      // Stream header ID     
   int                              iWifiStatus;                  // Flags: connected etc.
   WIFI_DATA                        sWifiAp[64];                  // Array of seen APs   
   uint8_t                          cApPtr;                       // Pointer in array
   uint8_t                          cWchanged;                    // New AP list   
   char                             cLastSsid[33];                // Last connected SSID
   unsigned long                    ulConntime;                   // Timeout
   
   // Turn-switch
   unsigned long                    ulLastChange;                 // Debounce
   unsigned long                    ulLastRight;                  // Debounce
   unsigned long                    ulLastLeft;                   // Debounce
   uint8_t                          uiButtonState;                // Current button state
   unsigned int                     uiValue;                      // Turn up/down counter  
   unsigned long                    ulPushlife;                   // Detect long push 
   unsigned long                    ulButtontime;                 // Time stamp of last twist action
   uint8_t                          cSwitchPrev;                  // Last bits
     
   // PWM & timers
   uint8_t                        * pbSoundbuff1;                 // Sound buffer 1
   uint8_t                        * pbSoundbuff2;                 // Sound buffer 2
   DRV_HANDLE                       handleTmr;                    // OC1 driver handle
   volatile uint8_t                 bSndReady;                    // File ready
   unsigned long                    ulSptr;                       // Sound buffer pointer 
   volatile uint8_t                 bVolume;                      // Volume multiplier
   uint8_t                          cTmr5State;                   // State of output
   volatile uint8_t                 cPwmDone;                     // Done with output
   volatile uint8_t                 ucReadSndBuff;                // Positive if sound buffer must be read
   volatile unsigned long           ulSndBytes;                   // Number of bytes read    
   volatile uint8_t                 ucSoundFirst;                 // Flag for playing initial buffer
   
    // Temperature
   uint8_t                          uiTempState;                  // 0: idle 1: Start sampling 2: Busy 3: Result ready           
   float                            fTempResult;                  // Temperature
   unsigned char                    ucTserial[8];                 // 64-bit serial number
   unsigned int                     uiTempCount;                  // Temperature read pacer
   
   // Sensors
   double                           dPressure;                    // Bosch BME280
   double                           dTemperature;                 // Bosch BME280
   double                           dVoltage;                     // TI INA219
   double                           dCurrent;                     // TI INA219
   double                           dOldPressure;                 // Bosch BME280
   double                           dOldTemperature;              // Bosch BME280
   double                           dOldVoltage;                  // TI INA219
   double                           dOldCurrent;                  // TI INA219
   unsigned long                    ulSensLife;                   // Last sensors display update
      
   // Auto contrast AD
   unsigned long                    ulADvalue;                    // AD return
   unsigned long                    ulADHvalue;                   // AD hold value
   unsigned long                    ulADAvalue;                   // AD hold value
   uint8_t                          uiContrastState;              // 0: idle 1: Start sample 2: Wait 3: Calculate contrast          
   uint8_t                          uiContrast;                   // Current display contrast (1-10)
   uint8_t                          uiContrastCount;              // Reset safety
   uint8_t                          uiContrastAve;                // Average count
   
   // RGB LED blink counters
   uint8_t                          uiRblink;                     // Red blink numbers
   uint8_t                          uiRblinkId;                   // Red blink LED ID
   uint8_t                          uiGblink;                     // Green blink numbers
   uint8_t                          uiGblinkId;                   // Green blink LED ID
   uint8_t                          uiBblink;                     // Blue blink numbers
   uint8_t                          uiBblinkId;                   // Blue blink LED ID
   
   // Altitude average for pressure calibration
   uint8_t                          ucCalDone;                    // Calibration is done
   uint8_t                          ucCalStarted;                 // Calibration is ongoing
   unsigned long                    ulAltNum;                     // Number of values accumulated
   unsigned long                    uiCalStart;                   // Start time for calibration
   double                           dAltAccu;                     // Accumulator
   double                           dCalAltitude;                 // Resulting averaged altitude
   double                           dCalPressure;                 // Saved pressure at calibrated altitude
   double                           dSealevel;                    // Normalized pressure at sealevel
   double                           dPressAlt;                    // Calculated pressure altitude
   double                           dAltChange;                   // Rate of change  meters/minute
   double                           dPrevPressAlt;                // Previous pressure altitude
   long                             ulAltChangeTime;              // Time of current calculation
   long                             ulLastAltChange;              // Time of last calculation
   int                              iAltScale;                    // Scale value
   int                              iPrevAltScale;                // Previous Scale value
   unsigned int                     uiPressState;                 // Pressure state machine               
   
   double                           fAngle;  // Test
   
   //********************************************************************
   // Common with sensors
   //********************************************************************
   // Random
 
   short                            sRandomCnt;
   long long                        llRandom;                     // From sensor board
   long long                        llRandom2;                    // From display
   
   float                            fVoltage;                     // Sensor board 12v value
   
   //********************************************************************
   // Gyro
   // Data
   short                            iGyroTemp;
   short                            iX;
   short                            iY;
   short                            iZ;
   short                            iOldX;                        // Prev value for spike detection   
   short                            iOldY;                        // Prev value for spike detection   
   short                            iPitch;                       // Pitch in degrees, negative is down
   short                            iRoll;                        // Roll in degrees, negativ is left
   short                            iPrevly;
   short                            iPrevry;
   unsigned long                    ulGyroPace;                   // ulLife time between each average update
   short                            iGyroCount;                   // Number of accumulated values
   long                             ulAccuX;                      // Average accumulator
   long                             ulAccuY;                      // Average accumulator
   
   //********************************************************************
   // Geiger
   uint8_t                          uiGeigerState;
   int                              iCounts;                      // Counts 
   int                              iCounts5;                     // Counts last 5 seconds 
   int                              iCountsMin;                   // Calculated counts per minute
   int                              iAve10;                       // Average CPM last 10 minutes
   float                            fRadLevel;                    // Radiation level uSi
   float                            fRad10;                       // Radiation level uSi average over 10 minutes
   GEIGER_GRAPH                     sGraph[128];                  // Graphing value array
   uint8_t                          cGraphPtr;                    // Pointer in graphing array
   uint8_t                          cGotGeiger;                   // Got new message
   
   //********************************************************************
   // CAN
   uint8_t                          uiCanState;
   uint8_t                          uiCanUp;                      // 1 if bus available
   uint8_t                          ucCanTXbuff[10];               // CAN TX buffer
   uint8_t                          ucCanRXbuff[10];               // CAN RX buffer
   uint8_t                       *  pucCanTXbuff;                 // Complete TX buffer
   uint8_t                       *  pucCanTXptr;                  // TX pos pointer
 
   // Protocol
   uint8_t                       *  pucCanRXbuff;                 // Complete RX buffer
   uint8_t                       *  pucCanRXptr;                  // RX pos pointer
   uint8_t                       *  pCanBuff;                     // Pointer to sequence buffer
   uint8_t                          ucCanTrans;                   // Single message or part of sequence
   uint8_t                          ucCanType;                    // Sequence message type
   unsigned int                     u1CanCurrent;                 // Current package number, header is 0
   unsigned int                     uiCanTotal;                   // Total number of packages in addition to header
   uint8_t                          cCanRemnant;                  // Bytes in last package
   unsigned int                     uiCanSize;                    // Size of transmission, bytes
   uint8_t                          cChecksum;                    // Expected checksum of received sequence
   uint8_t                          can_message_buffer0[512];     // Used by CAN
   unsigned long                    ulTxCount;                    // Total sent frames
   unsigned long                    ulRxCount;                    // Total received frames
   
   uint8_t                          cCanSendScreen;               // VDU screen changed, change message cadence
   
   
   //********************************************************************
   // GPS 
 
   uint8_t                          cGpsTime[7];
   uint8_t                          cGpsDate[7];
   unsigned int                     uiGpsTime;
  
   uint8_t                          cGpsLatitude[10];             // N/Sddmm.mmmm
   uint8_t                          cGpsLongitude[11];            // E/Wdddmm.mmmm
   double                           dGpsLatDec;                   // Decimal latitude
   double                           dGpsLonDec;                   // Decimal longitude
   uint8_t                          cGpsSpeed[5];                 // xxx.x km/h
   uint8_t                          cGpsBearing[5];               // xxx.x degrees
   double                           fGpsSpeed;                    // Speed
   double                           fGpsBearing;                  // Bearing   
   uint8_t                          cGpsLastTime[6];              // Last fix time
   uint8_t                          cGpsLastDate[6];              // Last fix date
   short                            sGpsMode;                     // 0: No fix  1:2D  2:3D  3:PPS
   short                            sGpsSatsUsed;                 // Sats used
   short                            sGpsSatsSeen;                 // GPS sats currently seen
   short                            sGalileoSatsSeen;             // Galileo sats currently seen
   short                            sGlonassSatsSeen;             // Glonass sats currently seen
   short                            sBeidouSatsSeen;              // Beidou sats currently seen
   double                           dGpsDop;                      // Current DOP
   double                           dGpsAlt;                      // Altitude
   uint8_t                          cMaidenhead[8];               // Maidenhead grid locator
   uint8_t                          cGpsSendCount;                // Counter to trigger satellite in view CAN msg 

   SAT_DATA                         sData[SAT_ARRAYSIZE];         // 32 sats, GPS only    
    
   //********************************************************************
   // GSM 
   uint8_t                          uiGsmState;   
   uint8_t                          uiGsmOldstate;   
   uint8_t                          uiGsmStatecount;   
   
   DRV_HANDLE                       drvGsmHandle;   
   uint8_t                          ucGsmLast[6];                 // Last 4 chars to look for complete command reply   
   uint8_t                          uiGsmRxptr;
   uint8_t                          uiGsmSync;                    // 0: wait for sync 1: Synced
   uint8_t                          uiGsmDouble;                  // 1: expect cr/lf in reply
   uint8_t                          uiGsmGotMsg;                  // Got message, can be deleted
   uint8_t                          uiGsmPending;                 // 1: request is not finished
   uint8_t                          uiGsmGotMsgRdy;               // 1 if previous transaction took place
   uint8_t                          uiGsmPendingTx;               // 1 if TX message waiting on GPS   
   uint8_t                          uiGsmPendingTxCount;          // Waiting limit before sending without position   
   uint8_t                        * pcBtArray;                    // CAN BT devices pointer
          
   // Data
   uint8_t                          cGsmRegistered;               // 1: registered to network
   short                            iGsmSignal;                   // Signal strength
   short                            iGsmMode;                     // GSM protocol mode (0-24) 
   uint8_t                          ucGsmOperator[20];            // Operator   
   short                            iGsmMsgSize;                  // Message size
   uint8_t                          ucGsmSender[20];              // Sender phone number
   uint8_t                          ucGsmMessage[160];            // Last message  
   uint8_t                          ucGsmTemp[10];                // Module temperature 
   double                           dGsmTemp;                     // Double temperature   
   uint8_t                          ucGsmBtScan;                  // 1: scanning 
   short                            iGsmBtStatus;                 // 
   uint8_t                          cGsmOnline;                   // Module is present
   BT_DATA                          sBtData[BT_ARRAYSIZE];        // BT devices
   uint8_t                          cJamming;                     // 1: No jamming, 2: jamming, 3: interference
   uint8_t                          cBchanged;                    // Device checksum   
   uint8_t                          cSmsConfig;                   // Config message received,only once per session
   
   //********************************************************************
   // APRS 
   uint8_t                          uiAprsState;       
   uint8_t                          uiAprsTxCount;       
   DRV_HANDLE                       drvAprsHandle;
 
   unsigned int                     uiAprsRxptr;
   uint8_t                          uiAprsSync;                   // 0: wait for sync 1: Synced   
   uint8_t                          bAprsGot;         
   APRS_DATA                        tAprsEntry[APRS_ARRAYSIZE];   // Array
   uint8_t                          cAprsBeaconInit;
   uint8_t                          cAprsBeaconCount;   
   uint8_t                          cAprsPosCount;   
   uint8_t                        * pcArray;
   double                           fScale;                       // Display scale
   unsigned long                    ulLastzoom;                   // Time of last zoom change
   uint8_t                          dAprsStations;                // Number of stations on screen
   unsigned int                     uiAprsAge;                    // Last seen 
   uint8_t                          cPrevChecksum;                // Array change checksum
   uint8_t                          cAprsRefresh;                 // Radar refresh flag
   HAM_RECORD                       sHamRecord;                   // Station data from database
   
   //********************************************************************
   // Alarms
   uint8_t                          uiAlarmState;                 // Alarms checking state
   unsigned long                    ulAlarmDelay;                 // Alarms display delay
   uint8_t                          cAlarmScreen;                 // Alarms screen on, do not refresh regular screen
   uint8_t                          cSmsChecksum;                 // Last SMS checksum
   uint8_t                          cWifiConn;                    // Connected to server
   double                           dPrevalt;                     // Previous altitude
   uint8_t                          cLatdeg;                      // Degree part of position
   uint8_t                          cLondeg;                      // " 
   uint8_t                          cPrevLatdeg;                  // "
   uint8_t                          cPrevLondeg;                  // "
   
   unsigned long                    ulLastAttAlm;                 // Time of last attitude alarm
   unsigned long                    ulLastTempAlm;                // Time of last temperature alarm
   unsigned long                    ulLastAprsAlm;                // Time of last APRS alarm
   unsigned long                    ulLastWifiAlm;                // Time of last Wifi alarm
   unsigned long                    ulLastRadAlm;                 // Time of last radiation alarm
   unsigned long                    ulLastSpdAlm;                 // Time of last speed alarm
   unsigned long                    ulLastAltAlm;                 // Time of last altitude alarm
   unsigned long                    ulLastConfAlm;                // Time of last confluence alarm
   unsigned long                    ulLastPwrAlm;                 // Time of last power alarm (Volt/Current)
   
   uint8_t                          cReturnAlarm;                 // Return to original screen, seconds, 0 is off 
   unsigned long                    ulAlarmReturn;                // Time of any alarm
   uint8_t                          uiLastScreen;                 // Last screen before alarm
   
   
   //********************************************************************
   // Debug buffers
   char                             uiDebugbuffer0[17];
   char                             uiDebugbuffer1[17];
   char                             uiDebugbuffer2[17];
   char                             uiDebugbuffer3[17];
   char                             uiDebugbuffer4[17];
   char                             uiDebugbuffer5[17];
   char                             uiDebugbuffer6[17];
   char                             uiDebugbuffer7[17];
   
   char                             sTrapDebug[8];                // Hint in exception handler
                                  
} APP_DATA;

// Serial RGB led state
typedef struct
{
   uint8_t                          uiLedIndex;                   // RGB LED index number, 0-n
   uint8_t                          uiBlink;                      // Number of on/off events
   unsigned long                    ulColor;                      // RGB color value
} RGB_LED;
 

//*********************************************************************************************************************
//  
//  
//*********************************************************************************************************************
//

void vCAN_MsgRecv(void);
int CAN_State(void);
void vDebug(char *, uint8_t, uint8_t);
void vBeep(int);
void vShortDelay(unsigned long);     // Delay Time (CoreTimer Ticks)
void vLongDelay(unsigned long);      // Delay Time (milliseconds at 200mhz)

void vSetRGBled(unsigned long *, unsigned char, unsigned char, unsigned char, unsigned char);
void vClearRGBled(unsigned long *, unsigned long);

void APP_Initialize (void);
void APP_Tasks(void);

// Extra
void APP_SwitchReceiveEventHandler(void);
void vTmr5_event(void);
unsigned char ucInitOwTemp(void);
void vWifi_Initialize(void); 
void vTimersInit(void);
void vPwmInit(void);
void vNvram_State(void);
int SD_State(void);
int Temperature_State(void); 
void vDisplay_State(void);
void vMisc_State(void);
void vAD_Initialize(void); 
void vCan_Initialize(void); 
void vDelayUs(unsigned long);
unsigned long ulGetBmpImage(SYS_FS_HANDLE, uint8_t *);
uint8_t ucDisplayImage(uint8_t *);
uint8_t ucPlaySound(uint8_t *);
void vClearIndicator(uint8_t);
void vSetIndicator(uint8_t, uint8_t, uint8_t, uint8_t);
uint8_t vPicturefromSD(uint8_t *);
uint8_t vSoundfromSD(uint8_t *); 
void vWifi_State(void);
void vAlarmsInit(void);
void vAlarms_State(void);
void Auto_Contrast(void);
void vTrace(char *);

//*********************************************************************************************************************
//  
//  
//*********************************************************************************************************************
//


#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END
 
//*********************************************************************************************************************
//
 
// Backup of system interrupt file
 
/*
 

void __ISR(_UART4_TX_VECTOR, ipl2AUTO) _IntHandlerDrvUsartTransmitInstance0(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart0);
}
void __ISR(_UART4_RX_VECTOR, ipl2AUTO) _IntHandlerDrvUsartReceiveInstance0(void)
{
    DRV_USART_TasksReceive(sysObj.drvUsart0);
}
void __ISR(_UART4_FAULT_VECTOR, ipl2AUTO) _IntHandlerDrvUsartErrorInstance0(void)
{
    DRV_USART_TasksError(sysObj.drvUsart0);
}
 
  
 
void __ISR(_CHANGE_NOTICE_B_VECTOR, ipl1AUTO) _IntHandlerChangeNotification_PortB(void)
{

    APP_SwitchReceiveEventHandler();   // kbdoo keep
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CHANGE_NOTICE_B);
}

void __ISR(_RTCC_VECTOR, ipl1AUTO) _IntHandlerSysRtcc (void)
{
    SYS_RTCC_Tasks(sysObj.sysRtcc);
}
 
 
void __ISR(_TIMER_1_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_1);
}
 
void __ISR(_TIMER_5_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance1(void)
{
   vTmr5_event();          // kbdoo keep
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_5);
}
 

void __ISR(_SPI1_RX_VECTOR, ipl3AUTO) _IntHandlerSPIRxInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void __ISR(_SPI1_TX_VECTOR, ipl3AUTO) _IntHandlerSPITxInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void __ISR(_SPI1_FAULT_VECTOR, ipl3AUTO) _IntHandlerSPIFaultInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}

void __ISR(_OUTPUT_COMPARE_1_VECTOR, ipl7AUTO) _IntHandlerDrvOCInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_OUTPUT_COMPARE_1);
}

void __ISR(_CAN2_VECTOR, IPL3AUTO) _IntHandlerDrvCANInstance0(void)
{
    vCAN_MsgRecv();  // kbdoo keep
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CAN_2);
}  

// kbdoo keep
void __ISR(_TIMER_4_VECTOR, ipl5auto) timer4_handler(void)
{
   uint8_t                       cVal        = 0;
//   char                          ucTemp[30];
    
   IFS0bits.T4IF = 0;               // Clear interrupt flag for timer 

   // Check buffers
   if (appData.ulSptr > 4095)       // Switch buffer, flag for next read 
   {           
      if (appData.bSndReady == 1)
      {   
         appData.bSndReady = 2;
         appData.ucReadSndBuff = 1;
      }   
      else
      {   
         appData.bSndReady = 1;
         appData.ucReadSndBuff = 2;
      }   
      appData.ulSptr = 0;    
   }
  
   // Read first second buffer
   if (appData.ucSoundFirst == 1)
   {   
      appData.ucReadSndBuff = 2;
      appData.ucSoundFirst = 0;
   }   
   
   // Play
   if (!appData.cPwmDone)
   {          
      if (appData.bSndReady == 1)
         memcpy(&cVal, appData.pbSoundbuff1 + appData.ulSptr, 1);       // Output next byte to OC1
      else if (appData.bSndReady == 2)
         memcpy(&cVal, appData.pbSoundbuff2 + appData.ulSptr, 1);                       
      
      appData.ulSptr++;         
            
      OC1RS = cVal * 8;     
      
      // Check for end of file
      if ((appData.ulSptr >= appData.ulSndBytes) && (appData.ulSndBytes < 4096))
      {         
         vSetIndicator(2, 0x02, 0x00, 0x00);
    
         //sprintf(ucTemp, " %d %c", appData.uiWifiState, '\0');
         //by1351_DrawText(1, 40, (char *)ucTemp, 31, 61, 31, 0x00, 0x00, 0x00, FONT_VGA, 1, ATTR_NONE); 
         
         appData.cPwmDone = 1;
         DRV_TMR_Stop(appData.handleTmr); 
         free(appData.pbSoundbuff1);
         free(appData.pbSoundbuff2);
         appData.ucReadSndBuff = 0;
         appData.cSdBusy = 0;
         OC1RS = 0;
         OC1CON = 0;  
         SYS_FS_FileClose(appData.fileHandle); 
      }      
   }            
}


 */