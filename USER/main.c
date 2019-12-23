//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//����Э���
#include "onenet.h"

//�����豸
#include "esp8266.h"

//Ӳ������
#include "delay.h"
#include "usart.h"

//C��
#include <string.h>


//////////////////////////////�̵�����ʼ��///////////////////////////////

void jidianqi_Init(void)
{
	
	GPIO_InitTypeDef gpio_initstruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	//��GPIOA��GPIOC��ʱ��
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;									//����Ϊ�������ģʽ
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1 ;					//��ʼ��Pin5
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;									//���ص����Ƶ��
	GPIO_Init(GPIOA, &gpio_initstruct);												//��ʼ��GPIOA
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);                                           //��ʼ���رշ���

}

/////////////////////////////////////////////////////////////////////////

/*
************************************************************
*	�������ƣ�	Hardware_Init
*
*	�������ܣ�	Ӳ����ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ����Ƭ�������Լ�����豸
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϿ�������������

	delay_init();									//systick��ʼ��
	
	//Usart1_Init(115200);							//����1����ӡ��Ϣ��
	
	Usart2_Init(115200);							//����2������ESP8266��
	
	jidianqi_Init();                                //�̵�����ʼ�� 
	
	//UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
}

/*
************************************************************
*	�������ƣ�	main
*
*	�������ܣ�	
*
*	��ڲ�����	��
*
*	���ز�����	0
*
*	˵����		
************************************************************
*/
int main(void)
{
	
	unsigned short timeCount = 0;	//���ͼ������
	
	unsigned char *dataPtr = NULL;
	
	Hardware_Init();				//��ʼ����ΧӲ��
	
	ESP8266_Init();					//��ʼ��ESP8266
	
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);

	
	while(1)
	{
		
		if(++timeCount >= 1000)									//���ͼ��10s
		{
		//	UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//��������
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
	    OneNet_RevPro(dataPtr);
		
		delay_ms(10);
	
	}

}
