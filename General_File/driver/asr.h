/*
 * asr.h
 *
 *  Created on: 2025年7月1日
 *      Author: Rock
 */

#ifndef GENERAL_FILE_DRIVER_ASR_H_
#define GENERAL_FILE_DRIVER_ASR_H_

#include "debug.h"

extern volatile uint64_t  fall_time;
extern volatile uint64_t last_time;
extern uint8_t fall_flag;

// 报警状态枚举
typedef enum
{
    ALARM_OFF = 0,
    ALARM_ON = 1,
    ALARM_PENDING = 2
} AlarmState;

// 报警配置结构体
typedef struct
{
    float speed_threshold;   // 速度阈值 (cm/s)
    uint8_t debounce_count;  // 防抖计数窗口
} AlarmConfig;

// 目标报警状态
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

extern AlarmConfig alarm_cfg;// 报警配置结构体
extern TargetAlarm target_alarms[3];// 目标报警状态

#endif /* GENERAL_FILE_DRIVER_ASR_H_ */
