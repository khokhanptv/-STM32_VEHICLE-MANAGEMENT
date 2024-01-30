
#include "stm32f4xx.h"                  // Device header
#include "RFID_rc522.h"
#include "stdio.h"
#include "string.h"


void UART1_Config(void);
void TIM_Config();
void Delay_ms(uint32_t time);
void put_char(uint8_t a);
//////////////////////////////////////
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/////////////////////////////////////

PUTCHAR_PROTOTYPE
{
/* Place your implementation of fputc here */
/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);
/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	return ch;
}

uchar i,tmp;
uchar status;
uchar str[MAX_LEN]; // Max_LEN = 16

uchar serNum[5];
char password[16]="123456"; //Max lenght of password is 16 charaters
char keypass[16];
int cnt=0;
uint8_t key;
uchar check,check2;

uchar Key_Card[5]  = {0x16, 0xe7, 0x23, 0xa3, 0x71};
uchar Key_Card2[5] = {0xc4, 0x24, 0x99, 0x22, 0x5b};
int main (void){
	TIM_Config();
	UART1_Config();		
  MFRC522_Init();
	while(1){
		printf("Waiting your Card! \r\n");
				
		status = MFRC522_Request(PICC_REQIDL, str);	
		if (status == MI_OK)
		{
			printf("Find out a card: %x, %x\r\n",str[0],str[1]);
		}

		//Chong va cham the, tra ve 5 byte ma the
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);
		if (status == MI_OK)
		{
			printf("Your card's number are: %x, %x, %x, %x, %x \r\n",serNum[0], serNum[1], serNum[2], serNum[3],serNum[4]);
			for(i=0;i<5;i++)
			{
				if(Key_Card[i]!=serNum[i]) check = 0;
				else check = 1;
				if(Key_Card2[i]!=serNum[i]) check2 = 0;
				else check2 = 1;
			}
		}
		if(check == 1||check2 ==1)
		{
			check = 0;
			check2 = 0;
			printf("The Card's number is Ok!\r\n");
			Delay_ms(1000);
		}
		
	}

}

void TIM_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 0xFFFF-1;
	TIM_InitStruct.TIM_Prescaler = 16800-1;
	TIM_TimeBaseInit(TIM1,&TIM_InitStruct);
	TIM_Cmd(TIM1,ENABLE);
}

void Delay_ms(uint32_t time){
	TIM_SetCounter(TIM1,0);
	while(TIM_GetCounter(TIM1) < time*10);
}
void UART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	
	/* GPIO Config PA9 -> TX, PA10 -> RX */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	/* UART1 Config */
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
}
void put_char(uint8_t a)
{
	USART_SendData(USART1,a);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}