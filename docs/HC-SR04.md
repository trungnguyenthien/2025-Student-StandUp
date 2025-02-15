Dưới đây là giới thiệu chi tiết về cảm biến HC-SR04 và triển khai class C++ để quản lý cảm biến:

---

### **Giới thiệu HC-SR04**
#### 1. Thông số kỹ thuật
| Thông số           | Giá trị            |
|---------------------|--------------------|
| Điện áp hoạt động   | 5V DC              |
| Dòng tiêu thụ       | 15mA               |
| Phạm vi đo          | 2cm – 400cm (4m)   |
| Độ phân giải        | 0.3cm              |
| Sai số              | ±3cm               |
| Góc phát            | 15 độ              |
| Tần số hoạt động    | 40kHz              |
| Kích thước          | 45x20x15mm         |

#### 2. Nguyên lý hoạt động
- **Bước 1**: Vi điều khiển phát xung 10µs vào chân **Trig**.
- **Bước 2**: Cảm biến phát 8 xung siêu âm 40kHz.
- **Bước 3**: Chân **Echo** lên mức HIGH và giữ đến khi nhận phản xạ.
- **Công thức tính khoảng cách**:
  \[
  \text{Khoảng cách (cm)} = \frac{\text{Thời gian phản hồi (µs)} \times 0.0343}{2}
  \]

#### 3. Ưu điểm
- Chi phí thấp (~50.000 VNĐ)
- Dễ tích hợp với Arduino/Raspberry Pi
- Ít bị ảnh hưởng bởi ánh sáng/màu sắc

#### 4. Hạn chế
- Không đo được vật liệu hấp thụ âm (vải, bông)
- Sai số tăng ở khoảng cách >2m
- Kém chính xác trong môi trường nhiễu âm

#### 5. Ứng dụng
- Hệ thống tránh vật cản robot
- Đo mực nước tự động
- Cảnh báo an ninh

---

### **Triển khai Class C++ HCSR04**

#### Code class:
```cpp
class HCSR04 {
private:
    int trigPin;    // Chân kích hoạt phát xung
    int echoPin;    // Chân nhận tín hiệu phản hồi
    const float soundSpeed = 0.0343; // Tốc độ âm thanh (cm/µs)
    const float cmToInch = 0.393701; // Hệ số chuyển đổi cm sang inch

public:
    // Hàm dựng - tuỳ chỉnh chân Trig và Echo
    HCSR04(int triggerPin, int echoPin) : trigPin(triggerPin), echoPin(echoPin) {
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }

    // Đo khoảng cách cơ bản (cm)
    float measure() {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        
        long duration = pulseIn(echoPin, HIGH);
        return (duration * soundSpeed) / 2;
    }

    // Đo khoảng cách trung bình với số mẫu tuỳ chỉnh
    float measureAverage(int samples = 5) {
        if(samples <= 0) samples = 1; // Đảm bảo ít nhất 1 mẫu
        float sum = 0;
        for(int i = 0; i < samples; i++) {
            sum += measure();
            delay(10); // Giãn cách giữa các lần đo
        }
        return sum / samples;
    }

    // Đo an toàn với kiểm tra giá trị hợp lệ
    bool measureSafe(float &distance, float maxDistance = 400.0) {
        distance = measure();
        return (distance > 0 && distance <= maxDistance);
    }

    // Đo khoảng cách theo inch
    float measureInches() {
        return measure() * cmToInch;
    }

    // Đo inch trung bình với số mẫu tuỳ chỉnh
    float measureInchesAverage(int samples = 5) {
        return measureAverage(samples) * cmToInch;
    }
};
```

#### Cách sử dụng:
```cpp
HCSR04 sensor(8, 7); // Trig=8, Echo=7

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Đo cơ bản
    Serial.print("Basic: ");
    Serial.print(sensor.measure());
    Serial.println(" cm");

    // Đo an toàn
    float distance;
    if(sensor.measureSafe(distance)) {
        Serial.print("Safe: ");
        Serial.print(distance);
        Serial.println(" cm");
    } else {
        Serial.println("Invalid measurement!");
    }

    // Đo trung bình 3 mẫu
    Serial.print("Average: ");
    Serial.print(sensor.measureAverage(3));
    Serial.println(" cm");

    // Đo inch
    Serial.print("Inches: ");
    Serial.print(sensor.measureInches());
    Serial.println(" in");

    delay(1000);
}
```

#### Tính năng class:
- **Tự động cấu hình chân** trong hàm dựng
- **Che dấu logic đo đạc** phức tạp
- **Trả về giá trị số thực** (cm) chính xác
- **Dễ dàng tái sử dụng** giữa các dự án
