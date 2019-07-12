#include "nrf24l01.h"
#include "stm32f10x.h"
#include "oled.h"

//***************************************NRF24L01寄存器指令*******************************************************
#define READ_REG_N        0x00   // 读寄存器指令
#define WRITE_REG_N       0x20   // 写寄存器指令
#define RD_RX_PLOAD     0x61   // 读取接收数据指令
#define WR_TX_PLOAD     0xA0   // 写待发数据指令
#define FLUSH_TX        0xE1   // 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2   // 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3   // 定义重复装载数据指令
#define NOP             0xFF   // 保留
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置

#define TX_ADR_WIDTH    5    // 5 uints TX address width
#define RX_ADR_WIDTH    5    // 5 uints RX address width
#define TX_PLOAD_WIDTH  32   // 32 uints TX payload
#define RX_PLOAD_WIDTH  32   // 32 uints TX payload
unsigned char TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //本地地址
unsigned char RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //接收地址

//SPI set
#define NRF24L01_SPI    SPI1
#define SPI_GPIO        GPIOA
#define SPI_CLK_PIN     GPIO_Pin_5
#define SPI_MISO_PIN    GPIO_Pin_6
#define SPI_MOSI_PIN    GPIO_Pin_7
//#define SPI_DMA_CHANNEL DMA2_Channel2
//CE 
#define CE_GPIO         GPIOC
#define CE_GPIO_PIN     GPIO_Pin_4
//CSN
#define CSN_GPIO        GPIOA
#define CSN_GPIO_PIN    GPIO_Pin_4
//IRQ
#define IRQ_GPIO         GPIOC
#define IRQ_GPIO_PIN     GPIO_Pin_5

#define SENDBUFF_SIZE   TX_PLOAD_WIDTH

unsigned char SendBuff[SENDBUFF_SIZE] = {55};

void Delay_Ms(unsigned int delay);
//spi init
void spi_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	SendBuff[SENDBUFF_SIZE-1] = 66;
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);
	
	//CLK,MISO,MOSI
	GPIO_InitStructure.GPIO_Pin   = SPI_CLK_PIN | SPI_MISO_PIN | SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	//CE
	GPIO_InitStructure.GPIO_Pin   = CE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CE_GPIO, &GPIO_InitStructure);
	//CSN 
	GPIO_InitStructure.GPIO_Pin   = CSN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CSN_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(CSN_GPIO, CSN_GPIO_PIN);
	//IRQ
	GPIO_InitStructure.GPIO_Pin   = IRQ_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IRQ_GPIO, &GPIO_InitStructure);

#if 0
//LED
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
#endif

	//--------------------- SPI configuration ------------------
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_I2S_DeInit(NRF24L01_SPI);
	SPI_Init(NRF24L01_SPI, &SPI_InitStructure);
	//--------- Enable SPI3 TXE interrupt ------------
	//SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);
	SPI_Cmd(NRF24L01_SPI, ENABLE);
	
	return;
}


static void wait(void){
	while(SPI_I2S_GetFlagStatus(NRF24L01_SPI, SPI_I2S_FLAG_TXE) == RESET);
	//must delay
	Delay_Ms(3);
	return;
}

static unsigned char spi_read_reg(unsigned char reg){
	//CSN = 0;
	GPIO_ResetBits(CSN_GPIO, CSN_GPIO_PIN);
	SPI_I2S_SendData(NRF24L01_SPI, reg);
	wait();
	SPI_I2S_SendData(NRF24L01_SPI, 0x00);
	while(SPI_I2S_GetFlagStatus(NRF24L01_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	//CSN = 1;
	GPIO_SetBits(CSN_GPIO, CSN_GPIO_PIN);
	return SPI_I2S_ReceiveData(NRF24L01_SPI);
}

static void spi_tx_byte(unsigned char reg, unsigned char value){
	//CSN = 0;
	GPIO_ResetBits(CSN_GPIO, CSN_GPIO_PIN);
	SPI_I2S_SendData(NRF24L01_SPI, reg);
	wait();
	SPI_I2S_SendData(NRF24L01_SPI, value);
	wait();
	//CSN = 1;
	GPIO_SetBits(CSN_GPIO, CSN_GPIO_PIN);
	return;
}

static void SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes){

	//CSN = 0;
	GPIO_ResetBits(CSN_GPIO, CSN_GPIO_PIN);
	SPI_I2S_SendData(NRF24L01_SPI, reg);
	wait();
	while(bytes--){
		SPI_I2S_SendData(NRF24L01_SPI, *pBuf++);
		wait();
	}
	//CSN = 1;
	GPIO_SetBits(CSN_GPIO, CSN_GPIO_PIN);
	return;
}

/*******************************发*****送*****模*****式*****代*****码*************************************/
void NRF24L01_TX_Mode(u8 *addr){
	unsigned char sta;
	//volatile unsigned int i = 0xFFFF;
	//CE=0;
	GPIO_ResetBits(CE_GPIO, CE_GPIO_PIN);	
	spi_tx_byte(FLUSH_TX,0x00);
	SPI_Write_Buf(WRITE_REG_N + TX_ADDR, addr, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
	SPI_Write_Buf(WRITE_REG_N + RX_ADDR_P0, addr, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
	spi_tx_byte(WRITE_REG_N + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
	spi_tx_byte(WRITE_REG_N + EN_RXADDR, 0x01);  // Enable Pipe0
	spi_tx_byte(WRITE_REG_N + SETUP_RETR, 0xFF); // 500us + 86us, 10 retrans...1a
	spi_tx_byte(WRITE_REG_N + RF_CH, 40);        // Select RF channel 40 
	spi_tx_byte(WRITE_REG_N + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
	spi_tx_byte(WRITE_REG_N + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度
	spi_tx_byte(WRITE_REG_N + CONFIG, 0x0e);
	//CE=1;
	GPIO_SetBits(CE_GPIO, CE_GPIO_PIN);

	sta = spi_read_reg(STATUS);
	spi_tx_byte(WRITE_REG_N+STATUS, sta);	
	Delay_Ms(10);
	return;
}

//返回0表示发送成功，-1失败
int NRF24L01_Send(unsigned char *data){
	int ret = 0;
	int cnt = 0;
	unsigned char sta;	
	//CE=0;   
	GPIO_ResetBits(CE_GPIO, CE_GPIO_PIN);
	spi_tx_byte(FLUSH_TX,0x00);
	GPIO_ResetBits(CSN_GPIO, CSN_GPIO_PIN);
	SPI_Write_Buf(WR_TX_PLOAD, data, TX_PLOAD_WIDTH);     // 装载数据 
	GPIO_SetBits(CSN_GPIO, CSN_GPIO_PIN);
	//CE=1;   //置高CE，激发数据发送
	GPIO_SetBits(CE_GPIO, CE_GPIO_PIN);
	while(GPIO_ReadInputDataBit(IRQ_GPIO, IRQ_GPIO_PIN) == Bit_SET){//等待发送完成
		cnt++;
		Delay_Ms(2);
		if(cnt>100){
			Delay_Ms(2000);
			return -1;
		}
	}
	while(GPIO_ReadInputDataBit(IRQ_GPIO, IRQ_GPIO_PIN) == Bit_RESET){
		sta = spi_read_reg(STATUS);
		spi_tx_byte(WRITE_REG_N+STATUS, sta);
		Delay_Ms(10);
		cnt++;
		if(cnt>20){
			Delay_Ms(2000);
			return -1;
		}		
	}
	if(sta&0x20){//发送成功
		//GPIO_SetBits(GPIOF, GPIO_Pin_8);
		ret = 0;
	} 
	if(sta&0x10){//发送失败
		//GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		ret = -1;
	}

	return ret;
}

