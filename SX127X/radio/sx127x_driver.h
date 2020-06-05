#ifndef __SX127X_DRIVER_H__
#define __SX127X_DRIVER_H__
#include "stdbool.h"
#include "stdint.h"

/*
 * LoRa sx1278/76驱动
 *
 * \version    1.0.0 
 * \date       Dec 17 2019
 * \author     Specter
 */
 
#define VERSION "2.0.0"

typedef struct _sLoRaSettings
{
	uint32_t RFFrequency;		//频率,单位Hz
	int8_t Power;						//发射功率2~20
	uint8_t SignalBw;       // 带宽 [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                          // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]  带宽
	uint8_t SpreadingFactor;// 扩频因子 [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips] 扩频因子
	uint8_t ErrorCoding;    // 误码编码率 [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]	编码率
	uint16_t PreambleLength;	//前导码长度 
}tLoRaSettings;

//定义模块状态机(这个是返回给用户看的，实际模块执行的状态机比这个多)
typedef enum
{
    RF_IDLE,	//空闲状态
    RF_BUSY,	//模块执行任务中
    RF_RX_DONE,	//接收完成
    RF_RX_TIMEOUT,	//接收超时
    RF_TX_DONE,	//发送完成
    RF_TX_TIMEOUT,	//发送超时
    RF_CAD_DETECTED,	//CAD检测到前导码
		RF_CAD_EMPTY,	//CAD检测完成，没有检测到前导码
		RF_CAD_TIMEOUT,	//CAD超时
		RF_UNKNOW_STATUS	//异常状态机
}tRFProcessReturnCodes;

//硬件工作模式
typedef enum{
	LORA_OPMODE_SLEEP=0,
	LORA_OPMODE_STANDBY,
	LORA_OPMODE_SYNTHESIZER_TX,
	LORA_OPMODE_TRANSMITTER,
	LORA_OPMODE_SYNTHESIZER_RX,
	LORA_OPMODE_RECEIVER,
	LORA_OPMODE_RECIVER_SINGLE,
	LORA_OPMODE_CAD,
}LoRaOpModeType;

//驱动结构体，用来保存相关操作的函数
typedef struct sRadioDriver
{
    void ( *Init )( tLoRaSettings *stting );
    void ( *Reset )( void );
    void ( *StartRx )( uint32_t timeoutSystick );
    void ( *GetRxPacket )( void *buffer, uint16_t *size );
    void ( *SetTxPacket )( const void *buffer, uint16_t size,uint32_t timeoutSystick);
		void ( *StartCAD )( void );
    tRFProcessReturnCodes ( *Process )( void );
}tRadioDriver;

extern tRadioDriver g_Radio;

void sx127xInit(tLoRaSettings *stting);
void Sx127xRestart(void);
void SX127xSetLoRaMode(void);
void Write127xReg(uint8_t addr,uint8_t data);
uint8_t Read127xReg(uint8_t addr);
void SX127xWriteFifo( uint8_t *buffer, uint8_t size );
void SX127xReadFifo( uint8_t *buffer, uint8_t size );
void SX127xSetOpMode(LoRaOpModeType opMode);
LoRaOpModeType SX127xGetOpMode(void);
void SX127xSetFrf(uint32_t fr);
uint8_t sx127xSend(uint8_t *data,uint8_t len,uint32_t timeoutMs);
uint8_t sx127xRx(uint8_t *buf,uint8_t *len,uint32_t timeoutMs);

tRFProcessReturnCodes SX127xProcess( void );

#endif	//end of __SX127X_DRIVER_H__
