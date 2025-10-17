#include "KeyboardController.h"

keyboardController::keyboardController()
{
}

keyboardController::~keyboardController()
{
}

void keyboardController::loop()
{

    KeyEvent event = KeyEvent::NONE;

    uint8_t ch = getchar();
    
    if (ch == 27)
    { // ESC
        char next1 = getchar();
        if (next1 == 91) 
        { // '['
            char next2 = getchar();
            switch (next2) {
                case 'A': event = KeyEvent::UP; break;
                case 'B': event = KeyEvent::DOWN; break;
                case 'C': event = KeyEvent::RIGHT; break;
                case 'D': event = KeyEvent::LEFT; break;
            }
        }
    } 
    else if (ch == 'q') 
    {
        event = KeyEvent::QUIT;
    }
    
    if (key_event_callback_ && event != KeyEvent::NONE)
    {
        key_event_callback_(event);
    }
}