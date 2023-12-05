#include "Common.hpp"
#include "Display.hpp"
#include "Light.hpp"
#include "Locator.hpp"

std::shared_ptr<Display> display{};
std::shared_ptr<Locator> locator{};
std::shared_ptr<Light> light{};

void setup(void)
{
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Log.println("Sabre setup: begin... ");
    M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_VERBOSE);
    M5.Log.setEnableColor(m5::log_target_serial, true);

    display = std::make_shared<Display>(M5.Display);
    locator = std::make_shared<Locator>(M5.Imu);
    light = std::make_shared<Light>();

    M5.BtnA.setHoldThresh(200);
    M5.delay(1000);
    M5.Log.println("Sabre setup: success");

    Vector3f const v3(1,2,3);
    M5_LOGI("v3: %f,%f,%f", v3[0], v3[1], v3[2]);
}

void loop(void)
{
    M5.delay(1);
    M5.update();

    locator->update();
    light->update();

    auto const& pos{locator->getPosition()};
    M5_LOGI("pos: %f, %f, %f", pos[0], pos[1], pos[2]);
    display->clear(pos[0], pos[1], pos[2]);

    if (M5.BtnA.wasDecideClickCount())
    {
        switch (M5.BtnA.getClickCount())
        {
            case 1:
                M5_LOGV("Show light effect");
                display->clear(rand() % 4 * 63, rand() % 4 * 63, rand() % 4 * 63);
                break;

            case 2:
                locator->calibrate();
                display->clear(0,128,0);
                break;

            default:
                M5_LOGD("Button click %d", M5.BtnA.getClickCount());
                break;
        }
    }
}
