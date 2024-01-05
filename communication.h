#include <Wire.h>
#include "millisTimer.hpp"


#define I2C_MOTOR_CTRL_ADDRESS 0xF2

enum class Claw_Controll_State : uint8_t
{
    CLAW_CONTROLL_STATE_IDLE    = 0b00000000,
    CLAW_CONTROLL_STATE_BUTTON  = 0b10000000,
    CLAW_CONTROLL_STATE_LEFT    = 0b00000001,
    CLAW_CONTROLL_STATE_RIGHT   = 0b00000010,
    CLAW_CONTROLL_STATE_UP      = 0b00000100,
    CLAW_CONTROLL_STATE_DOWN    = 0b00001000,
    CLAW_CONTROLL_STATE_ERROR   = 0b00010000
};

Claw_Controll_State operator|(Claw_Controll_State left, Claw_Controll_State right)
{
    return static_cast<Claw_Controll_State>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

Claw_Controll_State operator&(Claw_Controll_State left, Claw_Controll_State right)
{
    return static_cast<Claw_Controll_State>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}

Claw_Controll_State operator~(Claw_Controll_State input)
{
    return static_cast<Claw_Controll_State>(~static_cast<uint8_t>(input));
}


enum class Claw_Calibration : uint8_t
{
    CLAW_CALIB_IDLE_STATE =             0b00000000,
    CLAW_CALIB_INIT =                   0b00000001,
    CLAW_CALIB_TOP_STATE_IN_PROGRESS =  0b00000010,
    CLAW_CALIB_DOWN_STATE_IN_PROGRESS = 0b00000100,
    CLAW_CALIB_BAD =                    0b00100000,
    CLAW_CALIB_TOP_DONE =               0b01000000,
    CLAW_CALIB_DOWN_DONE =              0b10000000
};

Claw_Calibration operator|(Claw_Calibration left, Claw_Calibration right)
{
    return static_cast<Claw_Calibration>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

Claw_Calibration operator&(Claw_Calibration left, Claw_Calibration right)
{
    return static_cast<Claw_Calibration>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}

Claw_Calibration operator~(Claw_Calibration input)
{
    return static_cast<Claw_Calibration>(~static_cast<uint8_t>(input));
}