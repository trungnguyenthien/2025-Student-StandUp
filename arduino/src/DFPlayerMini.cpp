#include "DFPlayerMini.h"

// 🔹 Constructor: Lưu thông tin UART và chân RX/TX
DFPlayerMini::DFPlayerMini(HardwareSerial& serialPort, int rx, int tx) : serial(&serialPort), rxPin(rx), txPin(tx) {}

// 🔹 Hàm khởi động module DFPlayer Mini
bool DFPlayerMini::begin() {
  serial->begin(9600, SERIAL_8N1, rxPin, txPin);  // Baudrate mặc định của DFPlayer Mini
  delay(1000);                                    // Chờ module khởi động

  sendCommand(0x3F, 0, 0);  // Kiểm tra DFPlayer Mini có phản hồi không
  return true;
}

// 🔹 Hàm gửi lệnh UART đến DFPlayer Mini
void DFPlayerMini::sendCommand(uint8_t command, uint8_t param1, uint8_t param2) {
  uint8_t buffer[10] = {0x7E, 0xFF, 0x06, command, 0x00, param1, param2, 0x00, 0x00, 0xEF};

  // Tính toán checksum
  uint16_t checksum = 0;
  for (int i = 1; i < 7; i++) {
    checksum += buffer[i];
  }
  checksum = 0xFFFF - checksum + 1;

  buffer[7] = (checksum >> 8) & 0xFF;
  buffer[8] = checksum & 0xFF;

  // Gửi lệnh qua UART
  for (uint8_t i = 0; i < 10; i++) {
    serial->write(buffer[i]);
  }
}

// 🔹 Phát bài hát theo số
void DFPlayerMini::play(int trackNum) {
  sendCommand(0x03, 0x00, trackNum);
}

// 🔹 Dừng phát nhạc
void DFPlayerMini::stop() {
  sendCommand(0x16);
}

// 🔹 Phát bài tiếp theo
void DFPlayerMini::next() {
  sendCommand(0x01);
}

// 🔹 Phát bài trước đó
void DFPlayerMini::prev() {
  sendCommand(0x02);
}

// 🔹 Cài đặt âm lượng (0-30)
void DFPlayerMini::setVolume(int vol) {
  if (vol < 0) vol = 0;
  if (vol > 30) vol = 30;
  volume = vol;
  sendCommand(0x06, 0, volume);
}

// 🔹 Tạm dừng nhạc
void DFPlayerMini::pause() {
  sendCommand(0x0E);
}

// 🔹 Tiếp tục phát nhạc
void DFPlayerMini::resume() {
  sendCommand(0x0D);
}
