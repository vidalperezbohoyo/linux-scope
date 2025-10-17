#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "VideoProvider.h"
#include "VideoProcessor.h"

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

    int zoom_offset_x = 0;
    int zoom_offset_y = 0;
    while(true)
    {
        uint8_t ch = getchar();
        if (ch == 27) { // ESC
            char next1 = getchar();
            if (next1 == 91) { // '['
                char next2 = getchar();
                switch (next2) {
                    case 'A': std::cout << "↑\n"; zoom_offset_y--; break;
                    case 'B': std::cout << "↓\n"; zoom_offset_y++;break;
                    case 'C': std::cout << "→\n"; zoom_offset_x++;break;
                    case 'D': std::cout << "←\n"; zoom_offset_x--;break;
                }
                processor.setZoomOffset(zoom_offset_x, zoom_offset_y); // Reset offsets on arrow key press
            }
        } else if (ch == 'q') {
            std::cout << "Exit...\n";
            break;
        }
        usleep(100); // Para no saturar la CPU
    }

    return 0;
}