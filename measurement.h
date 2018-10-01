#pragma once
class Measurement
{
public:
    static void measure();
    static int getMotorVoltage();
    static int getMotorInputVoltage();
    static int get24VVoltage();
    static int getCp1Voltage();
    static int getcp2Voltage();
    static int getMotorCurrent();
private :
    static int mv_motor;
    static int mv_motor_input;
    static int mv_24V;
    static int mv_cp1;
    static int mv_cp2;
    static int a_motor;
};
