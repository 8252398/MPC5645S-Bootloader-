//***************************************************************** 
//* FILE        : S_FileReceive.c
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
#include "S_FileReceive.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

CANReceive_Type  CANRxMsg;		//CAN接收数据帧缓冲区
CANReceive_Type  CANTxMsg;		//CAN发送数据帧缓冲区

unsigned char TxDataBuffer[8]={0};		//CAN发送数据缓冲区
unsigned char S_FileOnelineData[128] = {0};
CANBootloaderStatus CANBootldSts;
SFileRecord SInfroRecord;
ProgramDataBuffer ProgramDataBuf;
DataBuffer	ExchangeDataBuffer;

unsigned short LowFlashSelectFlag;
unsigned char  MidFlashSelectFlag;
unsigned char  HigFlashSelectFlag;
unsigned char  EraseFlag=0;
/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/
#pragma push
#pragma section code_type ".boot_flash" 
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : FlexCANTxData()
//描  述  : FlexCAN发送函数 
//输入参数: CANChannel	: 待发送数据的CAN通道
//		  : MBNum		: 待发送的数据CAN报文缓冲区
//		  :	*pData		: 指向待发送的数据首地址
//返回参数: TRUE		: 发送成功
//		  :	FALSE		: 发送失败
//*****************************************************************
static unsigned char  FlexCANTxData(CANReceive_Type *pCANTxDatum)
{
    unsigned char cnts;
    unsigned short TimeCounts=0;
    static unsigned char datalength;

    datalength=(unsigned char)CAN_2.BUF[0].CS.B.LENGTH;;//获取待发送数据的长度
   
    if(TxInactiveFlag==CAN_2.BUF[0].CS.B.CODE)	//判断当前是否可以发送数据
    {
    	TxSuccFlg=FALSE;
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
	while(TxSuccFlg==FALSE)					//等待相应缓冲区发送数据完成
	{
		TimeCounts++;						//延时
		if(TimeCounts==2000)					
			return TxSuccFlg;				//延时时间到，依然没有发送完成，则返回发送失败信息
	};
	CAN_2.IFRL.B.BUF00I = 1;				//清除发送中断标志位
	return TxSuccFlg;						//返回发送成功信息
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : CANStatusAcknowledge()
//描  述  : 控制板与上位机的握手信息发送 
//输入参数: status		: 当前的传输状态
//返回参数: 无
//*****************************************************************
void MCUStatusAcknowledge(unsigned char status)
{
	unsigned char cnts,err;
	
	for(cnts=0;cnts<8;cnts++)		//对数据缓冲区初始化
	{
		TxDataBuffer[cnts] = 0xFF;
	}
	CANTxMsg.datalength = 3;
	TxDataBuffer[1]=status;			//填入要发送的数据到相应数据缓冲区
	
	CANTxMsg.data = (unsigned char*)&TxDataBuffer[0];
	for(cnts=0;cnts<CANTxMsg.datalength;cnts++)	//将数据帧放入发送缓冲区
	{
		CANTxMsg.data[cnts]=*(&TxDataBuffer[cnts]);
	}
	err=FlexCANTxData(&CANTxMsg);		//判断是否发送成功
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : CANWorkStatusSet()
//描  述  : 控制板与上位机的握手信息设置
//输入参数: status		: 需要设置的状态
//返回参数: 无
//*****************************************************************
static void CANWorkStatusSet(unsigned char status)
{
	MCUBootldState=status;
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : CanWorkStatusGet()
//描  述  : 取控制板与上位机的CAN线状态
//输入参数: 无
//返回参数: 当前CAN线的状态
//*****************************************************************
unsigned char CanWorkStatusGet(void)
{
    return MCUBootldState;
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : CAN_SRecrod_RxISR()
//描  述  : CAN接收和发送处理中断函数
//输入参数: 无
//返回参数: 无
//*****************************************************************
void CAN_SRecrod_RxISR(void)
{
	unsigned char cnts;
	uint32_t FlagWord;
	
		
	if(CAN_2.IFRL.B.BUF04I)						//判断接收中断标志位
	{
		FlagWord=CAN_2.BUF[04].CS.R;
		
		for(cnts=0;cnts<10;cnts++) NOP();
		if(!CAN_2.BUF[04].CS.B.IDE)				//判断是否为所需要的扩展帧格式
		{
			CANRxMsg.datalength=(unsigned char)CAN_2.BUF[04].CS.B.LENGTH; //得到接收到的数据长度
			CANRxMsg.data=(unsigned char*)&CAN_2.BUF[04].DATA.B[0];		  //将接收数据的存放指针指向接收缓冲区的数据首地址

			if(CANRxMsg.MsgID==0x200)				//判断是否是需要的MessageID
			{
				for(cnts=0;cnts<CANRxMsg.datalength;cnts++)  //取回接收到的数据信息
				{
					CANRxMsg.data[cnts]=CAN_2.BUF[04].DATA.B[cnts];
				}
			}
			CAN_2.BUF[04].CS.B.CODE=0x04;
		}
		CANRxNewFram = TRUE;
		FlagWord=CAN_2.TIMER.R;
		CAN_2.IFRL.B.BUF04I=TRUE;
		for(cnts=0;cnts<10;cnts++) NOP();
	}
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : CAN_0_ERR_ISR()
//描  述  : CAN错误中断处理函数
//输入参数: 无
//返回参数: 无
//*****************************************************************
void CAN_2_ERR_ISR(void)
{
	CAN_2.ESR.R=0XFFFF;
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : CAN_SRecrod_TxISR()
//描  述  : CAN发送中断处理函数
//输入参数: 无
//返回参数: 无
//*****************************************************************
void CAN_SRecrod_TxISR(void)
{
	if(CAN_2.IFRL.B.BUF00I == 1)
	{
		CAN_2.IFRL.B.BUF00I = 1;	//清楚中断标志位
		TxSuccFlg=TRUE;				//置位发送成功标志		
	}
	
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : CANBootloader_Analysis()
//描  述  : 对接收到的数据进行分析处理
//输入参数: 无
//返回参数: 无
//*****************************************************************
void CANBootloader_Analysis(void)
{
	unsigned char cnts,err;
	unsigned char ProcFlag;
	static unsigned char OutBootloaderFlag=0;
	static unsigned char pSDataNum=0;
	static SFileRecord SrcdDatum;
	
	CANRxNewFram = FALSE;						//清除CAN接收到新数据的标志位
	if(MCUBusyFlg)
	{
		if(CANRxMsg.data[0]!=DOWN_OUTBOOT_CMD)	//如果MCU还在处理上一次的命令，则返回MCU忙状态
		{
			MCUStatusAcknowledge(UP_BUSY);
			return;
		}		
	}
	switch(CANRxMsg.data[0])					//解析收到的数据帧第一帧
	{
		MCUBusyFlg=TRUE;
		case DOWN_INBOOTSUC_CMD:				//是否为上位机连接指令
		{
			NOP();
			NOP();
			pSDataNum=0;

			MCUStatusAcknowledge(UP_INBOOT_SUC);//设置CAN连接状态
			MCUBusyFlg=FALSE;					//清除当前MCU忙状态
		
			CANWorkStatusSet(STA_LINKED);		//设置MCU同上位机的连接状态为连接成功标志
			break;
		}
		case DOWN_ERASE_CMD:					//收到上位机的擦除Flash指令
		{
			 MCUStatusAcknowledge(UP_GETERASECMD);
			 if(!EraseFlag) 
			 {
			 	LowFlashSelectFlag=CANRxMsg.data[6];
			 	LowFlashSelectFlag=(unsigned short)((LowFlashSelectFlag<<8)|CANRxMsg.data[7]);
			 	MidFlashSelectFlag=CANRxMsg.data[5];
				HigFlashSelectFlag=CANRxMsg.data[4];
				if((CANRxMsg.data[7]&0x01)||(CANRxMsg.data[7]&0x02))
				{
					EraseFlag=0;
					MCUStatusAcknowledge(UP_ERASEBOOTAREA_ERR);
				}
				else
				{
					EraseFlag++;
					Erase_Flash();   //擦除Flash
					ProgramDataBuf.status=ProgramDataNotReady;
    
				    ExchangeDataBuffer.status=DataBufferEmpty;
				    ExchangeDataBuffer.addr=0;
				    ExchangeDataBuffer.data=0;
				   
					if(EraseFlagStatus==EraseSuce)//判断MCU是否已经对Flash进行过擦除操作，没有则进行Flash擦除
	                {
	                	MCUStatusAcknowledge(UP_ERASE_SUC);
	                	EraseFlag=0;
	                }
	                	
	                else if(EraseFlagStatus==EraseFail)
	                {
	                	MCUStatusAcknowledge(UP_ERASEERR);
	                	EraseFlag=0;
	                }	
				}			 	
			 }
			 else
			 {
			    MCUStatusAcknowledge(UP_BUSY);
			 }
			 break;
		}
		case DOWN_LINE_END:						//收到上位机传送完一行数据的指令
		{
			ProcFlag=S19DataConvert(&S_FileOnelineData[0], &SInfroRecord);//对收到的整行数据进行处理
			pSDataNum=0;
			
			 
			if (ProcFlag == SPASS)			//检验校验是否成功�
			{
				while(SInfroRecord.status!=LineDataEnd)
                {
                	DoubleWordProgramSytleProcess(&ExchangeDataBuffer,&SInfroRecord,&ProgramDataBuf);
                	if(ProgramDataBuf.status==ProgramDataReady)
                	{
                		if(ProgramDataBuf.addr>=APP_ENRTY_ADDR)
                		{
			                CAN_2.MCR.B.MDIS = 1;
			                for(cnts=0;cnts<20;cnts++)
			                {
			                	NOP();
			                }
			                err=(unsigned char)FlashProgramer(&ProgramDataBuf);
			                for(cnts=0;cnts<20;cnts++)
			                {
			                	NOP();
			                }
			                CAN_2.MCR.B.MDIS = 0;
			                if(err==ProFail)  
			                {
				            	MCUStatusAcknowledge(UP_FLASH_ERR);
				            	EraseFlag=0;
				            	return;
			                }
                		}
                		else
                		{
				            if(SInfroRecord.status==LineDataEnd)
				            	MCUStatusAcknowledge(UP_PROONELINE_SUC);
				            return;
                		}
                	}
                }
                FlashBlkRead(SrcdDatum.data, SInfroRecord.addr, SInfroRecord.datalen);//读取写入Flash中的数据
                
                for (cnts = 0; cnts < SInfroRecord.datalen; cnts++)  //比较收到的数据与写入的数据是否一致
	            {
	                if (SrcdDatum.data[cnts] != SInfroRecord.data[cnts])
	                {
						MCUStatusAcknowledge(UP_FLASH_ERR);
						EraseFlag=0;
	                    return;
	                }
	            }
	        	MCUStatusAcknowledge(UP_PROONELINE_SUC);
			}
     
            else if (ProcFlag == SERR)
            {
            	MCUStatusAcknowledge(UP_DOWN_ERR); //校验不成功，则返回错误信息
            	MCUBusyFlg=FALSE;				   //清除当前MCU忙状态	
            } 
            else if (ProcFlag == SInvalid)
            {
            	MCUStatusAcknowledge(UP_DOWN_ERR); //校验不成功，则返回错误信息
            	MCUBusyFlg=FALSE;				  //清除当前MCU忙状态
            }  
            else
            {
            	MCUStatusAcknowledge(UP_PROONELINE_SUC); //设置通信状态
            	MCUBusyFlg=FALSE;						//清除当前MCU忙状态
            }  
            break;
		}
		case DOWN_FILE_END:    					//收到文件传输完成指令
        {
            if(ExchangeDataBuffer.status==DataBufferFull)
            {
            	ProgramDataBuf.status=ProgramDataReady;
            	ProgramDataBuf.addr=ExchangeDataBuffer.addr;
            	ProgramDataBuf.data[0]=ExchangeDataBuffer.data;
            	ProgramDataBuf.data[1]=0x00;
            	
            	if(ProgramDataBuf.status==ProgramDataReady)
            	{
            		if(ProgramDataBuf.addr>=APP_ENRTY_ADDR)
            		{
            			err=(unsigned char)FlashProgramer(&ProgramDataBuf);
		                NOP();
		                if(err==ProFail)  
		                {
			            	MCUStatusAcknowledge(UP_FLASH_ERR);
			            	EraseFlag=0;
			            	return;
		                }
            		}
            	}
            }
            MCUStatusAcknowledge(UP_PRGEND);	//设置通信状态
            MCUBusyFlg=FALSE;					//清除当前MCU忙状态
            CANWorkStatusSet(STA_PRGEND);		
            break;
        }
        case DOWN_OUTBOOT_CMD:
        {
        	if((CANRxMsg.data[7]==0x80)&&(OutBootloaderFlag==0))
        	{
        		if(CanWorkStatusGet()==STA_PRGEND)
	        	{
	        		MCUStatusAcknowledge(UP_OUTBL_SUC);	 //设置通信状态
	        		CopyFlashDataToRAM((uint32_t*)EraseSetEnterBootFlagFlash,(uint32_t*)RAM_ADDR_ERASE_FLASH,Length_copy_ram);//将中断函数复制到RAM区运行
        			(*(APPFunction)RAM_ADDR_ERASE_FLASH)();
	        		//INTC.MCR.B.HVEN = 1; 			 	 //禁止中断
					CAN_2.MCR.B.MDIS = 1;          		 //禁止CAN
					DlyS_Ms(100);
			        (*(APPFunction)APP_ENRTY_ADDR)();    //跳转到应用程序
			        for(;;)
			        ;
	        	}
	        	else
	        	{
	        		MCUStatusAcknowledge(UP_PRO_NO_END_ERR);
	        		OutBootloaderFlag++;
	        	}
        	}
        	else if((CANRxMsg.data[7]==0xA0)&&(OutBootloaderFlag==1))
        	{
        		if(OutBootloaderFlag)
        		{
        			OutBootloaderFlag=0;
        			MCUStatusAcknowledge(UP_OUTBL_SUC);	 //设置通信状态
        			CopyFlashDataToRAM((uint32_t*)EraseSetEnterBootFlagFlash,(uint32_t*)RAM_ADDR_ERASE_FLASH,Length_copy_ram);//将中断函数复制到RAM区运行
        			(*(APPFunction)RAM_ADDR_ERASE_FLASH)();
	        		//INTC.MCR.B.HVEN = 1; 			 	 //禁止中断
					CAN_2.MCR.B.MDIS = 1;          		 //禁止CAN
					DlyS_Ms(100);
			        (*(APPFunction)APP_ENRTY_ADDR)();    //跳转到应用程序
			        for(;;)
			        ;
        		}	
        	}
        	else if(CANRxMsg.data[7]==0xF0)
        	{
        		OutBootloaderFlag=0;
        		MCUStatusAcknowledge(UP_CANCELINBOOT);
        	}
        	
        	break;
        }
        default:               
        {
            if (CanWorkStatusGet() == STA_LINKED)//接收到的为纯数据时
            {
                for (cnts = 0; cnts < CANRxMsg.datalength; cnts++) 
                {
                    S_FileOnelineData[pSDataNum++] = CANRxMsg.data[cnts];  //将数据存入缓冲区
                }
                MCUBusyFlg=FALSE;				//清除当前MCU忙状态
            }
            break;
        }
	}
}
#pragma pop