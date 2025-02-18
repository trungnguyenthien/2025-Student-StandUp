#include <Arduino.h>
#include "US015.cpp"
#include "DFPlayerController.cpp"
#include "SoftwareSerial.h"

// Định nghĩa chân kết nối với US-015
#define TRIG_PIN D5  // GPIO14
#define ECHO_PIN D6  // GPIO12 (Qua chia áp)

// Định nghĩa chân kết nối với DFPlayer Mini
#define DF_RX D7  // GPIO13 (ESP8266 nhận từ DFPlayer TX)
#define DF_TX D8  // GPIO15 (ESP8266 gửi đến DFPlayer RX)

US015 sensor(TRIG_PIN, ECHO_PIN);
DFPlayerController mp3(DF_RX, DF_TX);

void setup() {
    Serial.begin(115200);
    Serial.println("Khởi động NodeMCU...");

    // Khởi động DFPlayer Mini
    mp3.setVolume(15);  // Âm lượng 50%
    mp3.playFolder(1, 1);  // Phát file SD:/01/001.mp3
}

void loop() {
    // Đo cơ bản
    Serial.print("----Basic: ");
    Serial.print(sensor.measure());
    Serial.println(" cm");

    // Đo trung bình 3 mẫu
    Serial.print("Average: ");
    Serial.print(sensor.measureAverage(3));
    Serial.println(" cm");

    delay(1000);
}
