//***************************************************************** 
//* FILE        : FlexCANRxTx.c
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
#ifndef FLEXCAN_RXTX_H
#define FLEXCAN_RXTX_H

#include "MPC5645S.h"
#include "S_FileReceive.h"
#include "FlexCANInitial.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

typedef struct
{
	unsigned char  BOOTLOADERCANTXSUCFLAG		:1;		//BootloaderCAN发送状态标志位
	unsigned char  CANRXNEWDATUM				:1;
	unsigned char  GOTOBOOTSUC					:1;
	unsigned char  ERASE_ENTERBOOT				:1;
	unsigned char  PROG_ENTERBOOT				:1;
}BOOTLOADERCANFLAG;

extern BOOTLOADERCANFLAG	BootloaderCANStatus;
#define		BootloaderCANTransSucFlag		BootloaderCANStatus.BOOTLOADERCANTXSUCFLAG	
#define		CANRxNewDatum					BootloaderCANStatus.CANRXNEWDATUM
#define		GotBootloaderSuc				BootloaderCANStatus.GOTOBOOTSUC
#define		eraseEnterBootFlashFlag			BootloaderCANStatus.ERASE_ENTERBOOT
#define		progEnterBootFlashFlag			BootloaderCANStatus.PROG_ENTERBOOT


#define 	RAM_ADDR_ERASE_FLASH	    0x4000CA00  	//将擦写函数复制到RAM中的入口地址
#define 	RAM_ADDR_PROG_FLASH	        0x4000CD00		//将编程函数复制到RAM中的入口地址

#define	    Length_copy_ram				(0x00000300>>2)	//复制到RAM中的长度

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/

//*****************************************************************
//函数名  : TransAcknowledge()
//描  述  : 控制板与上位机的握手信息发送 
//输入参数: status		: 当前的传输状态
//返回参数: 无
//*****************************************************************
void TransAcknowledge(unsigned char status);

#endif