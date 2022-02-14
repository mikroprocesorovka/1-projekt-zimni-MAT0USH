/*
název: Parkovištì s bránou, zvukem a svìtly
autor: Matouš Páleník, 4B, VOŠ a SPŠE Olomouc
datum: 15.2.2021

Popis:
Poèítadlo prùjezdù, které je schopné rozeznat, z jakého smìru auto pøijíždí je realizované pomocí dvou IR senzorù umístìných vedle sebe.
Podle smìru prùjezdu se bude k promìnné volna_mista buï pøièítat, anebo odèítat 1 místo.
Pøi každém prùjezdu vydá piezomìniè na 200 ms zvukový signál. souèasnì 3 LED diody vizualizují smìr prùjezdu.
Aktuální stav volných míst zobrazuje alfanumerický LCD displej. 
Pøi naplnìní kapacity parkovištì se na 1 sekundu aktivuje piezomìniè pro zaznìní zvukového signálu znaèící plné parkovištì.
Pro zamezení pøíjezdu dalších aut se aktivuje micro servo, které zavøe bránu. 
Èervená LED signalizuje naplnìní parkovištì.
Zelená LED signalizuje, že parkovištì obsahuje volná místa.

základní èásti:
			- piezomìniè
			- 2 IR senzory
			- LCD displej
			- micro servo
			- AD pøevodník
			- zelená LED
			- èervená LED
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

//definice portù a pinù pro LED
#define LED_PORT	GPIOG       // pro všechny LED porty PG

#define LED_Y1_PIN  GPIO_PIN_1 // PG1 - žlutá 1 LED
#define LED_Y2_PIN  GPIO_PIN_2  // PG2 - žlutá 2 LED
#define LED_Y3_PIN  GPIO_PIN_3  // PG3 - žlutá 3 LED

#define LED_RED_PIN  GPIO_PIN_4 // PG4 - èervená LED
#define LED_GREEN_PIN  GPIO_PIN_5 // PG5 - zelená LED

// rozsvícení LED
#define LED_Y1_ON  GPIO_WriteLow(LED_PORT, LED_Y1_PIN)
#define LED_Y2_ON  GPIO_WriteLow(LED_PORT, LED_Y2_PIN)
#define LED_Y3_ON  GPIO_WriteLow(LED_PORT, LED_Y3_PIN)

#define LED_RED_ON  GPIO_WriteLow(LED_PORT, LED_RED_PIN)
#define LED_GREEN_ON  GPIO_WriteLow(LED_PORT, LED_GREEN_PIN)

// zhasnutí LED
#define LED_Y1_OFF GPIO_WriteHigh(LED_PORT, LED_Y1_PIN)
#define LED_Y2_OFF GPIO_WriteHigh(LED_PORT, LED_Y2_PIN)
#define LED_Y3_OFF GPIO_WriteHigh(LED_PORT, LED_Y3_PIN)

#define LED_RED_OFF GPIO_WriteHigh(LED_PORT, LED_RED_PIN)
#define LED_GREEN_OFF  GPIO_WriteHigh(LED_PORT, LED_GREEN_PIN)

// ovládání piezomìnièe
#define ZVUK_PORT GPIOG //port PG
#define ZVUK_PIN  GPIO_PIN_0 //PG0
#define ZVUK_DOWN   GPIO_WriteHigh(ZVUK_PORT, ZVUK_PIN); //zapnutí zvuku
#define ZVUK_UP  GPIO_WriteLow(ZVUK_PORT, ZVUK_PIN); //vypnutí zvuku
#define ZVUK_REVERSE GPIO_WriteReverse(ZVUK_PORT, ZVUK_PIN); //zmìna na opaèný stav (v naší realizaci nevyužíváme)

// nastavení TIM_2 pro ovládání microserva
void tim2_setup(void){
     TIM2_TimeBaseInit(TIM2_PRESCALER_8, 40000); 
    //TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_OC1Init(                // inicializujeme kanál 1 (TM2_CH1)
        TIM2_OCMODE_PWM1,        // režim PWM1
        TIM2_OUTPUTSTATE_ENABLE, // Výstup povolen (TIMer ovládá pin)
        2000,                    // výchozí hodnota šíøky pulzu (støídy) 1056/1600 = 66%
        TIM2_OCPOLARITY_HIGH      // Polarita LOW protože LED rozsvìcím 0 (spol. anoda)
     );

    // ošetøení nežádoucích jevù pøi zmìnì PWM
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE);
}


// veškeré inicializace
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
	
	//piezo-mìniè
	GPIO_Init(ZVUK_PORT, ZVUK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

	//AD pøevodník
	ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL0,DISABLE);
	ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
	ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
	ADC2_Select_Channel(ADC2_CHANNEL_0);
	ADC2_Select_Channel(ADC2_CHANNEL_1);
	ADC2_Cmd(ENABLE);
	ADC2_Startup_Wait();
	tim2_setup();
}

//promìnné
char text[32];
uint16_t snimac_prvni = 0; //vjezd
uint16_t snimac_druhy = 0; //výjezd
uint32_t time=0;
int16_t volna_mista=5; //poèet volných míst na parkovišti
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


//funkce pro èekání 1 sekundu
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
	
	//název parkovištì na LCD displeji
	lcd_gotoxy(0, 0);
	lcd_puts("PARKOVISTE");
	LED_GREEN_ON; 	//zelená LED - volná místa k dispozici
	LED_Y1_OFF;	//smìrové LED vypnuty
	LED_Y2_OFF;
	LED_Y3_OFF;
	ZVUK_DOWN; //zvuk vypnutý
	TIM2_SetCompare1(2500); //brána (microservo) otevøená

	
	while(1){
		if (milis()-time>300){
			time=milis();
			snimac_prvni=ADC_get(ADC2_CHANNEL_1); //zaznamenání pohybu na prvním snímaèi
			snimac_druhy=ADC_get(ADC2_CHANNEL_0); //zaznamenání pohybu na druhém snímaèi
			
			if (snimac_prvni < 500){
				prvni=1; //pomocná promìnná pro zapamatování, že auto se objevilo pøed prvním snímaèem
				
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
					
					prvni=0; //reset pomocných promìnných
					druhy=0;
				
					if (volna_mista>obsazeno){
						TIM2_SetCompare1(2500);
					}
				}
			}
			
			if (snimac_druhy < 500){
				druhy=1; //pomocná promìnná pro zapamatování, že auto se objevilo pøed druhým snímaèem
				
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
				
					prvni=0; //reset pomocných promìnných
					druhy=0;
				
					if (volna_mista<=obsazeno){
						ZVUK_UP; //zvukový signál znaèící plné parkovištì
						delay_ms(1000);
						ZVUK_DOWN;
						TIM2_SetCompare1(4500);
					}
			  }
			}
			
			//zajištìní, že LCD bude ukazovat max. 5 volných míst
			if (volna_mista>=pocet_mist){
				volna_mista=pocet_mist;
				lcd_clear();
				lcd_gotoxy(0, 0);
				lcd_puts("PARKOVISTE");
				lcd_gotoxy(0, 1);
				sprintf(text, "volna mista: %1u", volna_mista);
				lcd_puts(text);
			}
			
			//LCD zobrazí "obsazeno", bude-li vyèerpán poèet volných míst
			if (volna_mista<=obsazeno){
				LED_GREEN_OFF;
				LED_RED_ON; //èervená LED - plné parkovištì, žádná volná místa
				lcd_clear();
				lcd_gotoxy(0, 0);
				lcd_puts("PARKOVISTE");
				lcd_gotoxy(0, 1);
				lcd_puts("obsazeno");	
				volna_mista=obsazeno;
			}
			
			//LCD zobrazí aktuální stav poètu volných míst
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
