#include <Arduino.h>

class US015 {
private:
    int trigPin;
    int echoPin;
    const float soundSpeed = 0.0343; // Tốc độ âm thanh (cm/µs)
    const float cmToInch = 0.393701; // Hệ số chuyển đổi cm sang inch

public:
    // Hàm dựng - tùy chỉnh chân Trig và Echo
    US015(int triggerPin, int echoPin) : trigPin(triggerPin), echoPin(echoPin) {
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        digitalWrite(trigPin, LOW); // Đảm bảo chân Trig ở mức LOW
    }

    // Đo khoảng cách (cm) không chặn (non-blocking)
    float measure() {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(4);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH, 30000); // Timeout 30ms (~500cm)
        if (duration == 0) return -1; // Nếu không có tín hiệu, trả về -1 (lỗi)

        return (duration * soundSpeed) / 2;
    }

    // Đo khoảng cách trung bình với `samples` lần, sử dụng `millis()` (không chặn loop)
    float measureAverage(int samples = 5) {
        if (samples <= 0) samples = 1;
        float sum = 0;
        int validSamples = 0;

        unsigned long lastTime = millis();

        for (int i = 0; i < samples; i++) {
            while (millis() - lastTime < 5); // Chờ 5ms mà không chặn CPU
            lastTime = millis();

            float distance = measure();
            if (distance > 0) { 
                sum += distance;
                validSamples++;
            }
        }

        return (validSamples > 0) ? (sum / validSamples) : -1; // Trả về -1 nếu không có mẫu hợp lệ
    }

    // Đo khoảng cách an toàn: Trả về `true` nếu hợp lệ, `false` nếu lỗi
    bool measureSafe(float &distance, float maxDistance = 400.0) {
        distance = measure();
        return (distance > 0 && distance <= maxDistance);
    }

    // Đo khoảng cách theo inch
    float measureInches() {
        float distance = measure();
        return (distance > 0) ? (distance * cmToInch) : -1;
    }

    // Đo inch trung bình với số mẫu tùy chỉnh
    float measureInchesAverage(int samples = 5) {
        float distance = measureAverage(samples);
        return (distance > 0) ? (distance * cmToInch) : -1;
    }
};