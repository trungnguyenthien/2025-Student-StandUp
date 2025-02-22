#include <Arduino.h>

#include "SoftwareSerial.h"
#include "US015.h"

// Định nghĩa chân kết nối với US-015
#define TRIG_PIN 14  // GPIO14
#define ECHO_PIN 12  // GPIO12 (Qua chia áp)

US015 sensor(TRIG_PIN, ECHO_PIN);

void IRAM_ATTR handleInterrupt() { Serial.println("Interrupt triggered!"); }

void setup() {
  Serial.begin(115200);  // tốc độ truyền dữ liệu giữa ESP32 và máy tính.
  Serial.println("Khởi động NodeMCU...");

  // Khởi tạo dịch vụ ISR (Interrupt Service Routine) trên ESP32.
  if (gpio_install_isr_service(0) != ESP_OK) {
    Serial.println("Failed to install ISR service!");
  }

  sensor.init();
}

void loop() {
  // Đo cơ bản
  Serial.print("----Basic: ");
  Serial.print(sensor.measure());
  Serial.println(" cm");

  delay(1000);
}
