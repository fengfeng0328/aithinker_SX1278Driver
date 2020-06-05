#include "example/example_CAD.h"
#include "radio/sx127x_driver.h"
#include "hal/sx127x_hal.h"
#include "string.h"

/*
 * Copyright (c) 2019-2020 AIThinker.yangbin All rights reserved.
 *
 * 本工程只是SX127X的驱动demo，仅供参考，不保证商用稳定性。
 *
 * author     Specter
 *
 * 这个函数用来进行接收测试，需要另一个模块配合发送
 */
 
//配合main函数使用的接收demo，需要轮询调用（不要同时跑多个demo，容易造成混乱）
void exampleRx(void){
	tRFProcessReturnCodes processRet=RF_IDLE;
	uint8_t rxBuf[128]={0};
	uint16_t rxCount=128;
	
	processRet=g_Radio.Process();
	switch(processRet){
		case RF_IDLE:	//空闲状态
			g_Radio.StartRx(3000);
			break;
    case RF_RX_DONE:	//接收完成
			memset(rxBuf,0,128);
			g_Radio.GetRxPacket(rxBuf,&rxCount);
			DEBUG("[DEBUG %s()-%d]RF_RX_DONE (%d):%s\r\n",__func__,__LINE__,rxCount,rxBuf);
			g_Radio.StartRx(3000);
			break;
    case RF_RX_TIMEOUT:	//接收超时
			DEBUG("[DEBUG %s()-%d]RF_RX_TIMEOUT\r\n",__func__,__LINE__);
			g_Radio.StartRx(3000);
			break;
		case RF_BUSY:	//模块执行任务中
			break;
		case RF_UNKNOW_STATUS:	//异常状态机
		default:
			DEBUG("[ERROR %s()-%d]unknow status:%d\r\n",__func__,__LINE__,processRet);
			break;
	}
}
