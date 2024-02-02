#include "main.h"  

uchar i,tmp;
uchar status;
uchar str[MAX_LEN]; // Max_LEN = 16

FATFS FatFs;
FIL Fil;
FRESULT fr;



uchar serNum[5];
int cnt=0;
uint8_t key;
uchar check,check2;
char myString[100];
uchar Key_Card[5]  = {0x16, 0xe7, 0x23, 0xa3, 0x71};
uchar Key_Card2[5] = {0xc4, 0x24, 0x99, 0x22, 0x5b};



int main(){
	SPIRC552_Config();	
	TIMDelay_Config();
	TIMPWM_Config();
	UART1_Config();
	MFRC522_Init();
 

	while(1){
		Delay_ms(1000);
		sprintf(myString,"vui long dat card! \r\n");
		USART_SendString(myString);
		status = MFRC522_Request(PICC_REQIDL, str);	
		if (status == MI_OK)
		{
			sprintf(myString,"Find out a card: %x, %x\r\n",str[0],str[1]);
			USART_SendString(myString);
		}

		
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);
		if (status == MI_OK)
		{
			sprintf(myString,"Your card's number are: %x, %x, %x, %x, %x \r\n",serNum[0], serNum[1], serNum[2], serNum[3],serNum[4]);
			USART_SendString(myString);
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
			Delay_ms(1000);
			set_pwm(1499);
			Delay_ms(2000);
			set_pwm(999);
			sprintf(myString,"The Card's number is Ok!\r\n");
			USART_SendString(myString);
			Delay_ms(1000);
		}
		Delay_ms(1000);
	}

}

