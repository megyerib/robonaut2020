#include "LsUartRear.h"

#define RX_BUF_SIZE (100u)
#define UART_IRQ_HANDLER    UART5_IRQHandler
#define DMA_RX_IRQ_HANDLER  DMA1_Stream0_IRQHandler
#define DMA_TX_IRQ_HANDLER  DMA1_Stream7_IRQHandler
#define CLASS_NAME          LsUartRear

static uint8_t rxBuffer[RX_BUF_SIZE];

static DMA_UART_CFG uart_cfg =
{
	// Rx buffer
	.rxBuf         = rxBuffer,
	.rxBufSize     = RX_BUF_SIZE,

	// GPIO
	.gpioTxClkEn   = [](){__HAL_RCC_GPIOC_CLK_ENABLE();},
	.gpioTxPort    = GPIOC,
	.gpioTxPin     = GPIO_PIN_12,

	.gpioRxClkEn   = [](){__HAL_RCC_GPIOD_CLK_ENABLE();},
	.gpioRxPort    = GPIOD,
	.gpioRxPin     = GPIO_PIN_2,

	.gpioAf        = GPIO_AF8_UART5,

	// DMA
	.dmaClkEn      = [](){__HAL_RCC_DMA1_CLK_ENABLE();},

	.dmaTxStream   = DMA1_Stream7,
	.dmaTxChannel  = DMA_CHANNEL_4,
	.dmaTxIrq      = DMA1_Stream7_IRQn,
	.dmaTxNvicPrio = 0, // TODO

	.dmaRxStream   = DMA1_Stream0,
	.dmaRxChannel  = DMA_CHANNEL_4,
	.dmaRxIrq      = DMA1_Stream0_IRQn,
	.dmaRxNvicPrio = 0, // TODO

	// UART
	.uartClkEn     = [](){__HAL_RCC_UART5_CLK_ENABLE();},
	.uartInstance  = UART5,
	.uartBaudRate  = 115200,
	.uartIrq       = UART5_IRQn,
	.uartNvicPrio  = 0, // TODO
};

// No touching needed ----------------------------------------------------------

CLASS_NAME* CLASS_NAME::GetInstance()
{
	static CLASS_NAME instance;
	return &instance;
}

CLASS_NAME::CLASS_NAME() : DmaUart(uart_cfg)
{

}

// Interrupt handlers ----------------------------------------------------------

extern "C" void DMA_RX_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleDmaRxIrq();
}

extern "C" void DMA_TX_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleDmaTxIrq();
}

extern "C" void UART_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleUartIrq();
}
