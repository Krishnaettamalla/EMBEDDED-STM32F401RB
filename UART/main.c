/**
		Experiment 1: USART LOOPBACK POLLING
		Write a pseudo code and Embedded C program to Configure UART1 with 9600 8N1.
		And transmit character ‘A’ and Receive same character using hardware Loop back.
		a. System Clock Initialization to 16MHZ. 
		b. UART Communication protocol specifications –
				9600 8N1 = 1 start bit+8 word length +parity(0 bit)+ 1 stop bit = 10 bits. 
		c. Enable polling (nothing but disable UART Interrupts)
		
		USART1_TX	-	PA9
		USART1_RX	-	PA10
		
Platform:					Raayanmini Board V4.0/V4.1
Microcontroller:	STM32F401RBT6
**/

//RCC registers
#define		RCC_AHB1ENR				(*(volatile int *)0x40023830)				//system clock
#define		RCC_APB2ENR				(*(volatile int *)0x40023844)				//peripheral clock enable   
//GPIOA registers
#define		GPIO_PORTA_MODE		(*(volatile int *)0x40020000)     	//GPIOA MODE register
#define		GPIO_PORTA_ODR		(*(volatile int *)0x40020014)     	//GPIOA output data register
#define		GPIO_PORTA_AFRH		(*(volatile int *)0x40020024)				//GPIOA alternate function high register
//UART1 registers
#define   UART1_SR					(*(volatile int *)0x40011000)				//UART1 Status register
#define   UART1_DR					(*(volatile int *)0x40011004)				//UART1 Data register   
#define   UART1_BRR					(*(volatile int *)0x40011008)				//UART1 Baud rate register
#define   UART1_CR1					(*(volatile int *)0x4001100C)				//UART1 Control register 1
#define   UART1_CR2					(*(volatile int *)0x40011010)				//UART1 Control register 2

/*Function prototypes*/
void GPIO_PORTA_Init(void);															//Port A Initialization
void UART1_Init_Config(void);														//UART1 Initialization
void UART1_TX_Outchar(unsigned char out);								//function to transmit one byte
unsigned char UART1_RX_Inchar(void);										//function to receive one byte


void GPIO_PORTA_Init(void)
{
	RCC_AHB1ENR |= (0x01);															//Enable PORT A
	while(!(RCC_AHB1ENR & 0x01));
	GPIO_PORTA_MODE &= ~(0x3C0000);											//clear only the corresponding fields
	GPIO_PORTA_MODE |= 0x280000;												//Set PA9 and PA10 MODE to Alternate function mode
	GPIO_PORTA_AFRH &= ~(0x00000770);										//clear only the corresponding fields
	GPIO_PORTA_AFRH |= 0x00000770;											//Select the alternate functionality UART1
}
void UART1_Init_Config(void)
{
	RCC_APB2ENR |= (0x1<<4);														//Enable UART1
	while(!(RCC_APB2ENR & 0x10));
	UART1_BRR = 0x683;																	//Set baud rate 9600 =>Baud Rate = System clock/8*(2-0)*USARTDIV
																											//USARTDIV = System clock/8*(2-0)*BaudRate = 16000000/16*9600 =
																											//16000000/(16*9600)=104.1875 ==> 104+0.18==>68(integral part)
											
	UART1_CR1 &= ~(0x200C);															//clear the fields
	UART1_CR1 |= 0x200C;																//UART En-1, Tx En-1, Rx En-1
}
void UART1_TX_Outchar(unsigned char out)
{
	while(!(UART1_SR & 0x80));
	UART1_DR = out;
}
unsigned char UART1_RX_Inchar(void)
{
	while(!(UART1_SR & 0x20));
	return (UART1_DR & 0xFF);
}

volatile char ch;

int main(){
	GPIO_PORTA_Init();															//Initialize and configure PA9 and PA10 pins
	UART1_Init_Config();														//Initialize and configure UART1
	while(1)
	{
/*			Transmit part		*/
		while(!(UART1_SR & 0x80));										//check for the Tx data register empty (TXE) bit in status register
		UART1_DR = 'Z';																//Transmit
//		UART1_TX_Outchar(++ch);											//Transmit
				
/*			Receive part		*/
		while(!(UART1_SR & 0x20));										//check for the Read data register not empty (RXNE) bit in status register
		ch = (UART1_DR & 0xFF);												//Receive
//		ch = UART1_RX_Inchar();											//Receive
	}
}
