#pragma once

class Thread
{
public:
    Thread(); // No interval, full speed
    Thread(const std::duration<int, std::milli>& interval);
    ~Thread();

    void start();
    virtual void onStart() {/* Do nothing*/}

    void stop();
    virtual void onStop() {/* Do nothing*/}

    bool isRunning();

protected:
    virtual void loop() {/* Do nothing*/}

};  