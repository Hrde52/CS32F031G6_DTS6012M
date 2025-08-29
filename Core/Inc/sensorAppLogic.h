#ifndef __SENSOR_APP_LOGIC__H__
#define __SENSOR_APP_LOGIC__H__

#include "dts6012m.h"
#include "main.h"
#include "nd06av1c_app.h"

#define Unknown_STATUS                                         -1
#define NotDetected_STATUS                                      0
#define NormalWorking_STATUS                                    1
#define DistanceThresholdLearning_STATUS                        2
#define ClosingTimeLearning_STATUS                              3
#define Idle_STATUS                                             4
#define Fault_STATUS                                            5
#define setUp_STATUS                                            6

typedef union {
    struct
    {
        uint32_t bit0 : 1;
        uint32_t bit1 : 1;
        uint32_t bit2 : 1;
        uint32_t bit3 : 1;
        uint32_t bit4 : 1;
        uint32_t bit5 : 1;
        uint32_t bit6 : 1;
        uint32_t bit7 : 1;
        uint32_t bit8 : 1;
        uint32_t bit9 : 1;
        uint32_t bit10 : 1;
        uint32_t bit11 : 1;
        uint32_t bit12 : 1;
        uint32_t bit13 : 1;
        uint32_t bit14 : 1;
        uint32_t bit15 : 1;
        uint32_t bit16 : 1;
        uint32_t bit17 : 1;
        uint32_t bit18 : 1;
        uint32_t bit19 : 1;
        uint32_t bit20 : 1;
        uint32_t bit21 : 1;
        uint32_t bit22 : 1;
        uint32_t bit23 : 1;
        uint32_t bit24 : 1;
        uint32_t bit25 : 1;
        uint32_t bit26 : 1;
        uint32_t bit27 : 1;
        uint32_t bit28 : 1;
        uint32_t bit29 : 1;
        uint32_t bit30 : 1;
        uint32_t bit31 : 1;
    } bits;
    uint32_t data;
} ERR_DATA;

extern ERR_DATA ErrData;

#define	E001	ErrData.bits.bit0
#define	E002	ErrData.bits.bit1
#define	E003	ErrData.bits.bit2
#define	E004	ErrData.bits.bit3
#define	E005	ErrData.bits.bit4
#define	E006	ErrData.bits.bit5
#define	E007	ErrData.bits.bit6
#define	E008	ErrData.bits.bit7
#define	E009	ErrData.bits.bit8
#define	E0010	ErrData.bits.bit9
#define	E0011	ErrData.bits.bit0
#define	E0012	ErrData.bits.bit11
#define	ERR_D	ErrData.data

extern uint32_t sensor_status;
extern uint8_t ObjectIsDetectedFlag;
extern DTS6012_DATA dts6012_data;
extern ND06_DATA nd06_data;
extern uint32_t DistanceThresholdLearningSuccessFlg;
extern uint32_t dts6012DistancdLearnValue;
extern uint32_t nd06DistancdLearnValue;
extern uint32_t ClosingTimeLearningSuccessFlg;
extern uint32_t closeTimeCnt;
extern int8_t nd06AV1C_objDetectFlag;
extern uint8_t DistanceThresholdLearningFailed_Flg;
extern uint8_t ClosingTimeLearningFailed_Flg;

extern uint8_t IO_dts6012;
extern uint8_t IO_ND06;
extern uint8_t fingerDetected_flag;
extern uint8_t regionalObjectDetected_flag_ND06;
extern uint8_t masterElevator_OF;
extern uint8_t stopIOOutputFlag;

extern ERR_DATA ErrData;

void DistanceThresholdLearning(void);
void cargolift_DistanceThresholdLearning(void);
void ClosingTimeLearning();

void ObjectDetection(void);
void cargoLift_ObjectDetection(void);

// sensor_status = Idle_STATUS;
void sensor_status_control(void);
// sensor_status: LED, ObjectDetect, Learning
void sensor_action_control(void);


void cargoLift_sensor_status_control(void);

void cargoLift_sensor_action_control(void);

extern uint32_t objectDCnt_P;
extern uint32_t objectDCnt_H;

void ND06_Reset(void);
uint8_t detectError();

#endif
