/*Experiment 1: Write an Embedded C Program Blink RED LED every 100msec Software Delay 

RED LED -> PB13

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/

//RCC registers
#define		RCC_AHB1ENR								(*(volatile int *)0x40023830)						//RCC system clock
//GPIOB registers
#define		GPIO_PORTB_MODE						(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_IDR						(*(volatile int *)0x40020410)						//GPIOB input data register
#define		GPIO_PORTB_ODR						(*(volatile int *)0x40020414)						//GPIOB output data register

//Function declarations
void Delay(int n);															//Software delay Function

void Delay(int n)
{
	volatile int i;
	for(i=0; i < 1600*n; i++);
}

int main()
{
	RCC_AHB1ENR |= (0x1<<1);												//Enable PORT B clock
	while(!(RCC_AHB1ENR & 0x02));
	GPIO_PORTB_MODE &= 0xF3FFFFFF;									//clear the respective bits
	GPIO_PORTB_MODE |= 0x04000000;									//Enable OutPut Mode for PB13 
	GPIO_PORTB_ODR |= 0x1 << 13;										//Turn off the Red Led
	while(1)
	{
		GPIO_PORTB_ODR &= ~(0x1 << 13);								//Turn on the Red Led
		Delay(100);										//Delay of 100ms
		GPIO_PORTB_ODR |= (0x1 << 13);								//Turn off the Red Led
		Delay(100);										//Delay of 100ms
	}
}
