/*
 * init.c
 *
 *  Created on: 2025��7��1��
 *      Author: Rock
 */

#include "init.h"

//����������г�ʼ�����л��õ�������ʱ��
void RCC_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                          RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                          RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |
                            RCC_APB1Periph_UART4 |RCC_APB1Periph_UART5 |
                          RCC_APB1Periph_I2C1 |RCC_APB1Periph_I2C2 | RCC_APB1Periph_TIM2, ENABLE);
}

//�������������ʼ������GPIO����
void GPIO_Init_Single(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed;

    GPIO_Init(GPIOx, &GPIO_InitStructure);

    // ��������ģʽ��Ĭ������Ϊ�͵�ƽ
    if(GPIO_Mode == GPIO_Mode_Out_PP || GPIO_Mode == GPIO_Mode_Out_OD) {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
    }
}

//�������������ʼ������USARTģ�飬ע����Ҫ���ں�����ƥ��
void USART_Init_Single(USART_TypeDef* USARTx, uint32_t BaudRate,
                      GPIO_TypeDef* GPIO_TxPort, uint16_t GPIO_TxPin,
                      GPIO_TypeDef* GPIO_RxPort, uint16_t GPIO_RxPin)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // ����TX����Ϊ�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_TxPort, &GPIO_InitStructure);

    // ����RX����Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_RxPort, &GPIO_InitStructure);

    // USART��������
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, ENABLE);

}

//�������������ʼ������I2Cģ��
void I2C_Init_Single(I2C_TypeDef* I2Cx, uint32_t ClockSpeed,
                    GPIO_TypeDef* GPIO_SCLPort, uint16_t GPIO_SCLPin,
                    GPIO_TypeDef* GPIO_SDAPort, uint16_t GPIO_SDAPin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    // ����I2C����Ϊ��©���
    GPIO_InitStructure.GPIO_Pin = GPIO_SCLPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_SCLPort, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_SDAPin;
    GPIO_Init(GPIO_SDAPort, &GPIO_InitStructure);

    // I2C��������
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;// ��ģʽ����Ҫ��ַ
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE);
}

// ��ʱ��������ʱ���ܳ�ʼ����TIM1 - TIM10��
void TIM_Base_Init(TIM_TypeDef* TIMx, uint16_t psc, uint32_t arr)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    // ���ö�ʱ����������
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;      // Ԥ��Ƶϵ��
    TIM_TimeBaseInitStructure.TIM_Period = arr;         // �Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ʱ�ӷ�Ƶ,������ʱ�������ó�DIV1���ɣ���Ӱ��ʵ�ʹ���
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);

    TIM_Cmd(TIMx, ENABLE);
}
