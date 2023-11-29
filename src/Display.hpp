#pragma once
#include "Common.hpp"

class Display
{
public:
    Display(LovyanGFX& gfx) :
        m_gfx{gfx}
    {
        clear(0,0,128);
    }

    void clear(uint8_t r, uint8_t g, uint8_t b)
    {
        m_gfx.clear(m_gfx.color888(r, g, b));
    }

private:
    LovyanGFX& m_gfx;
};
