#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "FreeRTOSConfig.h"

#define TXD GPIO_Pin_9
#define RXD GPIO_Pin_10
#define USART_GPIO GPIOA

void USART_User_Init(uint32_t BaudRate)  
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure GPIO */
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = TXD;  	// Output PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(USART_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RXD;  	// Input 10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_GPIO, &GPIO_InitStructure);

	/* Configure USART */
	USART_InitTypeDef USART_InitStructure;

	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure); 

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART1, USART_IT_TXE,ENABLE);
	USART_Cmd(USART1, ENABLE);

	/* Configure NVIC */	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 16;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
  USART_SendData(pUSARTx,ch);
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
  unsigned int k=0;
  do 
  {
    Usart_SendByte(pUSARTx, *(str + k));
    k++;
  } while(*(str + k)!='\0');
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);
}
#if 0
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
        
	}
}
#endif
