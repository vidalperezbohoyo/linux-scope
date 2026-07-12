#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "BaseController.h"

class KeyboardController : public BaseController
{
public:
    KeyboardController();
    ~KeyboardController();

    void loop() override;
};