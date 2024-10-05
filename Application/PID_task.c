//
// Created by ��� on 24-9-20.
//

#include <stdbool.h>
#include "all.h"

int16_t current_1=0;
int16_t current_2=0;
float angle_now_1=0,angle_last_1=0,angle_total_1=0;//���λ��ͳ��
float angle_now_2=0,angle_last_2=0,angle_total_2=0;
bool Pos_flag=1;//�Ƿ���λ�ÿ���
void pid_task(void *arg){

    while(1){
//                printf("angle_now_1:%f\n",angle_now_1);
        xSemaphoreTake(g_SemaphoreHandle_For_Pid, portMAX_DELAY);//�ȴ��ź���
       // printf("angle_now_1:%f,%f\n",theta,angle_total_1);
        if(Pos_flag) {//(theta)>0?(theta):(theta+0.1)
            Target_Speed_1 = FW_PID_Realize(&pid_position,//λ�û�
                                            (theta)>0?(theta):(theta+0.1),
                                            angle_total_1);//����¼�ĵ��ת�ӳ�ʼ��е�ǶȲ�����λ�û����������ĳ�ʼƯ�ơ�
            Target_Speed_2 = FW_PID_Realize(&pid_position,
                                            Target_Position_2,
                                            angle_total_2);
//            printf("Target_Speed_1:%f,%f\n",Target_Speed_1,theta);
        }
        //�ٶȻ�
        current_1=FW_PID_Realize(&pid_speed, Target_Speed_actual_1, gm2006_1.rotor_speed / 36.0);
        current_2=FW_PID_Realize(&pid_speed, Target_Speed_2, gm2006_2.rotor_speed / 36.0);
        //�ٶ�����
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
        //���͵��can�����ź�
        GM2006_Current_Set(&hcan1, current_1, current_2, 0, 0, 0x200, GM_ID(1) | GM_ID(2));


    }
    vTaskDelete(NULL);
}