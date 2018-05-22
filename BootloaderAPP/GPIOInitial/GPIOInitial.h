//***************************************************************** 
//* FILE        : GPIOInitial.h
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
#ifndef _GPIOINITIAL_H_
#define _GPIOINITIAL_H_

#include "MPC5645S.h"
#include "MPC55xx_PCRdef.h"
#include "FlashOperation.h"
#include "S_FileReceive.h"
#include "ModeCtrDefine.h"
#include "FlexCAN_RxTx.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

#define		BootSelect_One			SIU.GPDI[PC7].B.PDI 
#define		BootSelect_Two			SIU.GPDI[PC8].B.PDI 

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/

//*****************************************************************
//函数名  : DlyS_Ms()
//描  述  : 延时函数
//输入参数: Mills		: 延时时间
//返回参数: 无
//*****************************************************************

//*****************************************************************
//函数名  : ForceIntoBootKEYInitial()
//描  述  : 对强制进入Bootloader的按键进行初始化
//输入参数: 无
//返回参数: 无
//*****************************************************************
void ForceIntoBootKEYInitial(void);

//*****************************************************************
//函数名  : ScanKEYStatus()
//描  述  : 检测按键是否按下
//输入参数: 无
//返回参数: 无
//*****************************************************************
unsigned char ScanKEYStatus(void);

//*****************************************************************
//函数名  : PITInitial()
//描  述  : 禁止定时器
//输入参数: 无
//返回参数: 无
//*****************************************************************
void PITInitial(void);

//*****************************************************************
//函数名  : MCUConfiguration()
//描  述  : MCU时钟等配置
//输入参数: 无
//返回参数: 无
//*****************************************************************
void MCUConfiguration(void);

#endif