#include "VideoProvider.h"
#include "VideoProcessor.h"
#include "KeyboardController.h"

#ifdef USE_CV_DISPLAY
    #include "CvDisplay.h"
#endif

#ifdef USE_FB_DISPLAY
    #include "FbDisplay.h"
#endif

void setNonBlockingInput(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);           
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);         
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
    }
}

int main() 
{
    setNonBlockingInput(true);

#ifdef USE_CV_DISPLAY
    CvDisplay display;
#endif

#ifdef USE_FB_DISPLAY
    FbDisplay display;
#endif

    if (!display.init()) 
    {
        std::cerr << "Failed to initialize display." << std::endl;
        return -1;
    }

    VideoProcessor processor;
    if (!processor.init()) 
    {
        std::cerr << "Failed to initialize video processor." << std::endl;
        return -1;
    }

    processor.setImageCallback([&display](const cv::Mat& frame) 
    {
        display.draw(frame);
    });

    processor.start();

    KeyboardController controller;
    controller.setKeyEventCallback([&processor](KeyEvent event) 
    {
        int zoom_offset_x = 0;
        int zoom_offset_y = 0;

        if (event == KeyEvent::QUIT) 
        {
            processor.stop();
        }
    });
    controller.start();

    while(true) {usleep(100000);}

    return 0;
}