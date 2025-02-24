#ifndef DFPLAYER_MINI_H
#define DFPLAYER_MINI_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class DFPlayerMini {
 private:
  HardwareSerial* serial;  // UART để giao tiếp với DFPlayer
  int rxPin, txPin;        // Chân UART
  int volume = 15;         // Mức âm lượng mặc định (0-30)

  void sendCommand(uint8_t command, uint8_t param1 = 0, uint8_t param2 = 0);  // Gửi lệnh UART

 public:
  DFPlayerMini(HardwareSerial& serialPort, int rx, int tx);  // Constructor
  bool begin();                                              // Khởi động module
  void play(int trackNum);                                   // Phát bài hát theo số
  void stop();                                               // Dừng phát nhạc
  void next();                                               // Chuyển bài tiếp theo
  void prev();                                               // Chuyển bài trước đó
  void setVolume(int vol);                                   // Cài đặt âm lượng
  void pause();                                              // Tạm dừng nhạc
  void resume();                                             // Tiếp tục phát nhạc
};

#endif
