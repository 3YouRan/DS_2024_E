//
// Created by 陈瑜 on 24-9-18.
//

#include "usart_task.h"
#include "all.h"
extern void Magnetic_on(void);
extern void Magnetic_off(void);
void usart_tx_task(void *arg) {
    while (1) {
//        printf("PID_controller: %.2f,%.2f,%.2f,%d\r\n", pid_current.kp, pid_current.ki, pid_current.kd, gm3508_1.rotor_angle);
//        printf("PID_controller: %.2f,%.2f,%.2f,%.5f,%.5f\r\n", pid_position.kp, pid_position.ki, pid_position.kd, angle_total_1,Target_Position_1);
//        GM2006_Current_Set(&hcan1,1000,0x405,1);
//        CAN_Transmit_DATA(&hcan2,0x406,NULL,0);
//        HAL_GPIO_TogglePin(Magnetic1_GPIO_Port, Magnetic1_Pin);
//        Magnetic_on();
//        vTaskDelay(1000);
//        Magnetic_off();
//        vTaskDelay(1000);
//        Magnetic_on();
        vTaskDelay(1000);
    }
}
void usart_rx_task(void *arg) {
    while (1) {
        xSemaphoreTake(g_SemaphoreHandle_For_Uart_RX, portMAX_DELAY);//等待接收数据

        USART_PID_Adjust(1);//PID参数调整

        memset(DataBuff,0,sizeof(DataBuff));  //清空接收缓存区
        RxLine=0;  //清空接收行数


    }
}

