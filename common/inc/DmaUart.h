#pragma once

#include "stm32f4xx_hal.h"
#include "Serial.h"

typedef void(*ClkEn)(void);

typedef struct
{
	bool                txEnabled;
	bool                rxEnabled;

	// Rx buffer -----------------------
	uint8_t*            rxBuf;
	size_t              rxBufSize;

	// GPIO ----------------------------
	ClkEn               gpioTxClkEn;
	GPIO_TypeDef*       gpioTxPort;
	uint16_t            gpioTxPin;

	ClkEn               gpioRxClkEn;
	GPIO_TypeDef*       gpioRxPort;
	uint16_t            gpioRxPin;

	uint8_t             gpioAf;

	// DMA -----------------------------
	ClkEn               dmaClkEn;

	DMA_Stream_TypeDef* dmaTxStream;
	uint32_t            dmaTxChannel;
	IRQn_Type           dmaTxIrq;
	uint32_t            dmaTxNvicPrio;

	DMA_Stream_TypeDef* dmaRxStream;
	uint32_t            dmaRxChannel;
	IRQn_Type           dmaRxIrq;
	uint32_t            dmaRxNvicPrio;

	// UART ----------------------------
	ClkEn               uartClkEn;
	USART_TypeDef*      uartInstance;
	uint32_t            uartBaudRate;
	IRQn_Type           uartIrq;
	uint32_t            uartNvicPrio;
}
DMA_UART_CFG;

class DmaUart : public Serial
{
public:
	size_t Transmit(void* buffer, size_t size) override;
	size_t Receive(void* buffer, size_t targetSize) override;

	void HandleUartIrq();
	void HandleDmaRxIrq();
	void HandleDmaTxIrq();

protected:
	DmaUart(DMA_UART_CFG& cfg);

private:
	size_t rxBufIndex = 0;
	DMA_UART_CFG& cfg;

	UART_HandleTypeDef huart;
	DMA_HandleTypeDef  hdma_uart_tx;
	DMA_HandleTypeDef  hdma_uart_rx;

	void InitUart();
	void InitGpio();
	void InitDma();
};
