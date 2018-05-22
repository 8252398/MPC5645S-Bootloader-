//***************************************************************** 
//* FILE        : FlexCANInitial.c
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
#include "FlexCANInitial.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/


#pragma push
#pragma section code_type ".boot_flash" 
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : FlexCANMBMsgConfg()
//描  述  : 对CAN的邮箱缓冲区进行配置 
//输入参数: CANChannel: CAN通道选择
//		  : MBNum	  : 缓冲区序号选择
//		  : Tx_RxFlag :	发送，接收设置
//		  : IDE       :	扩展帧，标准帧设置
//		  : DLC		  :	数据长度设置
//		  : Prio	  : 优先级设置
//		  : ID		  : 过滤掩码设置
//返回参数: 无
//*****************************************************************
static void FlexCANMBMsgConfg(unsigned char CANChannel,unsigned char MBNum,unsigned char Tx_RxFlag,unsigned char IDE,unsigned char DLC,unsigned char Prio,uint32_t ID)
{
	if(MBNum>63)	return;
	else
	{
		
		switch(CANChannel)
		{
			case CAN2:
			default:
			{
				if(IDE==Ext_Frame)
				{
					CAN_2.BUF[MBNum].ID.B.STD_ID=((ID>>18)&0x7FF);
					CAN_2.BUF[MBNum].ID.B.EXT_ID=(ID&0x3FFFF);
				}
				else
				{
					CAN_2.BUF[MBNum].ID.B.STD_ID=(ID&0x7FF);
					CAN_2.BUF[MBNum].ID.B.EXT_ID=0;
				}
				CAN_2.BUF[MBNum].CS.B.RTR=0;
				CAN_2.BUF[MBNum].CS.B.SRR=0;
				CAN_2.BUF[MBNum].CS.B.IDE=IDE;
				CAN_2.BUF[MBNum].CS.B.LENGTH=DLC;
				CAN_2.BUF[MBNum].CS.B.TIMESTAMP=0;
				CAN_2.BUF[MBNum].ID.B.PRIO=Prio;
				if(Tx_RxFlag==CAN_Tx)
				CAN_2.BUF[MBNum].CS.B.CODE=0x08;
				else
				CAN_2.BUF[MBNum].CS.B.CODE=0x04;
			}break;
		}
	}
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : FlexCANCONFG(FlexCANCfg FlexCanCfg)
//描  述  : 对CAN0通道初始化 
//输入参数: FlexCanCfg 包括所有需要进行配置的参数和标志位
//返回参数: 无
//*****************************************************************
static void FlexCANCONFG(FlexCANCfg *pFlexCanCfg)
{
	switch(pFlexCanCfg->CAN_Num)
	{
		case 2:
		default:
		{
			SIU.PCR[PM10].R=0x0900;
			SIU.PCR[PM11].R=0x0802;
			SIU.PSMI[2].R=1;
			
			CAN_2.MCR.R=0x50000000|0x0000003F;
			
			CAN_2.IFRL.R=0xFFFFFFFF;
			CAN_2.IFRH.R=0xFFFFFFFF;
			
			CAN_2.IMRL.R=pFlexCanCfg->IntMaskL;
			CAN_2.IMRH.R=pFlexCanCfg->IntMaskH;
			
			switch(pFlexCanCfg->BaudRate)		//波特率设置
			{
				case 125:
				{
					CAN_2.CR.B.PRESDIV=7;
					CAN_2.CR.B.RJW=1;
					CAN_2.CR.B.PSEG1=2;
					CAN_2.CR.B.PSEG2=1;
					CAN_2.CR.B.PROPSEG=1;	
				}break;
				case 250:
				default:
				{
					CAN_2.CR.B.PRESDIV=3;
					CAN_2.CR.B.RJW=1;
					CAN_2.CR.B.PSEG1=2;
					CAN_2.CR.B.PSEG2=1;
					CAN_2.CR.B.PROPSEG=1;	
				}break;
				case 500:
				{
					CAN_2.CR.B.PRESDIV=1;
					CAN_2.CR.B.RJW=1;
					CAN_2.CR.B.PSEG1=2;
					CAN_2.CR.B.PSEG2=1;
					CAN_2.CR.B.PROPSEG=1;	
				}break;
				case 800:
				{
					CAN_2.CR.B.PRESDIV=0;
					CAN_2.CR.B.RJW=1;
					CAN_2.CR.B.PSEG1=2;
					CAN_2.CR.B.PSEG2=1;
					CAN_2.CR.B.PROPSEG=3;	
				}break;
				case 1000:
				{
					CAN_2.CR.B.PRESDIV=0;
					CAN_2.CR.B.RJW=1;
					CAN_2.CR.B.PSEG1=2;
					CAN_2.CR.B.PSEG2=1;
					CAN_2.CR.B.PROPSEG=1;	
				}break;
			}	
				
			CAN_2.MCR.R=0x0000003F;
		}break;
	}
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : InitialFlexCAN()
//描  述  : 对CAN通道进行初始化 
//输入参数: 无
//返回参数: 无
//*****************************************************************
void InitialFlexCAN(void)
{
	FlexCANCfg FlexCANConfg;
	
	FlexCANConfg.CAN_Num=CAN2;				//设置CAN通道
	FlexCANConfg.BaudRate=1000;				//设置通信波特率
	FlexCANConfg.IntMaskL=0x11;				//设置中断允许位
	FlexCANConfg.IntMaskH=0;
	
	FlexCANCONFG(&FlexCANConfg);			//调用函数配置CAN通道
	
	CAN_2.RXIMR[4].R=CANRxFilterID;			//设置接收报文的过滤ID
	
	FlexCANMBMsgConfg(CAN2,0,CAN_Tx,Std_Frame,8,2,CANTxFilterID);//配置发送缓冲通道
	FlexCANMBMsgConfg(CAN2,4,CAN_Rx,Std_Frame,8,2,CANRxFilterID);//配置接收缓冲通道	
	
	//INTC_InstallINTCInterruptHandler(CAN_SRecrod_TxISR,FLEXCAN_2_BUF_00_03_IRQ,3);
	//INTC_InstallINTCInterruptHandler(CAN_SRecrod_RxISR,FLEXCAN_2_BUF_04_07_IRQ,5);
	
	
	//*(INT32U*)(0x400001B0) = (INT32U)CAN_SRecrod_TxISR;//测试用
	//*(INT32U*)(0x400001B4) = (INT32U)CAN_SRecrod_RxISR;//测试用

	//#if 0
	*(uint32_t*)(0x400001B0) = RAM_ADDR_ISR_FLEXCAN0003;//设置函数的入口地址
	*(uint32_t*)(0x400001B4) = RAM_ADDR_ISR_FLEXCAN0407;//设置函数的入口地址
	
	CopyFlashDataToRAM((uint32_t*)CAN_SRecrod_TxISR,(uint32_t*)RAM_ADDR_ISR_FLEXCAN0003,Length_ISR_FLEXCAN0003);//将中断函数复制到RAM区运行
	CopyFlashDataToRAM((uint32_t*)CAN_SRecrod_RxISR,(uint32_t*)RAM_ADDR_ISR_FLEXCAN0407,Length_ISR_FLEXCAN0407);//将中断函数复制到RAM区运行
	
	
	INTC.PSR[FLEXCAN_2_BUF_00_03_IRQ].B.PRI = 3;//设置发送中断的优先级
	INTC.PSR[FLEXCAN_2_BUF_04_07_IRQ].B.PRI = 5;//设置接收中断的优先级
	//#endif
}

#pragma pop