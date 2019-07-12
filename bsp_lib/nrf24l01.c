#include "nrf24l01.h"
#include "stm32f10x.h"
#include "oled.h"

//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define READ_REG_N        0x00   // ���Ĵ���ָ��
#define WRITE_REG_N       0x20   // д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61   // ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0   // д��������ָ��
#define FLUSH_TX        0xE1   // ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2   // ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3   // �����ظ�װ������ָ��
#define NOP             0xFF   // ����
//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������

#define TX_ADR_WIDTH    5    // 5 uints TX address width
#define RX_ADR_WIDTH    5    // 5 uints RX address width
#define TX_PLOAD_WIDTH  32   // 32 uints TX payload
#define RX_PLOAD_WIDTH  32   // 32 uints TX payload
unsigned char TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //���ص�ַ
unsigned char RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //���յ�ַ

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

/*******************************��*****��*****ģ*****ʽ*****��*****��*************************************/
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
	spi_tx_byte(WRITE_REG_N + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ���
	spi_tx_byte(WRITE_REG_N + CONFIG, 0x0e);
	//CE=1;
	GPIO_SetBits(CE_GPIO, CE_GPIO_PIN);

	sta = spi_read_reg(STATUS);
	spi_tx_byte(WRITE_REG_N+STATUS, sta);	
	Delay_Ms(10);
	return;
}

//����0��ʾ���ͳɹ���-1ʧ��
int NRF24L01_Send(unsigned char *data){
	int ret = 0;
	int cnt = 0;
	unsigned char sta;	
	//CE=0;   
	GPIO_ResetBits(CE_GPIO, CE_GPIO_PIN);
	spi_tx_byte(FLUSH_TX,0x00);
	GPIO_ResetBits(CSN_GPIO, CSN_GPIO_PIN);
	SPI_Write_Buf(WR_TX_PLOAD, data, TX_PLOAD_WIDTH);     // װ������ 
	GPIO_SetBits(CSN_GPIO, CSN_GPIO_PIN);
	//CE=1;   //�ø�CE���������ݷ���
	GPIO_SetBits(CE_GPIO, CE_GPIO_PIN);
	while(GPIO_ReadInputDataBit(IRQ_GPIO, IRQ_GPIO_PIN) == Bit_SET){//�ȴ��������
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
	if(sta&0x20){//���ͳɹ�
		//GPIO_SetBits(GPIOF, GPIO_Pin_8);
		ret = 0;
	} 
	if(sta&0x10){//����ʧ��
		//GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		ret = -1;
	}

	return ret;
}

