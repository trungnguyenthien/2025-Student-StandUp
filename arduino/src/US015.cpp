#include "US015.h"

// 🔹 `volatile` đảm bảo giá trị này có thể thay đổi bất cứ lúc nào bởi ISR (Interrupt Service Routine)
volatile unsigned long startTime = 0;

// 🔹 Mảng lưu thời gian xung Echo (pulse duration) cho tối đa 40 chân GPIO
volatile unsigned long pulseDurations[40] = {0};

// 🔹 Vector lưu danh sách tất cả các cảm biến US015 đã được khởi tạo
std::vector<US015 *> US015::sensors;

// 🔹 Hàm xử lý ngắt (ISR) được gọi khi có thay đổi trạng thái trên chân Echo
void IRAM_ATTR US015::echo_ISR(void *arg) {
  // Kiểm tra con trỏ NULL để tránh lỗi truy cập bộ nhớ
  if (arg == nullptr) {
    Serial.println("ISR: NULL pointer!");
    return;
  }

  // Chuyển con trỏ void* về kiểu US015*
  US015 *sensor = static_cast<US015 *>(arg);

  if (digitalRead(sensor->echoPin) == HIGH) {  // 🔹 Nếu chân Echo lên mức HIGH, bắt đầu ghi lại thời gian
    startTime = micros();
    Serial.printf("ISR TRIGGERED: Echo HIGH on pin %d\n", sensor->echoPin);
  } else {  // 🔹 Khi Echo xuống mức LOW, tính khoảng thời gian xung HIGH và lưu vào mảng
    pulseDurations[sensor->echoPin] = micros() - startTime;
    Serial.printf("Measured pulse duration on pin %d: %lu µs\n", sensor->echoPin, pulseDurations[sensor->echoPin]);
  }
}

// 🔹 Hàm dựng: Chỉ gán giá trị cho chân Trig và Echo
US015::US015(int triggerPin, int echoPin) : trigPin(triggerPin), echoPin(echoPin) {}

// 🔹 Hàm `init()`: Cấu hình GPIO và đăng ký ISR
void US015::init() {
  pinMode(trigPin, OUTPUT);    // Cấu hình chân Trig là OUTPUT để gửi xung kích hoạt
  pinMode(echoPin, INPUT);     // Cấu hình chân Echo là INPUT để nhận tín hiệu phản hồi
  digitalWrite(trigPin, LOW);  // Đảm bảo Trig bắt đầu ở mức LOW

  sensors.push_back(this);  // Thêm đối tượng cảm biến vào danh sách quản lý

  static bool isISRServiceInitialized = false;
  if (!isISRServiceInitialized) {
    gpio_install_isr_service(0);  // 🔹 Khởi tạo dịch vụ ISR (chỉ cần chạy 1 lần)
    isISRServiceInitialized = true;
  }

  // # gpio_isr_handler_add
  // 📌 Đăng ký một ISR (Interrupt Service Routine) cho một chân GPIO trên ESP32.
  // 👉 Cú pháp:
  //    gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void* args);
  //
  // 🔹 Tham số:
  //    - `gpio_num`: Chân GPIO mà ISR sẽ lắng nghe sự kiện.
  //    - `isr_handler`: Hàm ISR sẽ được gọi khi có sự kiện trên `gpio_num`.
  //    - `args`: Tham số tùy chỉnh (con trỏ `void*`) được truyền vào ISR, thường là `this` trong class.
  //
  // 🔹 Chức năng:
  //    - Khi `gpio_num` thay đổi trạng thái (HIGH → LOW hoặc LOW → HIGH),
  //      `isr_handler` sẽ được gọi để xử lý sự kiện.
  //    - `args` giúp ISR biết được nó đang xử lý chân GPIO nào.
  //
  // 🔹 Yêu cầu trước khi gọi:
  //    - Phải gọi `gpio_install_isr_service(0);` trước khi đăng ký ISR, nếu không sẽ bị lỗi.
  //    - ISR cần có từ khóa `IRAM_ATTR` để đảm bảo thực thi nhanh trong RAM.
  //
  // 🔹 Trả về:
  //    - `ESP_OK` nếu đăng ký thành công.
  //    - Lỗi khác nếu có vấn đề trong quá trình đăng ký.
  //
  // 🚀 Ví dụ:
  //    gpio_isr_handler_add((gpio_num_t)echoPin, echo_ISR, (void*)this);
  //    - Khi `echoPin` thay đổi trạng thái, hàm `echo_ISR()` sẽ được gọi.
  //
  esp_err_t err = gpio_isr_handler_add((gpio_num_t)echoPin, echo_ISR, (void *)this);
  if (err != ESP_OK) {
    Serial.printf("Error adding ISR to pin %d: %d\n", echoPin, err);
  } else {
    Serial.printf("ISR added successfully on pin %d!\n", echoPin);
  }
}

// 🔹 Đo khoảng cách bằng `pulseIn()` (đơn vị cm)
float US015::measure() {
  // 📌 Gửi xung kích hoạt (Trigger Pulse) cho cảm biến US-015.
  digitalWrite(trigPin, LOW);   // Đảm bảo Trig bắt đầu từ mức LOW
  delayMicroseconds(4);         // Chờ 4µs để đảm bảo ổn định
  digitalWrite(trigPin, HIGH);  // Gửi xung HIGH để kích hoạt đo
  delayMicroseconds(10);        // Giữ Trig HIGH trong 10µs (chuẩn của US015)
  digitalWrite(trigPin, LOW);   // Kết thúc xung Trig
  // 🔹 Tại sao cần `delayMicroseconds(4);`?
  //    - Một số cảm biến US-015 yêu cầu **Trig bắt đầu từ mức LOW** trước khi gửi xung HIGH.
  //    - Chờ **ít nhất 4µs** để đảm bảo cảm biến nhận được trạng thái ổn định trước khi kích hoạt.
  //
  // 🔹 Tại sao cần `delayMicroseconds(10);`?
  //    - Theo datasheet của US-015, cảm biến yêu cầu **xung HIGH tối thiểu 10µs** để kích hoạt đo khoảng cách.
  //    - Nếu thời gian xung HIGH **<10µs**, cảm biến có thể không nhận lệnh đo.
  //
  // 🚀 Tóm lại:
  //    - `delayMicroseconds(4);` đảm bảo Trig không bị nhiễu trước khi kích hoạt.
  //    - `delayMicroseconds(10);` đảm bảo cảm biến nhận được lệnh đo chính xác.

  // 🔹 Đo thời gian xung Echo HIGH bằng `pulseIn()`
  long duration = pulseIn(echoPin, HIGH, 30000);  // Timeout 30ms (~500cm)
  if (duration == 0) {                            // Nếu không nhận được tín hiệu, báo lỗi
    Serial.println("Lỗi: Không nhận được tín hiệu ECHO.");
    return -1;
  }

  // 🔹 Tính khoảng cách theo công thức: distance = (duration * speed_of_sound) / 2
  return (duration * soundSpeed) / 2;
}

// 🔹 Đo khoảng cách trung bình với `samples` lần, sử dụng `millis()` (không
// chặn loop)
float US015::measureAverage(int samples) {
  if (samples <= 0) samples = 1;  // Đảm bảo ít nhất có 1 lần đo

  float sum = 0;
  int validSamples = 0;
  unsigned long lastTime = millis();  // Lưu thời điểm bắt đầu đo

  for (int i = 0; i < samples; i++) {
    while (millis() - lastTime < 5) {  // 🔹 Chờ 5ms giữa mỗi lần đo mà không chặn chương trình
      yield();                         // Cho phép hệ thống thực hiện các tác vụ khác
    }
    lastTime = millis();  // Cập nhật thời gian mẫu tiếp theo

    float distance = measure();  // Gọi `measure()` để đo khoảng cách
    if (distance > 0) {          // Bỏ qua các giá trị lỗi (-1)
      sum += distance;
      validSamples++;
    }
  }

  // 🔹 Trả về trung bình cộng nếu có mẫu hợp lệ, nếu không thì trả về -1
  return (validSamples > 0) ? (sum / validSamples) : -1;
}
