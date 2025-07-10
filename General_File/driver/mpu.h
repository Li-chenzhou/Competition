#ifndef __MPU_H_
#define __MPU_H_

#include "debug.h"

void fallcheck(void);
float calculate_angle(void);

uint8_t MyI2C_WriteRegister(uint8_t devAddr, uint8_t regAddr, uint8_t data);
uint8_t MyI2C_ReadRegister(uint8_t devAddr, uint8_t regAddr, uint8_t* data);
void MPU6050_Init(void);
void I2C_RecoverBus(void);
void Delay(volatile uint32_t count);

uint8_t MPU6050_ReadAccelGyro(int16_t *accel, int16_t *gyro);

#endif /* GENERAL_FILE_DRIVER_MPU_H_ */
