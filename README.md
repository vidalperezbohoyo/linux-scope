# linux-scope (Work in progress)
A light-weight linux sniper scope with practical cool features.

Features:
- Digital zoom (3x or 4x plus the optical zoom)
- Imperceptible delay
- Configurable scope shapes
- Software calibration for different ranges and zooms
- IR night-vision control
- Screen brightness regulation
- Contrast and other image adjusts.
- Easy to make ;)

> Note:   
This proyect can run without hardware for testing and developing  
-> Uses the Webcam as **camera source**   
-> Uses the Keyboard as **buttons**  
->  Uses display as **display**  
For that, add the compilation flag **EMULATED_HARDWARE** to CMakeLists.txt
## My setup
- CCTV IP camera:
    - GOKE
    - Running OpenIPC
    - Configured at 720p 60fps
    - Changed the original lens to a 25mm focal lenght lens to have initial optical zoom.
- LCD:
- Single board computer: 
    - Ubuntu 22.04 lts server
    - Orange Pi Zero
- 5 Physical buttons connected to GPIOs
- IR Flashlight

This setup has a **glass to glass** latency 

## Screen
I am using a SPI screen connected to the PC. To 

> You can use other screens and by modifing this code.

## Camera source
Previous considerations:  
1. I recommend a **fixed zoom** camera for exactitude.
2. Use a camera that reach **60fps** on 720p/1080p
3. Use high zoom lenses as 25mm focal lenght
### CCTV camera
This PCBs can be found on Aliexpress or Amazon really cheap. They send H264 or H265 via ethernet instead of USB. Usually they have good low-light capabilities because they are used as survillance cameras.
#### Running OpenIPC firmware (optional, but recommended)
Open IPC is a custom open firmware to substitute the original privative firmware inside the CCTV camera.

### USB cameras
You can use common USB cameras but buy the ones that lenses can be changed with very focal ones. Fixed usb cameras has no enought magnification.

### IR filter
Your phone camera (and every normal camera) has a IR filter that blocks infrared light. To use night-vision you use invisible infrared light to iluminate the scene. So your camera must not have that IR filter.
Some CCTV cameras has a IR-CUT filter that can be activated or not to change between normal camera colors to see also the infrared spectrum.
Most of the camera lenses and sensors have this filter glued to the internal lens and can be removed with spetial care. With this modifications all colors will be altered but now you can see the infrared spectrum. So you can modify USB cameras and so. 

## Crop in input video
240x240 is the ***round screen*** resolution.  

- If input video is **1080p**, is crop centered to (960x960) (240px with 4x optical zoom).  
- If input video is **720p**, is crop centered to (720x720) (240px with 3x optical zoom).  

You sould configure the camera to have at least **60fps** and the better resolution possible at that speed for a better experience.  
