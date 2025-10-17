#pragma once
#include <functional>
#include "Thread.h"

enum class KeyEvent
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    QUIT
};

class BaseController : public Thread
{
public:
    virtual ~BaseController() = default;

    void loop() override = 0;

    void setKeyEventCallback(std::function<void(KeyEvent)> callback)
    {
        key_event_callback_ = callback;
    }

protected:
    std::function<void(KeyEvent)> key_event_callback_;
};