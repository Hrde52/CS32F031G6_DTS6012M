#include "sensorParaTable.h"
#include <string.h>
#include "stmflash.h"

#define PARA_TABLE_FLASH_SAVE_ADDR 0x08007C00 /* 设置FLASH 保存地址 STM32F031G6 Flash 32KB =0x08007FFF 0x08007C00 last page*/

uint32_t ParaTable_Default[FF_COUNT] =
{
	0x0100, // 程序版本
	0x11,	// 0x11平台梯，0x22货梯
	0x52,	// 传感器地址  51一楼53二楼，楼宇52

	/*平台梯参数*/
	2300, // dts6012学习距离，单位mm
	200,  // dts6012检测阈值，单位mm
	4000, // dts6012最大检测距离，单位mm
	200,  // dts6012最小检测距离，单位mm
	2300, // nd06学习距离，单位mm  大石二楼2100，楼宇2300
	200,  // nd06检测阈值，单位mm
	4000, // nd06最大检测距离，单位mm
	200,  // nd06最小检测距离，单位mm
	1400, // 关门时间，单位ms    // 问题1：1400改小 1200

	/*货梯参数*/
	2300, // dts6012学习距离，单位mm
	300,  // dts6012检测阈值，单位mm
	4000, // dts6012最大检测距离，单位mm
	300,  // dts6012最小检测距离，单位mm
	200,  // nd06检测阈值，单位mm
	4000, // nd06最大检测距离，单位mm
	300,  // nd06最小检测距离，单位mm
	2300, // nd06第1个像素
	2300, // nd06第2个像素
	2300, // nd06第3个像素
	2300, // nd06第4个像素
	2300, // nd06第5个像素
	2300, // nd06第6个像素
	2300, // nd06第7个像素
	2300, // nd06第8个像素
	2300, // nd06第9个像素
	2300, // nd06第10个像素
	2300, // nd06第11个像素
	2300, // nd06第12个像素
	2300, // nd06第13个像素
	2300, // nd06第14个像素
	2300, // nd06第15个像素
	2300  // nd06第16个像素

};

FF_PARA_USE PARA_TABLE_USE;
FF_PARA_SAVE FF_PARA_TABLE;
uint32_t ParaTableSave[FF_COUNT + 1];

uint32_t paraTableSum(uint32_t *num, uint32_t len)
{
	uint32_t sumValue = 0;
	uint32_t i = 0;
	for (i = 0; i < len; i++)
	{
		sumValue += num[i];
	}
	return sumValue;
}

void paraTable_Write(void)
{
	// memcpy(PARA_TABLE_USE.DATE,ParaTable_Default,sizeof(ParaTable_Default));
	memcpy(ParaTableSave, PARA_TABLE_USE.DATE, sizeof(PARA_TABLE_USE.DATE));
	ParaTableSave[FF_COUNT] = paraTableSum(PARA_TABLE_USE.DATE, sizeof(PARA_TABLE_USE.DATE) / 4);
	stmflash_write(PARA_TABLE_FLASH_SAVE_ADDR, ParaTableSave, sizeof(ParaTableSave) / 4);
}

void paraTable_Read(void)
{
	stmflash_read(PARA_TABLE_FLASH_SAVE_ADDR, ParaTableSave, sizeof(ParaTableSave) / 4);
	memcpy(PARA_TABLE_USE.DATE, ParaTableSave, sizeof(PARA_TABLE_USE.DATE));
}

void paraTable_Init(void)
{
	uint32_t datatemp = 0;
	datatemp = stmflash_read_word(PARA_TABLE_FLASH_SAVE_ADDR);
	
//	for(uint8_t i = 0; i < sizeof(ParaTable_Default); i++)
//	{
//		uint32_t tempDataAddr = PARA_TABLE_FLASH_SAVE_ADDR+4*i;
//		datatemp = stmflash_read_word(tempDataAddr);
//		if (datatemp == 0XFFFFFFFF)
//		{
//			break;
//		}
//	}
	
	if (datatemp == 0XFFFFFFFF) // 说明地址为空
	{
		uint32_t sizeTable = sizeof(ParaTable_Default);   // 8C
		memcpy(PARA_TABLE_USE.DATE, ParaTable_Default, sizeof(ParaTable_Default));
		paraTable_Write();
	}
	else // 如有参数则读取参数
	{
		paraTable_Read();
	}
}
