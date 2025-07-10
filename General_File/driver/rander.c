#include "system.h"
#include "interrupt.h"

#include "rander.h"

#include "math.h"
#include "stdbool.h"

uint8_t decmode_change_flag = 0;//切换检测模式中断标志，如果收到切换模式的命令则变化
uint8_t detect_flag = 0;//雷达工作模式标志，0是单目标，1是多目标

float distance[3] = {1.0,1.0,1.0};//存放距离角度的数组，用于给语音模块传输数据
float angle[3] = {1.0,1.0,1.0};

uint8_t send_dis[20] = {0};//float转化为ASCII码存放的数组
uint8_t send_ang[20] = {0};

uint8_t send_single[12] = {0xFD,0xFC,0xFB,0xFA,0x02,0x00,0x80,0x00,0x04,0x03,0x02,0x01};//设置为单目标模式的下行串口信息
uint8_t send_multiple[12] = {0xFD,0xFC,0xFB,0xFA,0x02,0x00,0x90,0x00,0x04,0x03,0x02,0x01};//设置为多目标模式
uint8_t rec_data[30] = {0};//从雷达模块接受的串口数据缓冲区
FrameData Frame = {{0,0,0,0},{{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},{0,0}};//存放收到并处理过后的雷达信息的结构体

//将xy数据转化为距离和角度值
void CartesianToPolar(float x, float y, float* angle_deg, float* distance)
{
    // 计算距离
    *distance = sqrtf(x * x + y * y);

    // 计算角度（弧度）
    float angle_rad = atan2f(y, x);

    // 转换为角度（0~360°）
    *angle_deg = angle_rad * 180.0f / (float)M_PI;
    if (*angle_deg < 0)
    {
        *angle_deg += 360.0f;  // 修正负角度
    }
}

//处理x，y坐标
float xy_pro(const uint8_t data[2])
{
    // 第一位是符号位 (1为正，0为负)
    bool is_positive = (data[1] & 0x80) != 0;
    // 后15位是数值的绝对值(mm)
    uint16_t absolute_mm = ((data[1] & 0x7F) << 8) | data[0];

    // 转换为cm并保留符号，使用乘法优化
    float value_cm = (float)absolute_mm * 0.1f;
    return is_positive ? value_cm : -value_cm;
}

//处理速度值
float speed_pro(const uint8_t data[2])
{
    // 第一位是符号位 (1为正，0为负)
    bool is_positive = (data[1] & 0x80) != 0;
    // 后15位是数值的绝对值(cm/s)
    uint16_t absolute = ((data[1] & 0x7F) << 8) | data[0];

    // 保留符号，使用乘法优化
    return (is_positive ? 1.0f : -1.0f) * (float)absolute;
}

//处理像素距离值
float dist_pro(const uint8_t data[2])
{
    // 获取原始的uint16值（毫米）
    uint16_t mm_value = (data[1] << 8) | data[0];

    // 使用乘法优化
    return (float)mm_value * 1.0f;
}

bool parse_frame_data(const uint8_t raw_data[30], FrameData *frame)
{
    // 检查帧头和帧尾
    if (raw_data[0] != 0xAA || raw_data[1] != 0xFF ||
        raw_data[2] != 0x03 || raw_data[3] != 0x00)
    {
        return false;
    }

    if (raw_data[28] != 0x55 || raw_data[29] != 0xCC)
    {
        return false;
    }

    // 拷贝帧头
    for (int i = 0; i < FRAME_HEADER_SIZE; i++)
    {
        frame->header[i] = raw_data[i];
    }

    // 解析3组数据
    for (int group = 0; group < DATA_GROUP_COUNT; group++)
    {
        const uint8_t *group_data = &raw_data[FRAME_HEADER_SIZE + group * DATA_GROUP_SIZE];

        // 解析x坐标 (2字节)
        frame->groups[group].x = xy_pro(&group_data[0]);

        // 解析y坐标 (2字节)
        frame->groups[group].y = xy_pro(&group_data[2]);

        // 解析速度 (2字节)
        frame->groups[group].speed = speed_pro(&group_data[4]);

        // 解析像素距离值 (2字节，无符号)
        frame->groups[group].resolution = dist_pro(&group_data[6]);
    }

    // 拷贝帧尾
    for (int i = 0; i < FRAME_TAIL_SIZE; i++)
    {
        frame->tail[i] = raw_data[FRAME_HEADER_SIZE + DATA_GROUP_SIZE * DATA_GROUP_COUNT + i];
    }

    return true;
}

//调整雷达工作模式，detect_flag是0为单目标，1为多目标
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
