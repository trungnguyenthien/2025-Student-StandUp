Bạn có thể cấu hình VSCode để tự động format code khi save và sử dụng `{}` theo phong cách Java. Dưới đây là các bước chi tiết:

---

### 1. **Cài đặt extension Prettier**
- Mở VSCode.
- Nhấn `Ctrl + Shift + X` để mở **Extensions**.
- Tìm `Prettier - Code formatter` và cài đặt.

---

### 2. **Cấu hình Prettier để format khi save**
Mở **VSCode Settings** (`Ctrl + Shift + P` → Tìm `"Preferences: Open Settings (JSON)"`) và thêm các dòng sau:

```json
{
  "editor.formatOnSave": true,
  "[cpp]": {
    "editor.defaultFormatter": "esbenp.prettier-vscode"
  }
}
```

---

### 3. **Cấu hình Prettier để xuống dòng `{}` giống Java**
Mặc định, Prettier không hỗ trợ C++, nhưng bạn có thể sử dụng **clang-format** để đạt được mục tiêu này.

#### **Cài đặt `clang-format`**
- Nếu chưa có `clang-format`, cài đặt nó bằng cách:
  - **Windows**: Dùng `choco install llvm` (nếu đã cài Chocolatey).
  - **Ubuntu**: `sudo apt install clang-format`
  - **Mac**: `brew install clang-format`

#### **Cấu hình `clang-format`**
Tạo file `.clang-format` trong thư mục gốc của project (hoặc trong `~/.clang-format` nếu muốn áp dụng toàn cục), rồi thêm:

```yaml
BasedOnStyle: Google
BreakBeforeBraces: Allman
ColumnLimit: 100
IndentWidth: 4
```

**Giải thích:**
- `BasedOnStyle: Google`: Dùng style Google làm cơ sở.
- `BreakBeforeBraces: Allman`: Xuống dòng `{}` theo phong cách Java.
- `ColumnLimit: 100`: Giới hạn độ dài dòng code.
- `IndentWidth: 4`: Sử dụng indentation 4 spaces.

---

### 4. **Sử dụng `clang-format` với Prettier**
VSCode không hỗ trợ trực tiếp `clang-format` qua Prettier, nhưng bạn có thể cài **C/C++ Extension Pack** và cấu hình:

- Cài đặt extension **C/C++** (`ms-vscode.cpptools`).
- Cập nhật VSCode settings (`Ctrl + Shift + P` → `"Preferences: Open Settings (JSON)"`):

```json
{
  "editor.formatOnSave": true,
  "[cpp]": {
    "editor.defaultFormatter": "ms-vscode.cpptools"
  },
  "C_Cpp.clang_format_fallbackStyle": "Google"
}
```

---

### 5. **Kiểm tra**
Mở file `.cpp`, thử sửa code rồi bấm `Ctrl + S`. Code sẽ tự động được format với `{}` xuống dòng đúng theo Java style.

---

## **Tóm tắt**
✅ **Cài đặt Prettier và Clang-Format**  
✅ **Bật format code khi save**  
✅ **Cấu hình xuống dòng `{}` theo Java style**  
✅ **Sử dụng Clang-Format làm formatter mặc định**  

Nếu có vấn đề gì, bạn cứ hỏi tiếp nhé! 🚀