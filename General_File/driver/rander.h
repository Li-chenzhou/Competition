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

//֡�ṹ����
#define FRAME_HEADER_SIZE 4//֡ͷ4���ֽ�
#define FRAME_TAIL_SIZE 2  //֡β2���ֽ�
#define DATA_GROUP_SIZE 8  //ÿ������8�ֽ�
#define DATA_GROUP_COUNT 3 //3������

//�������ݽṹ
typedef struct
{
    float x ;           // x���꣬��λcm
    float y ;           // y���꣬��λcm
    float speed ;       // �ٶȣ���λcm/s
    float resolution ;  // ����ֱ��ʣ���λmm
} SingleData;

//����֡�ṹ
typedef struct
{
    uint8_t header[FRAME_HEADER_SIZE]; // ֡ͷ
    SingleData groups[DATA_GROUP_COUNT]; // ������
    uint8_t tail[FRAME_TAIL_SIZE];     // ֡β
} FrameData;

void CartesianToPolar(float x, float y, float* angle_deg, float* distance);
float xy_pro(const uint8_t data[2]);
float speed_pro(const uint8_t data[2]);
float dist_pro(const uint8_t data[2]);
bool parse_frame_data(const uint8_t raw_data[30], FrameData *frame);

void change_decmode(void);

extern FrameData Frame;//ֻ�ܷ��ڽṹ�嶨��֮��

#endif
