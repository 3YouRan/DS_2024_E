//
// Created by ³Âè¤ on 24-10-2.
//
#include "all.h"
float a=135,b=132;
float Kinematics_Solution(int16_t x, int16_t y) {
    float c=sqrt(x*x+y*y);
    float theta_1=pi-acosf((b*b+a*a-c*c)/(2*b*a));
    return theta_1;
}