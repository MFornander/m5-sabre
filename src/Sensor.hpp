#pragma once
#include "Common.hpp"

class Sensor
{
public:
    Sensor(m5::IMU_Class& imu) :
        m_imu{imu}
    {
    }

    bool calibrate()
    {
        return false;
    }

    void update()
    {
    }  

private:
    m5::IMU_Class& m_imu;
};
