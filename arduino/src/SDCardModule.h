#ifndef SD_CARD_MODULE_H
#define SD_CARD_MODULE_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SDCardModule {
 private:
  int csPin;  // Chân CS của SD Card
  bool isInitialized = false;

 public:
  SDCardModule(int cs);                                           // Constructor
  bool begin();                                                   // Khởi tạo SD Card
  bool writeFile(const char* path, const char* message);          // Ghi file
  String readFile(const char* path);                              // Đọc file
  bool deleteFile(const char* path);                              // Xóa file
  void listFiles(const char* dirname = "/", uint8_t levels = 0);  // Liệt kê file
  uint64_t getTotalSize();                                        // Lấy tổng dung lượng thẻ nhớ
  uint64_t getUsedSize();                                         // Lấy dung lượng đã sử dụng
};

#endif

/**
 * #include <Arduino.h>
#include "SDCardModule.h"

#define SD_CS_PIN 5  // Chân CS kết nối với SD Card Module

SDCardModule sd(SD_CS_PIN);

void setup() {
    Serial.begin(115200);
    
    // Khởi động SD Card
    if (!sd.begin()) {
        Serial.println("🚨 Lỗi khởi động SD Card!");
        return;
    }

    // Ghi dữ liệu vào file
    sd.writeFile("/test.txt", "Xin chào, đây là file trên SD Card!");

    // Đọc dữ liệu từ file
    String content = sd.readFile("/test.txt");
    Serial.println("📄 Nội dung file: " + content);

    // Liệt kê danh sách file
    sd.listFiles("/");

    // Xóa file
    sd.deleteFile("/test.txt");
}

void loop() {
    // Không làm gì trong loop
}

 */