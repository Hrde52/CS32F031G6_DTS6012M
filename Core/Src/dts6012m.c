#include "dts6012m.h"

/*dts6012mÃüÁî*/
uint8_t dts6012m_setBuat_cmd[] = {0xA5,0x03,0x20,0x10,0x00,0x00,0x0A,0xA9,0xEA};  // 0x10,0x626B  0x0A,0xA9EA
void dts6012_setBuat(void)
{
	HAL_UART_Transmit(&huart1,dts6012m_setBuat_cmd,sizeof(dts6012m_setBuat_cmd),10);
}


/*dts6012mÆô¶¯ÃüÁî*/
uint8_t dts6012m_start_cmd[] = {0xA5,0x03,0x20,0x01,0x00,0x00,0x00,0x02,0x6E};
void dts6012_start(void)
{
	HAL_UART_Transmit(&huart1,dts6012m_start_cmd,sizeof(dts6012m_start_cmd),10);
}


uint8_t dts6012m_stop_cmd[] = {0xA5,0x03,0x20,0x02,0x00,0x00,0x00,0x46,0x6E};
void dts6012m_stop(void)
{
	HAL_UART_Transmit(&huart1,dts6012m_stop_cmd,sizeof(dts6012m_start_cmd),10);
}



uint8_t dTS6012M_Rdata[23] = {0};
uint16_t dTS6012M_errCnt = 0;
uint16_t dTS6012M_errFlag = 0;


HAL_StatusTypeDef dts6012_getDepthAndAmp(DTS6012_DATA *dts6012_data)
{
	HAL_StatusTypeDef status = HAL_OK;
	if(HAL_UART_Receive(&huart1,dTS6012M_Rdata,23,4) == HAL_OK)
	{
		if((dTS6012M_Rdata[0]==0xA5)&&(dTS6012M_Rdata[1]==0x03)&&(dTS6012M_Rdata[2]==0x20))
		{
			dts6012_data->firstPeakDistance	= (dTS6012M_Rdata[14]<<8) | (dTS6012M_Rdata[13]);
			dts6012_data->firstPeakAmp		= (dTS6012M_Rdata[18]<<8) | (dTS6012M_Rdata[17]);
			dts6012_data->secondPeakDistance	= (dTS6012M_Rdata[8]<<8) | (dTS6012M_Rdata[7]);
			dts6012_data->secondPeakAmp		= (dTS6012M_Rdata[12]<<8) | (dTS6012M_Rdata[11]);		
			
			dTS6012M_errCnt = 0;
			
			printf("dts6012_data.firstPeakDistance = %d\t",dts6012_data->firstPeakDistance);
			printf("dts6012_data.firstPeakAmp = %d\r\n",dts6012_data->firstPeakAmp);
			printf("dts6012_data.secondPeakDistance = %d\t",dts6012_data->secondPeakDistance);
			printf("dts6012_data.secondPeakAmp = %d\r\n",dts6012_data->secondPeakAmp);
		}
		else
		{
			if(++dTS6012M_errCnt >= 10)
			{
				dTS6012M_errFlag = 1;
				status = HAL_ERROR;
			}			
		}
	}
	else
	{
		if(++dTS6012M_errCnt >= 10)
		{
			dTS6012M_errFlag = 1;
			status = HAL_ERROR;
		}			
	}
	return status;
}


