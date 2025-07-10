#include "system.h"

#include "init.h"
#include "interrupt.h"

#include "asr.h"
#include "mpu.h"
#include "rander.h"

#include "OLED.h"

uint8_t show_x[16] = {0};//show开头的是用于显示到OLED显示屏的字符数组
uint8_t show_y[16] = {0};
uint8_t show_speed[16] = {0};
uint8_t show_resolution[16] = {0};

// 向指定USART发送单个字节
void USARTx_SendByte(USART_TypeDef *USARTx, uint8_t data)
{
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); // 等待发送缓冲区空
    USART_SendData(USARTx, data); // 发送单字节
}

// 向指定USART发送字符串（包含终止符'\0'前的所有字符）
void USARTx_SendString(USART_TypeDef *USARTx, char *str)
{
    while (*str != '\0')// 循环发送每个字符，直到遇到字符串结束符'\0'
    {
        USARTx_SendByte(USARTx, (uint8_t)*str); // 发送当前字符
        str++; // 指针移到下一个字符
    }
}

//在这里进行系统的初始化
void System_Init(void)
{
    RCC_Configuration();

    NVIC_InitTypeDef NVIC_InitStructure = {0};//新建中断初始化结构体

    TIM_Base_Init(TIM2, 39, 239);//TIM2的基本计时初始化，预分频系数为39，重装载值为239，系统主频96MHz，经过分频后周期为100us

    //使能TIM2基本定时中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;     // TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     // 使能IRQ通道
    NVIC_Init(&NVIC_InitStructure);

    USART_Init_Single(USART1, 256000, GPIOA, GPIO_Pin_9, GPIOA, GPIO_Pin_10);//USART1,PA9TX,PA10RX,用于雷达

    //USART1的接收中断使能
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Init_Single(USART2, 115200, GPIOA, GPIO_Pin_2, GPIOA, GPIO_Pin_3);//USART2，PA2TX,PA3RX，用于语音模块

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

//    USART_Init_Single(USART3, 115200, GPIOB, GPIO_Pin_10, GPIOB, GPIO_Pin_11);//USART3，PB10TX,PB11RX，暂时闲置用于后续拓展功能
//
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    USART_Init_Single(UART4, 115200, GPIOC, GPIO_Pin_10, GPIOC, GPIO_Pin_11);//UART4，PC10TX,PC11RX,用于语音模块

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Init_Single(UART5, 115200, GPIOC, GPIO_Pin_12, GPIOD, GPIO_Pin_2);//UART5，PC12TX,PD2RX，用于移动通信模块

//    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
//    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    I2C_Init_Single(I2C1, 100000, GPIOB, GPIO_Pin_6, GPIOB, GPIO_Pin_7);//I2C1，用于陀螺仪

    __enable_irq();

    MPU6050_Init();//mpu初始化

    OLED_Clear();
    OLED_Init();

    Warn_Init();//主动报警机制初始化

    return ;
}

//在这里写非循环非初始化内容
void System_Run(void)
{
    return ;
}

//在这里写主函数while(1)中实现的功能
void System_Loop(void)
{
    parse_frame_data(rec_data,&Frame);//从rec_data中获取三组的各四个数据并且各自进行处理

    if(detect_flag == 0)//单目标检测数据显示
    {
        snprintf(show_x,sizeof(show_x),"x:%.1fcm        ",Frame.groups[0].x);
        snprintf(show_y,sizeof(show_y),"y:%.1fcm        ",Frame.groups[0].y);
        snprintf(show_speed,sizeof(show_speed),"v:%.1fcm/s       ",Frame.groups[0].speed);
        snprintf(show_resolution,sizeof(show_resolution),"reso:%.1fmm",Frame.groups[0].resolution);
        OLED_ShowString(1, 1, "S");
    }else if(detect_flag == 1)//多目标显示
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

    Warn_Check();//主动报警机制检测

    fallcheck();//跌倒报警机制检测

    dataRes();//收到语音模块指令的数据响应函数

    return;
}
