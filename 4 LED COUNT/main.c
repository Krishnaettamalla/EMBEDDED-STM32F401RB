/*Experiment 4: 
Use SW_ENT and declare "counter" global variable. 
Your program should increment counter by one, every time switch is pressed. 
Note how the value of counter changes on each 'ENTER SWITCH' press. 
Use debugger and add Counter to “Watch Expression” window. 
Does the value of counter increment by one always?

SW_ENT				- PC10

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/

//RCC registers
#define		RCC_AHB1ENR								(*(volatile int *)0x40023830)						//RCC system clock
//GPIOC registers
#define		GPIO_PORTC_MODE						(*(volatile int *)0x40020800)						//GPIOC MODE register
#define		GPIO_PORTC_PUPDR					(*(volatile int *)0x4002080C)						//GPIOC pull-up/pull-down register
#define		GPIO_PORTC_IDR						(*(volatile int *)0x40020810)						//GPIOC input data register
#define		GPIO_PORTC_ODR						(*(volatile int *)0x40020814)						//GPIOC output data register

//Function declarations
void Delay(int n);																				//Software delay Function


void Delay(int n)
{
	volatile int i;
	for(i=0; i < 1600*n; i++);
}

int counter;

int main()
{
	RCC_AHB1ENR |= 0x04;																		//Enable clock for Port C
	while((RCC_AHB1ENR & 0x04) == 0);	   										//Wait till the bit is set
	GPIO_PORTC_MODE &= 0xFFFCFFFF;
	GPIO_PORTC_PUPDR |= 0x00100000;													//Enable pull up for SW_ENT
	while(1)
	{
		if(!(GPIO_PORTC_IDR & (0x1<<10)))											//check the SW_UP(PC8) bit
		{
			counter++;
			Delay(100);
		}
	}
}
