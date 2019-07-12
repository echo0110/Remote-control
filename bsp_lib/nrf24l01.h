#ifndef __NRF24L01_H__
#define __NRF24L01_H__

typedef struct nrf24l01{
	
	unsigned char addr[5];               //nrf24l01��ַ
	unsigned char channel;               //RFƵ��
	unsigned char data_len;              //��Ч���ݳ���
	
}nrf24l01_t;

void spi_init(void);
void DMA_Configuration(void);
void NRF24L01_TX_Mode(unsigned char *addr);
int NRF24L01_Send(unsigned char *data);
#endif
