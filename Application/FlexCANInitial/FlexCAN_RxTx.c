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
#include "FlexCAN_RxTx.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
CANReceive_Type		CAN_RxBootloaderInfo;
CANReceive_Type		CAN_TxBootloaderInfo;
BOOTLOADERCANFLAG	BootloaderCANStatus;

unsigned char  CANTxDataBuffer[8]={0};
#pragma push
#pragma section code_type ".userApplication_flash"
/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/
__declspec(section ".userApplication_flash")
void CAN_Tx_ISR(void)
{
	if(CAN_2.IFRL.B.BUF00I)
	{
		CAN_2.IFRL.B.BUF00I=TRUE;
		BootloaderCANTransSucFlag=TRUE;
	}
}

void CAN_Rx_ISR(void)
{
	unsigned char cnts;
	unsigned int FlagWord;
	
	if(CAN_2.IFRL.B.BUF04I)						//判断接收中断标志位
	{
		FlagWord=CAN_2.BUF[4].CS.R;
		
		for(cnts=0;cnts<10;cnts++) NOP();
		if(!CAN_2.BUF[4].CS.B.IDE)				//判断是否为所需要的扩展帧格式
		{
			CAN_RxBootloaderInfo.datalength=(unsigned char)CAN_2.BUF[4].CS.B.LENGTH; //得到接收到的数据长度
			CAN_RxBootloaderInfo.data=(unsigned char*)&CAN_2.BUF[4].DATA.B[0];
			
			if(CAN_2.BUF[4].ID.B.STD_ID==CANRxFilterID)				//判断是否是需要的MessageID
			{
				for(cnts=0;cnts<CAN_RxBootloaderInfo.datalength;cnts++)  //取回接收到的数据信息
				{
					CAN_RxBootloaderInfo.data[cnts]=*(&CAN_2.BUF[4].DATA.B[cnts]);
				}
			}
			CAN_2.BUF[4].CS.B.CODE=0x04;
		}
		CANRxNewDatum = TRUE;
		FlagWord=CAN_2.TIMER.R;
		
		CAN_2.IFRL.B.BUF04I=TRUE;
		for(cnts=0;cnts<10;cnts++) NOP();
	}
	
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//函数名  : BootloaderCANTxData()
//描  述  : FlexCAN发送函数 
//输入参数: CANChannel	: 待发送数据的CAN通道
//		  : MBNum		: 待发送的数据CAN报文缓冲区
//		  :	*pData		: 指向待发送的数据首地址
//返回参数: TRUE		: 发送成功
//		  :	FALSE		: 发送失败
//*****************************************************************
static unsigned char  BootloaderCANTxData(CANReceive_Type *pCANTxDatum)
{
    unsigned char cnts;
    unsigned short TimeCounts=0;
    static unsigned char datalength;

    datalength=(unsigned char)CAN_2.BUF[0].CS.B.LENGTH;;//获取待发送数据的长度
   
    if(TxInactiveFlag==CAN_2.BUF[0].CS.B.CODE)	//判断当前是否可以发送数据
    {
    	BootloaderCANTransSucFlag=FALSE;
    	for (cnts = 0; cnts < 8; cnts++)		//初始化发送缓冲区
	    {
	        CAN_2.BUF[0].DATA.B[cnts] = 0xFF;
	    }
	    
    	for (cnts = 0; cnts < datalength; cnts++)//将数据装入发送缓冲区的数据区域
	    {
	        CAN_2.BUF[0].DATA.B[cnts] = pCANTxDatum->data[cnts];
	    }
	     
    }
    CAN_2.BUF[0].CS.B.CODE=TxActiveFlag;	//设置发送缓冲区为激活状态，无条件发送数据	
	while(BootloaderCANTransSucFlag==FALSE)					//等待相应缓冲区发送数据完成
	{
		TimeCounts++;						//延时
		if(TimeCounts==2000)					
			return FALSE;				//延时时间到，依然没有发送完成，则返回发送失败信息
	};
	CAN_2.IFRL.B.BUF00I = 1;				//清除发送中断标志位
	return TRUE;						//返回发送成功信息
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//函数名  : TransAcknowledge()
//描  述  : 控制板与上位机的握手信息发送 
//输入参数: status		: 当前的传输状态
//返回参数: 无
//*****************************************************************
void TransAcknowledge(unsigned char status)
{
	unsigned char cnts,err;
	
	for(cnts=0;cnts<8;cnts++)		//对数据缓冲区初始化
	{
		CANTxDataBuffer[cnts] = 0xFF;
	}
	CAN_TxBootloaderInfo.datalength = 3;
	CANTxDataBuffer[1]=status;			//填入要发送的数据到相应数据缓冲区
	
	CAN_TxBootloaderInfo.data = (unsigned char*)&CANTxDataBuffer[0];
	for(cnts=0;cnts<CAN_TxBootloaderInfo.datalength;cnts++)	//将数据帧放入发送缓冲区
	{
		CAN_TxBootloaderInfo.data[cnts]=CANTxDataBuffer[cnts];
	}
	err=BootloaderCANTxData(&CAN_TxBootloaderInfo);		//判断是否发送成功
}
#pragma pop
