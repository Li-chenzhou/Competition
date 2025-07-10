//Air780e.c
//向移动通信模块发送求救信息发送指令

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

    // 发送短信内容（中文“用户摔倒”Unicode编码，UTF-16）
    uint8_t msg[] = {0x75, 0x28, 0x62, 0x37, 0x64, 0x54, 0x50, 0x12};
    for (int i = 0; i < sizeof(msg); i++) {
        USARTx_SendByte(UART5, msg[i]);
        Delay_Ms(10);
    }
    // 发送 Ctrl+Z (结束符)，16进制 0x1A
    USARTx_SendByte(UART5, 0x1A);
    Delay_Ms(10);
}
