#ifndef __RANDER_H
#define __RANDER_H

#include "debug.h"
#include "stdbool.h"

extern uint8_t decmode_change_flag;
extern uint8_t detect_flag ;

extern float distance[3] ;
extern float angle[3] ;

extern uint8_t send_dis[20] ;
extern uint8_t send_ang[20] ;

extern uint8_t send_single[12] ;
extern uint8_t send_multiple[12] ;
extern uint8_t rec_data[30] ;

//帧结构定义
#define FRAME_HEADER_SIZE 4//帧头4个字节
#define FRAME_TAIL_SIZE 2  //帧尾2个字节
#define DATA_GROUP_SIZE 8  //每组数据8字节
#define DATA_GROUP_COUNT 3 //3组数据

//单组数据结构
typedef struct
{
    float x ;           // x坐标，单位cm
    float y ;           // y坐标，单位cm
    float speed ;       // 速度，单位cm/s
    float resolution ;  // 距离分辨率，单位mm
} SingleData;

//完整帧结构
typedef struct
{
    uint8_t header[FRAME_HEADER_SIZE]; // 帧头
    SingleData groups[DATA_GROUP_COUNT]; // 数据组
    uint8_t tail[FRAME_TAIL_SIZE];     // 帧尾
} FrameData;

void CartesianToPolar(float x, float y, float* angle_deg, float* distance);
float xy_pro(const uint8_t data[2]);
float speed_pro(const uint8_t data[2]);
float dist_pro(const uint8_t data[2]);
bool parse_frame_data(const uint8_t raw_data[30], FrameData *frame);

void change_decmode(void);

extern FrameData Frame;//只能放在结构体定义之后

#endif
