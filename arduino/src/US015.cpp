#include "US015.h"

volatile unsigned long startTime = 0;
volatile unsigned long pulseDurations[40] = {0}; // Chỉ định nghĩa ở đây

std::vector<US015*> US015::sensors;

void IRAM_ATTR US015::echo_ISR(void* arg) {
    if (arg == nullptr) {
        Serial.println("ISR: NULL pointer!");
        return;
    }

    US015* sensor = static_cast<US015*>(arg);

    if (digitalRead(sensor->echoPin) == HIGH) {
        startTime = micros();
        Serial.printf("ISR TRIGGERED: Echo HIGH on pin %d\n", sensor->echoPin);
    } else {
        pulseDurations[sensor->echoPin] = micros() - startTime;
        Serial.printf("Measured pulse duration on pin %d: %lu µs\n", sensor->echoPin, pulseDurations[sensor->echoPin]);
    }
}

// 🔹 HÀM DỰNG CHỈ GÁN GIÁ TRỊ BIẾN
US015::US015(int triggerPin, int echoPin) : trigPin(triggerPin), echoPin(echoPin) {}

// 🔹 HÀM `init()` XỬ LÝ KHỞI TẠO ISR
void US015::init() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);

    sensors.push_back(this);

    static bool isISRServiceInitialized = false;
    if (!isISRServiceInitialized) {
        gpio_install_isr_service(0);
        isISRServiceInitialized = true;
    }

    esp_err_t err = gpio_isr_handler_add((gpio_num_t)echoPin, echo_ISR, (void*)this);
    if (err != ESP_OK) {
        Serial.printf("Error adding ISR to pin %d: %d\n", echoPin, err);
    } else {
        Serial.printf("ISR added successfully on pin %d!\n", echoPin);
    }    
}

float US015::measure() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(4);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000); // Timeout 30ms (~500cm)
    if (duration == 0) {
        Serial.println("Lỗi: Không nhận được tín hiệu ECHO.");
        return -1;
    }

    return (duration * soundSpeed) / 2;
}

float US015::measureAverage(int samples) {
    if (samples <= 0) samples = 1; // Đảm bảo ít nhất có 1 lần đo

    float sum = 0;
    int validSamples = 0;
    unsigned long lastTime = millis();

    for (int i = 0; i < samples; i++) {
        // Chờ 5ms mà không chặn CPU (Non-blocking delay)
        while (millis() - lastTime < 5) {
            // Cho phép chương trình tiếp tục chạy các tác vụ khác
            yield();
        }
        lastTime = millis(); // Cập nhật thời gian mẫu tiếp theo

        float distance = measure();
        if (distance > 0) { // Bỏ qua các giá trị lỗi (-1)
            sum += distance;
            validSamples++;
        }
    }

    return (validSamples > 0) ? (sum / validSamples) : -1; // Trả về -1 nếu không có mẫu hợp lệ
}


