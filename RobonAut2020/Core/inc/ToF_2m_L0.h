/*
 * ToF_2m_L0.h
 *
 *  Created on: Oct 23, 2019
 *      Author: Joci
 */

#ifndef TOF_2M_L0_H_
#define TOF_2M_L0_H_

#include "OVERRIDE_Types.h"
#include "vl53l0x_api.h"
#include "i2c.h"

class ToF_2m
{
private:
    VL53L0X_RangingMeasurementData_t distance;
	U32                  cntr;

	VL53L0X_Dev_t        devStruct;
	VL53L0X_DEV          myDevice = &devStruct;
	VL53L0X_DeviceInfo_t myDeviceInfo;
	VL53L0X_Error        status;

public:
	ToF_2m()
    {
	    cntr     = 0;
    }

	void Init()
	{
	    //HAL_GPIO_WritePin(ToF_XSDN_GPIO_Port, ToF_XSDN_Pin, GPIO_PIN_SET);

	    myDevice->I2cDevAddr      = 0x08;
	    myDevice->comms_type      = 1;
	    myDevice->comms_speed_khz = 400;

	    status = VL53L0X_DataInit(myDevice);
	}

	void GetDevcInfo()
	{
	    VL53L0X_PowerModes pwMode;

	    status = VL53L0X_GetPowerMode(myDevice, &pwMode);
	    //status = VL53L0X_SetDeviceAddress(myDevice, myDevice->I2cDevAddr);
	    //status = VL53L0X_GetDeviceInfo(myDevice, &myDeviceInfo);
	}

	void GetDistance()
	{
	    VL53L0X_PerformSingleMeasurement(myDevice);
	    VL53L0X_GetRangingMeasurementData(myDevice, &distance);
	}

	void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
	    cntr++;
	    //HAL_I2C_Mem_Read_IT(&hi2c1, 0x52, 0xC0, 1, (uint8_t*)distance, 1);
	}

    void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
    {
        cntr--;
        //HAL_I2C_Mem_Read_IT(&hi2c1, 0x52, 0xC0, 1, (uint8_t*)distance, 1);
    }
};


#endif /* TOF_2M_L0_H_ */
