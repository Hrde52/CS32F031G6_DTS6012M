#include "toolingtest.h"
#include "rs485.h"
#include "dts6012m.h"
#include "string.h"
#include "sensorAppLogic.h"
#include "gpio.h"

// PASS:1 ERROR:0
uint8_t testDTS()
{
	DTS6012_DATA dts6012DataTooling = {0, 0, 0, 0};

	dts6012DataTooling.firstPeakDistance = dts6012_data.firstPeakDistance;
	dts6012DataTooling.firstPeakAmp = dts6012_data.firstPeakAmp;
	dts6012DataTooling.secondPeakDistance = dts6012_data.secondPeakDistance;
	dts6012DataTooling.secondPeakAmp = dts6012_data.secondPeakAmp;

	if (dts6012DataTooling.firstPeakDistance == 0)
	{
		return 0;
	}
	
	if((dts6012DataTooling.firstPeakDistance < 400) || 
		(dts6012DataTooling.secondPeakDistance < 400))
	{
		toolingBits.bits.bit4DTSdata = 0;
	}
	else
	{
		toolingBits.bits.bit4DTSdata = 1;
	}
	
	resultUp[4] = (dts6012DataTooling.firstPeakDistance >> 0) & 0xFF;
	resultUp[5] = (dts6012DataTooling.firstPeakDistance >> 8) & 0xFF;

	resultUp[6] = (dts6012DataTooling.secondPeakDistance >> 0) & 0xFF;
	resultUp[7] = (dts6012DataTooling.secondPeakDistance >> 8) & 0xFF;
	dts6012m_stop();

	return 1;
}

void testIO()
{
	HAL_GPIO_WritePin(IO_OUT_GPIO_Port, IO_OUT_Pin, GPIO_PIN_RESET);
	LEDON;
	//HAL_Delay(1000);
	toolingBits.bits.bit2OUT = 0;
	return;
}

// PASS:1 ERROR:0
void testInX()
{
	if (getIN_IO == 0)
	{
		if (HAL_GPIO_ReadPin(IO_IN_GPIO_Port, IO_IN_Pin) == GPIO_PIN_RESET)
		{
			getIN_IO = 1;
		}
		else
		{
			getIN_IO = 0;
		}
	}
}
