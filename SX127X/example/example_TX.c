#include "example/example_TX.h"
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
 *  这个是发送测试，配合接收模块可以测试收发程序
 */
 
//配合main函数使用的发送测试，需要轮询调用（不要同时跑多个demo，容易造成混乱）
void exampleTx(void){
	tRFProcessReturnCodes processRet=RF_IDLE;
	static uint32_t systickBak=200000;
	uint8_t *txBuf=(uint8_t *)"hello world";
	
	processRet=g_Radio.Process();
	switch(processRet){
		case RF_IDLE:	//空闲状态
			if(GET_TICK_COUNT()-systickBak >1000){
				//开始一次发送
				DEBUG("[DEBUG %s()-%d]start send\r\n",__func__,__LINE__);
				g_Radio.SetTxPacket(txBuf,strlen((char *)txBuf),15000);
			}
			break;
    case RF_TX_DONE:	//发送完成
			DEBUG("[DEBUG %s()-%d]send OK\r\n",__func__,__LINE__);
			g_Radio.Reset();	//复位
			g_Radio.Init(NULL);	//加载上次的配置
			systickBak=GET_TICK_COUNT();
			break;
    case RF_TX_TIMEOUT:	//发送超时
			DEBUG("[DEBUG %s()-%d]send Timeout\r\n",__func__,__LINE__);
			g_Radio.Reset();	//复位
			g_Radio.Init(NULL);	//加载上次的配置
			systickBak=GET_TICK_COUNT();
			break;
		case RF_BUSY:	//模块执行任务中
			break;
		case RF_UNKNOW_STATUS:	//异常状态机
		default:
			DEBUG("[ERROR %s()-%d]unknow status:%d\r\n",__func__,__LINE__,processRet);
			break;
	}
}
