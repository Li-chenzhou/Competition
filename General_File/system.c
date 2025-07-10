#include "system.h"

#include "init.h"
#include "interrupt.h"

#include "asr.h"
#include "mpu.h"
#include "rander.h"

#include "OLED.h"

uint8_t show_x[16] = {0};//show��ͷ����������ʾ��OLED��ʾ�����ַ�����
uint8_t show_y[16] = {0};
uint8_t show_speed[16] = {0};
uint8_t show_resolution[16] = {0};

// ��ָ��USART���͵����ֽ�
void USARTx_SendByte(USART_TypeDef *USARTx, uint8_t data)
{
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); // �ȴ����ͻ�������
    USART_SendData(USARTx, data); // ���͵��ֽ�
}

// ��ָ��USART�����ַ�����������ֹ��'\0'ǰ�������ַ���
void USARTx_SendString(USART_TypeDef *USARTx, char *str)
{
    while (*str != '\0')// ѭ������ÿ���ַ���ֱ�������ַ���������'\0'
    {
        USARTx_SendByte(USARTx, (uint8_t)*str); // ���͵�ǰ�ַ�
        str++; // ָ���Ƶ���һ���ַ�
    }
}

//���������ϵͳ�ĳ�ʼ��
void System_Init(void)
{
    RCC_Configuration();

    NVIC_InitTypeDef NVIC_InitStructure = {0};//�½��жϳ�ʼ���ṹ��

    TIM_Base_Init(TIM2, 39, 239);//TIM2�Ļ�����ʱ��ʼ����Ԥ��Ƶϵ��Ϊ39����װ��ֵΪ239��ϵͳ��Ƶ96MHz��������Ƶ������Ϊ100us

    //ʹ��TIM2������ʱ�ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;     // TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     // ʹ��IRQͨ��
    NVIC_Init(&NVIC_InitStructure);

    USART_Init_Single(USART1, 256000, GPIOA, GPIO_Pin_9, GPIOA, GPIO_Pin_10);//USART1,PA9TX,PA10RX,�����״�

    //USART1�Ľ����ж�ʹ��
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Init_Single(USART2, 115200, GPIOA, GPIO_Pin_2, GPIOA, GPIO_Pin_3);//USART2��PA2TX,PA3RX����������ģ��

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

//    USART_Init_Single(USART3, 115200, GPIOB, GPIO_Pin_10, GPIOB, GPIO_Pin_11);//USART3��PB10TX,PB11RX����ʱ�������ں�����չ����
//
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    USART_Init_Single(UART4, 115200, GPIOC, GPIO_Pin_10, GPIOC, GPIO_Pin_11);//UART4��PC10TX,PC11RX,��������ģ��

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Init_Single(UART5, 115200, GPIOC, GPIO_Pin_12, GPIOD, GPIO_Pin_2);//UART5��PC12TX,PD2RX�������ƶ�ͨ��ģ��

//    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
//    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    I2C_Init_Single(I2C1, 100000, GPIOB, GPIO_Pin_6, GPIOB, GPIO_Pin_7);//I2C1������������

    __enable_irq();

    MPU6050_Init();//mpu��ʼ��

    OLED_Clear();
    OLED_Init();

    Warn_Init();//�����������Ƴ�ʼ��

    return ;
}

//������д��ѭ���ǳ�ʼ������
void System_Run(void)
{
    return ;
}

//������д������while(1)��ʵ�ֵĹ���
void System_Loop(void)
{
    parse_frame_data(rec_data,&Frame);//��rec_data�л�ȡ����ĸ��ĸ����ݲ��Ҹ��Խ��д���

    if(detect_flag == 0)//��Ŀ����������ʾ
    {
        snprintf(show_x,sizeof(show_x),"x:%.1fcm        ",Frame.groups[0].x);
        snprintf(show_y,sizeof(show_y),"y:%.1fcm        ",Frame.groups[0].y);
        snprintf(show_speed,sizeof(show_speed),"v:%.1fcm/s       ",Frame.groups[0].speed);
        snprintf(show_resolution,sizeof(show_resolution),"reso:%.1fmm",Frame.groups[0].resolution);
        OLED_ShowString(1, 1, "S");
    }else if(detect_flag == 1)//��Ŀ����ʾ
    {
        snprintf(show_x,sizeof(show_x),"x:%.1f %.1f %.1fcm\0    ",Frame.groups[0].x,Frame.groups[1].x,Frame.groups[2].x);
        snprintf(show_y,sizeof(show_y),"y:%.1f %.1f %.1fcm\0    ",Frame.groups[0].y,Frame.groups[1].y,Frame.groups[2].y);
        snprintf(show_speed,sizeof(show_speed),"v:%.1f %.1f %.1fcm/s\0",Frame.groups[0].speed,Frame.groups[1].speed,Frame.groups[2].speed);
        snprintf(show_resolution,sizeof(show_resolution),"reso:%.1f-%.1f-%.1fmm",Frame.groups[0].resolution,Frame.groups[1].resolution,Frame.groups[2].resolution);
        OLED_ShowString(1, 1, "M");
    }

    OLED_ShowString(2, 1, show_x);
    OLED_ShowString(3, 1, show_y);
    OLED_ShowString(4, 1, show_speed);

    CartesianToPolar(Frame.groups[0].x, Frame.groups[0].y, &angle[0], &distance[0]);
    CartesianToPolar(Frame.groups[1].x, Frame.groups[1].y, &angle[1], &distance[1]);
    CartesianToPolar(Frame.groups[2].x, Frame.groups[2].y, &angle[2], &distance[2]);

    Warn_Check();//�����������Ƽ��

    fallcheck();//�����������Ƽ��

    dataRes();//�յ�����ģ��ָ���������Ӧ����

    return;
}
