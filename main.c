/*
n�zev: Parkovi�t� s br�nou, zvukem a sv�tly
autor: Matou� P�len�k, 4B, VO� a SP�E Olomouc
popis: - piezo m�ni�
			 - 2 sn�ma�e
			 - LCD displej
			 - AD p�evodn�k
*/

#include "stm8s.h"
#include "delay.h"
#include "milis.h"
#include "stm8_hd44780.h"
#include "stdio.h"
#include "spse_stm8.h"

//definice port� a pin� pro LED
#define LED_PORT	GPIOG       // PG

#define LED_Y1_PIN  GPIO_PIN_1 // PG1 - �lut� 1 LED
#define LED_Y2_PIN  GPIO_PIN_2  // PG2 - �lut� 2 LED
#define LED_Y3_PIN  GPIO_PIN_3  // PG3 - �lut� 3 LED

#define LED_RED_PIN  GPIO_PIN_4 // PG4 - �erven� LED
#define LED_GREEN_PIN  GPIO_PIN_5  // PG5 - zelen� LED

// rozsv�cen� LED
#define LED_Y1_ON  GPIO_WriteLow(LED_PORT, LED_Y1_PIN)
#define LED_Y2_ON  GPIO_WriteLow(LED_PORT, LED_Y2_PIN)
#define LED_Y3_ON  GPIO_WriteLow(LED_PORT, LED_Y3_PIN)

#define LED_RED_ON  GPIO_WriteLow(LED_PORT, LED_RED_PIN)
#define LED_GREEN_ON  GPIO_WriteLow(LED_PORT, LED_GREEN_PIN)

// zhasnut� LED
#define LED_Y1_OFF GPIO_WriteHigh(LED_PORT, LED_Y1_PIN)
#define LED_Y2_OFF GPIO_WriteHigh(LED_PORT, LED_Y2_PIN)
#define LED_Y3_OFF GPIO_WriteHigh(LED_PORT, LED_Y3_PIN)

#define LED_RED_OFF GPIO_WriteHigh(LED_PORT, LED_RED_PIN)
#define LED_GREEN_OFF  GPIO_WriteHigh(LED_PORT, LED_GREEN_PIN)

// ovl�d�n� piezo-m�ni�e
#define ZVUK_PORT GPIOG
#define ZVUK_PIN  GPIO_PIN_0
#define ZVUK_DOWN   GPIO_WriteHigh(ZVUK_PORT, ZVUK_PIN);
#define ZVUK_UP  GPIO_WriteLow(ZVUK_PORT, ZVUK_PIN);
#define ZVUK_REVERSE GPIO_WriteReverse(ZVUK_PORT, ZVUK_PIN);


void setup(void){
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	//LCD displej
	GPIO_Init(LCD_RS_PORT, LCD_RS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LCD_RW_PORT, LCD_RW_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LCD_E_PORT, LCD_E_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LCD_D4_PORT, LCD_D4_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LCD_D5_PORT, LCD_D5_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LCD_D6_PORT, LCD_D6_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LCD_D7_PORT, LCD_D7_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	
	//LED diody
	GPIO_Init(LED_PORT, LED_Y1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LED_PORT, LED_Y2_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LED_PORT, LED_Y3_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LED_PORT, LED_GREEN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LED_PORT, LED_RED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	
	//piezo-m�ni�
	GPIO_Init(ZVUK_PORT, ZVUK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

	
	//AD p�evodn�k
	ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL0,DISABLE);
	ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
	ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
	ADC2_Select_Channel(ADC2_CHANNEL_0);
	ADC2_Select_Channel(ADC2_CHANNEL_1);
	ADC2_Cmd(ENABLE);
	ADC2_Startup_Wait();
}

//prom�nn�
char text[32];
uint16_t snimac_prvni = 0; //vjezd
uint16_t snimac_druhy = 0; //v�jezd
uint32_t time=0;
int16_t volna_mista=5; //po�et voln�ch m�st na parkovi�ti
int16_t pocet_mist=5;
int16_t obsazeno=0;

void delay_ms(uint16_t ms)
{
    uint16_t i;
    for (i = 0; i < ms; ++i) {
        _delay_us(250);
        _delay_us(250);
        _delay_us(250);
        _delay_us(248);
    }
}


//funkce pro �ek�n� 1 sekundu
void delay1s(void) {
	uint16_t i;
	for (i=0; i<5000; i++) {
		_delay_us(400);
	}
}

void main(void){
	setup();
	lcd_init();
	init_milis();
	
	//n�zev parkovi�t� na LCD dipsleji
	lcd_gotoxy(0, 0);
	lcd_puts("PARKOVISTE");
	//zelen� LED - voln� m�sta k dispozici
	LED_GREEN_ON;
	LED_Y1_ON;
	LED_Y2_ON;
	LED_Y3_ON;
	ZVUK_UP;
	
	while(1){
		if (milis()-time>300){
			time=milis();
			snimac_prvni=ADC_get(ADC2_CHANNEL_1); //zaznamen�n� pohybu na prvn�m sn�ma�i
			snimac_druhy=ADC_get(ADC2_CHANNEL_0); //zaznamen�n� pohybu na druh�m sn�ma�i
			
			if (snimac_prvni < 500){
				volna_mista--; //pohyb na vjezdu... -1 voln�ch m�st
			}
			
			if (snimac_druhy < 500){
				volna_mista++; //pohyb na v�jezdu... +1 voln�ch m�st
			}
			
			//LCD bude ukazovat max. 5 voln�ch m�st
			if (volna_mista<=pocet_mist+1){
				lcd_gotoxy(0, 1);
				lcd_puts("volna mista: 5");
				volna_mista==pocet_mist;
			}
			
			//LCD zobraz� "obsazeno", bude-li prom�nn� "volna_mista" rovna nebomen�� ne� 0
			if (volna_mista<=obsazeno){
				LED_GREEN_OFF;
				LED_RED_ON; //�erven� LED - pln� parkovi�t�, ��dn� voln� m�sta
				lcd_clear();
				lcd_gotoxy(0, 0);
				lcd_puts("PARKOVISTE");
				lcd_gotoxy(0, 1);
				lcd_puts("obsazeno");	
				volna_mista==obsazeno;
			}
			
			//LCD zobraz� aktu�ln� stav po�tu voln�ch m�st
			if (volna_mista>=1 && volna_mista<=pocet_mist){
				LED_GREEN_ON;
				LED_RED_OFF;
				lcd_gotoxy(0, 1);
				sprintf(text, "volna mista: %1u", volna_mista);
				lcd_puts(text);
				delay1s();
			}
			
		}
		
	}
}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
