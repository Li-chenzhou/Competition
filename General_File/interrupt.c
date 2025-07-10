#include "interrupt.h"

#include "system.h"

#include "asr.h"
#include "mpu.h"
#include "rander.h"

volatile uint8_t rec_count = 0;//�����״ﴮ�ڽ��յ�����ָ��

volatile uint8_t usart2_cmd_received = 0;//�ñ�����ȡ6��ֵ�����ڶ�ȡ����ģ�鷢�͵��������󣬸��ݸñ���ȡֵ�����ز�ͬ����

//�����յ��״����ݵ��жϴ���
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//��⵽�жϱ�־
    {
        uint8_t data = USART_ReceiveData(USART1);

        // ״̬1���ȴ�֡ͷ
        if (rec_count == 0)
        {
            if (data == 0xAA)//��⵽֡ͷ�ſ�ʼ����
            {
                rec_data[rec_count++] = data;
            }
            // ���Է�֡ͷ����
        }
        // ״̬2������ʣ������
        else if (rec_count < 30)
        {
            rec_data[rec_count++] = data;

            // ������ɼ��
            if (rec_count == 30)
            {
                rec_count = 0;// ���ü����������۳ɹ����
            }
        }
    }

    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

//����1������ģ�鷢������Ҫ�󷵻�3��Ŀ��ľ���Ƕ���Ϣ
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
            default:   usart2_cmd_received = 0; // ������Чֵ
        }

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

//����2���յ�����ģ���ָ����״﷢�͸ı���ģʽ���������4��������źŵ�ȡ��
void UART4_IRQHandler(void)
{
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//��⵽�жϱ�־
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

    // ������ܴ��ڵĴ����־
    if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET) {
        USART_ClearFlag(UART4, USART_FLAG_ORE);
    }
}

volatile uint64_t sys_time = 0;// 100us��ϵͳʱ���

//TIM2�Ļ�����ʱ���ܣ�ʵ���״��л��������ѯ�Լ��Զ���ϵͳʱ���
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // �������жϱ�־
    {
        sys_time ++;

        if (decmode_change_flag == 1)
        {
            change_decmode();//�л����ģʽ
            decmode_change_flag = 0;

            if(detect_flag == 0){
                SendTargetId(12);//�����źŸ�֪�����嵱ǰΪ��Ŀ��
            } else if(detect_flag == 1){
                SendTargetId(13);//�����źŸ�֪�����嵱ǰΪ��Ŀ��
            }
        }
    }

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);// ����жϱ�־
}

//ʹ���Զ���ϵͳ��ʱ���ʵ�ֵķ�������ʱ
void Delay_NonBlocking(uint64_t delay_100us)
{
    uint64_t current = sys_time;

    while (sys_time - current <= delay_100us);

    return ;
}

//����ϵͳʱ������ⲿ�ӿ�
uint64_t get_sys_time(void)
{
    return sys_time;
}
