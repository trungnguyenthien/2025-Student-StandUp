#ifndef DFPLAYER_MINI_H
#define DFPLAYER_MINI_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class DFPlayerMini {
 private:
  HardwareSerial* serial;  // UART để giao tiếp với DFPlayer
  int rxPin, txPin;        // Chân UART
  int volume = 15;         // Mức âm lượng mặc định (0-30)

  uint8_t sendCommand(uint8_t command, uint8_t param1, uint8_t param2);  // Gửi lệnh UART
  uint8_t readResponse();

 public:
  DFPlayerMini(HardwareSerial& serialPort, int rx, int tx);  // Constructor
  bool begin();                                              // Khởi động module
  uint8_t play(int trackNum);                                // Phát bài hát theo số
  uint8_t stop();                                            // Dừng phát nhạc
  uint8_t next();                                            // Chuyển bài tiếp theo
  uint8_t prev();                                            // Chuyển bài trước đó
  uint8_t setVolume(int vol);                                // Cài đặt âm lượng
  uint8_t pause();                                           // Tạm dừng nhạc
  uint8_t resume();                                          // Tiếp tục phát nhạc
};

#endif

/**
 * 
#include <Arduino.h>
#include "DFPlayerMini.h"

#define DF_RX 16  // GPIO16 (RX2)
#define DF_TX 17  // GPIO17 (TX2)

DFPlayerMini dfPlayer(Serial2, DF_RX, DF_TX);

void setup() {
    Serial.begin(115200);

    if (!dfPlayer.begin()) {
        Serial.println("🚨 Không tìm thấy DFPlayer Mini!");
        while (1);
    }
    
    Serial.println("✅ DFPlayer Mini sẵn sàng!");
    
    dfPlayer.setVolume(20);
    dfPlayer.play(1);  // Phát bài số 1
}

void loop() {
    delay(1000);
}

 */