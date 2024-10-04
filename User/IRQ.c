//
// Created by 陈瑜 on 24-9-18.
//
#include "all.h"

/*
 * @brief CAN接收数据回调函数
 *
 * @param hcan CAN句柄
 *
 * @return None
 */
uint32_t CAN_Get_times=0;
int16_t Start_angle_1=0;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){

    CAN_RxHeaderTypeDef rx_header;
    uint8_t             rx_data[8];
    if(hcan->Instance == CAN1)
    {
        CAN_Get_times++;
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); //receive can data
//        printf("CAN_get_id:%x\r\n",rx_header.StdId);
        switch(rx_header.StdId)
        {
            case 0x201:
            {
                if(CAN_Get_times<100){//由于电机转子的初始机械角度不一定为0，因此芯片刚上电时，获取到的电机实际角度可能不为0,在位置环的作用下可能导致电机上电时偏离初始位置
                    Start_angle_1    = ((rx_data[0] << 8) | rx_data[1]);//因此记录电机转子的初始机械角度，补偿给位置环，避免电机的初始漂移。
                    Start_angle_1= transfer(Start_angle_1, 0, 8191, -pi / 36.0, pi / 36.0);
                }
                gm2006_1.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);
                gm2006_1.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);
                gm2006_1.torque_current = ((rx_data[4] << 8) | rx_data[5]);
//                gm2006_1.temp           =   rx_data[6];


                break;
            }
            case 0x202:{
                gm2006_2.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);
                gm2006_2.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);
                gm2006_2.torque_current = ((rx_data[4] << 8) | rx_data[5]);


                break;
            }

        }
    }
    if(hcan->Instance == CAN2)
    {
//        uint8_t send_data[8]="get_id!";
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); //receive can data
//        printf("CAN_get_id:%x\r\n",rx_header.StdId);
        switch(rx_header.StdId)
        {
            case 0x405:{
                int16_t theta_2=((int16_t)((rx_data[0]<<8)|(rx_data[1])));
                theta=theta_2/1000.0;
//                  theta=1.57;
//                  x=rx_data[0]<<8|rx_data[1];
//                  y=(int16_t)(rx_data[2]<<8|rx_data[3]);
                xSemaphoreGiveFromISR(g_SemaphoreHandle_For_Kinematics, NULL);//使用信号量唤醒Kinematics任务
                printf("x,y,theta,ID=%f,%X\n",theta,rx_header.StdId);
                break;
            }
        }
    }

}
/*
 * @brief UART接收数据回调函数
 *
 * @param huart UART句柄
 * @return None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{


    if(huart==&huart4)
    {
        RxLine++;                      //姣忔帴鏀跺埌涓?涓暟鎹紝杩涘叆鍥炶皟鏁版嵁闀垮害鍔?1
        DataBuff[RxLine-1]=RxBuffer[0];  //鎶婃瘡娆℃帴鏀跺埌鐨勬暟鎹繚瀛樺埌缂撳瓨鏁扮粍
        if(RxBuffer[0]=='!')           //鎺ユ敹缁撴潫鏍囧織浣?
        {
            printf("RXLen=%d\r\n",RxLine);
            for(int i=0;i<RxLine;i++)
                printf("UART DataBuff[%d] = %c\r\n",i,DataBuff[i]);
//            xEventGroupSetBitsFromISR(g_xEventGroup_Uart_Rx,(1<<0),NULL);

//            USART_PID_Adjust(1);//鏁版嵁瑙ｆ瀽鍜屽弬鏁拌祴鍊煎嚱鏁?
//
//            memset(DataBuff,0,sizeof(DataBuff));  //娓呯┖缂撳瓨鏁扮粍
//            RxLine=0;  //娓呯┖鎺ユ敹闀垮害
            xSemaphoreGiveFromISR(g_SemaphoreHandle_For_Uart_RX, NULL);//使用信号量唤醒UART_Rx任务

        }
        RxBuffer[0]=0;
        HAL_UART_Receive_IT(&huart4, (uint8_t *)RxBuffer, 1);//姣忔帴鏀朵竴涓暟鎹紝灏辨墦寮?涓?娆′覆鍙ｄ腑鏂帴鏀?
    }
}


/**
  * @brief 非阻塞模式下的 Period elapsed 回调
  * @note 当 TIM4 中断发生时，在
  * HAL_TIM_IRQHandler（） 的它直接调用 HAL_IncTick（） 以递增
  * 用作应用程序时基的全局变量 “uwTick”。
  * @param htim ： TIM 句柄
  * @retval 无
  */
uint8_t time1=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM4) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */
    if(htim->Instance==TIM3){
        time1++;

        angle_last_1=angle_now_1;
        angle_last_2=angle_now_2;

        angle_now_1 = transfer(gm2006_1.rotor_angle, 0, 8191, -pi / 36.0, pi / 36.0);
        angle_now_2 = transfer(gm2006_2.rotor_angle, 0, 8191, -pi / 36.0, pi / 36.0);
        if(angle_now_1 - angle_last_1 > (pi / 36.0)){
            angle_total_1+=(angle_now_1 - angle_last_1 - 2 * pi / 36.0);
        }else if(angle_now_1 - angle_last_1 < (-pi / 36.0))
        {
            angle_total_1+=(angle_now_1 - angle_last_1 + 2 * pi / 36.0);
        }else{
            angle_total_1+=(angle_now_1 - angle_last_1);
        }
        if(angle_now_2 - angle_last_2 > (pi / 36.0)){
            angle_total_2+=(angle_now_2 - angle_last_2 - 2 * pi / 36.0);
        }else if(angle_now_2 - angle_last_2 < (-pi / 36.0))
        {
            angle_total_2+=(angle_now_2 - angle_last_2 + 2 * pi / 36.0);
        }else{
            angle_total_2+=(angle_now_2 - angle_last_2);
        }

        if(time1==10){
            time1=0;

//            if((Target_Speed_1 - Target_Speed_actual_1) > MIN_Spe_Increment){
//                Target_Speed_actual_1+=MIN_Spe_Increment;
//            } else if((Target_Speed_1 - Target_Speed_actual_1) < -MIN_Spe_Increment){
//                Target_Speed_actual_1-=MIN_Spe_Increment;
//            }
//            if((Target_Speed_2 - Target_Speed_actual_2) > MIN_Spe_Increment){
//                Target_Speed_actual_2+=MIN_Spe_Increment;
//            } else if((Target_Speed_2 - Target_Speed_actual_2) < -MIN_Spe_Increment){
//                Target_Speed_actual_2-=MIN_Spe_Increment;
//            }
            xSemaphoreGiveFromISR(g_SemaphoreHandle_For_Pid, NULL);//使用信号量唤醒PID任务
        }
    }
    /* USER CODE END Callback 1 */
}