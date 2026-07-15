#include "Camera/VideoProvider.h"
#include "UserInput/KeyboardController.h"
#include "Display/HUD.h"
#include "Utility/Log.h"

#if defined(BUILD_X86)
#include "Display/CvDisplay.h"
#elif defined(BUILD_LUCKFOX_PICO)
#include "Display/FbDisplay.h"
#endif



// void setNonBlockingInput(bool enable) {
//     static struct termios oldt, newt;
//     if (enable) {
//         tcgetattr(STDIN_FILENO, &oldt);           
//         newt = oldt;
//         newt.c_lflag &= ~(ICANON | ECHO);         
//         tcsetattr(STDIN_FILENO, TCSANOW, &newt);

//         fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
//     } else {
//         tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
//     }
// }

int main() 
{
    Log::instance().set_level(Log::Level::Debug);

#if defined(BUILD_X86)
    auto& display = CvDisplay::instance();
#elif defined(BUILD_LUCKFOX_PICO)
    auto& display = FbDisplay::instance();
#endif

    if (!display.init()) 
    {
        Log::instance().error("Failed to initialize display.");
        return -1;
    }

    // Example draw green screen
    cv::Mat greenScreen(240, 240, CV_8UC3, cv::Scalar(0, 255, 0));
    display.draw(greenScreen);

    return 0;
}