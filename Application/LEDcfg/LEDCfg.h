//***************************************************************** 
//* FILE        : bootloader.c
//* 
//* DESCRIPTION : This is the file to Initial the FlexCAN register
//*
//*               In some places this includes alternative register
//*               nomenclature. Comment out the unused version to
//*               match code requirements
//*               The default version is used by Freescale in code
//*               examples
//* 
//* COPYRIGHT   :(c) 2014-2020, by XueLiMan
//* 
//* VERSION     : 1.00
//* DATE        : 31 march 2014
//* AUTHOR      : lilin 
//******************************************************************
#ifndef LEDCFG_H
#define	LEDCFG_H

#include "MPC5645S.h"
#include "MPC55xx_PCRdef.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
#define 	LedOff		0
#define 	LedOn		1

#define LEDOUT_1      SIU.GPDO[PC3].B.PDO	
#define LEDOUT_2      SIU.GPDO[PC4].B.PDO	
#define LEDOUT_3      SIU.GPDO[PJ12].B.PDO	
#define LEDOUT_4      SIU.GPDO[PJ13].B.PDO
#define LEDOUT_5      SIU.GPDO[PJ14].B.PDO	
#define LEDOUT_6      SIU.GPDO[PK0].B.PDO
#define LEDOUT_7      SIU.GPDO[PK1].B.PDO
#define LEDOUT_8      SIU.GPDO[PF1].B.PDO		
#define LEDOUT_9      SIU.GPDO[PF0].B.PDO
#define LEDOUT_10     SIU.GPDO[PF3].B.PDO
#define LEDOUT_11     SIU.GPDO[PF6].B.PDO
#define LEDOUT_12     SIU.GPDO[PF7].B.PDO
#define LEDOUT_13     SIU.GPDO[PF11].B.PDO
#define LEDOUT_14     SIU.GPDO[PF14].B.PDO
#define LEDOUT_15     SIU.GPDO[PF4].B.PDO
#define LEDOUT_16     SIU.GPDO[PJ5].B.PDO
#define LEDOUT_17     SIU.GPDO[PJ4].B.PDO

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/

//*****************************************************************
//函数名  : LED_IOCfgInitial()
//描  述  : 对LED的IO端口进行初始化
//输入参数: 无
//返回参数: 无
//*****************************************************************
void LED_IOCfgInitial(void);

//*****************************************************************
//函数名  : LED_Init()
//描  述  : 对LED的状态进行初始化
//输入参数: 无
//返回参数: 无
//*****************************************************************
void LED_Init(void);

//*****************************************************************
//函数名  : LED_Light()
//描  述  : 点亮LED灯
//输入参数: 无
//返回参数: 无
//*****************************************************************
void LED_Light(uint32_t LightFlag);

//*****************************************************************
//函数名  : DlayFunction()
//描  述  : 延时函数
//输入参数: Seconds		: 延时秒
//	      : Ms			: 延时毫秒
//返回参数: 无
//*****************************************************************
void DlayFunction(uint32_t Seconds,uint32_t Ms); 

//*****************************************************************
//函数名  : Erase_Flash_Except_Bootloader()
//描  述  : 擦除除bootloader以外的所有Flash区域 
//输入参数: 无
//返回参数: 无
//*****************************************************************
unsigned char EraseEnterBootFlagFlash(void);

//*****************************************************************
//函数名  : FlashProgramer()
//描  述  : 对Flash进行编程
//输入参数: *pDataPtr		: 编程的数据内容
//返回参数: ProFail			: 写入Flash失败
//*****************************************************************
unsigned char EnterBootFlagFlashProgramer(void);

#endif