#include "Thread.h"

Thread::Thread()
    : Thread(std::chrono::milliseconds(0)) // No interval, full speed
{
}

Thread::Thread(const std::chrono::duration<int, std::milli>& interval)
{
}
Thread::~Thread()
{
}

void Thread::start()
{
    onStart();

    thread_ = std::thread([this]() 
    {
        running_ = true;
        while (running_) 
        {
            auto start = std::chrono::high_resolution_clock::now();
            loop();
            if (interval_.count() > 0) 
            {
                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                if (elapsed < interval_) 
                {
                    std::this_thread::sleep_for(interval_ - elapsed);
                }
            }
        }
    });
}

void Thread::stop()
{
    onStop();
    running_ = false;
    if (thread_.joinable())
    {
        thread_.join();
    }
}

bool Thread::isRunning()
{
    return running_;
}
