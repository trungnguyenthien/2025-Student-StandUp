#ifndef US015_H
#define US015_H

#include <Arduino.h>
#include <vector>

// Khai báo extern để tránh multiple definition
extern volatile unsigned long startTime;
extern volatile unsigned long pulseDurations[40];

class US015 {
private:
    int trigPin;
    int echoPin;
    const float soundSpeed = 0.0343;
    
    static std::vector<US015*> sensors;
    static void IRAM_ATTR echo_ISR(void* arg);

public:
    US015(int triggerPin, int echoPin);
    void init(); // Thêm hàm này để khởi tạo ISR
    float measure();
    float measureAverage(int samples);
};

#endif
