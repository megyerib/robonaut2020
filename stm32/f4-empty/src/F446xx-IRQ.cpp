// STM32 F446xx IRQ handler functions

#define ADC1_IRQ              0
#define ADC2_IRQ              0
#define ADC3_IRQ              0
#define CAN1_IRQ              0
#define CAN2_IRQ              0
#define CEC_IRQ               0
#define DAC1_IRQ              0
#define DAC2_IRQ              0
#define DCMI_IRQ              0
#define DMA1_IRQ              0
#define DMA2_IRQ              0
#define EXTI0_IRQ             0
#define EXTI1_IRQ             0
#define EXTI2_IRQ             0
#define EXTI3_IRQ             0
#define EXTI4_IRQ             0
#define EXTI5_IRQ             0
#define EXTI6_IRQ             0
#define EXTI7_IRQ             0
#define EXTI8_IRQ             0
#define EXTI9_IRQ             0
#define EXTI10_IRQ            0
#define EXTI11_IRQ            0
#define EXTI12_IRQ            0
#define EXTI13_IRQ            0
#define EXTI14_IRQ            0
#define EXTI15_IRQ            0
#define FLASH_IRQ             0
#define FMC_IRQ               0
#define FMPI2C1_IRQ           0
#define FPU_IRQ               0
#define I2C1_IRQ              0
#define I2C2_IRQ              0
#define I2C3_IRQ              0
#define OTG_FS_IRQ            0
#define OTG_HS_IRQ            0
#define PVD_IRQ               0
#define QuadSPI_IRQ           0
#define RCC_IRQ               0
#define RTC_IRQ               0
#define SAI1_IRQ              0
#define SAI2_IRQ              0
#define SDIO_IRQ              0
#define SPDIF_RX_IRQ          0
#define SPI1_IRQ              0
#define SPI2_IRQ              0
#define SPI3_IRQ              0
#define SPI4_IRQ              0
#define TAMP_STAMP_IRQ        0
#define TIM1_IRQ              0
#define TIM2_IRQ              0
#define TIM3_IRQ              0
#define TIM4_IRQ              0
#define TIM5_IRQ              0
#define TIM6_IRQ              0
#define TIM7_IRQ              0
#define TIM8_IRQ              0
#define TIM8_IRQ              0
#define TIM8_IRQ              0
#define TIM9_IRQ              0
#define TIM10_IRQ             0
#define TIM11_IRQ             0
#define TIM12_IRQ             0
#define TIM13_IRQ             0
#define TIM14_IRQ             0
#define USART1_IRQ            0
#define USART2_IRQ            0
#define USART3_IRQ            0
#define UART4_IRQ             0
#define UART5_IRQ             0
#define USART6_IRQ            0
#define WWDG_IRQ              0

#if (ADC1_IRQ | ADC2_IRQ | ADC3_IRQ)
extern "C" void ADC_IRQHandler()
{

}
#endif

#if CAN1_IRQ
extern "C" void CAN1_RX0_IRQHandler()
{

}
#endif

#if CAN1_IRQ
extern "C" void CAN1_RX1_IRQHandler()
{

}
#endif

#if CAN1_IRQ
extern "C" void CAN1_SCE_IRQHandler()
{

}
#endif

#if CAN1_IRQ
extern "C" void CAN1_TX_IRQHandler()
{

}
#endif

#if CAN2_IRQ
extern "C" void CAN2_RX0_IRQHandler()
{

}
#endif

#if CAN2_IRQ
extern "C" void CAN2_RX1_IRQHandler()
{

}
#endif

#if CAN2_IRQ
extern "C" void CAN2_SCE_IRQHandler()
{

}
#endif

#if CAN2_IRQ
extern "C" void CAN2_TX_IRQHandler()
{

}
#endif

#if CEC_IRQ
extern "C" void CEC_IRQHandler()
{

}
#endif

#if DCMI_IRQ
extern "C" void DCMI_IRQHandler()
{

}
#endif

#if DMA1_IRQ
extern "C" void DMA1_Stream0_IRQHandler()
{

}
#endif

#if DMA1_IRQ
extern "C" void DMA1_Stream1_IRQHandler()
{

}
#endif

#if DMA1_IRQ
extern "C" void DMA1_Stream2_IRQHandler()
{

}
#endif

#if DMA1_IRQ
extern "C" void DMA1_Stream3_IRQHandler()
{

}
#endif

#if DMA1_IRQ
extern "C" void DMA1_Stream4_IRQHandler()
{

}
#endif

#if DMA1_IRQ
extern "C" void DMA1_Stream5_IRQHandler()
{

}
#endif

#if DMA1_IRQ
extern "C" void DMA1_Stream6_IRQHandler()
{

}
#endif

#if DMA1_IRQ
extern "C" void DMA1_Stream7_IRQHandler()
{

}
#endif

#if DMA2_IRQ
extern "C" void DMA2_Stream0_IRQHandler()
{

}
#endif

#if DMA2_IRQ
extern "C" void DMA2_Stream1_IRQHandler()
{

}
#endif

#if DMA2_IRQ
extern "C" void DMA2_Stream2_IRQHandler()
{

}
#endif

#if DMA2_IRQ
extern "C" void DMA2_Stream3_IRQHandler()
{

}
#endif

#if DMA2_IRQ
extern "C" void DMA2_Stream4_IRQHandler()
{

}
#endif

#if DMA2_IRQ
extern "C" void DMA2_Stream5_IRQHandler()
{

}
#endif

#if DMA2_IRQ
extern "C" void DMA2_Stream6_IRQHandler()
{

}
#endif

#if DMA2_IRQ
extern "C" void DMA2_Stream7_IRQHandler()
{

}
#endif

#if EXTI0_IRQ
extern "C" void EXTI0_IRQHandler()
{

}
#endif

#if EXTI1_IRQ
extern "C" void EXTI1_IRQHandler()
{

}
#endif

#if (EXTI10_IRQ | EXTI11_IRQ | EXTI12_IRQ | EXTI13_IRQ | EXTI14_IRQ | EXTI15_IRQ)
extern "C" void EXTI15_10_IRQHandler()
{

}
#endif

#if EXTI2_IRQ
extern "C" void EXTI2_IRQHandler()
{

}
#endif

#if EXTI3_IRQ
extern "C" void EXTI3_IRQHandler()
{

}
#endif

#if EXTI4_IRQ
extern "C" void EXTI4_IRQHandler()
{

}
#endif

#if (EXTI5_IRQ | EXTI6_IRQ | EXTI7_IRQ | EXTI8_IRQ | EXTI9_IRQ)
extern "C" void EXTI9_5_IRQHandler()
{

}
#endif

#if FLASH_IRQ
extern "C" void FLASH_IRQHandler()
{

}
#endif

#if FMC_IRQ
extern "C" void FMC_IRQHandler()
{

}
#endif

#if FMPI2C1_IRQ
extern "C" void FMPI2C1_Error_IRQHandler()
{

}
#endif

#if FMPI2C1_IRQ
extern "C" void FMPI2C1_Event_IRQHandler()
{

}
#endif

#if FPU_IRQ
extern "C" void FPU_IRQHandler()
{

}
#endif

#if I2C1_IRQ
extern "C" void I2C1_ER_IRQHandler()
{

}
#endif

#if I2C1_IRQ
extern "C" void I2C1_EV_IRQHandler()
{

}
#endif

#if I2C2_IRQ
extern "C" void I2C2_ER_IRQHandler()
{

}
#endif

#if I2C2_IRQ
extern "C" void I2C2_EV_IRQHandler()
{

}
#endif

#if I2C3_IRQ
extern "C" void I2C3_ER_IRQHandler()
{

}
#endif

#if I2C3_IRQ
extern "C" void I2C3_EV_IRQHandler()
{

}
#endif

#if OTG_FS_IRQ
extern "C" void OTG_FS_IRQHandler()
{

}
#endif

#if OTG_FS_IRQ
extern "C" void OTG_FS_WKUP_IRQHandler()
{

}
#endif

#if OTG_HS_IRQ
extern "C" void OTG_HS_EP1_IN_IRQHandler()
{

}
#endif

#if OTG_HS_IRQ
extern "C" void OTG_HS_EP1_OUT_IRQHandler()
{

}
#endif

#if OTG_HS_IRQ
extern "C" void OTG_HS_IRQHandler()
{

}
#endif

#if OTG_HS_IRQ
extern "C" void OTG_HS_WKUP_IRQHandler()
{

}
#endif

#if PVD_IRQ
extern "C" void PVD_IRQHandler()
{

}
#endif

#if QuadSPI_IRQ
extern "C" void QuadSPI_IRQHandler()
{

}
#endif

#if RCC_IRQ
extern "C" void RCC_IRQHandler()
{

}
#endif

#if RTC_IRQ
extern "C" void RTC_Alarm_IRQHandler()
{

}
#endif

#if RTC_IRQ
extern "C" void RTC_WKUP_IRQHandler()
{

}
#endif

#if SAI1_IRQ
extern "C" void SAI1_IRQHandler()
{

}
#endif

#if SAI2_IRQ
extern "C" void SAI2_IRQHandler()
{

}
#endif

#if SDIO_IRQ
extern "C" void SDIO_IRQHandler()
{

}
#endif

#if SPDIF_RX_IRQ
extern "C" void SPDIF_RX_IRQHandler()
{

}
#endif

#if SPI1_IRQ
extern "C" void SPI1_IRQHandler()
{

}
#endif

#if SPI2_IRQ
extern "C" void SPI2_IRQHandler()
{

}
#endif

#if SPI3_IRQ
extern "C" void SPI3_IRQHandler()
{

}
#endif

#if SPI4_IRQ
extern "C" void SPI4_IRQHandler()
{

}
#endif

#if TAMP_STAMP_IRQ
extern "C" void TAMP_STAMP_IRQHandler()
{

}
#endif

#if (TIM1_IRQ | TIM9_IRQ)
extern "C" void TIM1_BRK_TIM9_IRQHandler()
{

}
#endif

#if TIM1_IRQ
extern "C" void TIM1_CC_IRQHandler()
{

}
#endif

#if (TIM1_IRQ | TIM11_IRQ)
extern "C" void TIM1_TRG_COM_TIM11_IRQHandler()
{

}
#endif

#if (TIM1_IRQ | TIM10_IRQ)
extern "C" void TIM1_UP_TIM10_IRQHandler()
{

}
#endif

#if TIM2_IRQ
extern "C" void TIM2_IRQHandler()
{

}
#endif

#if TIM3_IRQ
extern "C" void TIM3_IRQHandler()
{

}
#endif

#if TIM4_IRQ
extern "C" void TIM4_IRQHandler()
{

}
#endif

#if TIM5_IRQ
extern "C" void TIM5_IRQHandler()
{

}
#endif

#if (TIM6_IRQ | DAC1_IRQ | DAC2_IRQ)
extern "C" void TIM6_DAC_IRQHandler()
{

}
#endif

#if TIM7_IRQ
extern "C" void TIM7_IRQHandler()
{

}
#endif

#if (TIM8_IRQ | TIM12_IRQ)
extern "C" void TIM8_BRK_TIM12_IRQHandler()
{

}
#endif

#if TIM8_IRQ
extern "C" void TIM8_CC_IRQHandler()
{

}
#endif

#if (TIM8_IRQ | TIM14_IRQ)
extern "C" void TIM8_TRG_COM_TIM14_IRQHandler()
{

}
#endif

#if (TIM8_IRQ | TIM13_IRQ)
extern "C" void TIM8_UP_TIM13_IRQHandler()
{

}
#endif

#if UART4_IRQ
extern "C" void UART4_IRQHandler()
{

}
#endif

#if UART5_IRQ
extern "C" void UART5_IRQHandler()
{

}
#endif

#if USART1_IRQ
extern "C" void USART1_IRQHandler()
{

}
#endif

#if USART2_IRQ
extern "C" void USART2_IRQHandler()
{

}
#endif

#if USART3_IRQ
extern "C" void USART3_IRQHandler()
{

}
#endif

#if USART6_IRQ
extern "C" void USART6_IRQHandler()
{

}
#endif

#if WWDG_IRQ
extern "C" void WWDG_IRQHandler()
{

}
#endif
