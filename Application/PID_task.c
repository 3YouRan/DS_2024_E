//
// Created by 陈瑜 on 24-9-20.
//

#include <stdbool.h>
#include "all.h"

int16_t current_1=0;
int16_t current_2=0;
float angle_now_1=0,angle_last_1=0,angle_total_1=0;//电机位置统计
float angle_now_2=0,angle_last_2=0,angle_total_2=0;
bool Pos_flag=1;//是否开启位置控制
void pid_task(void *arg){

    while(1){
//                printf("angle_now_1:%f\n",angle_now_1);
        xSemaphoreTake(g_SemaphoreHandle_For_Pid, portMAX_DELAY);//等待信号量
       // printf("angle_now_1:%f,%f\n",theta,angle_total_1);
        if(Pos_flag) {//(theta)>0?(theta):(theta+0.1)
            Target_Speed_1 = FW_PID_Realize(&pid_position,//位置环
                                            (theta)>0?(theta):(theta+0.1),
                                            angle_total_1);//将记录的电机转子初始机械角度补偿给位置环，避免电机的初始漂移。
            Target_Speed_2 = FW_PID_Realize(&pid_position,
                                            Target_Position_2,
                                            angle_total_2);
//            printf("Target_Speed_1:%f,%f\n",Target_Speed_1,theta);
        }
        //速度环
        current_1=FW_PID_Realize(&pid_speed, Target_Speed_actual_1, gm2006_1.rotor_speed / 36.0);
        current_2=FW_PID_Realize(&pid_speed, Target_Speed_2, gm2006_2.rotor_speed / 36.0);
        //速度爬坡
        if((Target_Speed_1 - Target_Speed_actual_1) > MIN_Spe_Increment){
            Target_Speed_actual_1+=MIN_Spe_Increment;
        } else if((Target_Speed_1 - Target_Speed_actual_1) < -MIN_Spe_Increment){
            Target_Speed_actual_1-=MIN_Spe_Increment;
        }
        if((Target_Speed_2 - Target_Speed_actual_2) > MIN_Spe_Increment){
            Target_Speed_actual_2+=MIN_Spe_Increment;
        } else if((Target_Speed_2 - Target_Speed_actual_2) < -MIN_Spe_Increment){
            Target_Speed_actual_2-=MIN_Spe_Increment;
        }
        //发送电机can控制信号
        GM2006_Current_Set(&hcan1, current_1, current_2, 0, 0, 0x200, GM_ID(1) | GM_ID(2));


    }
    vTaskDelete(NULL);
}