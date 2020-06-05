#include "example/example_CAD.h"
#include "radio/sx127x_driver.h"
#include "hal/sx127x_hal.h"
#include "string.h"

 
//需要轮询调用（不要同时跑多个demo，容易造成混乱）
void exampleRx(void){
	tRFProcessReturnCodes processRet=RF_IDLE;	/* 置为空闲态 */
	uint8_t rxBuf[128]={0};
	uint16_t rxCount=128;
	
	processRet=g_Radio.Process();
	
	switch(processRet){
		case RF_IDLE:			//空闲状态
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
