/***************************************************************** 
 * FILE        : FlexCANInitial.c
 * 
 * DESCRIPTION : This is the file to Initial the FlexCAN register
 *
 *               In some places this includes alternative register
 *               nomenclature. Comment out the unused version to
 *               match code requirements
 *               The default version is used by Freescale in code
 *               examples
 * 
 * COPYRIGHT   :(c) 2014-2020, by XueLiMan
 * 
 * VERSION     : 1.00
 * DATE        : 1 apir 2014
 * AUTHOR      : lilin 
 ******************************************************************/
 #ifndef MODECTRDEFINE__H
 #define MODECTRDEFINE__H
// For MPC5645S cut 1
//Defines the peripheral control regsiter number
#define 	DSPI_0_Control	   		4
#define 	DSPI_1_Control   		5
#define 	DSPI_2_Control   		6

#define 	DUADSPI_Control		   	8

#define 	FlexCAN_0_Control   	16
#define 	FlexCAN_1_Control   	17
#define 	FlexCAN_2_Control   	18

#define 	DMA_CH_MUX_Control   	23

#define 	BAM_Control			   	31

#define 	ADC_0_Control   		32

#define 	RLE_Control   			37

#define 	VIU_Control   			41

#define 	DRAM_Controller_Control 42

#define 	I2C_DMA_0_Control   	44
#define 	I2C_DMA_1_Control   	45
#define 	I2C_DMA_2_Control   	46
#define 	I2C_DMA_3_Control   	47

#define 	LINFLEX_0_Control   	48
#define 	LINFLEX_1_Control   	49
#define 	LINFLEX_2_Control   	50
#define 	LINFLEX_3_Control   	51

#define 	GFX2D_Control   		52

#define 	GXG_Control   			53

#define 	DCULITE_Control   		54

#define 	DCU_3_Control   		55

#define 	GAUGEDRIVER_Control   	56

#define 	CANSAMPLER_Control   	60

#define 	SGM_Control   			62

#define 	TCON_Control   			63

#define 	CFLASH_0_Control   		66

#define 	SIUL_Control   			68

#define 	eMIOS_0_Control   		72

#define 	eMIOS_1_Control   		73

#define 	RTC_API_Control   		91
#define 	PIT_RTI_Control   		92

#define 	CMU_0_Control   		104




#endif