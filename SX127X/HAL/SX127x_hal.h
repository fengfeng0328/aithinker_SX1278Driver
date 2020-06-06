#ifndef __SX127X_HAL_H__
#define __SX127X_HAL_H__
/*
 * LoRa sx1278/76驱动
 *
 * \version    1.0.0
 * \date       Dec 17 2019
 * \author     Specter
 */

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stdio.h"

#define DEBUG( format , ... )   printf( format , ##__VA_ARGS__ )
#define GET_TICK_COUNT() GetTick()  	/*获取当前滴答计时器数值*/

void Soft_delay_ms(u16 time);        	/* 软件延迟器 */
void SX1276HALInit(void);               /* 硬件初始化 */
uint8_t SpiInOut(uint8_t outData);   	/* SPI输入输出函数 */
void SpiNSSEnable(uint8_t status);  	/* SPI NSS 片选使能 */
void SX127X_ResetPinControl(uint8_t status);     /* 复位控制 */
uint8_t SX1276ReadDio0(void);           /* 读取DIO0电平 */
uint8_t SX1276ReadDio1(void);           /* 读取DIO1电平 */
uint8_t SX1276ReadDio2(void);           /* 读取DIO2电平 */
uint8_t SX1276ReadDio3(void);           /* 读取DIO3电平 */
uint8_t SX1276ReadDio4(void);           /* 读取DIO4电平 */
uint8_t SX1276ReadDio5(void);           /* 读取DIO5电平 */

#endif //end of __SX127X_HAL_H__
