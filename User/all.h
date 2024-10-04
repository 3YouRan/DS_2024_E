//
// Created by 陈瑜 on 24-9-16.
//

#ifndef RC_WORK1_ALL_H
#define RC_WORK1_ALL_H

#include "retarget.h"

#include "string.h"

#include "main.h"

#include "PID_Adjust.h"

#include "PID.h"

#include "FreeRTOS.h"

#include "task.h"

#include "queue.h"

#include "event_groups.h"

#include "usart_task.h"

#include "usart.h"

#include "can.h"

#include "my_can.h"

#include "tim.h"

#include "PID_task.h"

#include "semphr.h"

#include <stdbool.h>

#include "PID.h"

#include "PID_Adjust.h"

#include "DJI_driver.h"

#include "math.h"

#include "Kinematics_Solution_task.h"

extern float Target_Speed_1;//电机1目标速度
extern float Target_Speed_2;//电机2目标速度
extern float Target_Speed_actual_1;//电机1实际目标速度
extern float Target_Speed_actual_2;//电机2实际目标速度
extern float MIN_Spe_Increment;//最小速度增量
extern float Target_Position_1;//电机1目标位置
extern float Target_Position_last_1;//电机1上一次目标位置
extern float Target_Position_2;//电机2目标位置
extern float Target_Position_last_2;//电机2上一次目标位置
extern float Target_Current;
extern PID pid_position;//位置PID
extern PID pid_speed;//速度PID
extern PID pid_current;


extern uint8_t RxBuffer[1];//接收缓冲区
extern uint16_t RxLine;//接收行数
extern uint8_t DataBuff[200];//数据缓存区

extern TaskHandle_t g_Uart_TX_TaskHandle;//串口发送任务句柄
extern TaskHandle_t g_Uart_RX_TaskHandle;//串口接收任务句柄
extern TaskHandle_t g_Pid_TaskHandle;//PID任务句柄
extern SemaphoreHandle_t g_SemaphoreHandle_For_Pid;//PID任务信号量句柄
extern SemaphoreHandle_t g_SemaphoreHandle_For_Uart_RX;//串口接收任务信号量句柄
extern TaskHandle_t g_Kinematics_TaskHandle;//运动学任务句柄
extern SemaphoreHandle_t g_SemaphoreHandle_For_Kinematics;//运动学任务信号量句柄

extern DJI_motor gm2006_1;//电机1
extern DJI_motor gm2006_2;//电机2
extern float angle_last_1, angle_now_1, angle_total_1;//电机1角度统计数据
extern float angle_last_2, angle_now_2, angle_total_2;//电机2角度统计数据
extern int16_t Start_angle_1;//电机1起始机械角度
extern int16_t current_1;

extern int16_t x;//机械臂端子x坐标
extern int16_t y;//机械臂端子y坐标
extern float theta;//电机1的目标theta角度
extern float theta_last;


extern void my_Init();//初始化函数
extern float Kinematics_Solution(int16_t x, int16_t y);//运动学解算函数
#define pi 3.1416


#endif //RC_WORK1_ALL_H
