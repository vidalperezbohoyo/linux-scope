#pragma once

#include "VideoProcessor.h"

class HUD
{

enum class CrosshairType
{
    NONE,
    SNIPER_LIKE,
    GREEN_DOT
};

struct Configuration
{
    CrosshairType crosshair;
    uint8_t zoom;
    uint8_t brightness_percent;
}

public:
    HUD();
    ~HUD();

};