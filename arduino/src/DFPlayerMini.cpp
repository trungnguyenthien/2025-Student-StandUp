#include "DFPlayerMini.h"

// 🔹 Constructor: Lưu thông tin UART và chân RX/TX
DFPlayerMini::DFPlayerMini(HardwareSerial& serialPort, int rx, int tx) : serial(&serialPort), rxPin(rx), txPin(tx) {}

// 🔹 Hàm khởi động module DFPlayer Mini
bool DFPlayerMini::begin() {
  serial->begin(9600, SERIAL_8N1, rxPin, txPin);  // Baudrate mặc định của DFPlayer Mini
  delay(1000);                                    // Chờ module khởi động

  // 📌 Gửi lệnh kiểm tra trạng thái của DFPlayer Mini.
  //    sendCommand(0x3F, 0, 0);
  //
  // 🔹 Cách hoạt động:
  //    - Lệnh `0x3F` là **"Query Current Status"** (Truy vấn trạng thái hiện tại).
  //    - DFPlayer Mini sẽ phản hồi nếu module hoạt động bình thường.
  //
  // 🔹 Ý nghĩa từng tham số:
  //    - `0x3F` → Mã lệnh: **Kiểm tra trạng thái DFPlayer Mini**.
  //    - `0x00` → Không sử dụng, luôn đặt là `0`.
  //    - `0x00` → Không sử dụng, luôn đặt là `0`.
  //
  // 🔹 Khi gửi lệnh này, DFPlayer Mini có thể phản hồi:
  //    - `0x41` → DFPlayer Mini đang phát nhạc.
  //    - `0x42` → DFPlayer Mini đang tạm dừng.
  //    - `0x43` → DFPlayer Mini đã dừng phát nhạc.
  //    - `0x40` → Không có thẻ SD hoặc thiết bị lưu trữ.
  sendCommand(0x3F, 0, 0);  // Kiểm tra DFPlayer Mini có phản hồi không
  return true;
}

// 📌 Bảng lệnh DFPlayer Mini kèm phản hồi trả về từ module.
//
// 🔹 Khi gửi một lệnh đến DFPlayer Mini, module sẽ phản hồi với một mã phản hồi
//    để cho biết trạng thái hiện tại.
//
// 🔹 Bảng mã lệnh và phản hồi:
//    | **Mã lệnh (Hex)** | **Chức năng** | **Phản hồi từ DFPlayer Mini** |
//    |----------------|------------------|------------------------------------|
//    | `0x01` | Phát bài hát tiếp theo | `0x41` nếu bài hát mới bắt đầu phát |
//    | `0x02` | Phát bài hát trước đó | `0x41` nếu bài hát mới bắt đầu phát |
//    | `0x03` | Phát bài hát theo số (tham số thứ 3 là số bài) | `0x41` nếu bài hát bắt đầu phát |
//    | `0x06` | Cài đặt âm lượng (tham số thứ 3 là mức âm lượng từ 0-30) | `0x40` nếu thay đổi thành công |
//    | `0x0E` | Tạm dừng nhạc | `0x42` nếu nhạc tạm dừng thành công |
//    | `0x0D` | Tiếp tục phát nhạc | `0x41` nếu tiếp tục phát nhạc thành công |
//    | `0x16` | Dừng nhạc | `0x43` nếu nhạc đã dừng thành công |
//    | `0x09` | Lặp lại bài hát hiện tại | `0x41` nếu bài hát lặp lại đang phát |
//    | `0x11` | Lặp lại toàn bộ thư mục | `0x41` nếu lặp lại thư mục được kích hoạt |
//    | `0x12` | Phát ngẫu nhiên các bài hát | `0x41` nếu phát ngẫu nhiên được kích hoạt |
//    | `0x3C` | Kiểm tra xem DFPlayer Mini có thẻ nhớ không | `0x40` nếu có thẻ SD, `0x4E` nếu không có thẻ SD |
//    | `0x3F` | Kiểm tra trạng thái hiện tại của module | `0x41` nếu đang phát nhạc, `0x42` nếu tạm dừng, `0x43` nếu đã dừng, `0x40` nếu không có thẻ SD |
//    | `0x48` | Kiểm tra số lượng bài hát trong thẻ SD | Trả về số bài hát (`0x4E` nếu lỗi) |
//    | `0x4C` | Kiểm tra số lượng thư mục trong thẻ SD | Trả về số thư mục (`0x4E` nếu lỗi) |
//    | `0x4D` | Kiểm tra bài hát hiện tại đang phát | Trả về số bài hát hiện tại (`0x4E` nếu lỗi) |
//
// 🔹 Giải thích phản hồi từ DFPlayer Mini:
//    - `0x40` → Lệnh thực hiện thành công.
//    - `0x41` → DFPlayer Mini đang phát nhạc.
//    - `0x42` → DFPlayer Mini đang tạm dừng.
//    - `0x43` → DFPlayer Mini đã dừng phát nhạc.
//    - `0x4E` → Lệnh thất bại hoặc không có dữ liệu.
//
// 🚀 Tóm lại:
//    - Khi gửi một lệnh đến DFPlayer Mini, luôn kiểm tra phản hồi để xác định trạng thái hiện tại.
//    - Nếu `0x4E` xuất hiện, có thể thẻ SD không tồn tại hoặc lệnh bị lỗi.
uint8_t DFPlayerMini::sendCommand(uint8_t command, uint8_t param1, uint8_t param2) {
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

  // Đọc phản hồi từ DFPlayer Mini
  return readResponse();
}

uint8_t DFPlayerMini::readResponse() {
  unsigned long startTime = millis();
  while (!serial->available()) {
    if (millis() - startTime > 1000) {  // Timeout sau 1 giây
      Serial.println("❌ Không nhận được phản hồi từ DFPlayer Mini!");
      return 0x00;  // Trả về 0 nếu không có phản hồi
    }
  }

  uint8_t response[10];  // DFPlayer Mini luôn gửi phản hồi 10 byte
  for (int i = 0; i < 10; i++) {
    response[i] = serial->read();
  }

  // In phản hồi để kiểm tra
  Serial.print("📩 Phản hồi từ DFPlayer: ");
  for (int i = 0; i < 10; i++) {
    Serial.printf("0x%02X ", response[i]);
  }
  Serial.println();

  return response[3];  // Byte thứ 4 là mã phản hồi từ DFPlayer Mini
}

// 🔹 Phát bài hát theo số
uint8_t DFPlayerMini::play(int trackNum) {
  return sendCommand(0x03, 0x00, trackNum);
}

// 🔹 Dừng phát nhạc
uint8_t DFPlayerMini::stop() {
  return sendCommand(0x16, 0x00, 0x00);
}

// 🔹 Phát bài tiếp theo
uint8_t DFPlayerMini::next() {
  return sendCommand(0x01, 0x00, 0x00);
}

// 🔹 Phát bài trước đó
uint8_t DFPlayerMini::prev() {
  return sendCommand(0x02, 0x00, 0x00);
}

// 🔹 Cài đặt âm lượng (0-30)
uint8_t DFPlayerMini::setVolume(int vol) {
  if (vol < 0) vol = 0;
  if (vol > 30) vol = 30;
  volume = vol;
  return sendCommand(0x06, 0, volume);
}

// 🔹 Tạm dừng nhạc
uint8_t DFPlayerMini::pause() {
  return sendCommand(0x0E, 0x00, 0x00);
}

// 🔹 Tiếp tục phát nhạc
uint8_t DFPlayerMini::resume() {
  return sendCommand(0x0D, 0x00, 0x00);
}
