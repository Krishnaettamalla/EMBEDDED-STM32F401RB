/*Experiment 8: 
Write a program Request an interrupt on the Falling edge of ENTER Switch, 
whenever the Enter button is pressed, increment a counter in the interrupt and RED LED is ON. 

RED LED 			- PB13
SW_ENT				- PC10

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/

//RCC registers
#define		RCC_AHB1ENR								(*(volatile int *)0x40023830)						//system clock
#define		RCC_APB2ENR								(*(volatile int *)0x40023844)						//peripheral clock enable 
//GPIOB registers
#define		GPIO_PORTB_MODE						(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_IDR						(*(volatile int *)0x40020410)						//GPIOB input data register
#define		GPIO_PORTB_ODR						(*(volatile int *)0x40020414)						//GPIOB output data register
//GPIOC registers
#define		GPIO_PORTC_MODE						(*(volatile int *)0x40020800)						//GPIOC MODE register
#define		GPIO_PORTC_PUPDR					(*(volatile int *)0x4002080C)						//GPIOC pull-up/pull-down register
#define		GPIO_PORTC_IDR						(*(volatile int *)0x40020810)						//GPIOC input data register
#define		GPIO_PORTC_ODR						(*(volatile int *)0x40020814)						//GPIOC output data register
//SYSCFG registers
#define		SYSCFG_EXTICR1						(*(volatile int *)0x40013808)						//SYSCFG external interrupt configuration register 1
#define		SYSCFG_EXTICR2						(*(volatile int *)0x4001380C)						//SYSCFG external interrupt configuration register 2
#define		SYSCFG_EXTICR3						(*(volatile int *)0x40013810)						//SYSCFG external interrupt configuration register 3
#define		SYSCFG_EXTICR4						(*(volatile int *)0x40013814)						//SYSCFG external interrupt configuration register 4
//EXTI registers
#define		EXTI_IMR									(*(volatile int *)0x40013C00)						//EXTI Interrupt mask register
#define		EXTI_EMR									(*(volatile int *)0x40013C04)						//EXTI Event mask register
#define		EXTI_RTSR									(*(volatile int *)0x40013C08)						//EXTI Rising trigger selection register
#define		EXTI_FTSR									(*(volatile int *)0x40013C0c)						//EXTI Falling trigger selection register
#define		EXTI_SWIER								(*(volatile int *)0x40013C10)						//EXTI Software interrupt event register
#define		EXTI_PR										(*(volatile int *)0x40013C14)						//EXTI Pending register
//NVIC registers
#define		NVIC_ISER0								(*(volatile int *)0xE000E100)						//Interrupt Range is 31 to 0
#define		NVIC_ISER1								(*(volatile int *)0xE000E104)						//Interrupt Range is 63 to 32
#define		NVIC_ISER2								(*(volatile int *)0xE000E108)						//Interrupt Range is upto end to 64

//Function prototype
void RED_LED_INIT(void);																//Initialize and configure RED LED
void SWITCH_ENT_INIT(void);															//Initialize and configure SW_ENT
void SWITCH_INTERRUPT_EN(void);													//Enable an interrupt on the Falling edge of ENTER Switch
void EXTI15_10_IRQHandler(void);
void Delay(int n);																			//Delay function


void Delay(int n)
{
	volatile int i;
	for(i=0; i < 1600*n; i++);
}

void RED_LED_INIT(void)
{
	RCC_AHB1ENR |= (0x1<<1);																//Enable clock to Port B
	while(!(RCC_AHB1ENR & 0x02));														//Wait till the bit is set
	GPIO_PORTB_MODE &= 0xF3FFFFFF;	
	GPIO_PORTB_MODE |= 0x04000000;													//Enable OutPut Mode for PB13 
	GPIO_PORTB_ODR |= 0x1 << 13;														//Turn off the Red Led
}
void SWITCH_ENT_INIT(void)
{
	RCC_AHB1ENR |= 0x04;																		//Enable clock for Port C
	while((RCC_AHB1ENR & 0x04) == 0);	   										//Wait till the bit is set
	GPIO_PORTC_MODE &= 0xFFCFFFF;
	GPIO_PORTC_PUPDR |= 0x00100000;													//Enable pull-up
}
void SWITCH_INTERRUPT_EN(void)
{
	RCC_APB2ENR |= 0x00004000;															//Enable System configuration controller clock
	SYSCFG_EXTICR3 |= 0x0000200;														//select the source input for the EXTI10 external interrupt.
	EXTI_IMR |= 0x00000400;																	//Interrupt request from line 10 is not masked
	EXTI_FTSR |= 0x00000400;																//Falling trigger enabled (for Event and Interrupt) for input line.
/**		Enable NVIC Interrupt		**/
	NVIC_ISER1 |= 0x00000100;																//Enable interrupt for EXTI15_10 position 40
}

int counter=0, flag=0;

int main()
{
	RED_LED_INIT();																				//Initialize and configure RED LED
	SWITCH_ENT_INIT();																		//Initialize and configure SW_ENT
	SWITCH_INTERRUPT_EN();																//Enable interrupt on the Falling edge of ENTER Switch
	while(1)
	{
		GPIO_PORTB_ODR |= (0x1<<13);												//Turn OFF the RED LED
		Delay(1000);
//		if(flag == 1)																			//check when the flag is set from interrupt
//		{
//			flag = 0;																				//clear the flag variable
//			GPIO_PORTB_ODR &= ~(0x1<<13);										//Turn ON the RED LED
//			Delay(500);
//			GPIO_PORTB_ODR |= (0x1<<13);										//Turn OFF the RED LED
//		}
	}
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI_PR & 0x400))
	{
		GPIO_PORTB_ODR &= ~(0x1<<13);												//Turn ON the RED LED
//		flag = 1;																					//set the flag
  	counter++;																					//Increment the Global variable- counter
		EXTI_PR |= 0x400;																		//Clear the pending register by setting the bit
	}
}
