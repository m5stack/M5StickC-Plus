#ifndef _IIC_H_
#define _IIC_H_

#include <M5StickCPlus.h>

#define SERVO_ADDRESS 0X38
void IIC_Init();  // sda  0     scl  26

void angle_all_set(uint8_t angle1, uint8_t angle2, uint8_t angle3,
                   uint8_t angle4);

uint8_t angle_set(uint8_t whitch, uint8_t anglex);
#endif
