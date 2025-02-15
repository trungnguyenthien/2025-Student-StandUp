## Hướng dẫn sử dụng **PlatformIO** trong **VSCode** để lập trình Arduino

**PlatformIO** là một nền tảng phát triển mạnh mẽ dành cho lập trình nhúng, hỗ trợ nhiều bo mạch, bao gồm cả **Arduino**. Dưới đây là hướng dẫn chi tiết để cài đặt và sử dụng **PlatformIO** trong **Visual Studio Code (VSCode)**.

---

### **1. Cài đặt PlatformIO vào VSCode**
#### **Bước 1: Cài đặt VSCode**
- Tải và cài đặt **Visual Studio Code** nếu chưa có:  
  🔗 [Download VSCode](https://code.visualstudio.com/)

#### **Bước 2: Cài đặt Extension PlatformIO**
1. Mở **VSCode**.
2. Nhấn **Ctrl + Shift + X** để mở trình quản lý extension.
3. Tìm kiếm **PlatformIO IDE** và nhấn **Install**.
4. Chờ cài đặt hoàn tất, sau đó **khởi động lại VSCode**.

---

### **2. Tạo Dự Án Arduino Mới**
#### **Bước 1: Mở PlatformIO**
- Nhấn vào biểu tượng **PlatformIO** trên thanh sidebar trái của VSCode.

#### **Bước 2: Tạo dự án mới**
1. Nhấn vào **"New Project"**.
2. Nhập **tên dự án** (VD: `Arduino_LED`).
3. Chọn **Board** tương ứng (VD: `Arduino Uno` nếu dùng Arduino Uno).
4. Chọn **Framework** là `Arduino`.
5. Chọn thư mục lưu trữ dự án (hoặc để mặc định).
6. Nhấn **"Finish"** để tạo dự án.

---

### **3. Cấu Trúc Thư Mục Dự Án PlatformIO**
Sau khi tạo xong, bạn sẽ thấy cấu trúc thư mục như sau:
```
Arduino_LED/
│-- include/      # Chứa file header (.h) nếu cần
│-- lib/          # Chứa các thư viện bổ sung
│-- src/          # Chứa mã nguồn chính
│   ├── main.cpp  # File code chính
│-- platformio.ini  # File cấu hình dự án
```
- **Mọi mã nguồn Arduino sẽ được viết trong file `main.cpp` trong thư mục `src/`**.

---

### **4. Viết Mã Lập Trình Arduino**
Mở file `src/main.cpp` và nhập đoạn code sau:

```cpp
#include <Arduino.h>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
```
⚡ **Lưu ý:** Không dùng `void setup()` và `void loop()` như trong Arduino IDE. **PlatformIO yêu cầu `#include <Arduino.h>` ở đầu chương trình**.

---

### **5. Cấu Hình `platformio.ini` (Tùy Chỉnh)**
Mở file `platformio.ini` để kiểm tra hoặc tùy chỉnh:
```ini
[env:arduino_uno]
platform = atmelavr
board = uno
framework = arduino
upload_port = COM3   ; Cổng kết nối với Arduino (Windows)
; upload_port = /dev/ttyUSB0  (Linux/Mac)
monitor_speed = 115200
```
🚀 **Lưu ý:**  
- **`board = uno`** → Thay đổi nếu dùng bo mạch khác (`esp32`, `mega2560`, ...).
- **`upload_port = COMx`** → Cổng kết nối của bo mạch, có thể kiểm tra bằng **Device Manager (Windows)** hoặc **`ls /dev/tty*` trên Linux/Mac**.

---

### **6. Build & Upload Code Lên Arduino**
Sau khi viết mã, tiến hành nạp chương trình vào bo mạch:

1. **Build (Kiểm tra lỗi)**: Nhấn `Ctrl + Alt + B` hoặc nhấn **"Build"** trên giao diện PlatformIO.
2. **Upload Code**: Nhấn `Ctrl + Alt + U` hoặc nhấn **"Upload"**.
3. **Mở Serial Monitor** (nếu cần kiểm tra dữ liệu Serial từ Arduino):
   - Nhấn `Ctrl + Alt + M` hoặc nhấn **"Monitor"** trên giao diện.

---

### **7. Quản Lý Thư Viện Trong PlatformIO**
**Cài đặt thư viện bằng Terminal:**
```sh
pio lib install "Adafruit NeoPixel"
```
Hoặc thêm vào `platformio.ini`:
```ini
lib_deps =
    Adafruit NeoPixel
```
📌 **Lưu ý:** PlatformIO tự động tải thư viện nếu chưa có.

---

### **8. Debug & Serial Monitor**
- **Mở Serial Monitor:** `Ctrl + Alt + M`
- **Dừng Serial Monitor:** `Ctrl + C`
- **Thay đổi Baud Rate:** Chỉnh giá trị `monitor_speed` trong `platformio.ini`.

---

## **🎯 Kết Luận**
🚀 **PlatformIO là một công cụ mạnh mẽ thay thế Arduino IDE** với nhiều tính năng nâng cao:
✅ Hỗ trợ **nhiều bo mạch** ngoài Arduino (ESP32, STM32, ...).  
✅ **Quản lý thư viện tự động**.  
✅ **Tích hợp Debug, Serial Monitor mạnh mẽ**.  
✅ **Giao diện hiện đại, tích hợp VSCode**.

Bạn có thể bắt đầu ngay bằng cách cài đặt và thử nghiệm một chương trình đơn giản. Nếu có bất kỳ vấn đề gì, cứ hỏi tôi nhé! 😃