#include "Steering.h"
#include "Defines.h"

#define SINGLE_SLOW_P   (15.0f)
#define SINGLE_SLOW_D   (3.0f)
#define SINGLE_FAST_P   (7.0f)
#define SINGLE_FAST_D   (1.4f)

Steering::Steering()
{
	InitEnablePin();
	EnableSteering(false);

	front.servo      = new Servo(eTIM12, TIM_CHANNEL_2);
	front.controller = new Pd_Controller(0.5f, 0.1f);
	front.line       = 0.0f;
	front.angle      = 0.0f;
	front.servo->Enable();

	rear.servo       = new Servo(eTIM12, TIM_CHANNEL_1);
	rear.controller  = new Pd_Controller(0.5f, 0.1f);
	rear.line        = 0.0f;
	rear.angle       = 0.0f;
	rear.servo->Enable();

	mode = SteeringMode::Off;
}

Steering* Steering::GetInstance()
{
    static Steering instance;
    return &instance;
}

void Steering::SetMode(SteeringMode mode)
{
    this->mode = mode;

    switch (mode)
    {
        case DualLineFollow_Slow:
        case SingleLineFollow_Slow:
        {
            front.controller->Set_P_Term(SINGLE_SLOW_P);
            front.controller->Set_D_Term(SINGLE_SLOW_D);
            break;
        }
        case DualLineFollow_Fast:
        case SingleLineFollow_Fast:
        {
            front.controller->Set_P_Term(SINGLE_FAST_P);
            front.controller->Set_D_Term(SINGLE_FAST_D);
            break;
        }
        default:
        {

            break;
        }
    }
}

void Steering::SetLine(float front_line, float rear_line)
{
    front.line = front_line;
    rear.line  = rear_line;
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
	//SetFrontAngle(front_angle);
	//SetRearAngle(rear_angle);

	front.angle = front_angle;
	rear.angle = rear_angle;
}

void Steering::Process()
{
    switch (mode)
    {
		case Off:
		{
		    // TODO servo enable pins.
			front.servo->Disable();
			rear.servo->Disable();
			break;
		}
    	case SingleLineFollow_Slow:
    	case SingleLineFollow_Fast:
        {
            front.controller->Process(front.line);

            SetFrontAngle(front.controller->GetControlValue());
            SetRearAngle(0.0f);
            break;
        }
    	case DualLineFollow_Slow:
    	case DualLineFollow_Fast:
    	{
    	    front.controller->Process(front.line);
    	    //rear.controller->Process(rear.line);

    	    SetFrontAngle(front.controller->GetControlValue());
    	    SetRearAngle(-front.controller->GetControlValue());
    	    break;
    	}
        case Free:
        {
        	SetFrontAngle(front.angle);
        	SetRearAngle(rear.angle);

            break;
        }
    }
}

void Steering::SetFrontAngle(float angle /* rad */)
{
	float offset        = PI/2.0f;
	float scale         = 1.0f;
	float servo_angle   = (angle + offset) * scale;

	front.servo->SetSteerAngle(servo_angle);
}

void Steering::SetRearAngle(float angle /* rad */)
{
	float offset        = PI/2.0f;
	float scale         = 1.0f;
	float servo_angle   = (angle + offset) * scale;

	rear.servo->SetSteerAngle(servo_angle);
}

float Steering::GetFrontAngle()
{
    float angle;
    float offset        = PI/2.0f;

    angle = front.servo->GetSteerAngle() - offset;

    return angle;
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
