#pragma once
#include "Common.hpp"

class Locator
{
public:
    Locator(m5::IMU_Class& imu) :
        m_imu{imu}
    {
    }

    void calibrate()
    {
        const char* name{""};
        auto imu_type{m_imu.getType()};
        switch (imu_type)
        {
            case m5::imu_none:        name = "not found";   break;
            case m5::imu_sh200q:      name = "sh200q";      break;
            case m5::imu_mpu6050:     name = "mpu6050";     break;
            case m5::imu_mpu6886:     name = "mpu6886";     break;
            case m5::imu_mpu9250:     name = "mpu9250";     break;
            case m5::imu_bmi270:      name = "bmi270";      break;
            default:                  name = "unknown";     break;
        };
        M5_LOGD("Begin Calibrate Locator IMU: %s", name);
    }

    void update()
    {
        static uint32_t frame_count = 0;
        static uint32_t prev_sec = 0;

        // To update the IMU value, use M5.Imu.update.
        // If a new value is obtained, the return value is non-zero.
        if (M5.Imu.update() != m5::IMU_Class::sensor_mask_none)
        {
            // Obtain data on the current value of the IMU.
            auto const& data{M5.Imu.getImuData()};
            Vector3f const gyroSpeed{m_imuData.gyro.x, m_imuData.gyro.y, m_imuData.gyro.z};
                // m_imuData.gyro.x - m_imuData.gyro.x,
                // m_imuData.gyro.y - m_imuData.gyro.y,
                // m_imuData.gyro.z - m_imuData.gyro.z};
            m_pos += (0.1F * gyroSpeed);
            //m_pos = mathfu::Max(m_pos, Vector3f());
            m_pos[0] = std::max(0.0F, m_pos[0]);
            m_pos[1] = std::max(0.0F, m_pos[1]);
            m_pos[2] = std::max(0.0F, m_pos[2]);
            m_pos[0] = std::min(255.0F, m_pos[0]);
            m_pos[1] = std::min(255.0F, m_pos[1]);
            m_pos[2] = std::min(255.0F, m_pos[2]);
            m_imuData = data;

            //drawGraph(rect_graph_area, data);
            // The data obtained by getImuData can be used as follows.
            // data.accel.x;      // accel x-axis value.
            // data.accel.y;      // accel y-axis value.
            // data.accel.z;      // accel z-axis value.
            // data.accel.value;  // accel 3values array [0]=x / [1]=y / [2]=z.

            // data.gyro.x;      // gyro x-axis value.
            // data.gyro.y;      // gyro y-axis value.
            // data.gyro.z;      // gyro z-axis value.
            // data.gyro.value;  // gyro 3values array [0]=x / [1]=y / [2]=z.

            // data.mag.x;       // mag x-axis value.
            // data.mag.y;       // mag y-axis value.
            // data.mag.z;       // mag z-axis value.
            // data.mag.value;   // mag 3values array [0]=x / [1]=y / [2]=z.

            // data.value;       // all sensor 9values array [0~2]=accel / [3~5]=gyro / [6~8]=mag

            //M5_LOGV("ax:%f  ay:%f  az:%f", data.accel.x, data.accel.y, data.accel.z);
            //M5_LOGV("gx:%f  gy:%f  gz:%f", data.gyro.x , data.gyro.y , data.gyro.z );
            //M5_LOGV("mx:%f  my:%f  mz:%f", data.mag.x  , data.mag.y  , data.mag.z  );
        
            //m_pos = Vector3f{m_imuData.gyro.x, m_imuData.gyro.y, m_imuData.gyro.z};
            ++frame_count;
        }
        else
        {
            //M5.update();

            // Calibration is initiated when a button or screen is clicked.
            // if (M5.BtnA.wasClicked() || M5.BtnPWR.wasClicked() || M5.Touch.getDetail().wasClicked())
            // {
            //     startCalibration();
            // }
        }

        int32_t sec = millis() / 1000;
        if (prev_sec != sec)
        {
            prev_sec = sec;
            M5_LOGI("sec:%d  frame:%d", sec, frame_count);
            frame_count = 0;

            // if (calib_countdown)
            // {
            //     updateCalibration(calib_countdown - 1);
            // }

            if ((sec & 7) == 0)
            { // prevent WDT.
                vTaskDelay(1);
            }
        }
    }

    Vector3f const& getPosition()
    {
        return m_pos;
    }  

private:
    m5::IMU_Class& m_imu;
    m5::IMU_Class::imu_data_t m_imuData{};
    Vector3f m_pos{};
};
