#include <Arduino.h>
//#include <SPIFFS.h>
#include <M5Unified.h>

void setup(void)
{
    auto cfg = M5.config();
    M5.begin(cfg);
    //M5.setPrimaryDisplayType({m5::board_t::board_M5AtomS3});

    M5.Log.setEnableColor(m5::log_target_serial, true);
    M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_VERBOSE);

    M5.Display.print("hello, world");

    M5.delay(1000);
    M5_LOGI("hello, world");
}

void draw(LovyanGFX* gfx)
{
    int const x{rand() % gfx->width()};
    int const y{rand() % gfx->height()};
    int const r{(gfx->width() >> 4) + 2};
    int const c{rand()};

    gfx->fillRect(x-r, y-r, r*2, r*2, c);
}

void clear(LovyanGFX* gfx)
{
    gfx->clear(gfx->color888(0, 255, 0));
}

void loop(void)
{
    M5.delay(1);
    M5.update();

    if (M5.BtnA.wasClicked())
    {
        M5_LOGD("Button click %d", M5.BtnA.getClickCount());
    }

    clear(&M5.Display);
}
