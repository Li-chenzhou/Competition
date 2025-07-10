/*
 * asr.c
 *
 *  Created on: 2025��7��1��
 *      Author: Rock
 */

#include "system.h"
#include "interrupt.h"

#include "Air780e.h"
#include "asr.h"
#include "rander.h"

#include "math.h"
#include "stdio.h"

uint8_t fall_flag = 0;//ˤ��Ԥ����ʶ
volatile uint64_t last_time = 0;//����������ִ��Ŀ���ٶ��ж���ʱ���
volatile uint64_t fall_time = 0;//��¼ˤ��Ԥ���״δ�����ˤ��ʱ���

void float_to_bytes(float f, uint8_t *bytes, uint8_t max_len)//��������ת��ΪASCII��
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

//���İ�ͨ��USART2�ж��ж���Ҫ����ʲô����
void dataRes(void)
{
    switch(usart2_cmd_received)
    {
        case 1://Ŀ��1����
        {
            float_to_bytes(distance[0],send_dis,20);
            float_to_bytes(angle[0],send_ang,20);
            usart2_cmd_received = 0;
            disSend();
            break;
        }
        case 2://Ŀ��1�Ƕ�
        {
            usart2_cmd_received = 0;
            angSend();
            break;
        }
        case 3://Ŀ��2����
        {
             float_to_bytes(distance[1],send_dis,20);//
             float_to_bytes(angle[1],send_ang,20);
             usart2_cmd_received = 0;
             disSend();
             break;
        }
        case 4://Ŀ��2�Ƕ�
        {
             usart2_cmd_received = 0;
             angSend();
             break;
        }
        case 5://Ŀ��3����
        {
             float_to_bytes(distance[2],send_dis,20);//
             float_to_bytes(angle[2],send_ang,20);
             usart2_cmd_received = 0;
             disSend();
             break;
        }
        case 6://Ŀ��3�Ƕ�
        {
             usart2_cmd_received = 0;
             angSend();
             break;
        }
    }
}

//USART2���ͽǶ�
void angSend(void)
{
    for (int i =0;i <= 19;i ++)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // �ȴ�DR�Ĵ�����
        USART_SendData(USART2, send_ang[i]);
        if (send_ang[i]=='\0')
        break;
    }
}

void disSend(void)//USART2���;���
{
    for (int i =0;i <= 19;i ++)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // �ȴ�DR�Ĵ�����
        USART_SendData(USART2, send_dis[i]);
        if (send_dis[i]=='\0')
        break;
    }
}

// Ĭ�ϱ�����ֵ����
AlarmConfig alarm_cfg =
{
        .speed_threshold = 110.0f,//�ٶ���ֵ110 cm/s
        .debounce_count = 3      // 0.15��������� (0.05s*3��ȡ�����0.05��fall_check������
};

// ����Ŀ��ı���״̬��Ĭ��Ϊ����״̬�����ټ�����0
TargetAlarm target_alarms[3] =
{
    {ALARM_OFF, 0},
    {ALARM_OFF, 0},
    {ALARM_OFF, 0}
};

// UART4���ַ�����ʽ������ģ�鷢�;����ź�target_id��1-7Ϊ7��Ŀ�곬����ϣ�8Ϊˤ��Ԥ����9Ϊȡ��ˤ����ȵķ�����10Ϊˤ����ȶ��ŷ��͵ķ�����11Ϊ���İ��ϵ��ʼ�����Ѷ�ţ�12��13Ϊ���ģʽ�����źš�
void SendTargetId(uint8_t target_id)
{
    char buffer[5]; // ��������3λ����  + '\0' = 5�ֽ�

    // 1. ����ת�ַ����������з���
    snprintf(buffer, sizeof(buffer), "%d", target_id);  // ��ʽ��Ϊ "��ֵ\n"

    // 2. ͨ�� UART4 �����ַ���
    char *p = buffer;
    while (*p != '\0')// ����ֱ���ַ���������
    {
        USARTx_SendByte(UART4, *p);  // ���ֽڷ���
        p++;
    }
    USARTx_SendByte(UART4, '\0');
}

// ��ʼ������ϵͳ(���ã�
void Warn_Init(void)
{
    // ��ʼ������Ŀ��״̬
    for (int i = 0; i < 3; i++)
    {
        target_alarms[i].state = ALARM_OFF;
        target_alarms[i].counter = 0;
    }
}

// ������⺯�� (0.05sһ�Σ�
void Warn_Check(void)
{
    if(get_sys_time() - last_time < 500)
    {
        return;
    }
    last_time = get_sys_time();

    uint8_t target_num = detect_flag ? 3 : 1;  // ����ģʽȷ��Ŀ������
    uint8_t alarm_states = 0;  // �洢����״̬λͼ

    // 1. ��������Ŀ��״̬
    for (int i = 0; i < target_num; i++)
    {
        float speed = Frame.groups[i].speed;//��ȡ�ٶ�
        float threshold = alarm_cfg.speed_threshold;  // ʹ��Ԥ������ֵ

        switch (target_alarms[i].state)
        {
            case ALARM_OFF:
                if (speed < -threshold)
                {  // Ŀ�곬��//speed > threshold || speed < -threshold
                    target_alarms[i].state = ALARM_PENDING; //����Ԥ��״̬�����ټ�������һ
                    target_alarms[i].counter = 1;
                }
                break;
            case ALARM_PENDING:
                if (speed < -threshold)
                {  // Ŀ�곬��//speed > threshold || speed < -threshold
                    if (++target_alarms[i].counter >= alarm_cfg.debounce_count)
                    {
                        target_alarms[i].state = ALARM_ON;//���ټ����������趨���������뱨��״̬
                    }
                }
                else
                {
                    if (target_alarms[i].counter > 0)
                    {
                        target_alarms[i].counter = 0;
                        target_alarms[i].state = ALARM_OFF;//Ԥ��״̬�²��δ���٣���������0����������״̬
                    }
                }
                break;
            case ALARM_ON:
                if (speed >= -threshold)
                {
                    target_alarms[i].counter = 0;
                    target_alarms[i].state = ALARM_OFF;//����״̬�²��δ���٣���������0����������״̬
                }
                else
                {
                    target_alarms[i].counter = alarm_cfg.debounce_count;//���٣�״̬�ͼ���������
                }
                break;
        }
        // ��¼����״̬(bit0-Ŀ��1, bit1-Ŀ��2, bit2-Ŀ��3)��������Ŀ���Ӧλ��1
        if (target_alarms[i].state == ALARM_ON) {
            alarm_states |= (1 << i);
        }

    }

    // 2. ���ݱ���״̬��Ϸ��Ͷ�ӦID
    uint8_t send_id = 0;

    // ��Ŀ��ģʽֻ�ᷢ��Ŀ��1
    if (target_num == 1)
    {
        if (alarm_states & 0x01)
        {
            send_id = 1;
        }
    }

    else // ��Ŀ��ģʽ
    {
        switch (alarm_states)
        {
            case 0x01: // 001��Ŀ��1����
                send_id = 1;
                break;
            case 0x02: // 010��Ŀ��2����
                send_id = 2;
                break;
            case 0x04: // 100��Ŀ��3����
                send_id = 3;
                break;
            case 0x03: // 011Ŀ��1+2����(Ŀ��3δ����)
                send_id = 6;
                break;
            case 0x05: // 101Ŀ��1+3����(Ŀ��2δ����)
                send_id = 5;
                break;
            case 0x06: // 110Ŀ��2+3����(Ŀ��1δ����)
                send_id = 4;
                break;
            case 0x07: // 111ȫ������
                send_id = 7;
                break;
        }
    }

    // 3. ���ͱ���ID
    if (send_id != 0)
    {
        SendTargetId(send_id);
    }
    //��������ģ��Ĭ�Ͻ��տ�ֵ0���˴�id�Ƿ�Ϊ0���ж�����ȥ��
}

//���ڲ��Ժ���
void serialtest(void)
{
    if(get_sys_time() - last_time < 20000)
    {
            return;
    }
    last_time = get_sys_time();
    SendTargetId(1);
}

//ˤ���������ͺ���������ˤ��Ԥ������10s��δ��ȡ��ʱ����
void fall_send(void)
{
    //ԭ��ִ��ʱ���Ϊ10s��������ʱ������100000�����ʱ��Լʵ��ʱ�����ֵӦ��10000���ҡ����ǲ���ʱ�䣬����2000.
    if(fall_flag == 1 && (get_sys_time() - fall_time > 160000) )
    {
        SendTargetId(9);//�������淢��ˤ���ź�
        fall_flag = 0;
        fall_time = 0;//����ˤ��ʱ���ˤ����ʶ
        //����ģ��ָ���
        message_send();
    }
    else
    {
        return;
    }

}

//ˤ��Ԥ������
void fallsend_pre(void)
{
    if(fall_flag == 0)//���״δ���ˤ��Ԥ��ʱ����ˤ��ʱ�����
    {
        fall_time = get_sys_time();
    }
    SendTargetId(8);//�������淢��ˤ��Ԥ���ź�
    fall_flag = 1;//ˤ����ʶ��һ
}

//ˤ������ȡ������
void fallsend_cancel(void)
{
    fall_flag = 0;
    fall_time = 0;//����ˤ��ʱ���ˤ����ʶ
    SendTargetId(10);//�������淢��ˤ������ȡ���ź�

}
