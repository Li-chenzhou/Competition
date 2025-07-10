/*
 * asr.h
 *
 *  Created on: 2025��7��1��
 *      Author: Rock
 */

#ifndef GENERAL_FILE_DRIVER_ASR_H_
#define GENERAL_FILE_DRIVER_ASR_H_

#include "debug.h"

extern volatile uint64_t  fall_time;
extern volatile uint64_t last_time;
extern uint8_t fall_flag;

// ����״̬ö��
typedef enum
{
    ALARM_OFF = 0,
    ALARM_ON = 1,
    ALARM_PENDING = 2
} AlarmState;

// �������ýṹ��
typedef struct
{
    float speed_threshold;   // �ٶ���ֵ (cm/s)
    uint8_t debounce_count;  // ������������
} AlarmConfig;

// Ŀ�걨��״̬
typedef struct
{
    AlarmState state;
    uint8_t counter;
} TargetAlarm;

void float_to_bytes(float f, uint8_t *bytes, uint8_t max_len);

void dataRes(void);
void angSend(void);
void disSend(void);

void Warn_Init(void);
void Warn_Check(void);

void SendTargetId(uint8_t target_id);

void fall_send(void);
void fallsend_pre(void);
void fallsend_cancel(void);
void serialtest(void);

extern AlarmConfig alarm_cfg;// �������ýṹ��
extern TargetAlarm target_alarms[3];// Ŀ�걨��״̬

#endif /* GENERAL_FILE_DRIVER_ASR_H_ */
