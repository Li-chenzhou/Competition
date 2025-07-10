#include "interrupt.h"

#include "system.h"

#include "asr.h"
#include "mpu.h"
#include "rander.h"

volatile uint8_t rec_count = 0;//用于雷达串口接收的数据指针

volatile uint8_t usart2_cmd_received = 0;//该变量可取6个值，用于读取语音模块发送的数据请求，根据该变量取值来返回不同数据

//串口收到雷达数据的中断处理
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//检测到中断标志
    {
        uint8_t data = USART_ReceiveData(USART1);

        // 状态1：等待帧头
        if (rec_count == 0)
        {
            if (data == 0xAA)//检测到帧头才开始接收
            {
                rec_data[rec_count++] = data;
            }
            // 忽略非帧头数据
        }
        // 状态2：接收剩余数据
        else if (rec_count < 30)
        {
            rec_data[rec_count++] = data;

            // 接收完成检查
            if (rec_count == 30)
            {
                rec_count = 0;// 重置计数器（无论成功与否）
            }
        }
    }

    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

//功能1，语音模块发送请求，要求返回3个目标的距离角度信息
void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        uint8_t recv = USART_ReceiveData(USART2);

        switch(recv)
        {
            case 0x01:  usart2_cmd_received = 1;break;
            case 0x02:  usart2_cmd_received = 2;break;
            case 0x03:  usart2_cmd_received = 3;break;
            case 0x04:  usart2_cmd_received = 4;break;
            case 0x05:  usart2_cmd_received = 5;break;
            case 0x06:  usart2_cmd_received = 6;break;
            default:   usart2_cmd_received = 0; // 处理无效值
        }

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

//功能2，收到语音模块的指令，向雷达发送改变检测模式的命令；功能4跌倒求救信号的取消
void UART4_IRQHandler(void)
{
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//检测到中断标志
    {
        uint8_t recv = USART_ReceiveData(UART4);

        if (recv == 0x01)
        {
            __disable_irq();
            decmode_change_flag = 1;
            __enable_irq();
        }
        else if(recv == 0x02)
        {
            fallsend_cancel();
        }

        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }

    // 清除可能存在的错误标志
    if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET) {
        USART_ClearFlag(UART4, USART_FLAG_ORE);
    }
}

volatile uint64_t sys_time = 0;// 100us级系统时间戳

//TIM2的基本定时功能，实现雷达切换命令的轮询以及自定义系统时间戳
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // 检查更新中断标志
    {
        sys_time ++;

        if (decmode_change_flag == 1)
        {
            change_decmode();//切换检测模式
            decmode_change_flag = 0;

            if(detect_flag == 0){
                SendTargetId(12);//发送信号告知语音板当前为单目标
            } else if(detect_flag == 1){
                SendTargetId(13);//发送信号告知语音板当前为多目标
            }
        }
    }

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);// 清除中断标志
}

//使用自定义系统级时间戳实现的非阻塞延时
void Delay_NonBlocking(uint64_t delay_100us)
{
    uint64_t current = sys_time;

    while (sys_time - current <= delay_100us);

    return ;
}

//保留系统时间戳的外部接口
uint64_t get_sys_time(void)
{
    return sys_time;
}
