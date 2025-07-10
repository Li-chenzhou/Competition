#include "stdio.h"
#include "math.h"
#include "mpu.h"
#include "system.h"
#include "init.h"
#include "asr.h"
#define MPU6050_ADDR (0x68 << 1)  // MPU6050 I2C��ַ������1λ
// I2C���Ŷ��壨PB6=SCL, PB7=SDA��
#define I2C_SCL_PORT GPIOB
#define I2C_SCL_PIN  GPIO_Pin_6
#define I2C_SDA_PORT GPIOB
#define I2C_SDA_PIN  GPIO_Pin_7
// I2C��ʱ����
#define I2C_TIMEOUT_MAX 100000  // ��ʱ���������ֵ
#define I2C_ERROR_NONE 0
#define I2C_ERROR_TIMEOUT 1
#define I2C_ERROR_ACK_FAIL 2

int16_t accel[3], gyro[3];//һ���Ǽ��ٶȣ�һ���ѿ������ٶ�
float acc_mag, gyro_mag;//��������ܼ��ٶ�
uint8_t i2c_status;
char buffer[100];

void Delay(volatile uint32_t count)
{
    while(count--) __NOP();
}//I2C���߻ָ��õĿ�ָ����ʱ

// I2C���߻ָ�����
void I2C_RecoverBus(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. ��ʱ��I2C��������Ϊͨ�����ģʽ
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // ��©���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

    // 2. ȷ��SDA��SCLΪ�ߵ�ƽ
    GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN);
    GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN);
    Delay(50);

    // 3. ���SDA���豸���ͣ�����ʱ������ֱ���ͷ�
    if(GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN) == RESET) {
        for(uint8_t i = 0; i < 16; i++) {
            // ����SCL
            GPIO_ResetBits(I2C_SCL_PORT, I2C_SCL_PIN);
            Delay(50);
            // �ͷ�SCL
            GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN);
            Delay(50);

            // ���SDA�Ƿ��ͷ�
            if(GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN) != RESET) {
                break;
            }
        }
    }

    // 4. ����STOP������SDA�ӵ͵���ʱSCLΪ��
    GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN);
    Delay(50);
    GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN);
    Delay(50);
    GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN);
    Delay(50);

    // 5. �ָ�GPIOΪI2C����
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  // ���ÿ�©
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);
}

void fallcheck(void)
{
     i2c_status = MPU6050_ReadAccelGyro(accel, gyro);

     if(i2c_status == I2C_ERROR_NONE) {
         acc_mag = sqrtf(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);
         gyro_mag = sqrtf(gyro[0]*gyro[0] + gyro[1]*gyro[1] + gyro[2]*gyro[2]);
         // ��ˤ�����
         // ���ڴ���Ԥ����������̬���
         if ((acc_mag < 2000 || acc_mag > 35000) && gyro_mag > 1500) {
             // �����ӳٺ��ȡ��̬
             Delay_Ms(500);  // ����ʽ��ʱ�ȴ�����ȶ�������̬���ж�
             float angle = calculate_angle(); // ͨ�����ٶȼ�����ǣ�ʾ��������

             if (angle > 20.0f) {  // ������б����20����Ϊˤ��
                 fallsend_pre();
             }
         }
     }
     else{
         // I2C���߻ָ�
         I2C_RecoverBus();
         // ���³�ʼ��I2C
         I2C_Init_Single(I2C1, 100000, GPIOB, GPIO_Pin_6, GPIOB, GPIO_Pin_7);//��ʼ��mpu�õ�I2C1
         MPU6050_Init();
     }

     fall_send();//ʱ���&ˤ����־�ж��Ƿ���ˤ�������ź�
}

// �����豸�봹ֱ����ļнǣ���λ���ȣ�
float calculate_angle(void)
{
    int16_t accel[3];
    float acc_mag;

    // ���¶�ȡ���ٶȼ����ݣ�ȷ��ʹ���������ݣ�
    if(MPU6050_ReadAccelGyro(accel, gyro) == I2C_ERROR_NONE)
    {
        // ������ٶ�ʸ��ģ��
        acc_mag = sqrtf(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);

        // ���������
        if(acc_mag < 1) return 0.0f;

        // ����Z������������ļнǣ���򷽷���
        // ����Z�ᴹֱʱaccel[2] = -16384����2g���̣�
        float cos_angle = -accel[2] / acc_mag;  // ������Ϊ��������

        // ����ֵ���ֹ��ֵ����
        if(cos_angle > 1.0f) cos_angle = 1.0f;
        if(cos_angle < -1.0f) cos_angle = -1.0f;

        // ������ת��Ϊ�Ƕ�
        return acosf(cos_angle) * (180.0f / M_PI);
    }
    return 0.0f;  // ��ȡʧ�ܷ���0
}



float Accel_Magnitude(int16_t *accel)//������ת�Ǽ��ٶ�
{
    return sqrtf(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);
}

float Gyro_Magnitude(int16_t *gyro)//
{
    return sqrtf(gyro[0]*gyro[0] + gyro[1]*gyro[1] + gyro[2]*gyro[2]);
}

// �Զ���д�Ĵ�������
uint8_t MyI2C_WriteRegister(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    uint32_t timeout = I2C_TIMEOUT_MAX;

    // 1. �������æ��־
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {
        if(timeout-- == 0) return I2C_ERROR_TIMEOUT;
    }

    // 2. ����START����
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 3. �����豸��ַ��дģʽ��
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

    // 4. ���ͼĴ�����ַ
    I2C_SendData(I2C1, regAddr);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 5. ��������
    I2C_SendData(I2C1, data);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 6. ����STOP����
    I2C_GenerateSTOP(I2C1, ENABLE);

    return I2C_ERROR_NONE;
}

uint8_t MyI2C_ReadRegister(uint8_t devAddr, uint8_t regAddr, uint8_t* data)
{
    uint32_t timeout = I2C_TIMEOUT_MAX;

    // 1. �������æ��־
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {
        if(timeout-- == 0) return I2C_ERROR_TIMEOUT;
    }

    // 2. ����START����
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 3. �����豸��ַ��дģʽ��
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

    // 4. ���ͼĴ�����ַ
    I2C_SendData(I2C1, regAddr);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 5. �����ظ�START����
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 6. �����豸��ַ����ģʽ��
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

    // 7. ׼���������ݣ����ֽڣ�
    I2C_AcknowledgeConfig(I2C1, DISABLE);  // ���һ���ֽ�ǰ����NACK
    I2C_GenerateSTOP(I2C1, ENABLE);        // ׼��STOP����

    // 8. �ȴ����ݽ������
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 9. ��ȡ����
    *data = I2C_ReceiveData(I2C1);

    return I2C_ERROR_NONE;
}

// MPU6050��ʼ�����˳�˯��ģʽ
void MPU6050_Init(void)
{
    MyI2C_WriteRegister(MPU6050_ADDR, 0x6B, 0x00);  // PWR_MGMT_1�Ĵ��������˯��λ
}

// ��ȡMPU6050���ٶȺ�����������
uint8_t MPU6050_ReadAccelGyro(int16_t *accel, int16_t *gyro)
{
    uint8_t buf[14];
    uint32_t timeout;
    //uint8_t status;

    // 1. �������æ��־
    timeout = I2C_TIMEOUT_MAX;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {
        if(timeout-- == 0) return I2C_ERROR_TIMEOUT;
    }

    // 2. ����START����
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 3. �����豸��ַ��дģʽ��
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

    // 4. ���ͼĴ�����ַ�����ٶ�������ʼ�Ĵ���0x3B��
    I2C_SendData(I2C1, 0x3B);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 5. �����ظ�START����
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }

    // 6. �����豸��ַ����ģʽ��
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

    // 7. ׼������14�ֽ�����
    I2C_AcknowledgeConfig(I2C1, ENABLE);  // ����ACK

    // ����ǰ13���ֽ�
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

        // ���һ���ֽ�ǰ����ACK
        if(i == 12) {
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
    }

    // 8. �������һ���ֽ�
    timeout = I2C_TIMEOUT_MAX;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
        if(timeout-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return I2C_ERROR_TIMEOUT;
        }
    }
    buf[13] = I2C_ReceiveData(I2C1);

    // 9. �ϳ�16λ����
    accel[0] = (buf[0] << 8) | buf[1];
    accel[1] = (buf[2] << 8) | buf[3];
    accel[2] = (buf[4] << 8) | buf[5];

    gyro[0] = (buf[8] << 8) | buf[9];
    gyro[1] = (buf[10] << 8) | buf[11];
    gyro[2] = (buf[12] << 8) | buf[13];

    return I2C_ERROR_NONE;
}
