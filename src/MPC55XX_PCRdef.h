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
 #ifndef MPC55XX_PCRDEF__H
 #define MPC55XX_PCRDEF__H
// For MPC5645S cut 1
//Defines port pins vs the relevant SIU port number
#define PA0  0
#define PA1  1
#define PA2  2
#define PA3  3
#define PA4  4
#define PA5  5
#define PA6  6
#define PA7  7
#define PA8  8
#define PA9  9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15

#define PB0  16
#define PB1  17
#define PB2  18
#define PB3  19
#define PB4  20
#define PB5  21
#define PB6  22
#define PB7  23
#define PB8  24
#define PB9  25
#define PB10 26
#define PB11 27
#define PB12 28
#define PB13 29

#define PC0  30
#define PC1  31
#define PC2  32
#define PC3  33
#define PC4  34
#define PC5  35
#define PC6  36
#define PC7  37
#define PC8  38
#define PC9  39
#define PC10 40
#define PC11 41
#define PC12 42
#define PC13 43
#define PC14 44
#define PC15 45

#define PD0  46
#define PD1  47
#define PD2  48
#define PD3  49
#define PD4  50
#define PD5  51
#define PD6  52
#define PD7  53
#define PD8  54
#define PD9  55
#define PD10 56
#define PD11 57
#define PD12 58
#define PD13 59
#define PD14 60
#define PD15 61

#define PE0  62
#define PE1  63
#define PE2  64
#define PE3  65
#define PE4  66
#define PE5  67
#define PE6  68
#define PE7  69

#define PF0  70
#define PF1  71
#define PF2  72
#define PF3  73
#define PF4  74
#define PF5  75
#define PF6  76
#define PF7  77
#define PF8  78
#define PF9  79
#define PF10 80
#define PF11 81
#define PF12 82
#define PF13 83
#define PF14 84
#define PF15 85

#define PG0  86
#define PG1  87
#define PG2  88
#define PG3  89
#define PG4  90
#define PG5  91
#define PG6  92
#define PG7  93
#define PG8  94
#define PG9  95
#define PG10 96
#define PG11 97
#define PG12 98

#define PH0  99
#define PH1  100
#define PH2  101
#define PH3  102
#define PH4  103
#define PH5  104

#define PJ0  105
#define PJ1  106
#define PJ2  107
#define PJ3  108
#define PJ4  109
#define PJ5  110
#define PJ6  111
#define PJ7  112
#define PJ8  113
#define PJ9  114
#define PJ10 115
#define PJ11 116
#define PJ12 117
#define PJ13 118
#define PJ14 119
#define PJ15 120

#define PK0  121
#define PK1  122
#define PK2  123
#define PK3  124
#define PK4  125
#define PK5  126
#define PK6  127
#define PK7  128
#define PK8  129
#define PK9  130
#define PK10 131
#define PK11 132

#define PL0  133
#define PL1  134
#define PL2  135
#define PL3  136
#define PL4  137
#define PL5  138
#define PL6  139
#define PL7  140
#define PL8  141
#define PL9  142
#define PL10 143
#define PL11 144
#define PL12 145
#define PL13 146

#define PM0  147
#define PM1  148
#define PM2  149
#define PM3  150
#define PM4  151
#define PM5  152
#define PM6  153
#define PM7  154
#define PM8  155
#define PM9  156
#define PM10 157
#define PM11 158
#define PM12 159
#define PM13 160

#define PN0  161
#define PN1  162
#define PN2  163
#define PN3  164
#define PN4  165
#define PN5  166
#define PN6  167
#define PN7  168
#define PN8  169
#define PN9  170
#define PN10 171
#define PN11 172
#define PN12 173
#define PN13 174
#define PN14 175
#define PN15 176

#define PP0  177
#define PP1  178
#define PP2  179
#define PP3  180
#define PP4  181
#define PP5  182
#define PP6  183
#define PP7  184


#endif