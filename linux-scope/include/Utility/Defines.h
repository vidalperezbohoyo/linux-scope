#pragma once

#if defined(BUILD_X86)
    //
#elif defined(BUILD_LUCKFOX_PICO)
    // Video Input Parameters
    #define VIDEO_IN_WIDTH 640
    #define VIDEO_IN_HEIGHT 480

    // Auto Exposure Algorithm Parameters
    #define VIDEO_AUTO_EXPOSURE_TARGET_LUMINANCE 128 // 0 to 255
    #define VIDEO_AUTO_EXPOSURE_ROI_WIDTH 200 // 0 to VIDEO_IN_WIDTH
    #define VIDEO_AUTO_EXPOSURE_ROI_HEIGHT 200 // 0 to VIDEO_IN_HEIGHT
    #define VIDEO_AUTO_EXPOSURE_FRAMES_BETWEEN_ITERATIONS 10 // How many frames between adjustements. Range: 1(fast adjustment-High CPU) to 255(slow adjustment-LOW CPU)

#endif


