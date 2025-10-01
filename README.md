# ESP32 Wi-Fi Client (Station Mode) - Bài tập 1

Tài liệu này mô tả bài tập thực hành với ESP32: Kết nối vào mạng Wi-Fi hiện có (station mode), nhận IP qua DHCP, và hoạt động như TCP client để gửi dữ liệu đến server (PC). Code còn hỗ trợ nhận chuỗi từ Serial Monitor và chuyển tiếp lên server (một chiều: PC → ESP32 → Server).

## Mục tiêu & Kết quả cần đạt

- ESP32 gia nhập mạng Wi-Fi hiện có (SSID/PASS).
- Lấy IP nội bộ (LAN) do router cấp qua DHCP và in ra Serial.
- Mở kết nối TCP đến server (PC chạy Python) và gửi chuỗi.
- Từ Serial Monitor, gõ một dòng → ESP32 gửi dòng đó tới server.

**Kết quả mong đợi:**
- ESP32 kết nối Wi-Fi thành công, hiển thị IP.
- Kết nối TCP đến server và gửi "Hello from ESP32!".
- Gõ lệnh trong Serial Monitor → Xem phản hồi gửi thành công và server nhận dữ liệu.

## Yêu cầu

- **Phần cứng:** ESP32 (bất kỳ board nào hỗ trợ Arduino IDE).
- **Phần mềm:**
  - Arduino IDE với ESP32 core (cài qua Board Manager).
  - Serial Monitor (trong Arduino IDE).
  - Python 3.x trên PC để chạy server mẫu.
- **Môi trường:** ESP32 và PC cùng mạng LAN (Wi-Fi).

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>

const char* ssid     = "P810";
const char* password = "KhongMatKhau";

// TCP server
const char* host = "192.168.1.11";
const uint16_t port = 5000;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n[ESP32] Kết nối Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[ESP32] Kết nối thành công!");
  Serial.print("[ESP32] IP ESP32: ");
  Serial.println(WiFi.localIP());
  Serial.printf("[ESP32] Đang kết nối đến %s:%d ...\n", host, port);
  if (client.connect(host, port)) {
    Serial.println("[ESP32] Đã kết nối server!");
    client.println("Hello from ESP32!");
  } else {
    Serial.println("[ESP32] Kết nối server thất bại!");
  }
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // đọc 1 dòng
    input.trim();
    if (input.length() > 0) {
      if (client.connected()) {
        client.println(input);
        Serial.println("[ESP32] Đã gửi: " + input);
      } else {
        Serial.println("[ESP32] Mất kết nối server, không gửi được.");
      }
    }
  }
  delay(10);
}
```

### Hướng dẫn upload code

1. Mở Arduino IDE.
2. Chọn board ESP32 (Tools > Board > ESP32 Arduino > ESP32 Dev Module).
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ssid`, `password`, `host` (IP PC), `port` cho phù hợp.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi.

## Giải thích từng dòng / từng hàm (thực dụng)

### Khối include & cấu hình

- `#include <WiFi.h>`: Thư viện Wi-Fi chính thức của ESP32 Arduino core. Cung cấp lớp `WiFi` (quản lý STA/AP), `WiFiClient` (TCP client), v.v.
- `const char* ssid = "...";`, `const char* password = "...";`: Chuỗi C bất biến. Dùng cho `WiFi.begin()` để bắt tay với AP.
- `const char* host = "192.168.1.11";`: IP IPv4 của PC chạy server (lấy từ `ipconfig` hoặc Task Manager).
- `const uint16_t port = 5000;`: Cổng TCP server lắng nghe.
- `WiFiClient client;`: Đối tượng TCP client, xử lý handshake và stream dữ liệu.

### setup()

- `Serial.begin(115200);`: Mở UART CDC với baud 115200 (đồng bộ Serial Monitor).
- `delay(1000);`: Chờ ổn định sau boot.
- `Serial.println("\n[ESP32] Kết nối Wi-Fi...");`: Log khởi đầu.
- `WiFi.begin(ssid, password);`: Bắt đầu kết nối STA mode (probe/auth/assoc + DHCP).
- `while (WiFi.status() != WL_CONNECTED) { ... }`: Poll trạng thái (blocking). In "." mỗi 500ms.
- `Serial.print("[ESP32] IP ESP32: "); Serial.println(WiFi.localIP());`: In IP do DHCP cấp.
- `Serial.printf("[ESP32] Đang kết nối đến %s:%d ...\n", host, port);`: Log đích TCP.
- `if (client.connect(host, port)) { ... }`: Kết nối TCP (true nếu ESTABLISHED). Gửi "Hello from ESP32!" nếu thành công.

### loop()

- `if (Serial.available()) { ... }`: Kiểm tra dữ liệu từ Serial (USB).
- `String input = Serial.readStringUntil('\n');`: Đọc một dòng (blocking đến timeout 1s).
- `input.trim();`: Xóa khoảng trắng đầu/cuối.
- `if (input.length() > 0) { ... }`: Bỏ qua dòng rỗng.
- `if (client.connected()) { ... }`: Kiểm tra socket còn mở. Gửi `client.println(input);` nếu OK.
- `delay(10);`: Yield CPU cho task nền (Wi-Fi/TCP).

**Lưu ý:** Code blocking đơn giản, phù hợp bài tập. Nâng cao: Thêm timeout, reconnect tự động.

## Luồng hoạt động (tóm tắt)

1. **Boot → Setup Serial.**
2. **Kết nối Wi-Fi:** `WiFi.begin()` → Đợi `WL_CONNECTED` → Nhận IP DHCP.
3. **In IP:** `WiFi.localIP()`.
4. **TCP Connect:** `client.connect(host, port)` → Gửi "Hello".
5. **Loop:** Nhận dòng từ Serial Monitor → Gửi lên server nếu connected.

## Server mẫu để test (Python)

Chạy trên PC để nhận dữ liệu từ ESP32.

```python
import socket

HOST = "0.0.0.0"   # lắng nghe trên mọi interface
PORT = 5000

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(1)
print(f"Server listening on {HOST}:{PORT}")

conn, addr = server.accept()
print("Kết nối từ:", addr)

while True:
    data = conn.recv(1024)
    if not data:
        break
    msg = data.decode(errors="ignore").strip()  # bỏ CRLF
    if msg:
        print("Nhận:", msg)
```

### Chạy server

1. Lưu code vào file `server.py`.
2. Chạy: `python server.py`.
3. **Firewall:** Cho phép Python qua Windows Firewall nếu cần.
4. Kết quả: Server in "Kết nối từ: (IP_ESP32, port)" và các msg từ ESP32.

## Troubleshooting

- **Wi-Fi không kết nối:** Kiểm tra SSID/pass, tín hiệu mạnh.
- **TCP fail:** Sai IP PC? Server chưa chạy? Firewall chặn? Khác subnet?
- **Serial rác:** Baud không khớp (115200).
- **Mất kết nối:** Thêm reconnect trong loop() cho production.

## Mở rộng

- Thêm HTTP client (WiFiClientSecure cho HTTPS).
- Xử lý dữ liệu hai chiều (recv từ server).
- Non-blocking với FreeRTOS tasks.

## License

MIT License - Feel free to use/modify.

---

*Author: [Your Name]* | *Last updated: October 01, 2025*
