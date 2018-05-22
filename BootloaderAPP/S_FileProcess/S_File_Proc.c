//***************************************************************** 
//* FILE        : S_File_Proc.c
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
#include "S_File_Proc.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/
#pragma push
#pragma section code_type ".boot_flash" 
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : Hex2Value()
//描  述  : 将收到的16进制数据转换成十进制数据
//输入参数: 无
//返回参数: 无
//*****************************************************************
static unsigned char Hex2Value(unsigned char *p)
{   
    unsigned char k,i;

    k=*p;
    p++;
    i=*p;
    p++;

    if(k>='0' && k<='9')
        k=k-'0';
    else
        k=k-'A'+10;
    
    if(i>='0' && i<='9')
        i=i-'0';
    else
        i=i-'A'+10;
    
    return k*16+i;
}
__declspec(section ".boot_flash")
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
unsigned char S19DataConvert(unsigned char *OneLineDatum, SFileRecord *pSDataRcd) 
{   
    unsigned char  SFrametype,AddressLen,SFramelength,Sdatum,SDatalen,oddflag = 0;
    unsigned char  checksum = 0;
    unsigned char  *pLineData;
    uint32_t  temp;
    unsigned short  i,j,k;
    
    pLineData=OneLineDatum;
    
    for(i=0;i<5;i++) pSDataRcd->data[i]=0;
  
    if(*pLineData != 'S') 				//判断报文开始标志
    {
        pSDataRcd->datalen = 0;      
		return SInvalid;					//如果报文开头不为'S'，则返回报文无效标志
    }
		
    pLineData++;							//累加报文地址
    SFrametype = *pLineData - '0';		//得到报文的格式

	if(SFrametype == 0) 					//判断报文的格式信息
	{
        pSDataRcd->datalen = 0;
        return SFileStart;					//如果为'S0'格式，则返回文件开始标志
	}
	else if(SFrametype > 6) 
	{
        pSDataRcd->datalen = 0;
        return SFileEnd;					//如果报文格式为'S7','S8','S9',则返回文件结束标志
	}
	else 
	{
        AddressLen = SFrametype + 1;        //得到报文中所传输的地址长度信息  

        pLineData++;						//累加报文地址
        SFramelength = Hex2Value(pLineData);  //得到报文传输的帧数据长度信息

        SDatalen = SFramelength-AddressLen-1;//得到报文传输的数据长度信息   
        if(SDatalen&0x01) 					 //判断是否为偶数长度数据
        {           
            oddflag = TRUE;             	//如果为奇数，则置位奇数标志位为1
        }
        else oddflag = FALSE; 
        pSDataRcd->datalen = (unsigned short)((SFramelength-AddressLen-1)/4); //保存报文传输的数据长度信息 
                                                      
        checksum += SFramelength;			//校验用

        pLineData += 2;					//地址累加
        
        for(pSDataRcd->addr = 0, i = 0; i < AddressLen; i++, pLineData += 2) //取报文传输的地址信息
        {
            Sdatum = Hex2Value(pLineData);
            pSDataRcd->addr = (pSDataRcd->addr<<8) | Sdatum;
            checksum += Sdatum;
        }
		
        j=k=0;
        for(i = 0; i < SDatalen; i++) 
        {
            Sdatum = Hex2Value(pLineData);	//取报文所传输的数据信息到缓冲区
        	pLineData += 2;
        	checksum += Sdatum;
        	pSDataRcd->data[k] = pSDataRcd->data[k]<<8;
        	pSDataRcd->data[k] = pSDataRcd->data[k]|Sdatum;
        	j++;
        	if(j>3)
    		{
    			j=0;
    			k++;
    		}
        }
    }
    
    for(i=0;i<pSDataRcd->datalen;i++)
    	temp=pSDataRcd->data[i];
    temp=pSDataRcd->addr;
    temp=pSDataRcd->datalen;
    
    pSDataRcd->status=LineDataEffect;
    
    if ( (checksum + Hex2Value(pLineData)) == 0xFF )  //校验报文
    {
        return SPASS;					//如果校验成功，则返回报文校验成功信息
    }
    else
    {
        return SERR;					//如果校验不成功，则返回报文校验出错信息
    }
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : DoubleWordProgramSytleProcess()
//描  述  : 将收到S19数据写进Flash
//输入参数: *pDataBuf	:数据暂存缓冲区
//		  : *pLineRec	:收到的行数据信息
//		  :	*pProDataBuf:编程数据暂存缓冲区
//返回参数: 无
//*****************************************************************
void DoubleWordProgramSytleProcess(DataBuffer *pDataBuf,SFileRecord *pLineRec,ProgramDataBuffer *pProDataBuf)
{
	unsigned char cnts;
	uint32_t  checkData=0;
	
	if(pLineRec->status!=LineDataEnd)			//判断当前一行数据是否结束
	{
		if(pDataBuf->status==DataBufferEmpty)	//判断数据暂存缓冲区是否为空
		{
			pLineRec->datalen--;				//行数据个数减一
			if(pLineRec->datalen==0)			//判断行数据是否已经取完
			{
				pDataBuf->addr=pLineRec->addr;	//如果数据已经取完，则将当前数据地址保存到数据暂存缓冲区
				pDataBuf->data=pLineRec->data[0];//将数据保存到数据暂存缓冲区
				pDataBuf->status=DataBufferFull; //置位暂存缓冲区数据已满
				pLineRec->status=LineDataEnd;	 //置位行数据保存缓冲区数据已经结束
				pProDataBuf->status=ProgramDataNotReady;//置位编程缓冲区数据没有准备好
			}
			else
			{
				pProDataBuf->addr=pLineRec->addr;		//将当前行数据的地址保存到编程数据暂存缓冲区
				pProDataBuf->data[0]=pLineRec->data[0];	//将要编程的数据保存到编程数据暂存缓冲区
				pProDataBuf->data[1]=pLineRec->data[1];	
				pProDataBuf->status=ProgramDataReady;	//置位编程数据已经准备好
				pLineRec->datalen--;					//行数据个数减一
				if(pLineRec->datalen==0)				//判断行数据是否已经编程结束
				{
					pLineRec->status=LineDataEnd;		//如果编程完，则置位行数据编程结束标志
				}
				else
				{
					for(cnts=0;cnts<pLineRec->datalen;cnts++)	//将行数据左移
					{
						pLineRec->data[cnts]=pLineRec->data[cnts+2];
					}
					pLineRec->addr=pLineRec->addr+8;		//数据地址加8
				}
			}
		}
		else
		{
			if((pDataBuf->addr+4)==pLineRec->addr)	  //如果数据暂存缓冲区地址与新接收数据地址一样
			{
				pProDataBuf->addr=pDataBuf->addr;	  //将数据暂存缓冲区的地址保存到编程数据暂存缓冲区
				pProDataBuf->data[0]=pDataBuf->data;  //将数据填入编程数据暂存缓冲区
				pProDataBuf->data[1]=pLineRec->data[0]; //将新接受的数据的第一个数据填入编程数据暂存缓冲区
				pDataBuf->status=DataBufferEmpty;		//置位数据咱曾缓冲区空
				pProDataBuf->status=ProgramDataReady;   //置位编程数据已经准备好
				pLineRec->datalen--;
				if(pLineRec->datalen==0)				//判断行数据是否已经编程结束
				{
					pLineRec->status=LineDataEnd;		//如果编程完，则置位行数据编程结束标志
				}
				else
				{
					for(cnts=0;cnts<pLineRec->datalen;cnts++)	//将行数据左移
					{
						pLineRec->data[cnts]=pLineRec->data[cnts+1];
					}
					pLineRec->addr=pLineRec->addr+4;	//数据地址加4
				}
			}
			else  //如果数据暂存缓冲区地址与新接收数据地址不一样
			{
				pProDataBuf->addr=pDataBuf->addr; 	//将数据暂存缓冲区的地址保存到编程数据暂存缓冲区
				pProDataBuf->data[0]=pDataBuf->data;	//将数据填入编程数据暂存缓冲区
				pProDataBuf->data[1]=0x00000000;	//连续地址填入0
				pProDataBuf->status=ProgramDataReady; //置位编程数据已经准备好
				pDataBuf->status=DataBufferEmpty;   //置位数据咱曾缓冲区空
			}
		}
	}
	checkData = pProDataBuf->addr;
	checkData = pProDataBuf->data[0];
	checkData = pProDataBuf->data[1];
	if(pDataBuf->status==DataBufferFull)
	{
		checkData = pProDataBuf->addr;
		checkData = pDataBuf->data;		
	}
}
#pragma pop
