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
#include "FlexCAN_RxTx.h"
#include "MPC55xx_PCRdef.h"
#include "InterruptsIRQDef.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/


/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/


//*****************************************************************
//函数名  : InitialFlexCANAPP()
//描  述  : 对CAN通道进行初始化 
//输入参数: 无
//返回参数: 无
//*****************************************************************
void InitialFlexCANAPP(void);


#endif