//***************************************************************** 
//* FILE        : FlexCANInitial.h
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
#ifndef FLEXCANINITIAL_H
#define FLEXCANINITIAL_H

#include "MPC5645S.h"
#include "MPC55xx_PCRdef.h"
#include "S_FileReceive.h"
#include "InterruptsIRQDef.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

#define   	CAN_Tx    	0		//发送帧
#define 	CAN_Rx		1		//接收帧
#define		Std_Frame	0		//标准帧
#define		Ext_Frame	1		//扩展帧

#define		CAN0		0		//第0通道
#define		CAN1		1		//第1通道
#define		CAN2		2		//第2通道


typedef struct 
{
	unsigned char 	CAN_Num		  ;   //选择CAN通道
	unsigned short	BaudRate	  ;	  //设置波特率
	uint32_t	IntMaskL	  ;	  //低32位中断允许位 
	uint32_t	IntMaskH	  ;	  //高32位中断允许位 
}FlexCANCfg;

#define 	RAM_ADDR_ISR_FLEXCAN0003	0x4000EA00  	//发送中断函数在RAMZ中的入口地址
#define 	RAM_ADDR_ISR_FLEXCAN0407	0x4000F000		//接收中断函数在RAMZ中的入口地址

#define 	Length_ISR_FLEXCAN0003		(0x00000600>>2)	//发送中断函数的地址长度
#define 	Length_ISR_FLEXCAN0407		(0x00000600>>2) //接收中断函数的地址长度

#define		CANRxFilterID				0x00000200		//接收CAN信息的报文ID
#define		CANTxFilterID				0x00000100		//发送CAN信息的报文ID

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/


//*****************************************************************
//函数名  : InitialFlexCAN()
//描  述  : 对CAN通道进行初始化 
//输入参数: 无
//返回参数: 无
//*****************************************************************
void InitialFlexCAN(void);


#endif