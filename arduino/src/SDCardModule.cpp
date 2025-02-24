#include "SDCardModule.h"

// 🔹 Constructor: Lưu chân CS
SDCardModule::SDCardModule(int cs) : csPin(cs) {}

// 🔹 Khởi tạo SD Card
bool SDCardModule::begin() {
  if (!SD.begin(csPin)) {
    Serial.println("❌ SD Card không thể khởi động!");
    return false;
  }
  Serial.println("✅ SD Card đã khởi động thành công!");
  isInitialized = true;
  return true;
}

// 🔹 Ghi dữ liệu vào file trên SD Card
bool SDCardModule::writeFile(const char* path, const char* message) {
  if (!isInitialized) {
    Serial.println("❌ SD Card chưa được khởi tạo!");
    return false;
  }

  Serial.printf("✍️ Ghi dữ liệu vào file: %s\n", path);
  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("❌ Không thể mở file để ghi!");
    return false;
  }

  file.println(message);
  file.close();
  Serial.println("✅ Ghi file thành công!");
  return true;
}

// 🔹 Đọc dữ liệu từ file trên SD Card
String SDCardModule::readFile(const char* path) {
  if (!isInitialized) {
    Serial.println("❌ SD Card chưa được khởi tạo!");
    return "";
  }

  Serial.printf("📖 Đọc file: %s\n", path);
  File file = SD.open(path);
  if (!file) {
    Serial.println("❌ Không thể mở file để đọc!");
    return "";
  }

  String content = "";
  while (file.available()) {
    content += (char)file.read();
  }
  file.close();
  Serial.println("✅ Đọc file thành công!");
  return content;
}

// 🔹 Xóa file trên SD Card
bool SDCardModule::deleteFile(const char* path) {
  if (!isInitialized) {
    Serial.println("❌ SD Card chưa được khởi tạo!");
    return false;
  }

  Serial.printf("🗑 Xóa file: %s\n", path);
  if (SD.remove(path)) {
    Serial.println("✅ File đã được xóa!");
    return true;
  } else {
    Serial.println("❌ Không thể xóa file!");
    return false;
  }
}

// 🔹 Liệt kê danh sách file trong thư mục
void SDCardModule::listFiles(const char* dirname, uint8_t levels) {
  if (!isInitialized) {
    Serial.println("❌ SD Card chưa được khởi tạo!");
    return;
  }

  Serial.printf("📂 Danh sách file trong thư mục: %s\n", dirname);
  File root = SD.open(dirname);
  if (!root) {
    Serial.println("❌ Không thể mở thư mục!");
    return;
  }

  if (!root.isDirectory()) {
    Serial.println("❌ Đây không phải là thư mục!");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    Serial.print(file.name());
    if (file.isDirectory()) {
      Serial.println(" 📁 [Thư mục]");
      if (levels) {
        listFiles(file.name(), levels - 1);
      }
    } else {
      Serial.printf(" 📄 [File] - %d bytes\n", file.size());
    }
    file = root.openNextFile();
  }
}

// 🔹 Lấy tổng dung lượng thẻ SD (đơn vị byte)
uint64_t SDCardModule::getTotalSize() {
  if (!isInitialized) return 0;
  return SD.cardSize();
}

// 🔹 Lấy dung lượng đã sử dụng trên thẻ SD (đơn vị byte)
uint64_t SDCardModule::getUsedSize() {
  if (!isInitialized) return 0;
  return SD.usedBytes();
}
