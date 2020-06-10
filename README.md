# aithinker_SX1278Driver
安信可Ra-01/SX1278优化版驱动


安信可 LoRa 系列模块是安信可科技基于 SX1278/SX1276 设计开发的，主要采用LoRa™远程调制解调器，用于超长距离扩频通信，抗干扰性强，
能够最大限度降低电流消耗。借助 SEMTECH 的 LoRa™ 专利调制技术，SX1278 具有超过 -148dBm 的高灵敏度，+20dBm 的功率输出，传输距离远，
可靠性高。同时，相对传统调制技术，LoRa™ 调制技术在抗阻塞和选择方面也具有明显优势，解决了传统设计方案无法同时兼顾距离、抗干扰和功耗的问题。

![ra-01_picture.png](https://docs.ai-thinker.com/_media/lora/spec/ra-01_picture.png?cache=&w=500&h=488&tok=bf34c5)
![ra-02_picture.png](https://docs.ai-thinker.com/_media/lora/spec/ra-02_picture.png?cache=&w=500&h=488&tok=7d38ae)

本驱动是基于STM32F103C8开发，直接编译可用，用于两个Ra-01节点之间进行数据收发：

接线如下：


| Ra-01模组 | STM32 |
| --------- | ----- |
| VCC       | 3.3V  |
| GND       | GND   |
| RST       | PB10  |
| NSS       | PA4   |
| SCK       | PA5   |
| MISO      | PA6   |
| MOSI      | PA7   |
| D0        | PB0   |
| D1        | PB1   |
| D3        | PA1   |
| D4        | PA0   |
