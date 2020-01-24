#pragma once

#include "Stm32Uart.h"

#define UART_HEADER   255

typedef enum
{
    ToF_Distance = 0,
    Car_Speed,
    Car_Mode,
    Car_State,
    Line_Pos,
    Encoder_Cnt,
    Servo_Front_Angle,
    Servo_Rear_Angle,
    Servo_Sensor_Angle
} MsgType;

typedef union
{
    float    floating;
    uint8_t  bin[4];
} TypeConverter;

class StarterUart : public Stm32Uart
{
	uint8_t rxChar = 0;

	StarterUart();

	void TxCompleteCallback() override;
	void RxCompleteCallback() override;
	void Init();

public:
	static StarterUart* GetInstance();

	uint8_t GetLastChar();
	void    SendByte(uint8_t byte);
	void    SendWord(uint16_t word);
	void    SendDWord(uint32_t dword);
	void    SendFloat(float value);
};
