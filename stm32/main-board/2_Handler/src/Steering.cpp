#include "Steering.h"
#include "Defines.h"

Steering::Steering()
{
	InitEnablePin();
	EnableSteering(false);

	srv_front = new Servo(eTIM12, TIM_CHANNEL_2);
    srv_rear  = new Servo(eTIM12, TIM_CHANNEL_1);

    srv_front->Enable();
    srv_rear->Enable();
}

Steering* Steering::GetInstance()
{
    static Steering instance;
    return &instance;
}

void Steering::SetMode(SteeringMode mode)
{
    this->mode = mode;
}

void Steering::SetLine(float front_line, float rear_line)
{

}

//! Steering wheel:
//!
//!                 0 rad
//!        0.52 rad   |   -0.52 rad
//!               \   |   /
//!                \  |  /
//!                 \ | /
//!  1.57 rad________\|/________-1.57 rad
//!  Left end                   Right end
void Steering::SetAngleManual(float front_angle, float rear_angle)
{
	this->front_angle = front_angle;
	this->rear_angle  = rear_angle;
}

void Steering::Process()
{
    switch (mode)
    {
		case Off:
		{
			// Disable servos
			break;
		}
    	case SingleLineFollow:
        {

            break;
        }
        case Free:
        {
        	SetFrontAngle(front_angle);
        	SetRearAngle(rear_angle);

            break;
        }
    }
}

void Steering::SetFrontAngle(float angle /* rad */)
{
	float offset        = PI/2.0f;
	float scale         = 1.0f;
	float servo_angle   = (angle + offset) * scale;

	srv_front->SetSteerAngle(servo_angle);
}

void Steering::SetRearAngle(float angle /* rad */)
{
	float offset        = PI/2.0f;
	float scale         = 1.0f;
	float servo_angle   = (angle + offset) * scale;

	srv_rear->SetSteerAngle(servo_angle);
}

void Steering::EnableSteering(bool enable)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void Steering::InitEnablePin()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin    = GPIO_PIN_0;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
