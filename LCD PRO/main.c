/*
LCD module bring up - Initialization, Configuration and Testing.

Board			: Raayanmini Board
Microcontroller	: STM32F401RBT6
*/

//RCC registers
#define		RCC_AHB1ENR							(*(volatile int *)0x40023830)						//RCC system clock
//Port-B registers
#define		GPIO_PORTB_MODE					(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_ODR					(*(volatile int *)0x40020414)						//GPIOB output data register
//Systick timer registers
#define		STK_CTRL								(*(volatile int *)0xE000E010)						//SYSTICK Timer Control register
#define		STK_LOAD								(*(volatile int *)0xE000E014)						//SYSTICK Timer Load register
#define		STK_VAL									(*(volatile int *)0xE000E018)						//SYSTICK Timer value register

#define		EN_ON()									GPIO_PORTB_ODR |= 0x100									//Set enable pin high
#define		EN_OFF()								GPIO_PORTB_ODR &= ~(0x100)							//Set enable pin low
#define		LCD_RS_DATA_MODE				GPIO_PORTB_ODR |= (0x10)								//Set register select high
#define		LCD_RS_CMD_MODE					GPIO_PORTB_ODR &= ~(0x10)								//Set register select low

/**function prototypes**/
void Systick_Init(void);															// systick timer init function
void delay_ms(int time);															// milli second delay function
void delay_Ms(int time);															// micro second delay function
void Port_B_Init(void);																//Port-B init function
void Enable(void);																		//Enable select
void Higher_Nibble(char Data);					
void Lower_Nibble(unsigned char Data);
void KM_LCD_Write_DATA(unsigned char Data);						//Function to print a character on the display
void KM_LCD_Write_CMD(unsigned char cmd);							//Function to send a command
void KM_LCD_Write_STR(char *str);											//Function to print a string on the display
void KM_Lcd_Init(void);																//LCD Init function


void Systick_Init(void)
{
	STK_CTRL |= 0x05;																			//initialize systick timer w/o interrupt
	STK_VAL |= 10;																				//clear the load register
}
void delay_ms(int msec)
{
	STK_LOAD = 16000 * msec;
	STK_VAL = 0;
	while(!(STK_CTRL & (0x1<<16)));												//systick delay
}
void delay_Ms(int usec)
{
	STK_LOAD = 16 * usec;
	STK_VAL = 0;
	while(!(STK_CTRL & (0x1<<16)));												//systick delay
}

void Enable(void)
{
	EN_ON();
	delay_ms(10);
	EN_OFF();
	delay_ms(10);
}
void Higher_Nibble(char Data)
{
	GPIO_PORTB_ODR &= ~(0x0F);
	GPIO_PORTB_ODR |= ((Data>>4)&0x0F);
	Enable();
}
void Lower_Nibble(unsigned char Data)
{
	GPIO_PORTB_ODR &= ~(0x0F);
	GPIO_PORTB_ODR |= ((Data)&0x0F);
	Enable();
}
void KM_LCD_Write_DATA(unsigned char Data)
{	
	LCD_RS_DATA_MODE; 
	Higher_Nibble(Data);
	Lower_Nibble(Data);		
}
void KM_LCD_Write_CMD(unsigned char cmd)
{
	LCD_RS_CMD_MODE;
	Higher_Nibble(cmd);
	Lower_Nibble(cmd);		
}
void Port_B_Init(void)
{
	RCC_AHB1ENR |= 0x02;																			//Enable clock for portB	
	while((RCC_AHB1ENR & 0x02) == 0);	   											//wait till the bit is set 
	GPIO_PORTB_MODE &= ~(0x00030FFF);					
	GPIO_PORTB_MODE |= 0x00010555;	  												//LCD All Pin Set as a OutPut Mode (PB0-D4,PB1-D5,PB2-D6,PB3-D7,PB4-RS,PB5-RW,PB8-EN)
	GPIO_PORTB_ODR = 0x00;																		//empty o/p data register
}
void KM_Lcd_Init(void)
{
	delay_ms(20);
	KM_LCD_Write_CMD(0x33);
	delay_ms(1);
	KM_LCD_Write_CMD(0x32);
	delay_ms(1);
	KM_LCD_Write_CMD(0x0C);
	KM_LCD_Write_CMD(0x01);
	KM_LCD_Write_CMD(0x80);
}
void KM_LCD_Write_STR(char *str)
{
	int i;
	for(i=0; str[i]!='\0'; i++)
	{
		KM_LCD_Write_DATA(str[i]);
	}
}

int main()
{
	Port_B_Init();																							//Initialize port B
	Systick_Init();																							//Initialise systick timer
	KM_Lcd_Init();																							//Initialize LCD
	KM_LCD_Write_CMD(0x01);																			//clear the screen
	KM_LCD_Write_CMD(0x80);																			//Force cursor to the beginning of row 1
	KM_LCD_Write_DATA('B');																			//write a character on LCD
	KM_LCD_Write_CMD(0xC0);																			//Force cursor to the beginning of row 2
	KM_LCD_Write_STR("  Hello World");													//Print Hello World on LCD
}
