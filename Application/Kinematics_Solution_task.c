//
// Created by 陈瑜 on 24-10-2.
//

#include "all.h"

int16_t x;
int16_t y;
float theta=0;
float theta_last;
uint8_t get_flag=false;
int16_t chess_times=0;
void Magnetic_on(void){
    HAL_GPIO_WritePin(Magnetic1_GPIO_Port, Magnetic1_Pin, GPIO_PIN_SET);
}
void Magnetic_off(void){
    HAL_GPIO_WritePin(Magnetic1_GPIO_Port, Magnetic1_Pin, GPIO_PIN_RESET);
}
void Kinematics_Solution_task(void* arg) {
    while (1) {
        xSemaphoreTake(g_SemaphoreHandle_For_Kinematics, portMAX_DELAY);

//        theta=Kinematics_Solution(x, y);//解算得到gm2006_1的theta角度

//        printf("x,y=%d,%d,%.2f\n",x,y,theta);
        vTaskDelay(2000);//等待位置改变
//        printf("PID_controller: %.2f,%.2f,%.2f,%.5f,%.5f\r\n", pid_position.kp, pid_position.ki, pid_position.kd, angle_total_1,Target_Position_1);
//        Magnetic_on();
        if(theta==0){
            vTaskDelay(2000);
            chess_times++;
            theta_2_last=0;
            theta_2_last_last=0;
            goto end;
        }
        //如果机械臂上没有棋子
        if(get_flag==0){
//            Target_Position_2=pi/4.0+0.5;//gm2006_2下降
            vTaskDelay(2000);//等待位置改变
            Target_Position_2=pi/4.0+0.1;//gm2006_2下降
            Magnetic_on();
            vTaskDelay(2000);//等待位置改变
            Target_Position_2=0;//gm2006_2上升
            get_flag=1;
            vTaskDelay(2000);
            CAN_Transmit_DATA(&hcan2,0x406,NULL,0);
            goto end;
        }
        //如果机械臂上有棋子
        if(get_flag==1){
            Target_Position_2=pi/4.0+0.1;//gm2006_2下降
            vTaskDelay(1000);//等待位置改变
            Magnetic_off();
             Target_Position_2=0;//gm2006_2上升
             vTaskDelay(1000);//等待位置改变


            get_flag=0;

            CAN_Transmit_DATA(&hcan2,0x407,NULL,0);
//            vTaskDelay(500);
         }
        end:
        vTaskDelay(500);

    }

}