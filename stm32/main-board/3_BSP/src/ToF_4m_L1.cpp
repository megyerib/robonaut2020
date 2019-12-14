#include "ToF_4m_L1.h"
#include "FreeRTOS.h"
#include "task.h"


#define TOF_CS1_Pin         GPIO_PIN_15
#define TOF_CS1_GPIO_Port   GPIOC
#define FREE3_LED_Pin       GPIO_PIN_3
#define FREE3_LED_GPIO_Port GPIOB
#define FREE2_Pin           GPIO_PIN_4
#define FREE2_GPIO_Port     GPIOB
#define FREE1_Pin           GPIO_PIN_5
#define FREE1_GPIO_Port     GPIOB

TOF_L1::TOF_L1()
{
    i2c = ToF_I2c::GetInstance();

    InitXsdnGpio();

    Dev->I2cDevAddr      = 0x52;
    Dev->I2cHandle       = i2c->GetHandle();
    Dev->comms_speed_khz = 400;
    Dev->comms_type      = 1;

    status              = VL53L1_ERROR_NONE;
    timingBudget_ms     = 200;

    XSDN_Port           = TOF_CS1_GPIO_Port;
    XSDN_Pin            = TOF_CS1_Pin;
}

TOF_L1::TOF_L1(uint8_t             const Addr,
               uint16_t            const Speed,
               uint32_t            const TB_ms,
               GPIO_TypeDef*       const XsdnPort,
               uint16_t            const XsdnPin)
{
    i2c = ToF_I2c::GetInstance();

    InitXsdnGpio();

    Dev->I2cDevAddr      = Addr;
    Dev->I2cHandle       = i2c->GetHandle();
    Dev->comms_speed_khz = Speed;
    Dev->comms_type      = 1;

    status          = VL53L1_ERROR_NONE;
    timingBudget_ms = TB_ms;

    XSDN_Port = XsdnPort;
    XSDN_Pin  = XsdnPin;
}

void TOF_L1::Init()
{
    uint16_t newAddr = Dev->I2cDevAddr;
    uint16_t wordData = 0x00;

    // Shutdown sensor.
    HAL_GPIO_WritePin(XSDN_Port, XSDN_Pin, GPIO_PIN_RESET);
    vTaskDelay(80);

    // Start the sensor.
    HAL_GPIO_WritePin(XSDN_Port, XSDN_Pin, GPIO_PIN_SET);

    // In case
    status = VL53L1_RdWord(Dev, 0x010F, &wordData);
    if ((status != VL53L1_ERROR_NONE) || (wordData != 0xEACC))
    {
        // default sensor I2C address.
        Dev->I2cDevAddr = 0x52;

        // Test if the device is available on the I2C network.
        status = VL53L1_RdWord(Dev, 0x010F, &wordData);
        if ((status == VL53L1_ERROR_NONE) && (wordData == 0xEACC))
        {
            // Set new address.
            status = VL53L1_SetDeviceAddress(Dev, newAddr);
            Dev->I2cDevAddr = newAddr;
        }
    }

    // Test I2C interface.
    status = VL53L1_RdWord(Dev, 0x0110, &wordData);
    if ((status == VL53L1_ERROR_NONE) && (wordData == 0xCC10))
    {
        status = VL53L1_WaitDeviceBooted(Dev);
        status = VL53L1_DataInit(Dev);
        status = VL53L1_StaticInit(Dev);
        status = VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
        status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, timingBudget_ms * 100);
        status = VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, timingBudget_ms);
        status = VL53L1_StartMeasurement(Dev);
    }
    else
    {
        // Reference register value is wrong.
        status = VL53L1_ERROR_CONTROL_INTERFACE;
    }
}

void TOF_L1::Process()
{
    status = VL53L1_StartMeasurement(Dev);
    status = VL53L1_WaitMeasurementDataReady(Dev);
    if(status == VL53L1_ERROR_NONE)
    {
        // DEB
        HAL_GPIO_WritePin(FREE2_GPIO_Port, FREE2_Pin, GPIO_PIN_SET);

        status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
        if (status == VL53L1_ERROR_NONE)
        {
            uint16_t milimeter = RangingData.RangeMilliMeter;
            if (milimeter > 300)
            {
                HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_RESET);
            }
        }
        else
        {
            HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_RESET);
        }
        status = VL53L1_ClearInterruptAndStartMeasurement(Dev);
    }
    else
    {
        // DEB
        HAL_GPIO_WritePin(FREE2_GPIO_Port, FREE2_Pin, GPIO_PIN_RESET);
    }
}

uint16_t TOF_L1::GetDistance_mm(void)
{
    return RangingData.RangeMilliMeter;
}

void TOF_L1::Calibrate_Offset_300mm(void)
{
    VL53L1_PerformOffsetCalibration(Dev, 300);
}

void TOF_L1::InitXsdnGpio()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure GPIO pin Output Level
    HAL_GPIO_WritePin(GPIOC, TOF_FRONT_XSDN_Pin|TOF_XSDN2_Pin|TOF_XSDN3_Pin, GPIO_PIN_RESET);

    // Configure GPIO pins : PCPin PCPin PCPin PCPin
    GPIO_InitStruct.Pin     = TOF_FRONT_XSDN_Pin|TOF_XSDN2_Pin|TOF_XSDN3_Pin;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

bool TOF_L1::isAddressForgotten()
{
    bool isForgotten = true;
    uint16_t wordData = 0x00;

    status = VL53L1_RdWord(Dev, 0x010F, &wordData);
    if ((status == VL53L1_ERROR_NONE) && (wordData == 0xEACC))
    {
        isForgotten = false;
    }

    return isForgotten;
}

void TOF_L1::Shutdown()
{
    HAL_GPIO_WritePin(XSDN_Port, XSDN_Pin, GPIO_PIN_RESET);
}

void TOF_L1::Startup()
{
    HAL_GPIO_WritePin(XSDN_Port, XSDN_Pin, GPIO_PIN_SET);
}

void TOF_L1::Restart()
{
    Shutdown();
    vTaskDelay(80);
    Startup();
}

VL53L1_Error ChangeAddress(uint8_t newAddr);
