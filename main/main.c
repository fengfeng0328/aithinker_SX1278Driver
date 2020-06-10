#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x_it.h"
#include "radio/sx127x_driver.h"
#include "example/example_TX.h"
#include "example/example_RX.h"
#include "example/example_CAD.h"


#define TEST_MOD    1   //测试模式 1表示非阻塞式模式，0表示阻塞式模式
#if !TEST_MOD
    #define IS_TX_MOD 1 //1表示发送模式，0表示接收模式(仅阻塞模式生效)
#endif

#pragma import(__use_no_semihosting)
//标准库需要支持的函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免工作在半主机状态
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
//这个需要根据MCU和我们希望printf从哪个串口输出来确认 __WAIT_TODO__
int fputc(int ch, FILE *f)
{
    //注意：USART_FLAG_TXE是检查发送缓冲区是否为空，这个要在发送前检查，检查这个提议提高发送效率，但是在休眠的时候可能导致最后一个字符丢失
    //USART_FLAG_TC是检查发送完成标志，这个在发送后检查，这个不会出现睡眠丢失字符问题，但是效率低（发送过程中发送缓冲区已经为空了，可以接收下一个数据了，但是因为要等待发送完成，所以效率低）
    //不要两个一起用，一起用效率最低

    //循环等待直到发送缓冲区为空(TX Empty)此时可以发送数据到缓冲区
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {}
    USART_SendData(USART2, (uint8_t) ch);

    /* 循环等待直到发送结束*/
    //while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    //{}

    return ch;
}

void uartInit()
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    USART_DeInit(USART2);  //复位串口

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);      // RCC_APB1Periph_USART2 时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);       // RCC_APB2Periph_GPIOA  时钟使能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           // GPIO 翻转速度为50MHz

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  	// 初始化GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  	// 初始化GPIO

    //USART配置
    USART_InitStructure.USART_BaudRate = 115200;                                    //设置串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     				//字长为8
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              			//1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                     		//无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 				//收发模式
    USART_Init(USART2, &USART_InitStructure);                                       //配置USART参数
    USART_Cmd(USART2, ENABLE);                                                      //使能USART
}

void delayMsBySystick(uint32_t timeoutMs)
{
    uint32_t systickBak, currTick;
    systickBak = GetTick();
    while (1)
    {
        currTick = GetTick();
        if (currTick >= systickBak)
        {
            if (currTick - systickBak > timeoutMs)
            {
                return;
            }
        }
        else
        {
            //currTick溢出了
            if (currTick + (~systickBak) > timeoutMs)
            {
                return;
            }
        }
    }
}

/* 程序函数入口 */
int main(void)
{
	
	/* -------------------------------------------------------------------- */
    tLoRaSettings setting =
    {
        435000000,      		/* 频率配置 435MHZ */
        20,                     /* 功率配置 20dBm*/
        7,                      /* 带宽配置 125kHz */
        12,                     /* 扩频因子 12 */
        1,                      /* 纠错编码 4/5*/
        0x0005              	/* 前导码 0x0005 */
    };


	/* -------------------------------------------------------------------- */

    SysTick_Config(SystemCoreClock / 1000); /* 时钟滴答设置成1MS */

    uartInit();     /* UART2 INIT */

    printf("aithinker Lora-version:%s\r\n", VERSION);

    g_Radio.Init(&setting); //初始化配置

    printf("aithinker Lora-init OK\r\n");

    while (1)
    {
#if TEST_MOD
        //非阻塞测试
        //exampleTx();        	//发送测试，发送完成等待1s后再次发送
        exampleRx();  			//循环接收
        //exampleCAD(); 		//CAD循环检测测试
#else   //else of #if IS_TX_MOD
        //阻塞收发测试
#if IS_TX_MOD
        if (0 == sx127xSend((uint8_t *)"hello", strlen("hello"), 1000)) //发送数据，软件超时时间1000Ms
        {
            printf("send ok\r\n");
        }
        else
        {
            printf("send timeout\r\n");
        }
        delayMsBySystick(10000);
#else   //else of #if IS_TX_MOD
        rxCount = 255;
        memset(rxBuf, 0, 255);
        printf("start rx\r\n");

        u8_ret = sx127xRx(rxBuf, &rxCount, 1000); //接收数据，rxBuf存储数据内容，rxCount存储接收到的数据长度，软件超时1000Ms
        switch (u8_ret)
        {
        case 0:
            printf("rx done\r\n  len:%d\r\n  playload:%s\r\n", rxCount, rxBuf);
            break;
        case 1:
        case 2:
            printf("rx timeout ret:%d\r\n", u8_ret);
            break;
        default:
            printf("unknow ret:%d\r\n", u8_ret);
            break;
        }
#endif  //end of #if IS_TX_MOD
#endif  //end of #if IS_TX_MOD
    }
}
