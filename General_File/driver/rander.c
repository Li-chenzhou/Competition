#include "system.h"
#include "interrupt.h"

#include "rander.h"

#include "math.h"
#include "stdbool.h"

uint8_t decmode_change_flag = 0;//�л����ģʽ�жϱ�־������յ��л�ģʽ��������仯
uint8_t detect_flag = 0;//�״﹤��ģʽ��־��0�ǵ�Ŀ�꣬1�Ƕ�Ŀ��

float distance[3] = {1.0,1.0,1.0};//��ž���Ƕȵ����飬���ڸ�����ģ�鴫������
float angle[3] = {1.0,1.0,1.0};

uint8_t send_dis[20] = {0};//floatת��ΪASCII���ŵ�����
uint8_t send_ang[20] = {0};

uint8_t send_single[12] = {0xFD,0xFC,0xFB,0xFA,0x02,0x00,0x80,0x00,0x04,0x03,0x02,0x01};//����Ϊ��Ŀ��ģʽ�����д�����Ϣ
uint8_t send_multiple[12] = {0xFD,0xFC,0xFB,0xFA,0x02,0x00,0x90,0x00,0x04,0x03,0x02,0x01};//����Ϊ��Ŀ��ģʽ
uint8_t rec_data[30] = {0};//���״�ģ����ܵĴ������ݻ�����
FrameData Frame = {{0,0,0,0},{{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},{0,0}};//����յ������������״���Ϣ�Ľṹ��

//��xy����ת��Ϊ����ͽǶ�ֵ
void CartesianToPolar(float x, float y, float* angle_deg, float* distance)
{
    // �������
    *distance = sqrtf(x * x + y * y);

    // ����Ƕȣ����ȣ�
    float angle_rad = atan2f(y, x);

    // ת��Ϊ�Ƕȣ�0~360�㣩
    *angle_deg = angle_rad * 180.0f / (float)M_PI;
    if (*angle_deg < 0)
    {
        *angle_deg += 360.0f;  // �������Ƕ�
    }
}

//����x��y����
float xy_pro(const uint8_t data[2])
{
    // ��һλ�Ƿ���λ (1Ϊ����0Ϊ��)
    bool is_positive = (data[1] & 0x80) != 0;
    // ��15λ����ֵ�ľ���ֵ(mm)
    uint16_t absolute_mm = ((data[1] & 0x7F) << 8) | data[0];

    // ת��Ϊcm���������ţ�ʹ�ó˷��Ż�
    float value_cm = (float)absolute_mm * 0.1f;
    return is_positive ? value_cm : -value_cm;
}

//�����ٶ�ֵ
float speed_pro(const uint8_t data[2])
{
    // ��һλ�Ƿ���λ (1Ϊ����0Ϊ��)
    bool is_positive = (data[1] & 0x80) != 0;
    // ��15λ����ֵ�ľ���ֵ(cm/s)
    uint16_t absolute = ((data[1] & 0x7F) << 8) | data[0];

    // �������ţ�ʹ�ó˷��Ż�
    return (is_positive ? 1.0f : -1.0f) * (float)absolute;
}

//�������ؾ���ֵ
float dist_pro(const uint8_t data[2])
{
    // ��ȡԭʼ��uint16ֵ�����ף�
    uint16_t mm_value = (data[1] << 8) | data[0];

    // ʹ�ó˷��Ż�
    return (float)mm_value * 1.0f;
}

bool parse_frame_data(const uint8_t raw_data[30], FrameData *frame)
{
    // ���֡ͷ��֡β
    if (raw_data[0] != 0xAA || raw_data[1] != 0xFF ||
        raw_data[2] != 0x03 || raw_data[3] != 0x00)
    {
        return false;
    }

    if (raw_data[28] != 0x55 || raw_data[29] != 0xCC)
    {
        return false;
    }

    // ����֡ͷ
    for (int i = 0; i < FRAME_HEADER_SIZE; i++)
    {
        frame->header[i] = raw_data[i];
    }

    // ����3������
    for (int group = 0; group < DATA_GROUP_COUNT; group++)
    {
        const uint8_t *group_data = &raw_data[FRAME_HEADER_SIZE + group * DATA_GROUP_SIZE];

        // ����x���� (2�ֽ�)
        frame->groups[group].x = xy_pro(&group_data[0]);

        // ����y���� (2�ֽ�)
        frame->groups[group].y = xy_pro(&group_data[2]);

        // �����ٶ� (2�ֽ�)
        frame->groups[group].speed = speed_pro(&group_data[4]);

        // �������ؾ���ֵ (2�ֽڣ��޷���)
        frame->groups[group].resolution = dist_pro(&group_data[6]);
    }

    // ����֡β
    for (int i = 0; i < FRAME_TAIL_SIZE; i++)
    {
        frame->tail[i] = raw_data[FRAME_HEADER_SIZE + DATA_GROUP_SIZE * DATA_GROUP_COUNT + i];
    }

    return true;
}

//�����״﹤��ģʽ��detect_flag��0Ϊ��Ŀ�꣬1Ϊ��Ŀ��
void change_decmode(void)
{
    if (detect_flag)
    {
        for (uint8_t i = 0;i <= 11;i ++)
        {
            USARTx_SendByte(USART1, send_single[i]);
        }
        detect_flag = 0;
    }
    else
    {
        for (uint8_t i = 0;i <= 11;i ++)
        {
            USARTx_SendByte(USART1, send_multiple[i]);
        }
        detect_flag = 1;
    }
    return ;
}
