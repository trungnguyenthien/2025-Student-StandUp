### **📌 Giải thích vì sao nối các chân SD Card Module với ESP32 như vậy?**

ESP32 hỗ trợ giao tiếp **SPI (Serial Peripheral Interface)** để kết nối với các thiết bị ngoại vi như **Micro SD Card Module TF SPI**.  
SPI trên ESP32 có **2 bus SPI phần cứng**:
- **SPI0 (HSPI)**
- **SPI1 (VSPI)** ➝ **Chúng ta sử dụng VSPI vì nó là bus mặc định cho thiết bị ngoại vi.**

Trong mã nguồn trên, chúng ta sử dụng **VSPI**, có các chân mặc định:

| **Chân SPI** | **Tên trên ESP32** | **Chức năng trong SPI**     |
| ------------ | ------------------ | --------------------------- |
| **GPIO18**   | **VSPI_CLK**       | Clock (SCK)                 |
| **GPIO19**   | **VSPI_MISO**      | Master In, Slave Out (MISO) |
| **GPIO23**   | **VSPI_MOSI**      | Master Out, Slave In (MOSI) |
| **GPIO5**    | **VSPI_CS**        | Chip Select (CS)            |

---

## **🔹 Giải thích từng kết nối**
| **Chân Micro SD Module (SPI)** | **Chân ESP32 (SPI Default)** | **Giải thích**                                                                             |
| ------------------------------ | ---------------------------- | ------------------------------------------------------------------------------------------ |
| **VCC (3.3V)**                 | **3.3V**                     | ✅ **Cấp nguồn cho module SD Card** (ESP32 chạy mức logic **3.3V**, nên không dùng 5V).     |
| **GND**                        | **GND**                      | ✅ **Mát chung giữa ESP32 và module SD Card**.                                              |
| **CS (Chip Select)**           | **GPIO5 (D1)**               | ✅ **Chọn thiết bị giao tiếp trên SPI**. Khi SD Card hoạt động, **ESP32 kéo CS xuống LOW**. |
| **MOSI (Data In)**             | **GPIO23 (D7)**              | ✅ **ESP32 gửi dữ liệu vào SD Card** (Master Out, Slave In).                                |
| **MISO (Data Out)**            | **GPIO19 (D6)**              | ✅ **ESP32 nhận dữ liệu từ SD Card** (Master In, Slave Out).                                |
| **SCK (Clock)**                | **GPIO18 (D5)**              | ✅ **Tín hiệu xung nhịp điều khiển truyền dữ liệu giữa ESP32 và SD Card**.                  |

---

## **🔹 Tại sao dùng GPIO5 làm CS (Chip Select)?**
- GPIO5 là **mặc định** của **VSPI_CS** trên ESP32, nên nó tương thích với thư viện `SD.h` mà không cần cấu hình thêm.
- Nếu đổi sang chân khác, bạn phải cấu hình lại trong `SD.begin(csPin);`.

---

## **🔹 Tại sao không dùng GPIO khác?**
| **GPIO**        | **Có thể dùng cho SPI?** | **Lý do**                                                   |
| --------------- | ------------------------ | ----------------------------------------------------------- |
| GPIO6 - GPIO11  | 🚫 Không                  | Dùng cho kết nối SPI Flash của ESP32, không thể sử dụng.    |
| GPIO34 - GPIO39 | 🚫 Không                  | Chỉ hỗ trợ INPUT, không dùng được cho SPI.                  |
| GPIO12          | ⚠️ Có thể                 | Nhưng cần cẩn thận, vì ảnh hưởng đến chế độ BOOT của ESP32. |
| GPIO0           | ⚠️ Không khuyến khích     | Ảnh hưởng đến chế độ FLASH khi khởi động.                   |

---

## **🔹 Kết luận**
✅ **Kết nối theo bảng trên là cách tối ưu nhất**, sử dụng **VSPI mặc định của ESP32**, đảm bảo hoạt động ổn định.  
✅ Nếu muốn đổi **CS (Chip Select)** sang GPIO khác, cần cập nhật trong **`SD.begin(csPin);`**.

🚀 **Bạn có thể thử nghiệm ngay với ESP32 và báo lại kết quả nếu cần hỗ trợ thêm!** 🎯