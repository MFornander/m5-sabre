#include "Common.hpp"
#include "Display.hpp"
#include "Light.hpp"
#include "Sensor.hpp"

std::shared_ptr<Display> display{};
std::shared_ptr<Sensor> sensor{};
std::shared_ptr<Light> light{};

void setup(void)
{
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Log.print("Sabre setup... ");
    M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_VERBOSE);
    M5.Log.setEnableColor(m5::log_target_serial, true);

    display = std::make_shared<Display>(M5.Display);
    sensor = std::make_shared<Sensor>(M5.Imu);
    light = std::make_shared<Light>();

    M5.BtnA.setHoldThresh(200);
    M5.delay(1000);
    M5.Log.println("success!");

    Vector3f const v3(1,2,3);
    M5_LOGI("v3: %f,%f,%f", v3[0], v3[1], v3[2]);
}

void loop(void)
{
    M5.delay(1);
    M5.update();

    sensor->update();
    light->update();


    if (M5.BtnA.wasDecideClickCount())
    {
        switch (M5.BtnA.getClickCount())
        {
            case 1:
                M5_LOGV("Show light effect");
                display->clear(rand() % 4 * 63, rand() % 4 * 63, rand() % 4 * 63);
                break;

            case 2:
                M5_LOGI("Calibrate: begin...");
                if (sensor->calibrate())
                {
                    M5_LOGI("Calibrate: success");
                    display->clear(0,128,0);
                }
                else
                {
                    M5_LOGE("Calibrate: FAIL");
                    display->clear(128,0,0);
                }
                break;

            default:
                M5_LOGD("Button click %d", M5.BtnA.getClickCount());
                break;
        }
    }
}
