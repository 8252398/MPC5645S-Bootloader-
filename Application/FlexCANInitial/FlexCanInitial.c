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
extern void CAN_Rx_ISR(void);
extern void CAN_Tx_ISR(void);

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/
#pragma push
#pragma section code_type ".userApplication_flash"
__declspec(section ".userApplication_flash")
//*****************************************************************
//������  : APPFlexCANMBMsgConfg()
//��  ��  : ��CAN�����仺������������ 
//�������: CANChannel: CANͨ��ѡ��
//		  : MBNum	  : ���������ѡ��
//		  : Tx_RxFlag :	���ͣ���������
//		  : IDE       :	��չ֡����׼֡����
//		  : DLC		  :	���ݳ�������
//		  : Prio	  : ���ȼ�����
//		  : ID		  : ������������
//���ز���: ��
//*****************************************************************
static void APPFlexCANMBMsgConfg(unsigned char CANChannel,unsigned char MBNum,unsigned char Tx_RxFlag,unsigned char IDE,unsigned char DLC,unsigned char Prio,uint32_t ID)
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
__declspec(section ".userApplication_flash")
//*****************************************************************
//������  : APPFlexCANCONFG(FlexCANCfg FlexCanCfg)
//��  ��  : ��CAN0ͨ����ʼ�� 
//�������: FlexCanCfg ����������Ҫ�������õĲ����ͱ�־λ
//���ز���: ��
//*****************************************************************
static void APPFlexCANCONFG(FlexCANCfg *pFlexCanCfg)
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

			CAN_2.IMRL.R=pFlexCanCfg->IntMaskL;
			CAN_2.IMRH.R=pFlexCanCfg->IntMaskH;
			
			CAN_2.IFRL.R=0xFFFFFFFF;
			CAN_2.IFRH.R=0xFFFFFFFF;
			
			switch(pFlexCanCfg->BaudRate)		//����������
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
			}
			//CAN_2.CR.B.LPB=1;
			CAN_2.MCR.R=0x0000003F;
		}break;
		
	}
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//������  : InitialFlexCANAPP()
//��  ��  : ��CANͨ�����г�ʼ�� 
//�������: ��
//���ز���: ��
//*****************************************************************
void InitialFlexCANAPP(void)
{
	FlexCANCfg FlexCANIntialConfg;
	
	FlexCANIntialConfg.CAN_Num=CAN2;				//����CANͨ��
	FlexCANIntialConfg.BaudRate=250;				//����ͨ�Ų�����
	FlexCANIntialConfg.IntMaskL=0x11;				//�����ж�����λ
	FlexCANIntialConfg.IntMaskH=0;
	
	//CAN_2.RXIMR[63].R=CANRxFilterID;		//���ý��ձ��ĵĹ���ID
		
	//APPFlexCANMBMsgConfg(CAN2,62,CAN_Tx,Std_Frame,8,2,CANTxFilterID);//���ý��ջ���ͨ��
	//APPFlexCANMBMsgConfg(CAN2,63,CAN_Rx,Std_Frame,8,2,CANRxFilterID);//���ý��ջ���ͨ��
	
	APPFlexCANCONFG(&FlexCANIntialConfg);			//���ú�������CANͨ��
	
	APPFlexCANMBMsgConfg(CAN2,0,CAN_Tx,Std_Frame,8,2,CANTxFilterID);//���ý��ջ���ͨ��
	APPFlexCANMBMsgConfg(CAN2,4,CAN_Rx,Std_Frame,8,2,CANRxFilterID);//���ý��ջ���ͨ��

	INTC_InstallINTCInterruptHandler(CAN_Tx_ISR,FLEXCAN_2_BUF_00_03_IRQ,3);
	INTC_InstallINTCInterruptHandler(CAN_Rx_ISR,FLEXCAN_2_BUF_04_07_IRQ,5);
		
}
#pragma pop


