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

CANReceive_Type  CANRxMsg;		//CAN��������֡������
CANReceive_Type  CANTxMsg;		//CAN��������֡������

unsigned char TxDataBuffer[8]={0};		//CAN�������ݻ�����
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
//������  : FlexCANTxData()
//��  ��  : FlexCAN���ͺ��� 
//�������: CANChannel	: ���������ݵ�CANͨ��
//		  : MBNum		: �����͵�����CAN���Ļ�����
//		  :	*pData		: ָ������͵������׵�ַ
//���ز���: TRUE		: ���ͳɹ�
//		  :	FALSE		: ����ʧ��
//*****************************************************************
static unsigned char  FlexCANTxData(CANReceive_Type *pCANTxDatum)
{
    unsigned char cnts;
    unsigned short TimeCounts=0;
    static unsigned char datalength;

    datalength=(unsigned char)CAN_2.BUF[0].CS.B.LENGTH;;//��ȡ���������ݵĳ���
   
    if(TxInactiveFlag==CAN_2.BUF[0].CS.B.CODE)	//�жϵ�ǰ�Ƿ���Է�������
    {
    	TxSuccFlg=FALSE;
    	for (cnts = 0; cnts < 8; cnts++)		//��ʼ�����ͻ�����
	    {
	        CAN_2.BUF[0].DATA.B[cnts] = 0xFF;
	    }
	    
    	for (cnts = 0; cnts < datalength; cnts++)//������װ�뷢�ͻ���������������
	    {
	        CAN_2.BUF[0].DATA.B[cnts] = pCANTxDatum->data[cnts];
	    }
	     
    }
    CAN_2.BUF[0].CS.B.CODE=TxActiveFlag;	//���÷��ͻ�����Ϊ����״̬����������������	
	while(TxSuccFlg==FALSE)					//�ȴ���Ӧ�����������������
	{
		TimeCounts++;						//��ʱ
		if(TimeCounts==2000)					
			return TxSuccFlg;				//��ʱʱ�䵽����Ȼû�з�����ɣ��򷵻ط���ʧ����Ϣ
	};
	CAN_2.IFRL.B.BUF00I = 1;				//��������жϱ�־λ
	return TxSuccFlg;						//���ط��ͳɹ���Ϣ
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : CANStatusAcknowledge()
//��  ��  : ���ư�����λ����������Ϣ���� 
//�������: status		: ��ǰ�Ĵ���״̬
//���ز���: ��
//*****************************************************************
void MCUStatusAcknowledge(unsigned char status)
{
	unsigned char cnts,err;
	
	for(cnts=0;cnts<8;cnts++)		//�����ݻ�������ʼ��
	{
		TxDataBuffer[cnts] = 0xFF;
	}
	CANTxMsg.datalength = 3;
	TxDataBuffer[1]=status;			//����Ҫ���͵����ݵ���Ӧ���ݻ�����
	
	CANTxMsg.data = (unsigned char*)&TxDataBuffer[0];
	for(cnts=0;cnts<CANTxMsg.datalength;cnts++)	//������֡���뷢�ͻ�����
	{
		CANTxMsg.data[cnts]=*(&TxDataBuffer[cnts]);
	}
	err=FlexCANTxData(&CANTxMsg);		//�ж��Ƿ��ͳɹ�
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : CANWorkStatusSet()
//��  ��  : ���ư�����λ����������Ϣ����
//�������: status		: ��Ҫ���õ�״̬
//���ز���: ��
//*****************************************************************
static void CANWorkStatusSet(unsigned char status)
{
	MCUBootldState=status;
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : CanWorkStatusGet()
//��  ��  : ȡ���ư�����λ����CAN��״̬
//�������: ��
//���ز���: ��ǰCAN�ߵ�״̬
//*****************************************************************
unsigned char CanWorkStatusGet(void)
{
    return MCUBootldState;
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : CAN_SRecrod_RxISR()
//��  ��  : CAN���պͷ��ʹ����жϺ���
//�������: ��
//���ز���: ��
//*****************************************************************
void CAN_SRecrod_RxISR(void)
{
	unsigned char cnts;
	uint32_t FlagWord;
	
		
	if(CAN_2.IFRL.B.BUF04I)						//�жϽ����жϱ�־λ
	{
		FlagWord=CAN_2.BUF[04].CS.R;
		
		for(cnts=0;cnts<10;cnts++) NOP();
		if(!CAN_2.BUF[04].CS.B.IDE)				//�ж��Ƿ�Ϊ����Ҫ����չ֡��ʽ
		{
			CANRxMsg.datalength=(unsigned char)CAN_2.BUF[04].CS.B.LENGTH; //�õ����յ������ݳ���
			CANRxMsg.data=(unsigned char*)&CAN_2.BUF[04].DATA.B[0];		  //���������ݵĴ��ָ��ָ����ջ������������׵�ַ

			if(CANRxMsg.MsgID==0x200)				//�ж��Ƿ�����Ҫ��MessageID
			{
				for(cnts=0;cnts<CANRxMsg.datalength;cnts++)  //ȡ�ؽ��յ���������Ϣ
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
//������  : CAN_0_ERR_ISR()
//��  ��  : CAN�����жϴ�����
//�������: ��
//���ز���: ��
//*****************************************************************
void CAN_2_ERR_ISR(void)
{
	CAN_2.ESR.R=0XFFFF;
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : CAN_SRecrod_TxISR()
//��  ��  : CAN�����жϴ�����
//�������: ��
//���ز���: ��
//*****************************************************************
void CAN_SRecrod_TxISR(void)
{
	if(CAN_2.IFRL.B.BUF00I == 1)
	{
		CAN_2.IFRL.B.BUF00I = 1;	//����жϱ�־λ
		TxSuccFlg=TRUE;				//��λ���ͳɹ���־		
	}
	
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : CANBootloader_Analysis()
//��  ��  : �Խ��յ������ݽ��з�������
//�������: ��
//���ز���: ��
//*****************************************************************
void CANBootloader_Analysis(void)
{
	unsigned char cnts,err;
	unsigned char ProcFlag;
	static unsigned char OutBootloaderFlag=0;
	static unsigned char pSDataNum=0;
	static SFileRecord SrcdDatum;
	
	CANRxNewFram = FALSE;						//���CAN���յ������ݵı�־λ
	if(MCUBusyFlg)
	{
		if(CANRxMsg.data[0]!=DOWN_OUTBOOT_CMD)	//���MCU���ڴ�����һ�ε�����򷵻�MCUæ״̬
		{
			MCUStatusAcknowledge(UP_BUSY);
			return;
		}		
	}
	switch(CANRxMsg.data[0])					//�����յ�������֡��һ֡
	{
		MCUBusyFlg=TRUE;
		case DOWN_INBOOTSUC_CMD:				//�Ƿ�Ϊ��λ������ָ��
		{
			NOP();
			NOP();
			pSDataNum=0;

			MCUStatusAcknowledge(UP_INBOOT_SUC);//����CAN����״̬
			MCUBusyFlg=FALSE;					//�����ǰMCUæ״̬
		
			CANWorkStatusSet(STA_LINKED);		//����MCUͬ��λ��������״̬Ϊ���ӳɹ���־
			break;
		}
		case DOWN_ERASE_CMD:					//�յ���λ���Ĳ���Flashָ��
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
					Erase_Flash();   //����Flash
					ProgramDataBuf.status=ProgramDataNotReady;
    
				    ExchangeDataBuffer.status=DataBufferEmpty;
				    ExchangeDataBuffer.addr=0;
				    ExchangeDataBuffer.data=0;
				   
					if(EraseFlagStatus==EraseSuce)//�ж�MCU�Ƿ��Ѿ���Flash���й�����������û�������Flash����
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
		case DOWN_LINE_END:						//�յ���λ��������һ�����ݵ�ָ��
		{
			ProcFlag=S19DataConvert(&S_FileOnelineData[0], &SInfroRecord);//���յ����������ݽ��д���
			pSDataNum=0;
			
			 
			if (ProcFlag == SPASS)			//����У���Ƿ�ɹ��
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
                FlashBlkRead(SrcdDatum.data, SInfroRecord.addr, SInfroRecord.datalen);//��ȡд��Flash�е�����
                
                for (cnts = 0; cnts < SInfroRecord.datalen; cnts++)  //�Ƚ��յ���������д��������Ƿ�һ��
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
            	MCUStatusAcknowledge(UP_DOWN_ERR); //У�鲻�ɹ����򷵻ش�����Ϣ
            	MCUBusyFlg=FALSE;				   //�����ǰMCUæ״̬	
            } 
            else if (ProcFlag == SInvalid)
            {
            	MCUStatusAcknowledge(UP_DOWN_ERR); //У�鲻�ɹ����򷵻ش�����Ϣ
            	MCUBusyFlg=FALSE;				  //�����ǰMCUæ״̬
            }  
            else
            {
            	MCUStatusAcknowledge(UP_PROONELINE_SUC); //����ͨ��״̬
            	MCUBusyFlg=FALSE;						//�����ǰMCUæ״̬
            }  
            break;
		}
		case DOWN_FILE_END:    					//�յ��ļ��������ָ��
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
            MCUStatusAcknowledge(UP_PRGEND);	//����ͨ��״̬
            MCUBusyFlg=FALSE;					//�����ǰMCUæ״̬
            CANWorkStatusSet(STA_PRGEND);		
            break;
        }
        case DOWN_OUTBOOT_CMD:
        {
        	if((CANRxMsg.data[7]==0x80)&&(OutBootloaderFlag==0))
        	{
        		if(CanWorkStatusGet()==STA_PRGEND)
	        	{
	        		MCUStatusAcknowledge(UP_OUTBL_SUC);	 //����ͨ��״̬
	        		CopyFlashDataToRAM((uint32_t*)EraseSetEnterBootFlagFlash,(uint32_t*)RAM_ADDR_ERASE_FLASH,Length_copy_ram);//���жϺ������Ƶ�RAM������
        			(*(APPFunction)RAM_ADDR_ERASE_FLASH)();
	        		//INTC.MCR.B.HVEN = 1; 			 	 //��ֹ�ж�
					CAN_2.MCR.B.MDIS = 1;          		 //��ֹCAN
					DlyS_Ms(100);
			        (*(APPFunction)APP_ENRTY_ADDR)();    //��ת��Ӧ�ó���
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
        			MCUStatusAcknowledge(UP_OUTBL_SUC);	 //����ͨ��״̬
        			CopyFlashDataToRAM((uint32_t*)EraseSetEnterBootFlagFlash,(uint32_t*)RAM_ADDR_ERASE_FLASH,Length_copy_ram);//���жϺ������Ƶ�RAM������
        			(*(APPFunction)RAM_ADDR_ERASE_FLASH)();
	        		//INTC.MCR.B.HVEN = 1; 			 	 //��ֹ�ж�
					CAN_2.MCR.B.MDIS = 1;          		 //��ֹCAN
					DlyS_Ms(100);
			        (*(APPFunction)APP_ENRTY_ADDR)();    //��ת��Ӧ�ó���
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
            if (CanWorkStatusGet() == STA_LINKED)//���յ���Ϊ������ʱ
            {
                for (cnts = 0; cnts < CANRxMsg.datalength; cnts++) 
                {
                    S_FileOnelineData[pSDataNum++] = CANRxMsg.data[cnts];  //�����ݴ��뻺����
                }
                MCUBusyFlg=FALSE;				//�����ǰMCUæ״̬
            }
            break;
        }
	}
}
#pragma pop