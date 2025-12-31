#include "sensorAppLogic.h"

#include "PDA_Communication.h"
//#include "elevatorSignalMonitor.h"
#include "gpio.h"
#include "main.h"
//#include "productToolingInspection.h"
#include "rs485.h"
#include "sensorParaTable.h"
#include "string.h"

//#include "nd06av1c_comm.h"
//#include "nd06av1c_data.h"
//#include "nd06av1c_def.h"
//#include "nd06av1c_dev.h"
//#include "platform.h"

ERR_DATA ErrData = {0};
uint8_t  nd06_data = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
//DTS6012_DATA dts6012_data = {0, 0, 0, 0};

uint32_t DistanceThresholdLearningSuccessFlg = 0;
uint32_t dts6012DistancdLearnValue = 0;
uint32_t dts6012DistanceThreshold = 500;

uint32_t nd06DistancdLearnValue = 0;
uint32_t nd06DistanceThreshold = 500;

uint32_t dts6012DistanceThresholdLearningSuccessFlg = 0;
uint32_t nd06DistanceThresholdLearningSuccessFlg = 0;
uint32_t objectDCnt_P = 0;
uint32_t objectDCnt_H = 0;

uint8_t IO_dts6012 = 0;
uint8_t IO_ND06 = 0;
uint8_t fingerDetected_flag = 0;
uint8_t regionalObjectDetected_flag_ND06 = 0;
uint8_t masterElevator_OF = 1;
uint8_t now_DistanceThresholdLearning_Flg = 0;
uint8_t DistanceThresholdLearningFailed_Flg = 0;
uint8_t times_DistanceThresholdLearn = 0;

uint32_t nd06PixelAddCnt[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint32_t nd06StudyDep[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint32_t nd06AddCnt = 0;
uint32_t dts6012AddCnt = 0;

uint32_t ClosingTimeLearningSuccessFlg = 0;
uint32_t closeTimeCnt = 0;
uint32_t closeTimeCountFlg = 0;
uint32_t pixelOccludedFlag = 0;
uint8_t now_ClosingTimeLearning_Flg = 0;
uint8_t times_ClosingTimeLearning = 0;
uint8_t ClosingTimeLearningFailed_Flg = 0;
uint8_t ObjectIsDetectedFlag = 0;
int8_t nd06AV1C_objDetectFlag = 0;
void ObjectDetection(void)
{

    /*单点TOF*/
    
    if ((dts6012_data.firstPeakDistance < (PARA_TABLE_USE.data.dts6012StudyDistance - PARA_TABLE_USE.data.dts6012DistanceChkThreshold)) && (dts6012_data.firstPeakDistance > PARA_TABLE_USE.data.dts6012MinChkDistance) /*&& (ds6012OutputEN == 1)*/)
    {
        dts6012_data.objDetectFlag = 1;
    }
    else if ((dts6012_data.secondPeakDistance < (PARA_TABLE_USE.data.dts6012StudyDistance - PARA_TABLE_USE.data.dts6012DistanceChkThreshold)) && (dts6012_data.secondPeakDistance > PARA_TABLE_USE.data.dts6012MinChkDistance) /*&& (ds6012OutputEN == 1)*/)
    {
        dts6012_data.objDetectFlag = 1;
    }
    else
    {
        dts6012_data.objDetectFlag = 0;
    }

   
    
		if ((dts6012_data.objDetectFlag == 1))
    {
        ObjectIsDetectedFlag = 1;
        
    }
    else
    {
        ObjectIsDetectedFlag = 0;
        //prevFlag = 0; // 重置状态
    }

    if (dts6012_data.objDetectFlag == 1)
    {
        IO_dts6012 = 1;
        fingerDetected_flag = 1;
        
    }
    else
    {
        IO_dts6012 = 0;
    }

    if (nd06AV1C_objDetectFlag == 1)
    {
        IO_ND06 = 1;
        regionalObjectDetected_flag_ND06 = 1;
    }
    else
    {
        IO_ND06 = 0;
        regionalObjectDetected_flag_ND06 = 0;
    }
}

uint8_t IdleFlg = 0;
uint8_t ToolingTestFlag = 1;
uint32_t sensor_status = NormalWorking_STATUS; // sensor_status初始状态为NormalWorking_STATUS
uint32_t sensor_old_status = Unknown_STATUS;
void sensor_status_control(void)
{
    sensor_old_status = sensor_status;
    if (ToolingTestFlag != 1)
    {
        sensor_status = NotDetected_STATUS;
    }
    // 工作异常状态
    if (ERR_D != 0)
    {
        sensor_status = Fault_STATUS;
        if (E001 == 1)
        {
            HAL_NVIC_SystemReset();
        }
        else if (E002 == 1)
        {
            
        }
        else if (E003 == 1)
        {
        }
    }
    else
    {
        sensor_status = NormalWorking_STATUS;
    }

    if ((sensor_status == NormalWorking_STATUS) && (IdleFlg == 1))
    {
        sensor_status = Idle_STATUS;
    }
    else if ((sensor_status == Idle_STATUS) && (IdleFlg == 0))
    {
        sensor_status = NormalWorking_STATUS;
    }
    else
    {
        // sensor_status = NormalWorking_STATUS;
        sensor_status = sensor_status;
    }


}


uint8_t objectDetectionErrorCnt = 0;
uint8_t stopIOOutputFlag = 0;
/*
according to sensor_status to do special works
*/
uint8_t closingNoObjectCnt = 0;
void sensor_action_control(void)
{
    if (sensor_status == NotDetected_STATUS)
    {
        LEDON;
    }
    else if (sensor_status == Fault_STATUS)
    {
        LEDON;
    }
    
    else if (sensor_status == NormalWorking_STATUS) // 正常工作
    {
        ClosingTimeLearningReqFlg = 0;
        ObjectDetection();
        // ObjectIsDetectedFlag = 1;   //E002
        
        //detectError();

        // IO输出控制
        if (stopIOOutputFlag == 1)
        {
            LEDOFF;
            HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_SET); // IO关闭
        }
        else
        {

            if (ObjectIsDetectedFlag == 1)
            {
                LEDON;
                HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_RESET); // IO输出
            }
            else
            {
                LEDOFF;
                HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_SET); // IO关闭
                //     LEDON;
                // HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_RESET); // IO输出
            }
        }
    }
    else if (sensor_status == Idle_STATUS)
    {
        ClosingTimeLearningReqFlg = 0;
        LEDOFF;
    }
    else
    {

        ;
    }
}


