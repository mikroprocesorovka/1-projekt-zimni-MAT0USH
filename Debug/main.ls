   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.12.6 - 16 Dec 2021
   3                     ; Generator (Limited) V4.5.4 - 16 Dec 2021
  52                     ; 51 void setup(void){
  54                     	switch	.text
  55  0000               _setup:
  59                     ; 52 	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  61  0000 4f            	clr	a
  62  0001 cd0000        	call	_CLK_HSIPrescalerConfig
  64                     ; 54 	GPIO_Init(LCD_RS_PORT, LCD_RS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  66  0004 4bc0          	push	#192
  67  0006 4b40          	push	#64
  68  0008 ae500a        	ldw	x,#20490
  69  000b cd0000        	call	_GPIO_Init
  71  000e 85            	popw	x
  72                     ; 55 	GPIO_Init(LCD_RW_PORT, LCD_RW_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  74  000f 4bc0          	push	#192
  75  0011 4b80          	push	#128
  76  0013 ae500a        	ldw	x,#20490
  77  0016 cd0000        	call	_GPIO_Init
  79  0019 85            	popw	x
  80                     ; 56 	GPIO_Init(LCD_E_PORT, LCD_E_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  82  001a 4bc0          	push	#192
  83  001c 4b20          	push	#32
  84  001e ae500a        	ldw	x,#20490
  85  0021 cd0000        	call	_GPIO_Init
  87  0024 85            	popw	x
  88                     ; 57 	GPIO_Init(LCD_D4_PORT, LCD_D4_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  90  0025 4bc0          	push	#192
  91  0027 4b10          	push	#16
  92  0029 ae5019        	ldw	x,#20505
  93  002c cd0000        	call	_GPIO_Init
  95  002f 85            	popw	x
  96                     ; 58 	GPIO_Init(LCD_D5_PORT, LCD_D5_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  98  0030 4bc0          	push	#192
  99  0032 4b20          	push	#32
 100  0034 ae5019        	ldw	x,#20505
 101  0037 cd0000        	call	_GPIO_Init
 103  003a 85            	popw	x
 104                     ; 59 	GPIO_Init(LCD_D6_PORT, LCD_D6_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
 106  003b 4bc0          	push	#192
 107  003d 4b40          	push	#64
 108  003f ae5019        	ldw	x,#20505
 109  0042 cd0000        	call	_GPIO_Init
 111  0045 85            	popw	x
 112                     ; 60 	GPIO_Init(LCD_D7_PORT, LCD_D7_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
 114  0046 4bc0          	push	#192
 115  0048 4b80          	push	#128
 116  004a ae5019        	ldw	x,#20505
 117  004d cd0000        	call	_GPIO_Init
 119  0050 85            	popw	x
 120                     ; 63 	GPIO_Init(LED_PORT, LED_RED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
 122  0051 4bc0          	push	#192
 123  0053 4b10          	push	#16
 124  0055 ae501e        	ldw	x,#20510
 125  0058 cd0000        	call	_GPIO_Init
 127  005b 85            	popw	x
 128                     ; 64 	GPIO_Init(LED_PORT, LED_GREEN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
 130  005c 4bc0          	push	#192
 131  005e 4b20          	push	#32
 132  0060 ae501e        	ldw	x,#20510
 133  0063 cd0000        	call	_GPIO_Init
 135  0066 85            	popw	x
 136                     ; 67 	ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL0,DISABLE);
 138  0067 5f            	clrw	x
 139  0068 cd0000        	call	_ADC2_SchmittTriggerConfig
 141                     ; 68 	ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
 143  006b a620          	ld	a,#32
 144  006d cd0000        	call	_ADC2_PrescalerConfig
 146                     ; 69 	ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
 148  0070 a608          	ld	a,#8
 149  0072 cd0000        	call	_ADC2_AlignConfig
 151                     ; 70 	ADC2_Select_Channel(ADC2_CHANNEL_0);
 153  0075 4f            	clr	a
 154  0076 cd0000        	call	_ADC2_Select_Channel
 156                     ; 71 	ADC2_Select_Channel(ADC2_CHANNEL_1);
 158  0079 a601          	ld	a,#1
 159  007b cd0000        	call	_ADC2_Select_Channel
 161                     ; 72 	ADC2_Cmd(ENABLE);
 163  007e a601          	ld	a,#1
 164  0080 cd0000        	call	_ADC2_Cmd
 166                     ; 73 	ADC2_Startup_Wait();
 168  0083 cd0000        	call	_ADC2_Startup_Wait
 170                     ; 74 }
 173  0086 81            	ret
 176                     	bsct
 177  0000               _snimac_prvni:
 178  0000 0000          	dc.w	0
 179  0002               _snimac_druhy:
 180  0002 0000          	dc.w	0
 181  0004               _time:
 182  0004 00000000      	dc.l	0
 183  0008               _volna_mista:
 184  0008 0005          	dc.w	5
 185  000a               _pocet_mist:
 186  000a 0005          	dc.w	5
 187  000c               _obsazeno:
 188  000c 0000          	dc.w	0
 223                     ; 86 void delay1s(void) {
 224                     	switch	.text
 225  0087               _delay1s:
 227  0087 89            	pushw	x
 228       00000002      OFST:	set	2
 231                     ; 88 	for (i=0; i<5000; i++) {
 233  0088 5f            	clrw	x
 234  0089 1f01          	ldw	(OFST-1,sp),x
 236  008b               L74:
 237                     ; 21 	_asm("nop\n $N:\n decw X\n jrne $L\n nop\n ", __ticks);
 241  008b ae02be        	ldw	x,#702
 243  008e 9d            nop
 244  008f                L01:
 245  008f 5a             decw X
 246  0090 26fd           jrne L01
 247  0092 9d             nop
 248                      
 250  0093               L72:
 251                     ; 88 	for (i=0; i<5000; i++) {
 253  0093 1e01          	ldw	x,(OFST-1,sp)
 254  0095 1c0001        	addw	x,#1
 255  0098 1f01          	ldw	(OFST-1,sp),x
 259  009a 1e01          	ldw	x,(OFST-1,sp)
 260  009c a31388        	cpw	x,#5000
 261  009f 25ea          	jrult	L74
 262                     ; 91 }
 265  00a1 85            	popw	x
 266  00a2 81            	ret
 308                     .const:	section	.text
 309  0000               L41:
 310  0000 0000012d      	dc.l	301
 311                     ; 93 void main(void){
 312                     	switch	.text
 313  00a3               _main:
 317                     ; 94 	setup();
 319  00a3 cd0000        	call	_setup
 321                     ; 95 	lcd_init();
 323  00a6 cd0000        	call	_lcd_init
 325                     ; 96 	init_milis();
 327  00a9 cd0000        	call	_init_milis
 329                     ; 99 	lcd_gotoxy(0, 0);
 331  00ac 5f            	clrw	x
 332  00ad cd0000        	call	_lcd_gotoxy
 334                     ; 100 	lcd_puts("PARKOVISTE");
 336  00b0 ae003d        	ldw	x,#L56
 337  00b3 cd0000        	call	_lcd_puts
 339                     ; 102 	LED_GREEN_ON;
 341  00b6 4b20          	push	#32
 342  00b8 ae501e        	ldw	x,#20510
 343  00bb cd0000        	call	_GPIO_WriteLow
 345  00be 84            	pop	a
 346  00bf               L76:
 347                     ; 105 		if (milis()-time>300){
 349  00bf cd0000        	call	_milis
 351  00c2 cd0000        	call	c_uitolx
 353  00c5 ae0004        	ldw	x,#_time
 354  00c8 cd0000        	call	c_lsub
 356  00cb ae0000        	ldw	x,#L41
 357  00ce cd0000        	call	c_lcmp
 359  00d1 25ec          	jrult	L76
 360                     ; 106 			time=milis();
 362  00d3 cd0000        	call	_milis
 364  00d6 cd0000        	call	c_uitolx
 366  00d9 ae0004        	ldw	x,#_time
 367  00dc cd0000        	call	c_rtol
 369                     ; 107 			snimac_prvni=ADC_get(ADC2_CHANNEL_1); //zaznamenání pohybu na prvním snímaèi
 371  00df a601          	ld	a,#1
 372  00e1 cd0000        	call	_ADC_get
 374  00e4 bf00          	ldw	_snimac_prvni,x
 375                     ; 108 			snimac_druhy=ADC_get(ADC2_CHANNEL_0); //zaznamenání pohybu na druhém snímaèi
 377  00e6 4f            	clr	a
 378  00e7 cd0000        	call	_ADC_get
 380  00ea bf02          	ldw	_snimac_druhy,x
 381                     ; 110 			if (snimac_prvni < 500){
 383  00ec be00          	ldw	x,_snimac_prvni
 384  00ee a301f4        	cpw	x,#500
 385  00f1 2407          	jruge	L57
 386                     ; 111 				volna_mista--; //pohyb na vjezdu... -1 volných míst
 388  00f3 be08          	ldw	x,_volna_mista
 389  00f5 1d0001        	subw	x,#1
 390  00f8 bf08          	ldw	_volna_mista,x
 391  00fa               L57:
 392                     ; 114 			if (snimac_druhy < 500){
 394  00fa be02          	ldw	x,_snimac_druhy
 395  00fc a301f4        	cpw	x,#500
 396  00ff 2407          	jruge	L77
 397                     ; 115 				volna_mista++; //pohyb na výjezdu... +1 volných míst
 399  0101 be08          	ldw	x,_volna_mista
 400  0103 1c0001        	addw	x,#1
 401  0106 bf08          	ldw	_volna_mista,x
 402  0108               L77:
 403                     ; 119 			if (volna_mista<=pocet_mist+1){
 405  0108 9c            	rvf
 406  0109 be0a          	ldw	x,_pocet_mist
 407  010b 5c            	incw	x
 408  010c b308          	cpw	x,_volna_mista
 409  010e 2f18          	jrslt	L101
 410                     ; 120 				lcd_gotoxy(0, 1);
 412  0110 ae0001        	ldw	x,#1
 413  0113 cd0000        	call	_lcd_gotoxy
 415                     ; 121 				lcd_puts("volna mista: 5");
 417  0116 ae002e        	ldw	x,#L301
 418  0119 cd0000        	call	_lcd_puts
 420                     ; 122 				volna_mista==pocet_mist;
 422  011c be08          	ldw	x,_volna_mista
 423  011e b30a          	cpw	x,_pocet_mist
 424  0120 2605          	jrne	L61
 425  0122 ae0001        	ldw	x,#1
 426  0125 2001          	jra	L02
 427  0127               L61:
 428  0127 5f            	clrw	x
 429  0128               L02:
 430  0128               L101:
 431                     ; 126 			if (volna_mista<=obsazeno){
 433  0128 9c            	rvf
 434  0129 be08          	ldw	x,_volna_mista
 435  012b b30c          	cpw	x,_obsazeno
 436  012d 2c39          	jrsgt	L501
 437                     ; 127 				LED_GREEN_OFF;
 439  012f 4b20          	push	#32
 440  0131 ae501e        	ldw	x,#20510
 441  0134 cd0000        	call	_GPIO_WriteHigh
 443  0137 84            	pop	a
 444                     ; 128 				LED_RED_ON; //èervená LED - plné parkovištì, žádná volná místa
 446  0138 4b10          	push	#16
 447  013a ae501e        	ldw	x,#20510
 448  013d cd0000        	call	_GPIO_WriteLow
 450  0140 84            	pop	a
 451                     ; 129 				lcd_clear();
 453  0141 a601          	ld	a,#1
 454  0143 cd0000        	call	_lcd_command
 456                     ; 130 				lcd_gotoxy(0, 0);
 458  0146 5f            	clrw	x
 459  0147 cd0000        	call	_lcd_gotoxy
 461                     ; 131 				lcd_puts("LEGO PARKOVISTE");
 463  014a ae001e        	ldw	x,#L701
 464  014d cd0000        	call	_lcd_puts
 466                     ; 132 				lcd_gotoxy(0, 1);
 468  0150 ae0001        	ldw	x,#1
 469  0153 cd0000        	call	_lcd_gotoxy
 471                     ; 133 				lcd_puts("obsazeno");	
 473  0156 ae0015        	ldw	x,#L111
 474  0159 cd0000        	call	_lcd_puts
 476                     ; 134 				volna_mista==obsazeno;
 478  015c be08          	ldw	x,_volna_mista
 479  015e b30c          	cpw	x,_obsazeno
 480  0160 2605          	jrne	L22
 481  0162 ae0001        	ldw	x,#1
 482  0165 2001          	jra	L42
 483  0167               L22:
 484  0167 5f            	clrw	x
 485  0168               L42:
 486  0168               L501:
 487                     ; 138 			if (volna_mista>=1 && volna_mista<=pocet_mist){
 489  0168 9c            	rvf
 490  0169 be08          	ldw	x,_volna_mista
 491  016b 2c03          	jrsgt	L62
 492  016d cc00bf        	jp	L76
 493  0170               L62:
 495  0170 9c            	rvf
 496  0171 be08          	ldw	x,_volna_mista
 497  0173 b30a          	cpw	x,_pocet_mist
 498  0175 2d03          	jrsle	L03
 499  0177 cc00bf        	jp	L76
 500  017a               L03:
 501                     ; 139 				LED_GREEN_ON;
 503  017a 4b20          	push	#32
 504  017c ae501e        	ldw	x,#20510
 505  017f cd0000        	call	_GPIO_WriteLow
 507  0182 84            	pop	a
 508                     ; 140 				LED_RED_OFF;
 510  0183 4b10          	push	#16
 511  0185 ae501e        	ldw	x,#20510
 512  0188 cd0000        	call	_GPIO_WriteHigh
 514  018b 84            	pop	a
 515                     ; 141 				lcd_gotoxy(0, 1);
 517  018c ae0001        	ldw	x,#1
 518  018f cd0000        	call	_lcd_gotoxy
 520                     ; 142 				sprintf(text, "volna mista: %1u", volna_mista);
 522  0192 be08          	ldw	x,_volna_mista
 523  0194 89            	pushw	x
 524  0195 ae0004        	ldw	x,#L511
 525  0198 89            	pushw	x
 526  0199 ae0000        	ldw	x,#_text
 527  019c cd0000        	call	_sprintf
 529  019f 5b04          	addw	sp,#4
 530                     ; 143 				lcd_puts(text);
 532  01a1 ae0000        	ldw	x,#_text
 533  01a4 cd0000        	call	_lcd_puts
 535                     ; 144 				delay1s();
 537  01a7 cd0087        	call	_delay1s
 539  01aa acbf00bf      	jpf	L76
 574                     ; 160 void assert_failed(u8* file, u32 line)
 574                     ; 161 { 
 575                     	switch	.text
 576  01ae               _assert_failed:
 580  01ae               L531:
 581  01ae 20fe          	jra	L531
 660                     	xdef	_main
 661                     	xdef	_delay1s
 662                     	xdef	_obsazeno
 663                     	xdef	_pocet_mist
 664                     	xdef	_volna_mista
 665                     	xdef	_time
 666                     	xdef	_snimac_druhy
 667                     	xdef	_snimac_prvni
 668                     	switch	.ubsct
 669  0000               _text:
 670  0000 000000000000  	ds.b	32
 671                     	xdef	_text
 672                     	xdef	_setup
 673                     	xref	_ADC2_Startup_Wait
 674                     	xref	_ADC2_AlignConfig
 675                     	xref	_ADC2_Select_Channel
 676                     	xref	_ADC_get
 677                     	xref	_sprintf
 678                     	xref	_lcd_puts
 679                     	xref	_lcd_gotoxy
 680                     	xref	_lcd_init
 681                     	xref	_lcd_command
 682                     	xref	_init_milis
 683                     	xref	_milis
 684                     	xdef	_assert_failed
 685                     	xref	_GPIO_WriteLow
 686                     	xref	_GPIO_WriteHigh
 687                     	xref	_GPIO_Init
 688                     	xref	_CLK_HSIPrescalerConfig
 689                     	xref	_ADC2_SchmittTriggerConfig
 690                     	xref	_ADC2_PrescalerConfig
 691                     	xref	_ADC2_Cmd
 692                     	switch	.const
 693  0004               L511:
 694  0004 766f6c6e6120  	dc.b	"volna mista: %1u",0
 695  0015               L111:
 696  0015 6f6273617a65  	dc.b	"obsazeno",0
 697  001e               L701:
 698  001e 4c45474f2050  	dc.b	"LEGO PARKOVISTE",0
 699  002e               L301:
 700  002e 766f6c6e6120  	dc.b	"volna mista: 5",0
 701  003d               L56:
 702  003d 5041524b4f56  	dc.b	"PARKOVISTE",0
 722                     	xref	c_rtol
 723                     	xref	c_lcmp
 724                     	xref	c_lsub
 725                     	xref	c_uitolx
 726                     	end
