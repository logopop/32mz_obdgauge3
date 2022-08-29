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
#include "can.h"
 
//*********************************************************************************************************************
// Globals
// 
//*********************************************************************************************************************
//
 
extern APP_DATA                  appData;
extern NVM_CONFIG                sConfig;

 
//*********************************************************************************************************************
// Sequence buffer available
// Put into correct struct
//*********************************************************************************************************************
//

void vFetchData(void)
{
   GPS_POS_MSG                   sGpsPosMsg;
   GEIGER_MSG                    sGeigerMsg;                  
   GSMS_MSG                      sGsmsMsg;               
   GSM_MSG                       sGsmMsg;
   LIFE_MSG                      sLifeMsg;  
//   uint8_t                       ucCbuff[8];                      // Incoming single buffer
   int                           i                          = 0;
   int                           j                          = 0;
   APRS_DATA                     sDataTemp;   
//   uint8_t                       k                          = 0;
//   uint8_t                       ucTemp[20];
//   short                         s;
   
   
   switch(appData.ucCanType)
   {   
      // Sequences
      case CID_ALIVE_H:                  
         memcpy(&sLifeMsg, appData.pCanBuff, sizeof(sLifeMsg));           
         memcpy(appData.cGpsTime, sLifeMsg.cTime, 6);
         memcpy(appData.cGpsDate, sLifeMsg.cDate, 6);
         sscanf((char *)appData.cGpsTime, "%d", &appData.uiGpsTime);
         appData.dGsmTemp = sLifeMsg.dGsmTemp;
         appData.iGyroTemp = sLifeMsg.iGyroTemp;  
         appData.fVoltage = sLifeMsg.fVoltage;
         appData.llRandom = sLifeMsg.llRandom;          
         appData.ulAltChangeTime = appData.ulLife;         
         appData.dTemperature = sLifeMsg.dTemperature;
         appData.dPressure = sLifeMsg.dPressure;
         appData.dVoltage = sLifeMsg.dVoltage;
         appData.dCurrent = sLifeMsg.dCurrent; 
         // Status
         appData.usStatus &= ~(STAT_SENS);
         if ((appData.dCurrent < 180) && (appData.dCurrent > 60))       
            appData.usStatus &= ~(STAT_CURR);
         if ((appData.dVoltage < 14.8) && (appData.dVoltage > 10.5))       
            appData.usStatus &= ~(STAT_VOLT);
         if (strlen(appData.cGpsTime) > 4);       
            appData.usStatus &= ~(STAT_GPS);                                             
         if ((appData.iGyroTemp) > 0.0)       
            appData.usStatus &= ~(STAT_GYRO);                                             
      break;            
      case CID_GEIGER_H:
         memcpy(&sGeigerMsg, appData.pCanBuff, sizeof(sGeigerMsg)); 
         appData.iCounts = sGeigerMsg.iCounts;
         appData.iCounts5 = sGeigerMsg.iCounts5;
         appData.iCountsMin = sGeigerMsg.iCountsMin;
         appData.iAve10 = sGeigerMsg.iAve10;
         appData.fRadLevel = sGeigerMsg.fRadLevel;
         appData.fRad10 = sGeigerMsg.fRad10;

         if (appData.iCounts5)
            appData.usStatus &= ~(STAT_GEIG);                                    

         // Add to array
         appData.sGraph[appData.cGraphPtr].iCountsMin = appData.iCountsMin;
         appData.sGraph[appData.cGraphPtr].iAve10 = appData.iAve10;
         appData.cGraphPtr++;
         if (appData.cGraphPtr > 127)
            appData.cGraphPtr = 0;    
         
         // Indicator
         if ((appData.uiDisplaySubscreen == SCREEN_GEIGER_SUB) && (appData.uiDispFunc == FUNC_SUBSCREEN))
         {
            vSetIndicator(0, 0x08, 0x00, 0x06);
            vLongDelay(3);
            vClearIndicator(0);
            appData.cGotGeiger = 1;
         }
      break;   
      case CID_GPS_SATS_H:
         memcpy(&appData.sData, appData.pCanBuff, sizeof(appData.sData));    
      break;   
      case CID_GPS_POS_H:
         memcpy(&sGpsPosMsg, appData.pCanBuff, sizeof(sGpsPosMsg));  
         appData.dGpsLatDec = sGpsPosMsg.dGpsLatDec; 
         appData.dGpsLonDec = sGpsPosMsg.dGpsLonDec; 
         appData.fGpsSpeed = sGpsPosMsg.fGpsSpeed; 
         appData.fGpsBearing = sGpsPosMsg.fGpsBearing; 
         appData.sGpsSatsUsed = sGpsPosMsg.sGpsSatsUsed; 
         appData.sGpsSatsSeen = sGpsPosMsg.sGpsSatsSeen; 
         appData.dGpsDop = sGpsPosMsg.dGpsDop; 
         appData.dGpsAlt = sGpsPosMsg.dGpsAlt; 
         appData.sGpsMode = sGpsPosMsg.dGpsMode;
         memcpy(appData.cMaidenhead, sGpsPosMsg.cMaidenhead, 8); 
                  
         //sprintf((char *)appData.uiDebugbuffer2, "%d %f %c", appData.sGpsSatsUsed, appData.dGpsAlt, '\0');
                                                    
      break;   
      case CID_GSMS_H:
         memcpy(&sGsmsMsg, appData.pCanBuff, sizeof(sGsmsMsg)); 
         appData.cGsmRegistered = sGsmsMsg.cGsmRegistered; 
         appData.iGsmSignal = sGsmsMsg.iGsmSignal; 
         memcpy(appData.ucGsmOperator, sGsmsMsg.ucGsmOperator, 20); 
         appData.cJamming = sGsmsMsg.cJamming;
         appData.iGsmMode = sGsmsMsg.iGsmMode;
         appData.cGsmOnline = sGsmsMsg.cGsmOnline;
         
         if (appData.cGsmOnline)
            appData.usStatus &= ~(STAT_GSM);                     
      break;   
      case CID_GSM_H: 
         memcpy(&sGsmMsg, appData.pCanBuff, sizeof(sGsmMsg));   
         appData.iGsmMsgSize = sGsmMsg.iGsmMsgSize;
         memcpy(appData.ucGsmSender, sGsmMsg.ucGsmSender, 20); 
         memcpy(appData.ucGsmMessage, sGsmMsg.ucGsmMessage, 160);                        
      break;   
      case CID_APRS_H:
         memcpy(&appData.tAprsEntry, appData.pCanBuff, sizeof(appData.tAprsEntry));        
         // Sort array by distance
         for (j = 0; j < (APRS_ARRAYSIZE - 1); j++)
         {
            for (i = 0; i < (APRS_ARRAYSIZE - 1 - j); i++)
            {
               if (appData.tAprsEntry[i].dDistance > .3)
                  appData.usStatus &= ~(STAT_APRS);   
               
               if (appData.tAprsEntry[i].dDistance > appData.tAprsEntry[i+1].dDistance)   // Swap
               {   
                  memcpy(&sDataTemp, &appData.tAprsEntry[i], sizeof(sDataTemp));
                  memcpy(&appData.tAprsEntry[i], &appData.tAprsEntry[i+1], sizeof(sDataTemp));
                  memcpy(&appData.tAprsEntry[i+1], &sDataTemp, sizeof(sDataTemp));
               }   
            }   
         }                          
      break;   
      case CID_BT_H:
         memcpy(&appData.sBtData, appData.pCanBuff, sizeof(appData.sBtData));             
      break;    
                              
            
   }   
}   
   
//*********************************************************************************************************************
// CAN message received
// Called from ISR routine
//*********************************************************************************************************************
//

/*

 In system_interrupt.c
 
void __ISR(_CAN1_VECTOR, IPL1AUTO) _IntHandlerDrvCANInstance0(void)
{
   vCAN_MsgRecv();
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CAN_1);
}

*/

void vCAN_MsgRecv(void)
{
   CAN_RX_MSG_BUFFER           * bpBuffer; 
//   uint8_t                       uiTemp[17]                 = {0};
   uint8_t                       ucMessage[8]               = {0};
//   unsigned int                  uiSize                     = 0;   
   uint8_t                       cChecksum                  = 0;
   unsigned int                  uiI                        = 0;
   unsigned int                  uiRemain                   = 0;
    
   
   LED_RED = LED_ON;
     
   if((PLIB_CAN_ModuleEventGet(CAN_ID_2) & (CAN_RX_EVENT)) != 0)
   {               
      appData.ulRxCount++;
      bpBuffer = (CAN_RX_MSG_BUFFER *) PLIB_CAN_ReceivedMessageGet(CAN_ID_2, CAN_CHANNEL1); 
      memcpy(ucMessage, bpBuffer->data, 8);
      // Receive state
      if (appData.ucCanTrans == CST_SEQUENCE)         // We're in a sequence
      {        
         uiRemain = appData.uiCanSize - (appData.pucCanRXptr - appData.pCanBuff);            
         if (uiRemain > 8)    // Not last
         {   
            memcpy(appData.pucCanRXptr, ucMessage, 8);
            appData.pucCanRXptr += 8;
         }
         else     // We're done                  
         {
            memcpy(appData.pucCanRXptr, ucMessage, uiRemain);
            
            // Generate checksum         
            for (uiI = 0; uiI < appData.uiCanSize; uiI++)
               cChecksum ^= appData.pCanBuff[uiI];      
            // Debug
            //sprintf(appData.uiDebugbuffer1, "%Id: %d %d  \0", bpBuffer->msgSID.sid, appData.uiCanSize);
            //sprintf(appData.uiDebugbuffer2, "%02x - %02x \0", appData.cChecksum, cChecksum); 
                         
            if (cChecksum == appData.cChecksum)
            {
               vFetchData();
               //vSetRGBled(appData.ulRGB, 1, 0x00, 0x00, 0x00);
            }   
            //else
            //   vSetRGBled(appData.ulRGB, 1, 0x02, 0x00, 0x00);
                                    
            appData.ucCanTrans = CST_SINGLE;
            free(appData.pCanBuff);
         }   
      }
      else                                            // Single message
      {                  
         appData.ucCanType = bpBuffer->msgSID.sid;
         switch (bpBuffer->msgSID.sid)
         {        
            // Header of sequence?
            case CID_ALIVE_H:
            case CID_CONFIG_H:
            case CID_GEIGER_H:
            case CID_GPS_SATS_H:
            case CID_GPS_POS_H:
            case CID_GSMS_H:
            case CID_GSM_H: 
            case CID_APRS_H:
            case CID_BT_H:
               appData.cChecksum = ucMessage[0];
               memcpy(&appData.uiCanSize, (unsigned int *)(ucMessage + 1), 4);  // Get sequence size
               appData.uiCanTotal = appData.uiCanSize / 8;
               appData.cCanRemnant = appData.uiCanSize%8; 
               appData.pCanBuff = malloc(appData.uiCanSize + 8);               
               if (appData.pCanBuff)
               {
                  appData.ucCanTrans = CST_SEQUENCE;
                  appData.pucCanRXptr = appData.pCanBuff;
               }  
            break;  
            case CID_GYRO:            
               appData.iOldX = appData.iX;
               appData.iOldY = appData.iY;
               memcpy(&appData.iX, ucMessage, 2);
               memcpy(&appData.iY, ucMessage + 2, 2);   
               
               appData.iGyroCount++;
               appData.ulAccuX += appData.iX; 
               appData.ulAccuY += appData.iY; 
           
               // Reduce noise by averaging
               if ((appData.ulLife - appData.ulGyroPace) > 100)      // Calculate and move
               {                                     
                  appData.iRoll =  (((short)(appData.ulAccuX / appData.iGyroCount) + sConfig.iXoff) / GYRO_FACTOR);
                  appData.iPitch = (((short)(appData.ulAccuY / appData.iGyroCount) + sConfig.iYoff) / GYRO_FACTOR);               
                  appData.ulGyroPace = appData.ulLife;
                  appData.ulAccuX = 0;
                  appData.ulAccuY = 0;
                  appData.iGyroCount = 0;
               }               
            break;   
            case CID_TICK:
               if (sConfig.cConfig1 & CFG_GEIGER)
                  vBeep(8);               
            break;               
            case CID_STATUS:
               // not used
            break;         
         }
      }           
      PLIB_CAN_ChannelUpdate(CAN_ID_2, CAN_CHANNEL1);       
   }
   //Clear CAN Interrupt flag     
   PLIB_CAN_ModuleEventClear(CAN_ID_2, CAN_RX_EVENT); 

   LED_RED = LED_OFF;
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

void vCanTransmit(short uiAddress, uint8_t * cMessage, uint8_t uiBytes)
{    
   CAN_TX_MSG_BUFFER           * msgBuffer                  = NULL;
   int                           status                     = CAN_TX_CHANNEL_TRANSMITTING;
   unsigned int                  uiWait                     = 0;   
      
   LED_RED = LED_ON;
   while ((status & CAN_TX_CHANNEL_TRANSMITTING) != 0)
   {   
      status = PLIB_CAN_TransmitChannelStatusGet(CAN_ID_2, CAN_CHANNEL0);
      uiWait++;
      if (uiWait > 10000)
         break;         
   }      
   if (uiWait < 10000)
   {
      uiWait = 0;
      while (msgBuffer == NULL)
      {   
         msgBuffer = PLIB_CAN_TransmitBufferGet(CAN_ID_2, CAN_CHANNEL0);
         uiWait++;
         if (uiWait > 10000)
            appData.uiCanUp = 0;
      }   
      if (uiWait < 10000)
      {  
         appData.uiCanUp = 1;
         memcpy(msgBuffer->data, cMessage, uiBytes);      
         msgBuffer->msgSID.sid = uiAddress;
         msgBuffer->msgEID.ide = 0;        
         msgBuffer->msgEID.data_length_code = uiBytes;
         PLIB_CAN_ChannelUpdate(CAN_ID_2, CAN_CHANNEL0);
         PLIB_CAN_TransmitChannelFlush(CAN_ID_2, CAN_CHANNEL0);  
         appData.ulTxCount++;
         appData.uiCanUp = 1;
      }   
   }     
}    
   
//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

void vCan_Initialize(void)
{
    
    /* Switch the CAN module ON */
    PLIB_CAN_Enable(CAN_ID_2);

    /* Switch the CAN module to Configuration mode. Wait until the switch is complete */
    PLIB_CAN_OperationModeSelect(CAN_ID_2, CAN_CONFIGURATION_MODE);
    while(PLIB_CAN_OperationModeGet(CAN_ID_2) != CAN_CONFIGURATION_MODE);
    
    PLIB_CAN_PhaseSegment2LengthFreelyProgrammableEnable(CAN_ID_2);

    //Set the Baud rate to 500 kbps
    PLIB_CAN_PropagationTimeSegmentSet(CAN_ID_2, 1-1);
    PLIB_CAN_PhaseSegment1LengthSet(CAN_ID_2, 4-1);   
    PLIB_CAN_PhaseSegment2LengthSet(CAN_ID_2, 4-1);   
    PLIB_CAN_SyncJumpWidthSet(CAN_ID_2, 1-1);
    PLIB_CAN_BaudRatePrescaleSet(CAN_ID_2, 9);     
 
    PLIB_CAN_MemoryBufferAssign(CAN_ID_2, appData.can_message_buffer0);

    PLIB_CAN_ChannelForTransmitSet(CAN_ID_2, CAN_CHANNEL0, 8, CAN_TX_RTR_DISABLED, CAN_LOW_MEDIUM_PRIORITY);
    PLIB_CAN_ChannelForReceiveSet(CAN_ID_2, CAN_CHANNEL1, 1, CAN_RX_FULL_RECEIVE);

    // Filter and mask
    PLIB_CAN_FilterConfigure(CAN_ID_2, CAN_FILTER0, 0x7ff, CAN_SID);
    PLIB_CAN_FilterMaskConfigure(CAN_ID_2, CAN_FILTER_MASK0, 0x0, CAN_SID, CAN_FILTER_MASK_IDE_TYPE);
    PLIB_CAN_FilterToChannelLink(CAN_ID_2, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
    PLIB_CAN_FilterEnable(CAN_ID_2, CAN_FILTER0);
    
    /* Setup CAN_ID_2 Interrupt */   
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_CAN2, INT_PRIORITY_LEVEL3);
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_CAN2, INT_SUBPRIORITY_LEVEL0);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_CAN_2);
    
    PLIB_CAN_ChannelEventEnable(CAN_ID_2, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY|CAN_RX_CHANNEL_FULL);
    PLIB_CAN_ModuleEventEnable(CAN_ID_2, CAN_RX_EVENT);//|CAN_RX_OVERFLOW_EVENT);
    
    // Normal
    PLIB_CAN_OperationModeSelect(CAN_ID_2, CAN_NORMAL_MODE);
    while(PLIB_CAN_OperationModeGet(CAN_ID_2) != CAN_NORMAL_MODE);
//    strcpy(appData.uiDebugbuffer0, "Init ok!\0");   
    
   
}   

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

void vCanSendBuffer(uint8_t * pcData, unsigned int uiSize, unsigned short sMessage)
{
   uint8_t                       ucBytes[8]                 = {0};
   uint8_t                       cChecksum                  = 0;
   uint8_t                       cRemnant                   = 0;
   unsigned int                  uiI                        = 0;
   unsigned int                  uiLoops                    = 0;   
   
   // Generate checksum
   for (uiI = 0; uiI < uiSize; uiI++)
      cChecksum ^= pcData[uiI];
      
   // Send transmission header
   ucBytes[0] = cChecksum; 
   memcpy(ucBytes + 1, (uint8_t *)&uiSize, 4);   
   vCanTransmit(sMessage, ucBytes, 5);
   
   uiLoops = uiSize / 8;
   cRemnant = uiSize%8; 
   
   for (uiI = 0; uiI < uiLoops; uiI++)
   {
      memcpy(ucBytes, pcData + (uiI * 8), 8);
      vCanTransmit(sMessage + 1, ucBytes, 8);
   }
   if (cRemnant)
   {
      memcpy(ucBytes, pcData + (uiI * 8), cRemnant);
      vCanTransmit(sMessage + 1, ucBytes, cRemnant);
   }   
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//

void vCan_State(void)
{             
   uint8_t                       ucBytes[8]                 = {0};
   
  	switch (appData.uiCanState)
   {
      case 0:        // Start
         appData.uiCanState = 1;         
      break;
      case 1:                
         // Send configuration struct
         if (((appData.ulLife - appData.ulConfigSent) > 15000) || (appData.cCfgChanged))   // every 15 seconds
         {                    
            vCanSendBuffer((uint8_t *)&sConfig, sizeof(NVM_CONFIG), CID_CONFIG_H); 
            appData.ulConfigSent = appData.ulLife;                 
            appData.cCfgChanged = 0;
         }   
         appData.uiCanState = 2;  
      break;                         
      case 2: 
         appData.uiCanState = 3;  
      break;           
      case 3: 
         //vCanReceive();
         appData.uiCanState = 4;  
      break;           
      case 4: 
         LED_RED = LED_OFF;            
         appData.uiCanState = 5;  
      break;           
      case 5: 
         // Send screen number if changed
         if (appData.cCanSendScreen)
         {
            ucBytes[0] = appData.cCanSendScreen;
            if (appData.uiDispFunc == FUNC_SUBSCREEN)
               ucBytes[1] = 1;
            else
               ucBytes[1] = 0;
            vCanTransmit(CID_SCREEN, ucBytes, 2);
            appData.cCanSendScreen = 0;
         }   
          
         //vCanTransmit(CID_ALIVE, ucBytes, 6);
         appData.uiCanState = 1;    
         
      break;           
   }   
 
}

//*********************************************************************************************************************
// 
// 
//*********************************************************************************************************************
//
















