### **Bảng kết nối các module với NodeMCU (ESP8266)**

| **Thiết bị**            | **Chân trên Module** | **Chân trên NodeMCU (ESP8266)** | **Giải thích chi tiết** |
|-------------------------|---------------------|--------------------------------|-------------------------|
| **Nguồn 5V**            | VCC                 | VIN                            | Cấp nguồn 5V từ NodeMCU cho DFPlayer Mini và US-015. |
| **GND (Mass)**          | GND                 | GND                            | Nối tất cả GND chung để đảm bảo tín hiệu ổn định. |
| **US-015 Trig**         | TRIG                | D5 (GPIO14)                    | Chân kích hoạt phát xung siêu âm. |
| **US-015 Echo**         | ECHO                | D6 (GPIO12) _(qua chia áp 5V→3.3V)_ | Echo trả về tín hiệu 5V, cần chia áp xuống 3.3V để bảo vệ NodeMCU. |
| **DFPlayer Mini TX**     | TX                  | D7 (GPIO13) _(RX của NodeMCU)_  | DFPlayer gửi dữ liệu đến NodeMCU, nhận lệnh phát nhạc. |
| **DFPlayer Mini RX**     | RX                  | D8 (GPIO15) _(TX của NodeMCU)_  | NodeMCU gửi lệnh điều khiển đến DFPlayer. |
| **Thẻ nhớ TF (MicroSD)** | Gắn vào DFPlayer    | -                              | Chứa file nhạc dạng `.mp3`, tổ chức theo thư mục `/01/001.mp3`. |

---

### **Lưu ý quan trọng**
1. **Chia áp điện áp cho US-015 Echo (5V → 3.3V)**  
   - Echo của US-015 trả về tín hiệu **5V**, nhưng **NodeMCU chỉ chấp nhận tối đa 3.3V**.  
   - Dùng **mạch chia áp điện trở**:
     - **R1 = 1kΩ** (GND).
     - **R2 = 2kΩ** (kết nối với Echo).
   - Công thức:  
     $
     V_{out} = V_{in} \times \frac{R1}{R1 + R2}
     $
     $
     V_{out} = 5V \times \frac{1kΩ}{1kΩ + 2kΩ} = 3.3V
     $

2. **Giao tiếp DFPlayer Mini với NodeMCU**
   - NodeMCU **giao tiếp với DFPlayer Mini bằng UART (serial)**.
   - **ESP8266SoftwareSerial** được sử dụng thay vì `SoftwareSerial` để đảm bảo ổn định.

3. **Cấp nguồn 5V**
   - **NodeMCU có thể cấp 5V từ chân VIN**, đủ để cấp cho cả **US-015 và DFPlayer Mini**.

---

### **Tóm lại**
✔ **Chân Echo của US-015 cần chia áp trước khi vào NodeMCU**.  
✔ **DFPlayer Mini giao tiếp với NodeMCU bằng `ESP8266SoftwareSerial` trên chân D7 và D8**.  
✔ **Tất cả thiết bị đều dùng chung GND để đảm bảo tín hiệu ổn định**.  

🚀 **Bạn có muốn thêm kết nối với WiFi để điều khiển từ xa không?** 🚀


----------

### **Hướng dẫn chia áp từ 5V xuống 3.3V bằng 2 điện trở**  

Mạch chia áp là cách đơn giản và hiệu quả để giảm điện áp **từ 5V xuống 3.3V**, giúp bảo vệ **ESP32 hoặc các vi điều khiển chạy mức logic 3.3V**.

---

## **1️⃣ Nguyên lý hoạt động của mạch chia áp**  
Mạch chia áp hoạt động theo nguyên lý phân áp điện trở:

$
V_{out} = V_{in} \times \frac{R1}{R1 + R2}
$

Trong đó:
- **\(V_{in}\)**: Điện áp đầu vào (5V từ US-015 Echo hoặc thiết bị khác).  
- **\(V_{out}\)**: Điện áp đầu ra (cần hạ xuống 3.3V cho ESP32).  
- **\(R1\)**: Điện trở nối giữa tín hiệu đầu vào và GND.  
- **\(R2\)**: Điện trở nối giữa tín hiệu đầu vào và đầu ra cần sử dụng.  

---

## **2️⃣ Sơ đồ mạch chia áp**
```
           5V (Echo từ US-015)
                     │
                     ├─── [2kΩ] ─── GPIO ESP32 (3.3V)
                     │
                   [1kΩ]
                     │
                    GND
```

---

## **3️⃣ Chọn giá trị điện trở phù hợp**
Dùng công thức:

$
V_{out} = V_{in} \times \frac{R1}{R1 + R2}
$

- **Mục tiêu:** Hạ từ **5V xuống 3.3V**.  
- **Giả sử chọn \(R1 = 1kΩ\), ta tính \(R2\):**  

$
3.3V = 5V \times \frac{1kΩ}{1kΩ + R2}
$

$
R2 = \frac{1kΩ \times 5V}{3.3V} - 1kΩ = 2kΩ
$

✅ **Cặp điện trở phổ biến**:  
- **\(R1 = 1kΩ\), \(R2 = 2kΩ\)** _(chính xác 3.3V)_.  
- **Hoặc \(R1 = 10kΩ\), \(R2 = 20kΩ\)** _(cũng hoạt động tốt)_.  

---

## **4️⃣ Cách lắp mạch thực tế**
1. **Nối chân Echo (5V) của US-015 với một đầu của \(R2 = 2kΩ\)**.  
2. **Nối đầu kia của \(R2\) với GPIO của ESP32 (3.3V)**.  
3. **Nối một đầu \(R1 = 1kΩ\) vào điểm giữa \(R2\) và ESP32 GPIO**.  
4. **Nối đầu còn lại của \(R1\) xuống GND**.  

---

## **5️⃣ So sánh với phương pháp khác**
| **Phương pháp**        | **Linh kiện cần thiết**   | **Ưu điểm**                                | **Nhược điểm**                     |
|------------------------|------------------------|--------------------------------|--------------------------------|
| **Chia áp bằng 2 điện trở**  | 2 điện trở (1kΩ, 2kΩ)  | Dễ tính toán, chính xác, rẻ  | Không phù hợp với dòng cao |
| **Dùng Diode Zener 3.3V**   | 1 Diode Zener + 1 điện trở 1kΩ | Đơn giản hơn, không tốn nhiều linh kiện | Điện áp có thể dao động nhẹ |
| **Dùng Logic Level Converter** | 1 Module | Chính xác, phù hợp cho tốc độ cao | Cần module riêng, phức tạp hơn |

---

## **6️⃣ Kết luận**
✅ **Nếu bạn cần giải pháp đơn giản và rẻ:** Dùng **chia áp điện trở** (1kΩ - 2kΩ).  
✅ **Nếu bạn có sẵn diode Zener:** Dùng **Zener 3.3V + điện trở 1kΩ**.  
✅ **Nếu bạn cần chuyển đổi nhanh với nhiều tín hiệu:** Dùng **Logic Level Converter**.  