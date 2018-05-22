//***************************************************************** 
//* FILE        : S_FileReceive.h
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
//* DATE        : 3 apir 2014
//* AUTHOR      : lilin 
//******************************************************************
#ifndef   S_FILERECIVE_H
#define	  S_FILERECIVE_H

#include "MPC5645S.h"
#include "S_File_Proc.h"
#include "FlexCANInitial.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
#define     NOP()				 asm("nop")	



#define		TxInactiveFlag        0x08		//禁止发送标志
#define		TxActiveFlag          0x0C		//启动发送标志

#define     TxSuccessful		  1			//发送成功标志
#define     TxFail				  0			//发送失败标志

typedef struct
{
	uint32_t MsgID;
	unsigned char  datalength;
	unsigned char  *data;
}CANReceive_Type;

typedef struct
{
	unsigned char   CANGetNewFrame		:1;
	unsigned char   BootloaderStatus	:2;
	unsigned char   FlashIsBusy			:1;
	unsigned char 	EraseFlg			:1;
	unsigned char 	TxSuccessfulFlag	:1;
	unsigned char   MCUBusyFlag			:1;
	unsigned char 						:1;
}CANBootloaderStatus;

extern CANBootloaderStatus CANBootldSts;

#define CANRxNewFram		CANBootldSts.CANGetNewFrame		//MCU接收到新CAN数据帧标志位
#define MCUBootldState		CANBootldSts.BootloaderStatus	//当前MCU所处连接状态
#define EraseFlagStatus		CANBootldSts.EraseFlg			//擦除成功标志
#define TxSuccFlg			CANBootldSts.TxSuccessfulFlag	//MCU响应数据发送成功标志
#define MCUBusyFlg			CANBootldSts.MCUBusyFlag		//MCU当前是否正在处理上一条命令标志位

extern unsigned char  EraseFlag;


//****************************************************************
//***********    控制板与上位机之间的通讯连接命令  ***************
//****************************************************************
#define  	UP_FLASH_ERR 	0x01     	//下载错误
#define  	UP_INBOOT_SUC 	0xC0		//进入Bootloader成功
#define  	UP_PRGEND 		0xC1		//编程结束
#define  	UP_BUSY 		0xC2		//MCU正在处理其他命令
#define  	UP_READY 		0xC3		//MCU准备好接收新数据
#define  	UP_IDLE 		0xC4		//MCU处于空闲状态
#define  	UP_ERASEERR 	0xC5		//Flash擦写出现错误
#define  	UP_DOWN_ERR 	0xC6		//收到的数据有错误
#define  	UP_PRO_NO_END_ERR 	0xC7	//当前程序尚未烧写完成
#define  	UP_OUTBL_SUC 	0xC8		//退出Bootloader成功
#define  	UP_PROONELINE_SUC 	0xC9	//一行数据编程成功
#define  	UP_ERASE_SUC 	0xCA		//Flash擦除成功
#define	    UP_PROADDR_ERR  0xCB		//烧写数据地址错误，进入BOOTLOADER区域
#define    	UP_ERASEBOOTAREA_ERR  0xCC	//擦除Bootloader区域报错
#define    	UP_CANCELINBOOT  0xCE	//擦除Bootloader区域报错
#define		UP_SUREINBOOT	0xCD		//询问是否需要进入Bootloader	
#define		UP_GETERASECMD	0xD0		//通知上位机收到擦除Flash的命令

//****************************************************************
//***********     控制板的Bootloader的状态标志     ***************
//****************************************************************
#define  	STA_NOT_LINKED 	0x00		//上位机与MCU没有建立连接
#define  	STA_LINKED 		0x01		//上位机与MCU连接成功
#define  	STA_PRGEND 		0x02		//MCU编程结束


//****************************************************************
//***********     上位机与控制板之间的连接命令     ***************
//****************************************************************
#define  	DOWN_LINK 			0xFF   	//让MCU进入Bootloader模式命令
#define  	DOWN_LINE_END 		0xFE	//传输一行数据结束命令
#define  	DOWN_FILE_END 		0xFD	//文件传输结束命令
#define  	DOWN_ERASE_CMD 		0xFC	//擦除Flash命令
#define  	DOWN_OUTBOOT_CMD 	0xFB	//退出Bootloader模式命令
#define  	DOWN_INBOOTSUC_CMD 	0xFA	//是否成功进入Bootloader

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/

//*****************************************************************
//函数名  : CAN_SRecrod_RxISR()
//描  述  : 接收中断函数
//输入参数: 无
//返回参数: 无
//*****************************************************************
void CAN_SRecrod_RxISR(void);

//*****************************************************************
//函数名  : CAN_0_ERR_ISR()
//描  述  : CAN错误中断标志
//输入参数: 无
//返回参数: 无
//*****************************************************************
void CAN_0_ERR_ISR(void);

//*****************************************************************
//函数名  : CAN_SRecrod_TxISR()
//描  述  : CAN发送中断处理函数
//输入参数: 无
//返回参数: 无
//*****************************************************************
void CAN_SRecrod_TxISR(void);

//*****************************************************************
//函数名  : CANStatusAcknowledge()
//描  述  : 控制板与上位机的握手信息发送 
//输入参数: status		: 当前的传输状态
//返回参数: 无
//*****************************************************************
void CANStatusAcknowledge(unsigned char status);

//*****************************************************************
//函数名  : CanWorkStatusGet()
//描  述  : 取控制板与上位机的CAN线状态
//输入参数: 无
//返回参数: 当前CAN线的状态
//*****************************************************************
unsigned char CanWorkStatusGet(void);


#endif