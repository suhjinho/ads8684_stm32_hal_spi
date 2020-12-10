
#ifndef __DRV_ADS8684_H__
#define __DRV_ADS8684_H__


#include "stm32f4xx_hal.h"


extern uint8_t Init_ADS8684(void);										  /* ADS8684 초기화 (A/D Convert 초기화) */
extern void Get_AUTO_RST_Mode_Data(uint16_t *pADCData, uint32_t chNum);	  /* 스캔 채널 시퀀스의 A/D 변환 데이터 읽기 */
extern void MX_SPI6_Init(void);
SPI_HandleTypeDef hspi6;

/************************** ADS8684 레지스터 ***********************************/
/* Command Register */
#define NO_OP						0x0000
#define STDBY						0x8200
#define PWR_DN						0x8300
#define RST							0x8500
#define AUTO_RST					0xA000
#define MAN_Ch_0					0xC000
#define MAN_CH_1					0xC400
#define MAN_Ch_2					0xC800
#define MAN_Ch_3					0xCC00
#define MAN_Ch_4					0xD000
#define MAN_Ch_5					0xD400
#define MAN_Ch_6					0xD800
#define MAN_Ch_7					0xDC00
#define MAN_AUX						0xE000


#define	ADS8684_CS_L					HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, RESET);       /* CS(NSS) PIN LOW  */

#define	ADS8684_CS_H					HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, SET);		   /* CS(NSS) PIN HIGH */


#define	ADS8684_SDI_L					HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, RESET);      /* 사용 시 해당 핀 GPIO 설정 필요				     */
#define	ADS8684_SDI_H					HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, SET);        /* Requires corresponding pin GPIO settings when used */


/* #define	ADS8684_SDO_L				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, RESET); */   /* 사용 시 해당 핀 GPIO 설정 필요				    */
/* #define	ADS8684_SDO_H				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, SET);   */   /* Requires corresponding pin GPIO settings when used */

/* #define	ADS8684_CLK_L				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, RESET); */   /* 사용 시 해당 핀 GPIO 설정 필요  					*/
/* #define	ADS8684_CLK_H				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, SET);   */   /* Requires corresponding pin GPIO settings when used */


#define	ADS8684_CLK_L					__HAL_SPI_DISABLE(&hspi6);                         /* CLK PIN LOW  */
#define	ADS8684_CLK_H					__HAL_SPI_ENABLE(&hspi6);					       /* CLK PIN HIGH */



/*#define ADS8684_SD0_STATUS()	    	HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_12);   */     	   /* 사용 시 해당 핀 GPIO 설정 필요	*/



/* Program Register */
#define AUTO_SEQ_EN					0x01
#define Channel_Power_Down 			0x02
#define Feature_Select				0x03

#define Channel_0_Input_Range		0x05
#define Channel_1_Input_Range		0x06
#define Channel_2_Input_Range		0x07
#define Channel_3_Input_Range		0x08
#define Channel_4_Input_Range		0x09
#define Channel_5_Input_Range		0x0A
#define Channel_6_Input_Range		0x0B
#define Channel_7_Input_Range		0x0C

#define Ch_0_Hysteresis				0x15
#define Ch_0_High_Threshold_MSB		0x16
#define Ch_0_High_Threshold_LSB		0x17
#define Ch_0_Low_Threshold_MSB		0x18
#define Ch_0_Low_Threshold_LSB		0x19

#define Ch_7_Hysteresis				0x38
#define Ch_7_High_Threshold_MSB		0x39
#define Ch_7_High_Threshold_LSB		0x3A
#define Ch_7_Low_Threshold_MSB		0x3B
#define Ch_7_Low_Threshold_LSB		0x3C

#define Command_Read_Back			0x3F	

#define WRITE 	1
#define READ 	0

#define CH7_EN						0x80
#define CH6_EN						0x40
#define CH5_EN						0x20
#define CH4_EN						0x10
#define CH3_EN						0x08
#define CH2_EN						0x04
#define CH1_EN						0x02
#define CH0_EN						0x01

#define CH7_PD						0x80
#define CH6_PD						0x40
#define CH5_PD						0x20
#define CH4_PD						0x10
#define CH3_PD						0x08
#define CH2_PD						0x04
#define CH1_PD						0x02
#define CH0_PD						0x01

#define VREF_25_25					0x00		/* ±2.5 × VREF */
#define VREF_125_125				0x01		/* ±1.25 × VREF */
#define VREF_0625_0625				0x02		/* ±0.625 × VREF */
#define VREF_0_25					0x05		/* 0 to 2.5 × VREF */
#define VREF_0_125					0x06		/* 0 to 1.25 × VREF */




#endif
