#pragma once

#include "vl53l1_api.h"
#include "vl53l1_platform.h"
#include "vl53l1_def.h"

#include "Tof_I2c.h"

class TOF_L1
{
private:
    ToF_I2c*        i2c;

    VL53L1_Dev_t    dev;
    VL53L1_DEV      Dev = &dev;

    VL53L1_Error    status;
    uint32_t        timingBudget_ms;
    VL53L1_RangingMeasurementData_t RangingData;

    GPIO_TypeDef*   XSDN_Port;
    uint16_t        XSDN_Pin;

public:
    TOF_L1();
    TOF_L1(uint8_t              const Addr,
           uint16_t             const Speed,
           I2C_HandleTypeDef*   const Hi2c,
           uint32_t             const TB_ms,
           GPIO_TypeDef*        const XsdnPort,
           uint16_t             const XsdnPin);

    void Init();
    void Process();

    uint16_t GetDistance_mm(void);

    void Calibrate_Offset_300mm(void);
};
