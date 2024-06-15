/*
Experiment 15: 
Write an Embedded C program to initialize ADC Single Conversion mode. 
Using regular channel software trigger samples ADC channel 11 (potentiometer) and
stores the result to a global variable ADC that can be accessed with the ST-Link Debugger.
Use ADC interrupt method.

Potentiometer	-	ADC1_IN11	-	PC1

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/


#define RCC_AHB1ENR  	*((volatile int*)0x40023830)       //RCC AHB1 peripheral clock enable register
#define RCC_APB2ENR  	*((volatile int*)0x40023844)       //RCC APB2 peripheral clock enable register
#define GPIOC_MODER  	*((volatile int*)0x40020800)       //GPIO_PORT_C mode register
#define ADC_SR       	*((volatile int*)0x40012000)       //ADC status register
#define ADC_CR1      	*((volatile int*)0x40012004)       //ADC control register 1
#define ADC_CR2      	*((volatile int*)0x40012008)       //ADC control register 2
#define ADC_SMPR1    	*((volatile int*)0x4001200c)       //ADC sample time register 1
#define ADC_SQR3     	*((volatile int*)0x40012034)       //ADC regular sequence register 3
#define ADC_DR       	*((volatile int*)0x4001204c)	     //ADC regular data register
#define NVIC_ISER0   	*((volatile int*)0xE000E100)       //Interrupt set-enable register 0             

void GPIO_PORT_C_Init(void);                  		//For Initilizing GPIO_PORT_C 
void KM_ADC_Init(void);                       		//For Initilizing ADC 
void ADC_IRQHandler(void);                    		//ADC IRQ Handler

volatile int POT_Value = 0;

void GPIO_PORT_C_Init()
{
			RCC_AHB1ENR |= 0x4; 						        		//Enable PORT_C clock (ADC1_IN11)
			while(!(RCC_AHB1ENR & 0x4)); 								//Wait Untill PORT_C clock Enable
			GPIOC_MODER &= ~(0x3<<2); 									//Clear PC1 2-3 positions
			GPIOC_MODER |= 0x3<<2; 								  		//Load '11' in PC1 2-3 positions for Enable Analog Mode
}
void KM_ADC_Init()
{
			RCC_APB2ENR |= (0x1<<8); 								  	//Enable ADC clock
			while(!(RCC_APB2ENR & 0x1<<8)); 				  	//Wait Untill ADC Clock Enable				
			ADC_SMPR1  |= 0x7;													//Load '111' in (SMP10)0-2 positions to set sample time for 480 cycles
			ADC_SQR3   |= 0x0B; 												//Load '1010' in 0-4 positions to set ADC1_10 as 1st conversion in regular sequence
			ADC_CR1    &= ~(0x3<<24); 									//Load '00' in 24-25 positions to set 12 bit resolution			
			ADC_CR2    |= 0x1;													//Set 0th bit (ADON) to turn on ADC1
	    ADC_CR1    |= 0x01<<5; 	                  	//Interrupt enable for EOC
	    NVIC_ISER0 |= 0x01 << 18;                 	//Enable 18th in NVIC_ISR to enable interrupt at Processor
}

int main()
{
	GPIO_PORT_C_Init(); 
	KM_ADC_Init();
	while(1)
	{
	  ADC_CR2 |= 0x1<<30;                         //Start Conversion ( Enable software trigger )		
	}
}

void ADC_IRQHandler(void)
{
	  POT_Value = ADC_DR;                         //Collect data From ADC_DR register 		
}
