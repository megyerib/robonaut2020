#pragma once

// SENSOR CONFIGURATION CHECKER
//
// Values can be set in the build configuration. This file only checks if the
// configuration is valid and generates a compilation time error if not.
// SENSOR_REV  : Hardware revision
//               1 : 2019
//               2 : 2020
// SENSOR_SIZE : Number of TCRT5000 sensors (24 or 32)

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
