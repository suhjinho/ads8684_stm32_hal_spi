/*=============================================================================
* 작성자 : SUHJINHO
*
* PHONE : 010-7676-8380
*

* 최초작성일 : 2020.12.10
* 최초변경일 : 2020.12.10

* File Nane : drv_ads8684.c
* Function  : ADS8684 A/D 컨버터 읽기

*

*
*
* HAL DRIVER 설치 필요
*
* Requires HAL DRIVER installation
*
*
* CPOL : LOW (0), CPHA : 2 EDGE (1)
=============================================================================*/

#include "drv_ads8684.h"


/* Private function prototypes -----------------------------------------------*/
uint8_t Init_ADS8684(void);														/* ADS8684 초기화 (A/D Convert 초기화) */
static void WriteCommandReg(uint32_t command);									/* ADS8684 명령 레지스터 쓰기 */
void AUTO_RST_Mode(void);														/* 자동 스캔 모드 들어가기 */
static void Write_Program_Register(uint32_t Addr, uint32_t data);				/* 프로그램 레지스터 쓰기 */
uint32_t Read_Program_Register(uint32_t Addr);									/* 프로그램 레지스터 읽기 */
void Set_Auto_Scan_Sequence(uint32_t seq);										/* 자동 스캔 시퀀스 채널 설정 */
void Set_CH_Range_Select(uint32_t ch, uint32_t range);							/* 스캔 채널 시퀀스의 A/D 변환 데이터 읽기 */
void Get_AUTO_RST_Mode_Data(uint16_t *pADCData, uint32_t chNum);				/* 스캔 채널 시퀀스의 A/D 변환 데이터 읽기 */
static uint32_t ReadWriteByte(uint32_t data);									/* SPI를 시뮬레이션하여 8비트 데이터 읽기 및 쓰기 */

void MX_SPI6_Init(void);														/* SPI6 초기화      */

uint16_t  Get_MAN_Mode_Data(uint16_t  MAN_Ch);



/* 자동 스캔 모드에서 범위를 변경 한 후 AUTO_RST_Mode()를 추가해야한다. */

/*=============================================================================
* Function Name : Init_ADS8684
* Version            : V1.0
* Description        : ADS8684 초기화 (A/D Convert 초기화)
* Input              : None
* Output             : 1 = 초기화 성공
*                      0 = 초기화 실패
=============================================================================*/

uint8_t Init_ADS8684(void)
{
	uint8_t ret;

    /* ads8684 RESET delete */

	ADS8684_CS_H;

	Set_CH_Range_Select(Channel_0_Input_Range, VREF_0_125);		/* 0 ~ 5.12V    0x05 0x06  */
	Set_CH_Range_Select(Channel_1_Input_Range, VREF_0_125);		/* 0 ~ 5.12V    0x06 0x06  */
	Set_CH_Range_Select(Channel_2_Input_Range, VREF_0_125);		/* 0 ~ 5.12V    0x07 0x06  */
	Set_CH_Range_Select(Channel_3_Input_Range, VREF_0_125);		/* 0 ~ 5.12V    0x08 0x06  */
  /*Set_CH_Range_Select(Channel_3_Input_Range, VREF_0_25);	*/	/* 0 ~ 10.24V 	0x08 0x05  */


	Set_Auto_Scan_Sequence(0x0F);						    	/* 0x0F == 허용 채널      0 ~ 3 */
													            /* 0x0F == Enable CH 0 ~ 3 */

	if(Read_Program_Register(AUTO_SEQ_EN) != 0x0F)   			/* AUTO_SEQ_EN == 0x01     */
	{
		ret = 0;
	}
	else
	{
		AUTO_RST_Mode();

		ret = 1;
	}

	return (ret);
}


uint8_t Init_ADS8684_2(void)
{

	uint8_t rx_add1[2] = {0};
	uint8_t rx_add2[2] = {0};
	uint8_t rx_add3[2] = {0};
	uint8_t rx_add4[2] = {0};


	uint8_t ads_1[2] = { 11, 6  };
	uint8_t ads_2[2] = { 13,6 };
	uint8_t ads_3[2] = { 15, 6 };
	uint8_t ads_4[2] = { 17, 6};

	uint8_t ads2[2] = { 3, 0x0F };

	uint8_t ads2_1[2] = { 5, 0xF0};

	uint8_t ads3[2] = { 160,160 };

	uint16_t dumy_2byte[2]={0,0};

	HAL_SPI_TransmitReceive(&hspi6, (uint8_t*)dumy_2byte,(uint8_t*)dumy_2byte,2, 1000);



    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8, RESET);

    HAL_SPI_Transmit(&hspi6,(uint8_t *)ads2, 2, 1000);

    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8, SET);



    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8, RESET);

    HAL_SPI_Transmit(&hspi6,(uint8_t *)ads2_1, 2, 1000);

    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8, SET);



    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, RESET);

    HAL_SPI_TransmitReceive(&hspi6, (uint8_t*) ads_1,(uint8_t *)rx_add1, 2, 1000);

    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, SET);




    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, RESET);

    HAL_SPI_TransmitReceive(&hspi6, (uint8_t*) ads_2,(uint8_t *)rx_add2, 2, 1000);

    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, SET);




    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, RESET);

    HAL_SPI_TransmitReceive(&hspi6, (uint8_t*) ads_3,(uint8_t *)rx_add3,2, 10000);

    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, SET);




    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, RESET);

    HAL_SPI_TransmitReceive(&hspi6, (uint8_t*) ads_4,(uint8_t *)rx_add4, 2, 10000);

    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, SET);






    	 /*
    	 HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8, RESET);


    	 uint32_t data = 0;
    	 uint16_t buffer1[2]={0};
    	 uint8_t ret;
    	 HAL_SPI_Transmit(&hspi6,0x02, 1, 1000);
    	 data=HAL_SPI_TransmitReceive(&hspi6, 0xff, buffer1,2, 1000);

    	 HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8, SET);


    	 */
    	/*    if(data != 0x0F)   /*	  AUTO_SEQ_EN=0x01
    	 {
    	 ret = 0;
    	 }
    	 else
    	 {

    	 HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8, RESET);
    	 HAL_SPI_Transmit(&hspi6,(uint8_t *)ads3, 2, 1000);
    	 HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8, SET);

    	 ret = 1;
    	 }


    	 */



    	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, RESET);


    	HAL_SPI_Transmit(&hspi6, (uint8_t*) ads3, 2, 10000);


    	while(HAL_SPI_GetState(&hspi6) != HAL_SPI_STATE_READY);

    	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, SET);
}

/*=============================================================================
* Function Name : WriteCommandReg
* Version            : V1.0
* Description       : ADS8684 명령 레지스터 쓰기
* Input                : command = command register
* Output             : None
=============================================================================*/
static void WriteCommandReg(uint32_t command)
{
	ADS8684_CS_L;

	uint8_t write_command1 = { ((command & 0xFF00) >> 8) };      /* wirte_command1 == write_command2 */
	uint8_t write_command2 = { (command & 0xFF) };
	uint8_t write_buffer[2]={write_command1,write_command2};

	HAL_SPI_Transmit(&hspi6,(uint8_t *) write_buffer, 2, 1000);


/*	HAL_SPI_Transmit(&hspi6,(uint8_t *) write_command1, 1, 1000);
	HAL_SPI_Transmit(&hspi6,(uint8_t *) write_command2, 1, 1000);*/


	ADS8684_CS_H;

}


/*=============================================================================
* Function Name : AUTO_RST_Mode
* Version            : V1.0
* Description       : 자동 스캔 모드 들어가기
* Input                : None
* Output             : None
=============================================================================*/
void AUTO_RST_Mode(void)
{
	WriteCommandReg(AUTO_RST);                                 /*	AUTO_RST  =  0xA0    */
}


/*=============================================================================
* Function Name : Write_Program_Register
* Version            : V1.0
* Description       : 프로그램 레지스터 쓰기
* Input                : Addr = Program Register address
*                          data = 프로그램 레지스터에 쓰여진 값
* Output             : None
=============================================================================*/
static void Write_Program_Register(uint32_t Addr, uint32_t data)    /* 05   06   */
																	/* 06   06   */
																	/* 07   06   */
																	/* 08   06   */

																	/* 0x01 0x0F */
{
	ADS8684_CS_L;

    uint8_t write_register[2] = {  ((Addr << 1) | WRITE) ,  data };    /* CH0  0x0B , 0x06     VREF_0_125 */
    															       /* CH1  0x0D , 0x06     VREF_0_125 */
    															       /* CH2  0x0F , 0x06     VREF_0_125 */
    															       /* CH3  0x11 , 0x06     VREF_0_125 */

																	   /* ((Addr << 1) | WRITE) = 0x03, data = 0x0F == Enable CH  0 ~ 3  */

    HAL_SPI_Transmit(&hspi6, (uint8_t *) write_register, 2, 1000);

	ADS8684_CS_H;
}


/*=============================================================================
* Function Name : Read_Program_Register
* Version            : V1.0
* Description       : 프로그램 레지스터 읽기
* Input                : Addr = Program Register address
* Output             : 프로그램 레지스터 값
=============================================================================*/
uint32_t Read_Program_Register(uint32_t Addr)
{
	uint32_t data = 0;
	uint8_t read_command2[2] = {0xFF,0xFF};
	uint8_t read_buffer1[2] = {0x00,0x00};
	uint8_t addr_read[1] = {((Addr) << 1 | READ)};

	ADS8684_CS_L;

	HAL_SPI_Transmit(&hspi6, (uint8_t *)addr_read, 1, 1000);       /* 2   */
	HAL_SPI_TransmitReceive(&hspi6, (uint8_t *)read_command2, (uint8_t *) read_buffer1, 2, 1000);

	data = read_buffer1[1];

	ADS8684_CS_H;


	return data;
}


/*=============================================================================
* Function Name : Set_Auto_Scan_Sequence
* Version            : V1.0
* Description       : 자동 스캔 시퀀스 채널 설정
* Input                : seq = 자동 스캔 시퀀스 활성화 레지스터의 값을 설정
* Output             : 프로그램 레지스터 값
=============================================================================*/
void Set_Auto_Scan_Sequence(uint32_t seq)
{
	Write_Program_Register(AUTO_SEQ_EN, seq); 		/*  AUTO_SEQ_EN =  0x01 , seq = 0x0F(0~3 채널 허용 시)   */
}


/*=============================================================================
* Function Name : Set_CH_Range_Select
* Version            : V1.0
* Description       : 각 채널의 범위를 설정
* Input                : ch = 채널 입력 범위 레지스터 주소
*                          range = 범위
* Output             : None
=============================================================================*/

void Set_CH_Range_Select(uint32_t ch, uint32_t range)
{
	Write_Program_Register(ch, range);       /*   0x05  0x06     */
											 /*   0x06  0x06     */
											 /*   0x07  0x06     */
											 /*   0x08  0x06     */
}


/*=============================================================================
* Function Name : Get_AUTO_RST_Mode_Data
* Version            : V1.0
* Description       : 스캔 채널 시퀀스의 A/D 변환 데이터 읽기
* Input                : pADCData = gUADCData.ChADCData에 전달
*                          chNum = 총 채널 수
* Output             : None
=============================================================================*/
void Get_AUTO_RST_Mode_Data(uint16_t *pADCData, uint32_t chNum)
{
	uint32_t i;
	uint32_t datal = 0, datah = 0;
	uint32_t data = 0;

	uint8_t read_command1[2] = {0x00,0x00};
	uint8_t read_command2[2] = {0xFF,0xFF};

	uint8_t read_buffer1[2]  = {0x00,0x00};

	for(i = 0; i < chNum; i++)
	{
		ADS8684_CS_L;

		HAL_SPI_Transmit(&hspi6,  (uint8_t *)read_command1 , 2, 1000);
		HAL_SPI_TransmitReceive(&hspi6, (uint8_t *)read_command2, (uint8_t *) read_buffer1, 2, 1000);

		datah = read_buffer1[0];
		datal = read_buffer1[1];

		ADS8684_CS_H;

		data =(datah << 8) | datal;


		if ( data >10000)
		{
		*(pADCData + i) = data;
		}

	}
}



uint16_t  Get_MAN_Mode_Data(uint16_t  MAN_Ch)
{
	uint32_t datal = 0, datah = 0;
	uint16_t ival;
	uint8_t read_buffer1[2]  = {0x00,0x00};
	uint8_t read_command2[2] = {0xFF,0xFF};

	ADS8684_CS_L;

	/* uint8_t MAN_CH_buffer[2] = {(MAN_Ch>>8),MAN_Ch};     */   /*  (MAN_Ch>>8)==MAN_Ch */

	HAL_SPI_Transmit(&hspi6, MAN_Ch , 2, 1000);

	datah = HAL_SPI_TransmitReceive(&hspi6, (uint8_t *)read_command2, (uint8_t *) read_buffer1, 2, 1000);
	datal = HAL_SPI_TransmitReceive(&hspi6, (uint8_t *)read_command2, (uint8_t *) read_buffer1, 2, 1000);

	ADS8684_CS_H;

	ival = (uint16_t)((datah << 8) | datal);

	return (ival);

}









uint32_t ADS8684_pressure_selectCH(uint32_t ch_Num , uint16_t ch)
{
	/* ch_Num == total ch  ,   ch == channel to select*/

	uint16_t x=0;
	uint16_t division = 256;
	uint32_t ch_sum=0;
	uint32_t ch_average=0;
	uint16_t ADCData[4]={0};


	switch(ch)
	{
		case 0 :

			while(x != division)
			{


				Get_AUTO_RST_Mode_Data((uint16_t *)ADCData, ch_Num);

				if(ADCData[0] != 0 && ADCData[0] != 65535)
				{
					ch_sum += ADCData[0];
					x++;

					if(x==division)
					{
						ch_average = ch_sum / x;

					}

				}

			}
			break;


		case 1 :

			while(x != 256)
			{


				Get_AUTO_RST_Mode_Data((uint16_t *)ADCData, ch_Num);

				if(ADCData[1] != 0 && ADCData[1] != 65535)
				{

					ch_sum += ADCData[1];
					x++;

					if(x==256)
					{
						ch_average = ch_sum / x;

					}

				}


			}
			break;


		case 2 :

			while(x != division)
			{


				Get_AUTO_RST_Mode_Data((uint16_t *)ADCData, ch_Num);

				if(ADCData[2] != 0 && ADCData[2] != 65535)
				{
					ch_sum += ADCData[2];
					x++;

					if(x==division)
					{
						ch_average = ch_sum / x;

					}

				}
			}
			break;


		case 3 :

			while(x != division)
			{


				Get_AUTO_RST_Mode_Data((uint16_t *)ADCData, ch_Num);

				if(ADCData[3] != 0 && ADCData[3] != 65535)
				{
					ch_sum += ADCData[3];
					x++;

					if(x==division)
					{
						ch_average = ch_sum / x;

					}

				}


			}
			break;

	}

	return ch_average;

}



uint32_t ADS8684_ch0_presuure(void)
{

	uint32_t ch0_average = 0;
	uint32_t ch0_value;
	uint32_t pressure_bar_ch0;

	ch0_average=ADS8684_pressure_selectCH(4, 0);
	if (ch0_average > 13100)
	{
		ch0_value=ch0_average;
		pressure_bar_ch0 = (ch0_value - 13100)  / 874 ;

	}

	return pressure_bar_ch0;
}


uint32_t ADS8684_ch1_presuure(void)
{

	uint32_t ch1_average1 = 0;
	uint32_t ch1_value;
	uint32_t pressure_bar_ch1;

	ch1_average1=ADS8684_pressure_selectCH(4, 1);
	if (ch1_average1 > 13100)
	{
		ch1_value=ch1_average1;
		pressure_bar_ch1 = (ch1_value - 13100)  / 874 ;

	}

	return pressure_bar_ch1;
}



uint32_t ADS8684_ch2_presuure(void)
{
	uint32_t ch2_average = 0;
	uint32_t ch2_value;
	uint32_t pressure_bar_ch2;

	ch2_average=ADS8684_pressure_selectCH(4, 2);
	if (ch2_average > 13100)
	{
		ch2_value=ch2_average;
		pressure_bar_ch2 = (ch2_value - 13100)  / 874 ;

	}

	return pressure_bar_ch2;
}


uint32_t ADS8684_ch3_presuure(void)
{
	uint32_t ch3_average = 0;
	uint32_t ch3_value;
	uint32_t pressure_bar_ch3;

	ch3_average=ADS8684_pressure_selectCH(4, 3);
	if (ch3_average > 13100)
	{
		ch3_value=ch3_average;
		pressure_bar_ch3 = (ch3_value - 13100)  / 874 ;

	}

	return pressure_bar_ch3;
}


/*=============================================================================
* Function Name : ReadWriteByte
* Version            : V1.0
* Description       : SPI를 시뮬레이션하여 8비트 데이터 읽기 및 쓰기,
*                          상승 에지에서 데이터 쓰기, 하강 에지에서 데이터 읽기
* Input                : data = Write data
* Output             : 8비트 데이터 읽기
=============================================================================*/
static uint32_t ReadWriteByte(uint32_t data)
{
	int i;
	uint32_t status = 0;
	uint32_t result = 0;

	for(i = 0; i < 8; i++)
	{
		if (data & (1 << (7 - i)))
		{
			ADS8684_SDI_H;
		}
		else
		{
			ADS8684_SDI_L;
		}

		ADS8684_CLK_H;
		ADS8684_CLK_H;
		ADS8684_CLK_H;
		status = ADS8684_SD0_STATUS();
		ADS8684_CLK_L;

		if (status)
		{
			result |= 1;
		}

		result <<= 1;
	}

  return (result >> 1);
}


void MX_SPI6_Init(void) {

	hspi6.Instance = SPI6;
	hspi6.Init.Mode = SPI_MODE_MASTER;
	hspi6.Init.Direction = SPI_DIRECTION_2LINES;
	hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi6.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi6.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi6.Init.NSS = SPI_NSS_SOFT;
	hspi6.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi6.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi6) != HAL_OK)
	{
		Error_Handler();
	}
}


/*
	uint32_t ch1_average1 = 0;
	uint32_t ch1_value;

	ch1_average1=ADS_8684_get_average(4);
	if (ch1_average1 != 0)
	{
		ch1_value=ch1_average1;
	}
*/


/************************ (C) COPYRIGHT NSTek *****END OF FILE****/
