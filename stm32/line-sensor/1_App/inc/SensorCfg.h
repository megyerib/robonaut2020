#pragma once

// SENSOR CONFIGURATION CHECKER
//
// Values can be set in the build configuration. This file only checks if the
// configuration is valid and generates a compilation time error if not.
// SENSOR_REV  : Hardware revision
//               1 : 2019
//               2 : 2020
// SENSOR_SIZE : Number of TCRT5000 sensors (24 or 32)

// CONFIGURATION VALIDITY CHECK ----------------------------------------------------------------------------------------

#if (SENSOR_REV == 1 && SENSOR_SIZE == 32)
	#define SENSOR_CONF_VALID
#endif

#if (SENSOR_REV == 2 && SENSOR_SIZE == 24)
	#define SENSOR_CONF_VALID
#endif

#if (SENSOR_REV == 2 && SENSOR_SIZE == 32)
	#define SENSOR_CONF_VALID
#endif

#ifndef SENSOR_CONF_VALID
#error "Sensor configuration invalid!"
#endif

// CONFIGURATION -------------------------------------------------------------------------------------------------------

#define IR_DIST_MM          9                               /* X distance of the IR sensors */
#define MID_IR_POS_MM       (IR_DIST_MM*(SENSOR_SIZE-1)/2)  /* Distance from the leftmost sensor to the middle of the sensor*/
#define GROUP_SIZE          8
