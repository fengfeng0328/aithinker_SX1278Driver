#include "example/example_CAD.h"
#include "radio/sx127x_driver.h"
#include "hal/sx127x_hal.h"

/*
 * Copyright (c) 2019-2020 AIThinker.yangbin All rights reserved.
 *
 * 本工程只是SX127X的驱动demo，仅供参考，不保证商用稳定性。
 *
 * author     Specter
 */
 
//配合main函数使用的CAD检测demo，需要轮询调用（不要同时跑多个demo，容易造成混乱）
//注意CAD监测只是检测前导码，如果当前没有前导码，但是有数据发送也是无法检测到的
void exampleCAD(void){
	tRFProcessReturnCodes processRet=RF_IDLE;
	
	processRet=g_Radio.Process();
	switch(processRet){
		case RF_IDLE:	//空闲状态
			g_Radio.StartCAD();
			break;
    case RF_CAD_DETECTED:	//CAD检测到前导码
			DEBUG("[DEBUG %s()-%d]Cad detected--------------------\r\n",__func__,__LINE__);
			g_Radio.Init(NULL);	//加载上次的配置
			g_Radio.StartCAD();
			break;
		case RF_CAD_EMPTY:	//CAD检测完成没有检测到前导码
			DEBUG("[DEBUG %s()-%d]Cad empty\r\n",__func__,__LINE__);
			g_Radio.Init(NULL);	//加载上次的配置
			g_Radio.StartCAD();
			break;
		case RF_CAD_TIMEOUT:	//CAD超时
			DEBUG("[ERROR %s()-%d]Cad timeout\r\n",__func__,__LINE__);
			g_Radio.Init(NULL);	//加载上次的配置
			g_Radio.StartCAD();
			break;
		case RF_BUSY:	//模块执行任务中
			break;
		case RF_UNKNOW_STATUS:	//异常状态机
		default:
			DEBUG("[ERROR %s()-%d]unknow status:%d\r\n",__func__,__LINE__,processRet);
			break;
	}
}
