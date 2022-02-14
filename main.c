/*
n�zev: Parkovi�t� s br�nou, zvukem a sv�tly
autor: Matou� P�len�k, 4B, VO� a SP�E Olomouc
datum: 15.2.2021

Popis:
Po��tadlo pr�jezd�, kter� je schopn� rozeznat, z jak�ho sm�ru auto p�ij�d� je realizovan� pomoc� dvou IR senzor� um�st�n�ch vedle sebe.
Podle sm�ru pr�jezdu se bude k prom�nn� volna_mista bu� p�i��tat, anebo od��tat 1 m�sto.
P�i ka�d�m pr�jezdu vyd� piezom�ni� na 200 ms zvukov� sign�l. sou�asn� 3 LED diody vizualizuj� sm�r pr�jezdu.
Aktu�ln� stav voln�ch m�st zobrazuje alfanumerick� LCD displej. 
P�i napln�n� kapacity parkovi�t� se na 1 sekundu aktivuje piezom�ni� pro zazn�n� zvukov�ho sign�lu zna��c� pln� parkovi�t�.
Pro zamezen� p��jezdu dal��ch aut se aktivuje micro servo, kter� zav�e br�nu. 
�erven� LED signalizuje napln�n� parkovi�t�.
Zelen� LED signalizuje, �e parkovi�t� obsahuje voln� m�sta.

z�kladn� ��sti:
			- piezom�ni�
			- 2 IR senzory
			- LCD displej
			- micro servo
			- AD p�evodn�k
			- zelen� LED
			- �erven� LED
			- 3x LED 
*/

#include "stm8s.h"
#include "delay.h"
#include "milis.h"
#include "stm8_hd44780.h"
#include "stdio.h"
#include "spse_stm8.h"

#define _ISOC99_SOURCE
#define _GNU_SOURCE

//definice port� a pin� pro LED
#define LED_PORT	GPIOG       // pro v�echny LED porty PG

#define LED_Y1_PIN  GPIO_PIN_1 // PG1 - �lut� 1 LED
#define LED_Y2_PIN  GPIO_PIN_2  // PG2 - �lut� 2 LED
#define LED_Y3_PIN  GPIO_PIN_3  // PG3 - �lut� 3 LED

#define LED_RED_PIN  GPIO_PIN_4 // PG4 - �erven� LED
#define LED_GREEN_PIN  GPIO_PIN_5 // PG5 - zelen� LED

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

// ovl�d�n� piezom�ni�e
#define ZVUK_PORT GPIOG //port PG
#define ZVUK_PIN  GPIO_PIN_0 //PG0
#define ZVUK_DOWN   GPIO_WriteHigh(ZVUK_PORT, ZVUK_PIN); //zapnut� zvuku
#define ZVUK_UP  GPIO_WriteLow(ZVUK_PORT, ZVUK_PIN); //vypnut� zvuku
#define ZVUK_REVERSE GPIO_WriteReverse(ZVUK_PORT, ZVUK_PIN); //zm�na na opa�n� stav (v na�� realizaci nevyu��v�me)

// nastaven� TIM_2 pro ovl�d�n� microserva
void tim2_setup(void){
     TIM2_TimeBaseInit(TIM2_PRESCALER_8, 40000); 
    //TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_OC1Init(                // inicializujeme kan�l 1 (TM2_CH1)
        TIM2_OCMODE_PWM1,        // re�im PWM1
        TIM2_OUTPUTSTATE_ENABLE, // V�stup povolen (TIMer ovl�d� pin)
        2000,                    // v�choz� hodnota ���ky pulzu (st��dy) 1056/1600 = 66%
        TIM2_OCPOLARITY_HIGH      // Polarita LOW proto�e LED rozsv�c�m 0 (spol. anoda)
     );

    // o�et�en� ne��douc�ch jev� p�i zm�n� PWM
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE);
}


// ve�ker� inicializace
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
	tim2_setup();
}

//prom�nn�
char text[32];
uint16_t snimac_prvni = 0; //vjezd
uint16_t snimac_druhy = 0; //v�jezd
uint32_t time=0;
int16_t volna_mista=5; //po�et voln�ch m�st na parkovi�ti
int16_t pocet_mist=5;
int16_t obsazeno=0;
uint16_t PWM = 2000;
uint16_t prvni = 0;
uint16_t druhy = 0;

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
	
	//n�zev parkovi�t� na LCD displeji
	lcd_gotoxy(0, 0);
	lcd_puts("PARKOVISTE");
	LED_GREEN_ON; 	//zelen� LED - voln� m�sta k dispozici
	LED_Y1_OFF;	//sm�rov� LED vypnuty
	LED_Y2_OFF;
	LED_Y3_OFF;
	ZVUK_DOWN; //zvuk vypnut�
	TIM2_SetCompare1(2500); //br�na (microservo) otev�en�

	
	while(1){
		if (milis()-time>300){
			time=milis();
			snimac_prvni=ADC_get(ADC2_CHANNEL_1); //zaznamen�n� pohybu na prvn�m sn�ma�i
			snimac_druhy=ADC_get(ADC2_CHANNEL_0); //zaznamen�n� pohybu na druh�m sn�ma�i
			
			if (snimac_prvni < 500){
				prvni=1; //pomocn� prom�nn� pro zapamatov�n�, �e auto se objevilo p�ed prvn�m sn�ma�em
				
				if (druhy==1){
					volna_mista++;
					LED_Y1_ON;
					delay_ms(90);
					LED_Y1_OFF;
					LED_Y2_ON;
					delay_ms(90);
					LED_Y2_OFF;
					LED_Y3_ON;
					delay_ms(90);
					LED_Y3_OFF;
					ZVUK_UP;
					delay_ms(200);
					ZVUK_DOWN;
					delay_ms(500);
					
					prvni=0; //reset pomocn�ch prom�nn�ch
					druhy=0;
				
					if (volna_mista>obsazeno){
						TIM2_SetCompare1(2500);
					}
				}
			}
			
			if (snimac_druhy < 500){
				druhy=1; //pomocn� prom�nn� pro zapamatov�n�, �e auto se objevilo p�ed druh�m sn�ma�em
				
				if (prvni==1) {
					volna_mista--; 
					LED_Y3_ON;
					delay_ms(90);
					LED_Y3_OFF;
					LED_Y2_ON;
					delay_ms(90);
					LED_Y2_OFF;
					LED_Y1_ON;
					delay_ms(90);
					LED_Y1_OFF;	
					ZVUK_UP;
					delay_ms(200);
					ZVUK_DOWN;
					delay_ms(500);
				
					prvni=0; //reset pomocn�ch prom�nn�ch
					druhy=0;
				
					if (volna_mista<=obsazeno){
						ZVUK_UP; //zvukov� sign�l zna��c� pln� parkovi�t�
						delay_ms(1000);
						ZVUK_DOWN;
						TIM2_SetCompare1(4500);
					}
			  }
			}
			
			//zaji�t�n�, �e LCD bude ukazovat max. 5 voln�ch m�st
			if (volna_mista>=pocet_mist){
				volna_mista=pocet_mist;
				lcd_clear();
				lcd_gotoxy(0, 0);
				lcd_puts("PARKOVISTE");
				lcd_gotoxy(0, 1);
				sprintf(text, "volna mista: %1u", volna_mista);
				lcd_puts(text);
			}
			
			//LCD zobraz� "obsazeno", bude-li vy�erp�n po�et voln�ch m�st
			if (volna_mista<=obsazeno){
				LED_GREEN_OFF;
				LED_RED_ON; //�erven� LED - pln� parkovi�t�, ��dn� voln� m�sta
				lcd_clear();
				lcd_gotoxy(0, 0);
				lcd_puts("PARKOVISTE");
				lcd_gotoxy(0, 1);
				lcd_puts("obsazeno");	
				volna_mista=obsazeno;
			}
			
			//LCD zobraz� aktu�ln� stav po�tu voln�ch m�st
			if (volna_mista>=1 && volna_mista<=pocet_mist){
				LED_GREEN_ON;
				LED_RED_OFF;
				lcd_clear();
				lcd_gotoxy(0, 0);
				lcd_puts("PARKOVISTE");
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
