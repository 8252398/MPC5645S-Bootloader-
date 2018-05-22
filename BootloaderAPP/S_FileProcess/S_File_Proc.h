//***************************************************************** 
//* FILE        : S_File_Proc.h
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
#ifndef  S_FILE_PROC_H
#define  S_FILE_PROC_H

#include "MPC5645S.h"
#include "MPC55xx_PCRdef.h"
#include "FlashOperation.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
#define  	SPASS       0		//接收数据校验通过
#define  	SERR        1		//接收数据校验不通过
#define  	SFileStart  2		//S文件数据开始标志
#define  	SFileEnd    3		//S文件数据结束标志
#define  	SInvalid    0xFF	//S文件无效

#define	 	AddressContinue		0//连续地址
#define		AddressBreak		1//地址不连续

#define		FileTransEnd		2 	//文件传输结束
#define		FileTransNotEnd		3	//文件传输未结束

#define		DataBufferEmpty		4	//数据交换区空
#define		DataBufferFull		5	//数据交换区满

#define		LineDataEnd			6	//行数据结束
#define		LineDataEffect		7	//行数据未结束

#define		ProgramDataReady	8	//编程数据准备好
#define		ProgramDataNotReady	9	//编程数据未准备好



typedef struct 
{
    unsigned char	status;		//当前缓冲区数据状态
    uint32_t 	addr;		//数据保存起始地址
    unsigned short  datalen;	//数据长度
    uint32_t  data[5];	//数据保存缓冲区
} SFileRecord;

typedef struct 
{
    unsigned char	status;		//当前数据交换区数据状态
    uint32_t 	addr;		//数据保存起始地址
    uint32_t  data;		//数据
} DataBuffer;

typedef struct 
{
    unsigned char	status;		//当前缓冲区数据状态
    uint32_t 	addr;		//数据保存起始地址
    uint32_t  data[2];	//数据
} ProgramDataBuffer;
/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/

//*****************************************************************
//函数名  : S19DataConvert()
//描  述  : 对收到的一行S报文进行解析
//输入参数: *OneLineDatum	: 所收到的一行报文
//        : *pSDataRcd		: 存储报文的基本信息
//返回参数: SERR			: 报文校验出错
//		  : SPASS			: 报文校验成功
//		  : SFileStart		: 报文传输开始标志
//		  : SFileEnd		: 报文传输结束标志
//		  : SInvalid		: 报文无效标志
//*****************************************************************
unsigned char S19DataConvert(unsigned char *OneLineDatum, SFileRecord *pSDataRcd);

//*****************************************************************
//函数名  : DoubleWordProgramSytleProcess()
//描  述  : 将收到S19数据写进Flash
//输入参数: *pDataBuf	:数据暂存缓冲区
//		  : *pLineRec	:收到的行数据信息
//		  :	*pProDataBuf:编程数据暂存缓冲区
//返回参数: 无
//*****************************************************************
void DoubleWordProgramSytleProcess(DataBuffer *pDataBuf,SFileRecord *pLineRec,ProgramDataBuffer *pProDataBuf);

#endif