/*
 * asr.c
 *
 *  Created on: 2025年7月1日
 *      Author: Rock
 */

#include "system.h"
#include "interrupt.h"

#include "Air780e.h"
#include "asr.h"
#include "rander.h"

#include "math.h"
#include "stdio.h"

uint8_t fall_flag = 0;//摔倒预警标识
volatile uint64_t last_time = 0;//用于周期性执行目标速度判定的时间戳
volatile uint64_t fall_time = 0;//记录摔倒预警首次触发的摔倒时间戳

void float_to_bytes(float f, uint8_t *bytes, uint8_t max_len)//将浮点数转换为ASCII码
{
    char temp_str[16];
    snprintf(temp_str, sizeof(temp_str), "%.1f", f);
    uint8_t i = 0;
    while (temp_str[i] != '\0' && i < max_len - 1)
    {
        bytes[i] = (uint8_t)temp_str[i];
        i++;
    }
    bytes[i] = 0x00;
}

//核心板通过USART2中断判断需要的是什么数据
void dataRes(void)
{
    switch(usart2_cmd_received)
    {
        case 1://目标1距离
        {
            float_to_bytes(distance[0],send_dis,20);
            float_to_bytes(angle[0],send_ang,20);
            usart2_cmd_received = 0;
            disSend();
            break;
        }
        case 2://目标1角度
        {
            usart2_cmd_received = 0;
            angSend();
            break;
        }
        case 3://目标2距离
        {
             float_to_bytes(distance[1],send_dis,20);//
             float_to_bytes(angle[1],send_ang,20);
             usart2_cmd_received = 0;
             disSend();
             break;
        }
        case 4://目标2角度
        {
             usart2_cmd_received = 0;
             angSend();
             break;
        }
        case 5://目标3距离
        {
             float_to_bytes(distance[2],send_dis,20);//
             float_to_bytes(angle[2],send_ang,20);
             usart2_cmd_received = 0;
             disSend();
             break;
        }
        case 6://目标3角度
        {
             usart2_cmd_received = 0;
             angSend();
             break;
        }
    }
}

//USART2发送角度
void angSend(void)
{
    for (int i =0;i <= 19;i ++)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // 等待DR寄存器空
        USART_SendData(USART2, send_ang[i]);
        if (send_ang[i]=='\0')
        break;
    }
}

void disSend(void)//USART2发送距离
{
    for (int i =0;i <= 19;i ++)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // 等待DR寄存器空
        USART_SendData(USART2, send_dis[i]);
        if (send_dis[i]=='\0')
        break;
    }
}

// 默认报警阈值配置
AlarmConfig alarm_cfg =
{
        .speed_threshold = 110.0f,//速度阈值110 cm/s
        .debounce_count = 3      // 0.15秒防抖窗口 (0.05s*3，取样间隔0.05见fall_check函数）
};

// 三个目标的报警状态，默认为正常状态，超速计数器0
TargetAlarm target_alarms[3] =
{
    {ALARM_OFF, 0},
    {ALARM_OFF, 0},
    {ALARM_OFF, 0}
};

// UART4以字符串形式向语音模块发送警报信号target_id，1-7为7种目标超速组合；8为摔倒预警，9为取消摔倒求救的反馈，10为摔倒求救短信发送的反馈；11为核心板上电初始化完成讯号；12、13为检测模式反馈信号。
void SendTargetId(uint8_t target_id)
{
    char buffer[5]; // 缓冲区：3位数字  + '\0' = 5字节

    // 1. 数字转字符串（含换行符）
    snprintf(buffer, sizeof(buffer), "%d", target_id);  // 格式化为 "数值\n"

    // 2. 通过 UART4 发送字符串
    char *p = buffer;
    while (*p != '\0')// 遍历直到字符串结束符
    {
        USARTx_SendByte(UART4, *p);  // 逐字节发送
        p++;
    }
    USARTx_SendByte(UART4, '\0');
}

// 初始化报警系统(重置）
void Warn_Init(void)
{
    // 初始化所有目标状态
    for (int i = 0; i < 3; i++)
    {
        target_alarms[i].state = ALARM_OFF;
        target_alarms[i].counter = 0;
    }
}

// 报警检测函数 (0.05s一次）
void Warn_Check(void)
{
    if(get_sys_time() - last_time < 500)
    {
        return;
    }
    last_time = get_sys_time();

    uint8_t target_num = detect_flag ? 3 : 1;  // 根据模式确定目标数量
    uint8_t alarm_states = 0;  // 存储报警状态位图

    // 1. 更新所有目标状态
    for (int i = 0; i < target_num; i++)
    {
        float speed = Frame.groups[i].speed;//获取速度
        float threshold = alarm_cfg.speed_threshold;  // 使用预定义阈值

        switch (target_alarms[i].state)
        {
            case ALARM_OFF:
                if (speed < -threshold)
                {  // 目标超速//speed > threshold || speed < -threshold
                    target_alarms[i].state = ALARM_PENDING; //进入预警状态，超速计数器置一
                    target_alarms[i].counter = 1;
                }
                break;
            case ALARM_PENDING:
                if (speed < -threshold)
                {  // 目标超速//speed > threshold || speed < -threshold
                    if (++target_alarms[i].counter >= alarm_cfg.debounce_count)
                    {
                        target_alarms[i].state = ALARM_ON;//超速计数器到达设定次数，进入报警状态
                    }
                }
                else
                {
                    if (target_alarms[i].counter > 0)
                    {
                        target_alarms[i].counter = 0;
                        target_alarms[i].state = ALARM_OFF;//预警状态下测得未超速，计数器置0，返回正常状态
                    }
                }
                break;
            case ALARM_ON:
                if (speed >= -threshold)
                {
                    target_alarms[i].counter = 0;
                    target_alarms[i].state = ALARM_OFF;//报警状态下测得未超速，计数器置0，返回正常状态
                }
                else
                {
                    target_alarms[i].counter = alarm_cfg.debounce_count;//超速，状态和计数器不变
                }
                break;
        }
        // 记录报警状态(bit0-目标1, bit1-目标2, bit2-目标3)，将超速目标对应位置1
        if (target_alarms[i].state == ALARM_ON) {
            alarm_states |= (1 << i);
        }

    }

    // 2. 根据报警状态组合发送对应ID
    uint8_t send_id = 0;

    // 单目标模式只会发送目标1
    if (target_num == 1)
    {
        if (alarm_states & 0x01)
        {
            send_id = 1;
        }
    }

    else // 三目标模式
    {
        switch (alarm_states)
        {
            case 0x01: // 001仅目标1超速
                send_id = 1;
                break;
            case 0x02: // 010仅目标2超速
                send_id = 2;
                break;
            case 0x04: // 100仅目标3超速
                send_id = 3;
                break;
            case 0x03: // 011目标1+2超速(目标3未超速)
                send_id = 6;
                break;
            case 0x05: // 101目标1+3超速(目标2未超速)
                send_id = 5;
                break;
            case 0x06: // 110目标2+3超速(目标1未超速)
                send_id = 4;
                break;
            case 0x07: // 111全部超速
                send_id = 7;
                break;
        }
    }

    // 3. 发送报警ID
    if (send_id != 0)
    {
        SendTargetId(send_id);
    }
    //鉴于语音模块默认接收空值0，此处id是否为0的判定可以去除
}

//串口测试函数
void serialtest(void)
{
    if(get_sys_time() - last_time < 20000)
    {
            return;
    }
    last_time = get_sys_time();
    SendTargetId(1);
}

//摔倒警报发送函数，仅在摔倒预警触发10s且未被取消时触发
void fall_send(void)
{
    //原定执行时间差为10s，即理论时间戳相差100000，检测时大约实际时间戳插值应在10000左右。考虑播报时间，增添2000.
    if(fall_flag == 1 && (get_sys_time() - fall_time > 160000) )
    {
        SendTargetId(9);//向语音版发送摔倒信号
        fall_flag = 0;
        fall_time = 0;//重置摔倒时间和摔倒标识
        //短信模块指令函数
        message_send();
    }
    else
    {
        return;
    }

}

//摔倒预警函数
void fallsend_pre(void)
{
    if(fall_flag == 0)//仅首次触发摔倒预警时触发摔倒时间更新
    {
        fall_time = get_sys_time();
    }
    SendTargetId(8);//向语音版发送摔倒预警信号
    fall_flag = 1;//摔倒标识置一
}

//摔倒警报取消函数
void fallsend_cancel(void)
{
    fall_flag = 0;
    fall_time = 0;//重置摔倒时间和摔倒标识
    SendTargetId(10);//向语音版发送摔倒警报取消信号

}
