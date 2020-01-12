#include "TraceUart.h"
#include "NvicPrio.h"

#define RX_BUF_SIZE (1024u)
#define UART_IRQ_HANDLER    USART6_IRQHandler
#define DMA_TX_IRQ_HANDLER  DMA2_Stream6_IRQHandler
#define DMA_RX_IRQ_HANDLER  DMA2_Stream1_IRQHandler
#define CLASS_NAME          TraceUart

static uint8_t rxBuffer[RX_BUF_SIZE];

static DMA_UART_CFG uart_cfg =
{
	.txEnabled     = true,
	.rxEnabled     = true,

	// Rx buffer
	.rxBuf         = rxBuffer,
	.rxBufSize     = RX_BUF_SIZE,

	// GPIO
	.gpioTxClkEn   = [](){__HAL_RCC_GPIOC_CLK_ENABLE();},
	.gpioTxPort    = GPIOC,
	.gpioTxPin     = GPIO_PIN_6,

	.gpioRxClkEn   = [](){__HAL_RCC_GPIOC_CLK_ENABLE();},
	.gpioRxPort    = GPIOC,
	.gpioRxPin     = GPIO_PIN_7,

	.gpioAf        = GPIO_AF8_USART6,

	// DMA
	.dmaClkEn      = [](){__HAL_RCC_DMA2_CLK_ENABLE();},

	.dmaTxStream   = DMA2_Stream6,
	.dmaTxChannel  = DMA_CHANNEL_5,
	.dmaTxIrq      = DMA2_Stream6_IRQn,
	.dmaTxNvicPrio = DMA_NVIC_PRIO,

	.dmaRxStream   = DMA2_Stream1,
	.dmaRxChannel  = DMA_CHANNEL_5,
	.dmaRxIrq      = DMA2_Stream1_IRQn,
	.dmaRxNvicPrio = DMA_NVIC_PRIO,

	// UART
	.uartClkEn     = [](){__HAL_RCC_USART6_CLK_ENABLE();},
	.uartInstance  = USART6,
	.uartBaudRate  = 1000000,
	.uartIrq       = USART6_IRQn,
	.uartNvicPrio  = DMA_NVIC_PRIO,
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
