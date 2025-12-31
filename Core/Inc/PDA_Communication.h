#ifndef __PDA_COMMUNICATION_H__
#define __PDA_COMMUNICATION_H__

#include "main.h"
#include "rs485.h"

#define HANDSHAKE_CMD 0x01
#define HEARTBEAT_CMD 0x02
#define CONTROL_CMD 0x03
#define PARAM_READ_CMD 0x04
#define PARAM_WRITE_CMD 0x07


extern uint32_t DistanceThresholdLearningReqFlg;
extern uint32_t ClosingTimeLearningReqFlg;
extern uint8_t now_DistanceThresholdLearning_Flg;
extern uint8_t now_ClosingTimeLearning_Flg;

extern uint8_t STOP_DistanceThresholdLearning_ReqFlg;  
extern uint8_t  STOP_ClosingTimeLearningReqFlg;

uint16_t CalcCRC16(uint8_t *data, uint16_t len);

void WriteU32LittleEndian(uint8_t *buf, uint32_t value);
void WriteU16LittleEndian(uint8_t *buf, uint32_t value);

void ProcessPacket(SensorProtocol *pkt);

void HandleHandshake(SensorProtocol *pkt);

void HandleHeartbeat(SensorProtocol *pkt);


void HandleControl(SensorProtocol *pkt);

void HandleParamRead(SensorProtocol *pkt);

void HandleParamWrite(SensorProtocol *pkt);






#endif