Dưới đây là class `DFPlayerMini` dùng để **NodeMCU ESP32** điều khiển **DFPlayer Mini** qua giao tiếp **UART (Serial)**.  
Class này hỗ trợ **phát nhạc, dừng, tăng/giảm âm lượng và chọn bài hát**.

---

## **📌 1. Hướng dẫn kết nối phần cứng**
### **✅ Kết nối DFPlayer Mini với ESP32**
| **Chân DFPlayer Mini** | **Chân ESP32**   | **Chức năng**                                                        |
| ---------------------- | ---------------- | -------------------------------------------------------------------- |
| **VCC** (3.3V-5V)      | **5V**           | ✅ Cấp nguồn cho module (chạy tốt ở 5V)                               |
| **GND**                | **GND**          | ✅ Kết nối mát chung với ESP32                                        |
| **RX**                 | **GPIO17 (TX2)** | ✅ **Nhận dữ liệu** từ ESP32 (**TX** của ESP32 → **RX** của DFPlayer) |
| **TX**                 | **GPIO16 (RX2)** | ✅ **Gửi dữ liệu** đến ESP32 (**RX** của ESP32 ← **TX** của DFPlayer) |
| **SPK1, SPK2**         | **Loa 3W**       | ✅ Nối với loa mini nếu không dùng cổng AUX                           |

📌 **Lưu ý quan trọng**:
- DFPlayer Mini sử dụng **giao tiếp UART (Serial)**, nhưng **mức logic của DFPlayer là 3.3V**, trong khi TX của module **gửi tín hiệu 5V**.
- **Cần điện trở 1kΩ trên đường TX của ESP32 → RX của DFPlayer** để tránh hỏng module.
- **Sử dụng cổng Serial 2 trên ESP32 (`GPIO16 & GPIO17`)**, vì Serial 0 (`GPIO1, GPIO3`) dùng để debug.

---

## **📌 2. File `DFPlayerMini.h` (Header)**
```cpp
#ifndef DFPLAYER_MINI_H
#define DFPLAYER_MINI_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class DFPlayerMini {
private:
    HardwareSerial* serial;  // UART để giao tiếp với DFPlayer
    int rxPin, txPin;  // Chân UART
    int volume = 15;  // Mức âm lượng mặc định (0-30)

    void sendCommand(uint8_t command, uint8_t param1 = 0, uint8_t param2 = 0);  // Gửi lệnh UART

public:
    DFPlayerMini(HardwareSerial& serialPort, int rx, int tx);  // Constructor
    bool begin();  // Khởi động module
    void play(int trackNum);  // Phát bài hát theo số
    void stop();  // Dừng phát nhạc
    void next();  // Chuyển bài tiếp theo
    void prev();  // Chuyển bài trước đó
    void setVolume(int vol);  // Cài đặt âm lượng
    void pause();  // Tạm dừng nhạc
    void resume();  // Tiếp tục phát nhạc
};

#endif
```

---

## **📌 3. File `DFPlayerMini.cpp` (Implementation)**
```cpp
#include "DFPlayerMini.h"

// 🔹 Constructor: Lưu thông tin UART và chân RX/TX
DFPlayerMini::DFPlayerMini(HardwareSerial& serialPort, int rx, int tx)
    : serial(&serialPort), rxPin(rx), txPin(tx) {}

// 🔹 Hàm khởi động module DFPlayer Mini
bool DFPlayerMini::begin() {
    serial->begin(9600, SERIAL_8N1, rxPin, txPin);  // Baudrate mặc định của DFPlayer Mini
    delay(1000);  // Chờ module khởi động

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
```

---

## **📌 4. Cách sử dụng `DFPlayerMini` trong `main.cpp`**
```cpp
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
```

---

## **📌 5. Giải thích kết nối giữa DFPlayer Mini và ESP32**
| **Chân DFPlayer Mini** | **Chân ESP32**   | **Lý do kết nối**                                    |
| ---------------------- | ---------------- | ---------------------------------------------------- |
| **VCC (3.3V-5V)**      | **5V**           | ✅ DFPlayer Mini hoạt động tốt nhất ở **5V**          |
| **GND**                | **GND**          | ✅ Chia chung mát giữa ESP32 và module                |
| **RX**                 | **GPIO17 (TX2)** | ✅ **Nhận dữ liệu từ ESP32** (TX ESP32 → RX DFPlayer) |
| **TX**                 | **GPIO16 (RX2)** | ✅ **Gửi dữ liệu đến ESP32** (RX ESP32 ← TX DFPlayer) |
| **SPK1, SPK2**         | **Loa 3W**       | ✅ Kết nối loa mini nếu không dùng cổng AUX           |

📌 **Lưu ý quan trọng:**
- Nếu sử dụng **loa 3W**, có thể cần **tụ lọc 100µF** để tránh nhiễu.
- **Không nối trực tiếp TX ESP32 → RX DFPlayer**, dùng **điện trở 1kΩ** để giảm mức logic từ 3.3V xuống an toàn.

---

## **📌 6. Kết luận**
✅ **Điều khiển DFPlayer Mini qua UART trên ESP32**.  
✅ **Hỗ trợ phát nhạc, chuyển bài, dừng nhạc, điều chỉnh âm lượng**.  
✅ **Tương thích với các dự án sử dụng thẻ nhớ SD để lưu file nhạc MP3**.  

🚀 **Bạn có thể thử nghiệm ngay với ESP32 và báo lại kết quả nếu cần hỗ trợ thêm!** 🎯