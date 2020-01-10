#include "TraceUart.h"

#define RX_BUF_SIZE (1024u)

static uint8_t rx[RX_BUF_SIZE];

static DMA_UART_CFG trace_cfg =
{
	// Rx buffer
	.rxBuf = rx,
	.rxBufSize = RX_BUF_SIZE,

	// GPIO
	.gpioTxClkEn  = [](){__HAL_RCC_GPIOA_CLK_ENABLE();},
	.gpioTxPort   = GPIOA,
	.gpioTxPin    = GPIO_PIN_2,

	.gpioRxClkEn  = [](){__HAL_RCC_GPIOA_CLK_ENABLE();},
	.gpioRxPort   = GPIOA,
	.gpioRxPin    = GPIO_PIN_3,

	.gpioAf       = GPIO_AF7_USART2,

	// DMA
	.dmaClkEn     = [](){__HAL_RCC_DMA1_CLK_ENABLE();},

	.dmaTxStream  = DMA1_Stream6,
	.dmaTxChannel = DMA_CHANNEL_4,
	.dmaTxIrq     = DMA1_Stream6_IRQn,

	.dmaRxStream  = DMA1_Stream5,
	.dmaRxChannel = DMA_CHANNEL_4,
	.dmaRxIrq     = DMA1_Stream5_IRQn,

	// UART
	.uartClkEn     = [](){__HAL_RCC_USART2_CLK_ENABLE();},
	.uartInstance  = USART2,
	.uartBaudRate  = 115200,
	.uartIrq       = USART2_IRQn
};

TraceUart* TraceUart::GetInstance()
{
	static TraceUart instance;
	return &instance;
}

TraceUart::TraceUart() : DmaUart(trace_cfg)
{

}

// Interrupt handlers ----------------------------------------------------------

extern "C" void DMA1_Stream5_IRQHandler(void)
{
	TraceUart::GetInstance()->HandleDmaRxIrq();
}

extern "C" void DMA1_Stream6_IRQHandler(void)
{
	TraceUart::GetInstance()->HandleDmaTxIrq();
}

extern "C" void USART2_IRQHandler(void)
{
	TraceUart::GetInstance()->HandleUartIrq();
}
