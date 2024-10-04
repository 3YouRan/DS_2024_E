//
// Created by ��� on 24-9-18.
//
#include "all.h"

void my_Init() {
    RetargetInit(&huart4);//��ʼ������
    HAL_UART_Receive_IT(&huart4, (uint8_t *)RxBuffer, 1);//�������ڽ����ж�

    //CAN1�˲�����ʼ��
    CAN_Filter_Mask_Config_32bit(&hcan1,CAN_FILTER(0) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE,0x201,0);
    //CAN2�˲�����ʼ��
    CAN_Filter_Mask_Config_32bit(&hcan2,CAN_FILTER(15) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE,0x201,0);
    //CAN��ʼ��
    CAN_Init(&hcan1);
    CAN_Init(&hcan2);

    PID_Init(&pid_speed,&pid_position);//PID��ʼ��
    HAL_TIM_Base_Start_IT(&htim3);//��ʱ��3��ʼ��

}