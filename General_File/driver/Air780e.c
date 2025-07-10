//Air780e.c
//���ƶ�ͨ��ģ�鷢�������Ϣ����ָ��

#include "system.h"

void message_send (void)
{
    USARTx_SendString(UART5, "AT+CMGF=1\r\n");
    Delay_Ms(10);
    USARTx_SendString(UART5, "AT+CSCS=\"GSM\"\r\n");
    Delay_Ms(10);
    USARTx_SendString(UART5, "AT+CSMP=17,167,0,8\r\n");
    Delay_Ms(10);
    USARTx_SendString(UART5, "AT+CMGS=\"18272077924\"\r\n");
    Delay_Ms(100);

    // ���Ͷ������ݣ����ġ��û�ˤ����Unicode���룬UTF-16��
    uint8_t msg[] = {0x75, 0x28, 0x62, 0x37, 0x64, 0x54, 0x50, 0x12};
    for (int i = 0; i < sizeof(msg); i++) {
        USARTx_SendByte(UART5, msg[i]);
        Delay_Ms(10);
    }
    // ���� Ctrl+Z (������)��16���� 0x1A
    USARTx_SendByte(UART5, 0x1A);
    Delay_Ms(10);
}
