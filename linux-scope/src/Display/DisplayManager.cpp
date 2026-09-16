#include "Display/DisplayManager.h"

bool DisplayManager::init()
{
    #if defined(BUILD_X86)
        BaseDisplay& display = CvDisplay::instance();
    #elif defined(BUILD_LUCKFOX_PICO)
        BaseDisplay& display = FbDisplay::instance();
    #endif

    return display.init();
}


void DisplayManager::setCameraImage(const cv::Mat& image)
{
    {
        std::unique_lock<std::mutex> lock(camera_image_mutex_, std::try_to_lock);

        if (!lock.owns_lock())
        {
            //Log::instance().warning("[DisplayManager::setCameraImage] Could not acquire lock, previous image processing is still ongoing, skipping this frame.");
            return; // Bad thing, optimization: skip this frame, processing is too long
        }

        latest_camera_image_ = image.clone(); // Copy buffer
        latest_camera_image_ready_ = true; // Condition flag
    }

    camera_image_cv.notify_one();
    // This function runs in VideoProvider Thread, so no more code here.
}

void DisplayManager::loop()
{
    // Sleep thread until new image is ready.
    std::unique_lock<std::mutex> lock(camera_image_mutex_); // Unique lock is for condition variable
   
    camera_image_cv.wait(lock, [this] { return latest_camera_image_ready_; });
    
    // The condition variable unlocks the mutex and sleep until condition is met, then locks mutex again
    // Now the mutex is locked in whole processing, so no more image can be set until processing is done.

    // Perform image processing

    Log::instance().debug("[DisplayManager::loop] Received new camera image, processing...");

    // 1. Image is on NV12 format, convert to BGR
    #if defined(BUILD_LUCKFOX_PICO)
        cv::cvtColor(latest_camera_image_, latest_camera_image_, cv::COLOR_YUV2BGR_NV12);
    #endif

    // 2. ROI Crop
    int crop_size = 240;
    int x_offset = (latest_camera_image_.cols - crop_size) / 2;
    int y_offset = (latest_camera_image_.rows - crop_size) / 2;
    
    cv::Rect roi(x_offset, y_offset, crop_size, crop_size);
    cv::Mat crop = latest_camera_image_(roi).clone(); // Copy...


    // crop = zoomOptimal(crop, 5);
    /*
    // 3. Crosshair
    cv::line(crop, cv::Point(crop.rows / 2, 0), cv::Point(crop.rows / 2, crop.cols), cv::Scalar(0,0,0), 2);
    cv::line(crop, cv::Point(0, crop.cols / 2), cv::Point(crop.rows, crop.cols / 2), cv::Scalar(0,0,0), 2);


    cv::line(crop, cv::Point(crop.rows / 2, 0), cv::Point(crop.rows / 2, crop.cols), cv::Scalar(0,255,0));
    cv::line(crop, cv::Point(0, crop.cols / 2), cv::Point(crop.rows, crop.cols / 2), cv::Scalar(0,255,0));


    cv::putText(
        crop, 
        "23m", 
        cv::Point(205, 118), 
        cv::FONT_HERSHEY_PLAIN, 
        1,                       // Escala más razonable (ajusta si es necesario)
        cv::Scalar(0, 0, 0),       // Negro (BGR)
        2,                         // Grosor proporcional a la escala
        cv::LINE_8                // Suavizado de bordes (¡clave para evitar el bug visual!)
    );

        cv::putText(
        crop, 
        "23m", 
        cv::Point(205, 118), 
        cv::FONT_HERSHEY_PLAIN, 
        1,                       // Escala más razonable (ajusta si es necesario)
        cv::Scalar(0, 255, 0),       // Negro (BGR)
        1,                         // Grosor proporcional a la escala
        cv::LINE_8                // Suavizado de bordes (¡clave para evitar el bug visual!)
    );

    cv::putText(
        crop, 
        "2x", 
        cv::Point(2, 118), 
        cv::FONT_HERSHEY_PLAIN, 
        1,                       // Escala más razonable (ajusta si es necesario)
        cv::Scalar(0, 0, 0),       // Negro (BGR)
        2,                         // Grosor proporcional a la escala
        cv::LINE_8                // Suavizado de bordes (¡clave para evitar el bug visual!)
    );

        cv::putText(
        crop, 
        "2x", 
        cv::Point(2, 118), 
        cv::FONT_HERSHEY_PLAIN, 
        1,                       // Escala más razonable (ajusta si es necesario)
        cv::Scalar(0, 255, 0),       // Negro (BGR)
        1,                         // Grosor proporcional a la escala
        cv::LINE_8                // Suavizado de bordes (¡clave para evitar el bug visual!)
    );
    */
    cv::putText(
        crop, 
        "CALIBRATION", 
        cv::Point(70, 40), 
        cv::FONT_HERSHEY_PLAIN, 
        1,                       // Escala más razonable (ajusta si es necesario)
        cv::Scalar(255, 0, 0),       // Negro (BGR)
        2,                         // Grosor proporcional a la escala
        cv::LINE_8                // Suavizado de bordes (¡clave para evitar el bug visual!)
    );

    cv::circle(crop, cv::Point(120, 120), 2, cv::Scalar(255, 0, 0), -1);

    // Finally: Show
    #if defined(BUILD_X86)
        CvDisplay::instance().draw(crop);
    #elif defined(BUILD_LUCKFOX_PICO)
        FbDisplay::instance().draw(crop);
    #endif
    latest_camera_image_ready_ = false;

}

cv::Mat DisplayManager::zoomOptimal(const cv::Mat& src, float zoom_factor) {
    // 1. Calcular el tamaño de la región central que necesitamos cortar
    // Si zoom_factor = 2.0, recortaremos exactamente la mitad de la imagen original
    int crop_w = static_cast<int>(src.cols / zoom_factor);
    int crop_h = static_cast<int>(src.rows / zoom_factor);

    // 2. Definir las coordenadas del ROI (Region of Interest) centrado
    int x = (src.cols - crop_w) / 2;
    int y = (src.rows - crop_h) / 2;
    cv::Rect roi(x, y, crop_w, crop_h);

    // 3. RECORTAR PRIMERO (CERO coste computacional)
    // Usamos .clone() sólo para tener memoria contigua limpia en el MCU
    cv::Mat cropped = src(roi).clone();

    // 4. ESCALAR AHORA a los 240x240 finales
    cv::Mat result;
    cv::resize(
        cropped, 
        result, 
        cv::Size(240, 240), 
        0, 0, 
        cv::INTER_NEAREST // <--- ¡CLAVE ABSOLUTA DE VELOCIDAD!
    );

    return result;
}