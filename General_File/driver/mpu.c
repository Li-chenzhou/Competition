#include "stdio.h"
#include "math.h"
#include "mpu.h"
#include "system.h"
#include "init.h"
#include "asr.h"
#define MPU6050_ADDR (0x68 << 1)  // MPU6050 I2C地址，左移1位
// I2C引脚定义（PB6=SCL, PB7=SDA）
#define I2C_SCL_PORT GPIOB
#define I2C_SCL_PIN  GPIO_Pin_6
#define I2C_SDA_PORT GPIOB
#define I2C_SDA_PIN  GPIO_Pin_7
// I2C超时参数
#define I2C_TIMEOUT_MAX 100000  // 超时计数器最大值
#define I2C_ERROR_NONE 0
#define I2C_ERROR_TIMEOUT 1
#define I2C_ERROR_ACK_FAIL 2

int16_t accel[3], gyro[3];//一个角加速度，一个笛卡尔加速度
float acc_mag, gyro_mag;//计算出的总加速度
uint8_t i2c_status;
char buffer[100];

void Delay(volatile uint32_t count)
{
    while(count--) __NOP();
}//I2C总线恢复用的空指令延时

// I2C总线恢复函数
void I2C_RecoverBus(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. 暂时将I2C引脚配置为通用输出模式
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

    // 2. 确保SDA和SCL为高电平
    GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN);
    GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN);
    Delay(50);

    // 3. 如果SDA被设备拉低，发送时钟脉冲直到释放
    if(GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN) == RESET) {
        for(uint8_t i = 0; i < 16; i++) {
            // 拉低SCL
            GPIO_ResetBits(I2C_SCL_PORT, I2C_SCL_PIN);
            Delay(50);
            // 释放SCL
            GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN);
            Delay(50);

            // 检查SDA是否被释放
            if(GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN) != RESET) {
                break;
            }
        }
    }

    // 4. 发送STOP条件：SDA从低到高时SCL为高
    GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN);
    Delay(50);
    GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN);
    Delay(50);
    GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN);
    Delay(50);

    // 5. 恢复GPIO为I2C功能
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  // 复用开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);
}

void fallcheck(void)
{
     i2c_status = MPU6050_ReadAccelGyro(accel, gyro);

     if(i2c_status == I2C_ERROR_NONE) {
         acc_mag = sqrtf(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);
         gyro_mag = sqrtf(gyro[0]*gyro[0] + gyro[1]*gyro[1] + gyro[2]*gyro[2]);
         // 简单摔倒检测
         // 并在触发预警后增加姿态检查
         if ((acc_mag < 2000 || acc_mag > 35000) && gyro_mag > 1500) {
             // 短暂延迟后读取姿态
             Delay_Ms(500);  // 阻塞式延时等待落地稳定进行姿态角判定
             float angle = calculate_angle(); // 通过加速度计算倾角（示例函数）

             if (angle > 20.0f) {  // 身体倾斜超过20度视为摔倒
                 fallsend_pre();
             }
         }
     }
     else{
         // I2C总线恢复
         I2C_RecoverBus();
         // 重新初始化I2C
         I2C_Init_Single(I2C1, 100000, GPIOB, GPIO_Pin_6, GPIOB, GPIO_Pin_7);//初始化mpu用的I2C1
         MPU6050_Init();
     }

     fall_send();//时间戳&摔倒标志判定是否发送摔倒警报信号
}

// 计算设备与垂直方向的夹角（单位：度）
float calculate_angle(void)
{
    int16_t accel[3];
    float acc_mag;

    // 重新读取加速度计数据（确保使用最新数据）
    if(MPU6050_ReadAccelGyro(accel, gyro) == I2C_ERROR_NONE)
    {
        // 计算加速度矢量模长
        acc_mag = sqrtf(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);

        // 避免除以零
        if(acc_mag < 1) return 0.0f;

        // 计算Z轴与重力方向的夹角（最简方法）
        // 假设Z轴垂直时accel[2] = -16384（±2g量程）
        float cos_angle = -accel[2] / acc_mag;  // 负号因为重力向下

        // 限制值域防止数值错误
        if(cos_angle > 1.0f) cos_angle = 1.0f;
        if(cos_angle < -1.0f) cos_angle = -1.0f;

        // 将弧度转换为角度
        return acosf(cos_angle) * (180.0f / M_PI);
    }
    return 0.0f;  // 读取失败返回0
}



float Accel_Magnitude(int16_t *accel)//计算旋转角加速度
{
    return sqrtf(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);
}

float Gyro_Magnitude(int16_t *gyro)//
{
    return sqrtf(gyro[0]*gyro[0] + gyro[1]*gyro[1] + gyro[2]*gyro[2]);
}

// 自定义写寄存器函数
uint8_t MyI2C_WriteRegister(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    uint32_t timeout = I2C_TIMEOUT_MAX;

    // 1. 检查总线忙标志
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {
        if(timeout-- == 0) return I2C_ERROR_TIMEOUT;
    }

    // 2. 发送START条件
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 3. 发送设备地址（写模式）
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_AF)) {
            I2C_ClearFlag(I2C1, I2C_FLAG_AF);
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_ACK_FAIL;
        }
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 4. 发送寄存器地址
    I2C_SendData(I2C1, regAddr);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 5. 发送数据
    I2C_SendData(I2C1, data);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 6. 发送STOP条件
    I2C_GenerateSTOP(I2C1, ENABLE);

    return I2C_ERROR_NONE;
}

uint8_t MyI2C_ReadRegister(uint8_t devAddr, uint8_t regAddr, uint8_t* data)
{
    uint32_t timeout = I2C_TIMEOUT_MAX;

    // 1. 检查总线忙标志
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {
        if(timeout-- == 0) return I2C_ERROR_TIMEOUT;
    }

    // 2. 发送START条件
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 3. 发送设备地址（写模式）
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_AF)) {
            I2C_ClearFlag(I2C1, I2C_FLAG_AF);
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_ACK_FAIL;
        }
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 4. 发送寄存器地址
    I2C_SendData(I2C1, regAddr);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 5. 发送重复START条件
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 6. 发送设备地址（读模式）
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Receiver);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_AF)) {
            I2C_ClearFlag(I2C1, I2C_FLAG_AF);
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_ACK_FAIL;
        }
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 7. 准备接收数据（单字节）
    I2C_AcknowledgeConfig(I2C1, DISABLE);  // 最后一个字节前发送NACK
    I2C_GenerateSTOP(I2C1, ENABLE);        // 准备STOP条件

    // 8. 等待数据接收完成
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 9. 读取数据
    *data = I2C_ReceiveData(I2C1);

    return I2C_ERROR_NONE;
}

// MPU6050初始化，退出睡眠模式
void MPU6050_Init(void)
{
    MyI2C_WriteRegister(MPU6050_ADDR, 0x6B, 0x00);  // PWR_MGMT_1寄存器，清除睡眠位
}

// 读取MPU6050加速度和陀螺仪数据
uint8_t MPU6050_ReadAccelGyro(int16_t *accel, int16_t *gyro)
{
    uint8_t buf[14];
    uint32_t timeout;
    //uint8_t status;

    // 1. 检查总线忙标志
    timeout = I2C_TIMEOUT_MAX;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {
        if(timeout-- == 0) return I2C_ERROR_TIMEOUT;
    }

    // 2. 发送START条件
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 3. 发送设备地址（写模式）
    I2C_Send7bitAddress(I2C1, MPU6050_ADDR, I2C_Direction_Transmitter);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_AF)) {
            I2C_ClearFlag(I2C1, I2C_FLAG_AF);
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_ACK_FAIL;
        }
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 4. 发送寄存器地址（加速度数据起始寄存器0x3B）
    I2C_SendData(I2C1, 0x3B);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 5. 发送重复START条件
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 6. 发送设备地址（读模式）
    I2C_Send7bitAddress(I2C1, MPU6050_ADDR, I2C_Direction_Receiver);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_AF)) {
            I2C_ClearFlag(I2C1, I2C_FLAG_AF);
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_ACK_FAIL;
        }
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 7. 准备接收14字节数据
    I2C_AcknowledgeConfig(I2C1, ENABLE);  // 启用ACK

    // 接收前13个字节
    for(uint8_t i = 0; i < 13; i++)
    {
        timeout = I2C_TIMEOUT_MAX;
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
            if(timeout-- == 0) {
                I2C_GenerateSTOP(I2C1, ENABLE);
                return I2C_ERROR_TIMEOUT;
            }
        }
        buf[i] = I2C_ReceiveData(I2C1);

        // 最后一个字节前禁用ACK
        if(i == 12) {
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
    }

    // 8. 接收最后一个字节
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }
    buf[13] = I2C_ReceiveData(I2C1);

    // 9. 合成16位数据
    accel[0] = (buf[0] << 8) | buf[1];
    accel[1] = (buf[2] << 8) | buf[3];
    accel[2] = (buf[4] << 8) | buf[5];

    gyro[0] = (buf[8] << 8) | buf[9];
    gyro[1] = (buf[10] << 8) | buf[11];
    gyro[2] = (buf[12] << 8) | buf[13];

    return I2C_ERROR_NONE;
}
