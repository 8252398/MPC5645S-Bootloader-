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
 #ifndef INTERRUPTSIRQDEF__H
 #define INTERRUPTSIRQDEF__H
// For MPC5645S cut 1
//Defines port pins vs the relevant SIU port number
#define 	Soft_Setable_Flag_0   	0
#define 	Soft_Setable_Flag_1   	1
#define 	Soft_Setable_Flag_2   	2
#define 	Soft_Setable_Flag_3   	3
#define 	Soft_Setable_Flag_4   	4
#define 	Soft_Setable_Flag_5   	5
#define 	Soft_Setable_Flag_6   	6
#define 	Soft_Setable_Flag_7   	7

#define		eDMA_Combined_Error		10
#define		eDMA_Channel_0			11
#define		eDMA_Channel_1			12
#define		eDMA_Channel_2			13
#define		eDMA_Channel_3			14
#define		eDMA_Channel_4			15
#define		eDMA_Channel_5			16
#define		eDMA_Channel_6			17
#define		eDMA_Channel_7			18
#define		eDMA_Channel_8			19
#define		eDMA_Channel_9			20
#define		eDMA_Channel_10			21
#define		eDMA_Channel_11			22
#define		eDMA_Channel_12			23
#define		eDMA_Channel_13			24
#define		eDMA_Channel_14			25
#define		eDMA_Channel_15			26

#define		Watchdot_Timeout		28

#define		STM_MatchOnChannel_0	30
#define		STM_MatchOnChannel_1	31
#define		STM_MatchOnChannel_2	32
#define		STM_MatchOnChannel_3	33

#define		ECSM_ECC_DBDPlatfFlash_RAM	35

#define		ECSM_ECC_SBCPlatfFlash_RAM	36

#define		RTCIRQ					38
#define		APIIRQ					39

#define		SIU_Ext_IRQ_0			41
#define		SIU_Ext_IRQ_1			42
#define		SIU_Ext_IRQ_2			43

#define		WakeUp_IRQ_0			46
#define		WakeUp_IRQ_1			47
#define		WakeUp_IRQ_2			48
#define		WakeUp_IRQ_3			49

#define		Safe_Mode_IRQ			51
#define		Mode_Trans_IRQ			52
#define		Invalid_Mode_IRQ		53
#define		Invalid_Mode__CFG_IRQ	54

#define		IPI_Init_IRQ			56
#define		IPI_Init_OSC_IRQ		57

#define		PIT_TimeChannel_0		59
#define		PIT_TimeChannel_1		60
#define		PIT_TimeChannel_2		61

#define     ADC_EOC_IRQ				62
#define     ADC_ER_IRQ				63
#define     ADC_WD_IRQ				64

#define     FLEXCAN_0_ESR_IRQ		65
#define     FLEXCAN_0_ESR_BOFF_IRQ	66

#define     FLEXCAN_0_BUF_00_03_IRQ	68
#define     FLEXCAN_0_BUF_04_07_IRQ	69
#define     FLEXCAN_0_BUF_08_11_IRQ	70
#define     FLEXCAN_0_BUF_12_15_IRQ	71
#define     FLEXCAN_0_BUF_16_31_IRQ	72
#define     FLEXCAN_0_BUF_32_63_IRQ	73

#define     DSPI_0_SR_TFUF_RFOF_IRQ	74
#define     DSPI_0_SR_EOQF_IRQ		75
#define     DSPI_0_SR_TFFF_IRQ		76
#define 	DSPI_0_SR_TCF_IRQ		77
#define 	DSPI_0_SR_RFDF_IRQ		78

#define		LINFlex_0_RX_IRQ		79
#define		LINFlex_0_TX_IRQ		80
#define		LINFlex_0_ERR_IRQ		81

#define     FLEXCAN_1_ESR_IRQ		85
#define     FLEXCAN_1_ESR_BOFF_IRQ	86

#define     FLEXCAN_1_BUF_00_03_IRQ	88
#define     FLEXCAN_1_BUF_04_07_IRQ	89
#define     FLEXCAN_1_BUF_08_11_IRQ	90
#define     FLEXCAN_1_BUF_12_15_IRQ	91
#define     FLEXCAN_1_BUF_16_31_IRQ	92
#define     FLEXCAN_1_BUF_32_63_IRQ	93

#define     DSPI_1_SR_TFUF_RFOF_IRQ	94
#define     DSPI_1_SR_EOQF_IRQ		95
#define     DSPI_1_SR_TFFF_IRQ		96
#define 	DSPI_1_SR_TCF_IRQ		97
#define 	DSPI_1_SR_RFDF_IRQ		98

#define		LINFlex_1_RX_IRQ		99
#define		LINFlex_1_TX_IRQ		100
#define		LINFlex_1_ERR_IRQ		101

#define     FLEXCAN_2_ESR_IRQ		105
#define     FLEXCAN_2_ESR_BOFF_IRQ	106

#define     FLEXCAN_2_BUF_00_03_IRQ	108
#define     FLEXCAN_2_BUF_04_07_IRQ	109
#define     FLEXCAN_2_BUF_08_11_IRQ	110
#define     FLEXCAN_2_BUF_12_15_IRQ	111
#define     FLEXCAN_2_BUF_16_31_IRQ	112
#define     FLEXCAN_2_BUF_32_63_IRQ	113

#define     DSPI_2_SR_TFUF_RFOF_IRQ	114
#define     DSPI_2_SR_EOQF_IRQ		115
#define     DSPI_2_SR_TFFF_IRQ		116
#define 	DSPI_2_SR_TCF_IRQ		117
#define 	DSPI_2_SR_RFDF_IRQ		118

#define		LINFlex_2_RX_IRQ		119
#define		LINFlex_2_TX_IRQ		120
#define		LINFlex_2_ERR_IRQ		121

#define		LINFlex_3_RX_IRQ		122
#define		LINFlex_3_TX_IRQ		123
#define		LINFlex_3_ERR_IRQ		124

#define		I2C_0_IBIF_IRQ			125
#define		I2C_1_IBIF_IRQ			126

#define		PIT_TimeChannel_3		127
#define		PIT_TimeChannel_4		128
#define		PIT_TimeChannel_5		129
#define		PIT_TimeChannel_6		130
#define		PIT_TimeChannel_7		131

#define		EMIOS_0_GFR_F8_IRQ		141
#define		EMIOS_0_GFR_F9_IRQ		142
#define		EMIOS_0_GFR_F10_IRQ		143
#define		EMIOS_0_GFR_F11_IRQ		144
#define		EMIOS_0_GFR_F12_IRQ		145
#define		EMIOS_0_GFR_F13_IRQ		146
#define		EMIOS_0_GFR_F14_IRQ		147
#define		EMIOS_0_GFR_F15_IRQ		148
#define		EMIOS_0_GFR_F16_IRQ		149
#define		EMIOS_0_GFR_F17_IRQ		150
#define		EMIOS_0_GFR_F18_IRQ		151
#define		EMIOS_0_GFR_F19_IRQ		152
#define		EMIOS_0_GFR_F20_IRQ		153
#define		EMIOS_0_GFR_F21_IRQ		154
#define		EMIOS_0_GFR_F22_IRQ		155
#define		EMIOS_0_GFR_F23_IRQ		156

#define		EMIOS_1_GFR_F8_IRQ		157
#define		EMIOS_1_GFR_F9_IRQ		158
#define		EMIOS_1_GFR_F10_IRQ		159
#define		EMIOS_1_GFR_F11_IRQ		160
#define		EMIOS_1_GFR_F12_IRQ		161
#define		EMIOS_1_GFR_F13_IRQ		162
#define		EMIOS_1_GFR_F14_IRQ		163
#define		EMIOS_1_GFR_F15_IRQ		164
#define		EMIOS_1_GFR_F16_IRQ		165
#define		EMIOS_1_GFR_F17_IRQ		166
#define		EMIOS_1_GFR_F18_IRQ		167
#define		EMIOS_1_GFR_F19_IRQ		168
#define		EMIOS_1_GFR_F20_IRQ		169
#define		EMIOS_1_GFR_F21_IRQ		170
#define		EMIOS_1_GFR_F22_IRQ		171
#define		EMIOS_1_GFR_F23_IRQ		172

#define		I2C_2_IBIF_IRQ			173
#define		I2C_3_IBIF_IRQ			174

#define		VIU_2_IRQ				175

#define		DRAM_FIFO_OV_UV_IRQ		180
#define		DRAM_INT_IRQ			181

#define		SGM_IRG_IRQ				183
#define		DCU_3_BLANK_LS_VSYNC_IRQ		184
#define		DCU_3_UNDRUN_IRQ		185
#define		DCU_3_PARERR_IRQ		186
#define		DCU_3_PDI_IRQ			187

#define		DCU_Lite_BLANK_LS_VSYNC_IRQ		188
#define		DCU_Lite_UNDRUN_IRQ		189
#define		DCU_Lite_PARERR_IRQ		190
#define		DCU_Lite_PDI_IRQ		191

#define		SMC_0_MCTOI_IRQ			192

#define		SSD_0_MCZI_AOVI_IRQ		193
#define		SSD_1_MCZI_AOVI_IRQ		194
#define		SSD_2_MCZI_AOVI_IRQ		195
#define		SSD_3_MCZI_AOVI_IRQ		196
#define		SSD_4_MCZI_AOVI_IRQ		197
#define		SSD_5_MCZI_AOVI_IRQ		198

#define		RLE_INT_IRQ				207

#define		DuadSPI_OVERRUN_IRQ		228
#define		DuadSPI_TFFF_IRQ		230
#define		DuadSPI_TCF_IRQ			231
#define		DuadSPI_RFDF_IRQ		232
#define		DuadSPI_CERR_IRQ		233

#define		GFX2D_IRRQ_0_IRQ		238


#endif