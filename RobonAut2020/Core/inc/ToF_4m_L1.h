/*
 * ToF_4m_L1.h
 *
 *  Created on: 2019. nov. 24.
 *      Author: Joci
 */

#ifndef TOF_4M_L1_H_
#define TOF_4M_L1_H_

#include "OVERRIDE_Types.h"

#include "vl53l1_api.h"
#include "vl53l1_platform.h"
#include "vl53l1_def.h"

class TOF_L1
{
private:
    VL53L1_Dev_t    dev;
    VL53L1_DEV      Dev = &dev;

    VL53L1_Error    status;
    U32             timingBudget_ms;
    VL53L1_RangingMeasurementData_t RangingData;

    GPIO_TypeDef*   XSDN_Port;
    U16             XSDN_Pin;

public:
    TOF_L1();
    TOF_L1(U8                   const Addr,
           U16                  const Speed,
           I2C_HandleTypeDef*   const Hi2c,
           U32                  const TB_ms,
           GPIO_TypeDef*        const XsdnPort,
           U16                  const XsdnPin);

    void Init();
    void Process();

    S16  GetDistance_mm(void);

    void Calibrate_Offset_300mm(void);
};

#endif /* TOF_4M_L1_H_ */
