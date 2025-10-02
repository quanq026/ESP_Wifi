# ESP32 Wi-Fi

## Mục lục

- [Bài tập 1: ESP32 Wi-Fi Client (Station Mode)](https://github.com/quanq026/ESP_Wifi/blob/main/README.md#b%C3%A0i-t%E1%BA%ADp-1-esp32-wi-fi-client-station-mode)

- [Bài tập 2: ESP32 Wi-Fi Access Point (AP Mode)](https://github.com/quanq026/ESP_Wifi/blob/main/README.md#b%C3%A0i-t%E1%BA%ADp-2-esp32-wi-fi-access-point-ap-mode)

- [Bài tập 3: ESP32 Dual Mode (AP + STA)](https://github.com/quanq026/ESP_Wifi/blob/main/README.md#b%C3%A0i-t%E1%BA%ADp-3-esp32-dual-mode-ap--sta)

- [Bài tập 4:...]()

- [Bài tập 5.1: ESP32 Wi-Fi Scanner](https://github.com/quanq026/ESP_Wifi/blob/main/README.md#b%C3%A0i-t%E1%BA%ADp-51-esp32-wi-fi-scanner)

- [Bài tập 5.2: ESP32 Wi-Fi Reconnect](https://github.com/quanq026/ESP_Wifi/blob/main/README.md#b%C3%A0i-t%E1%BA%ADp-52-esp32-wi-fi-reconnect)

- [Bài tập 5.3: ESP32 Wi-Fi Status]()

## Phần cứng & phần mềm chung
- **Phần cứng:** ESP32 (bất kỳ board nào hỗ trợ Arduino IDE).
- **Phần mềm:**
  - Arduino IDE với ESP32 core (cài qua Board Manager).
  - Serial Monitor (trong Arduino IDE).
  - Python 3.x trên PC để chạy server mẫu. (Part 1)
- **Môi trường:** ESP32 và PC (Part 1) cùng mạng LAN (Wi-Fi).
---

# Bài tập 1: ESP32 Wi-Fi Client (Station Mode)

Tài liệu này mô tả bài tập thực hành với ESP32: Kết nối vào mạng Wi-Fi hiện có (station mode), nhận IP qua DHCP, và hoạt động như TCP client để gửi dữ liệu đến server (PC). Code còn hỗ trợ nhận chuỗi từ Serial Monitor và chuyển tiếp lên server.

Qua bài tập:
- Hiểu cách ESP32 kết nối Wi-Fi và nhận IP DHCP.
- Quan sát kết nối TCP đến server và gửi dữ liệu.
- Theo dõi việc chuyển tiếp dữ liệu từ Serial Monitor lên server.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>

const char* ssid     = "W_I_F_I";
const char* password = "P_A_S_S";

// TCP server
const char* host = "IP_PC";
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
    String input = Serial.readStringUntil('\n');
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

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32.
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ssid`, `password`, `host` (IP PC), `port` cho phù hợp.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi.

## Giải thích mã nguồn
### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi chính thức của ESP32 Arduino core. Cung cấp lớp `WiFi` (quản lý STA/AP), `WiFiClient` (TCP client), v.v.

### Khối include & cấu hình

- `const char* ssid = "...";`, `const char* password = "...";`: Chuỗi C bất biến. Dùng cho `WiFi.begin()` để bắt tay với AP.
- `const char* host = "IP_PC";`: IP IPv4 của PC chạy server (lấy từ `ipconfig` hoặc Task Manager).
- `const uint16_t port = 5000;`: Cổng TCP server lắng nghe.
- `WiFiClient client;`: Đối tượng TCP client, xử lý handshake và stream dữ liệu.

### Cấu hình STA (trong setup())

- `Serial.begin(115200);`: Mở UART CDC với baud 115200 (đồng bộ Serial Monitor).
- `Serial.println("\n[ESP32] Kết nối Wi-Fi...");`: Log khởi đầu.
- `WiFi.begin(ssid, password);`: Bắt đầu kết nối STA mode (probe/auth/assoc + DHCP).
- `while (WiFi.status() != WL_CONNECTED) { ... }`: Poll trạng thái (blocking). In "." mỗi 500ms.
- `Serial.print("[ESP32] IP ESP32: "); Serial.println(WiFi.localIP());`: In IP do DHCP cấp.
- `Serial.printf("[ESP32] Đang kết nối đến %s:%d ...\n", host, port);`: Log đích TCP.
- `if (client.connect(host, port)) { ... }`: Kết nối TCP (true nếu ESTABLISHED). Gửi "Hello from ESP32!" nếu thành công.

### Theo dõi và gửi dữ liệu (trong loop())

- `if (Serial.available()) { ... }`: Kiểm tra dữ liệu từ Serial (USB).
- `String input = Serial.readStringUntil('\n');`: Đọc một dòng (blocking đến timeout 1s).
- `input.trim();`: Xóa khoảng trắng đầu/cuối.
- `if (input.length() > 0) { ... }`: Bỏ qua dòng rỗng.
- `if (client.connected()) { ... }`: Kiểm tra socket còn mở. Gửi `client.println(input);` nếu OK.

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

---

# Bài tập 2: ESP32 Wi-Fi Access Point (AP Mode)

Bài tập này hướng dẫn cấu hình ESP32 ở chế độ Access Point (AP), nơi ESP32 phát sóng một mạng Wi-Fi riêng biệt (SSID/PASS). Các thiết bị khác như điện thoại, laptop hoặc ESP32 khác có thể kết nối trực tiếp mà không cần router trung gian.

Qua bài tập:
- Hiểu cách ESP32 tạo mạng Wi-Fi cục bộ.
- Quan sát địa chỉ IP mặc định của AP (192.168.4.1).
- Theo dõi danh sách các client (thiết bị) kết nối, bao gồm MAC ID và IP của chúng.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>
#include "esp_wifi.h"   // Để sử dụng esp_wifi_ap_get_sta_list

const char* ap_ssid = "ESP32_AP";
const char* ap_pass = "12345678";

int lastClientCount = -1; // Lưu số client lần trước để tránh spam log

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Tạo Access Point
  WiFi.softAP(ap_ssid, ap_pass);

  // Tạo Access Point
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("[ESP32] AP đã tạo");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("Password: ");
  Serial.println(ap_pass);
  Serial.print("[ESP32] IP AP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  wifi_sta_list_t stationList;
  tcpip_adapter_sta_list_t adapter_sta_list;

  // Lấy danh sách client đang kết nối
  esp_wifi_ap_get_sta_list(&stationList);
  tcpip_adapter_get_sta_list(&stationList, &adapter_sta_list);

  int currentCount = adapter_sta_list.num;

  // Chỉ in khi số lượng client thay đổi
  if (currentCount != lastClientCount) {
    Serial.print("[ESP32] Số client kết nối: ");
    Serial.println(currentCount);

    for (int i = 0; i < currentCount; i++) {
      tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
      Serial.printf("Client %d MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", i + 1,
                    station.mac[0], station.mac[1], station.mac[2],
                    station.mac[3], station.mac[4], station.mac[5]);
      Serial.print("IP: ");
      Serial.println(IPAddress(station.ip.addr));
    }

    lastClientCount = currentCount;
  }

  delay(1000);
}
```

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32 (Tools > Board > ESP32 Arduino > ESP32 Dev Module).
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ap_ssid` và `ap_pass` nếu cần.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi log.

## Giải thích mã nguồn

### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi, hỗ trợ chế độ AP/STA.
- `#include "esp_wifi.h"`: Thư viện nâng cao để truy cập hàm ESP-IDF như lấy danh sách client và MAC.

### Cấu hình AP (trong setup())

- `WiFi.softAP(ap_ssid, ap_pass);`: Khởi tạo ESP32 thành Access Point với SSID và mật khẩu. ESP32 sẽ tự động cấp IP cho client qua DHCP nội bộ.
- `WiFi.softAPIP();`: Trả về IP của AP (mặc định 192.168.4.1, dùng để truy cập nếu cần web server).
- `WiFi.softAPmacAddress();`: Trả về địa chỉ MAC của ESP32 trong vai trò AP.
- `WiFi.channel();`: Trả về kênh Wi-Fi đang sử dụng.

### Lấy danh sách client (trong loop())

- `esp_wifi_ap_get_sta_list(&stationList);`: Lấy danh sách thô các client (chủ yếu MAC).
- `tcpip_adapter_get_sta_list(&stationList, &adapter_sta_list);`: Bổ sung thông tin IP từ adapter TCP/IP. `adapter_sta_list.num` là số client hiện tại.
- `station.mac[]`: Mảng 6 byte chứa địa chỉ MAC của client.
- `station.ip.addr`: Địa chỉ IP được cấp cho client.

### In log khi số client thay đổi

- `lastClientCount`: Biến lưu số client vòng lặp trước.
- Nếu `currentCount != lastClientCount`: In số lượng và chi tiết (MAC, IP) của từng client qua vòng lặp `for`. Cập nhật `lastClientCount` để tránh lặp lại log không cần thiết.

---

# Bài tập 3: ESP32 Dual Mode (AP + STA)

Bài tập này hướng dẫn cấu hình ESP32 ở chế độ Dual Mode, kết hợp cả Access Point (AP) và Station (STA). ESP32 sẽ vừa kết nối tới router Wi-Fi như một thiết bị thông thường (STA), vừa phát sóng mạng Wi-Fi riêng (AP). Điều này biến ESP32 thành một gateway/bridge nhỏ: liên lạc với internet qua router và cho phép các thiết bị khác kết nối trực tiếp vào AP của ESP32.

Qua bài tập:
- Hiểu cách kích hoạt chế độ kép (AP + STA).
- Quan sát IP của cả AP (192.168.4.1) và STA (từ router).
- Theo dõi số lượng client kết nối vào AP.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>
#include "esp_wifi.h"

// Cấu hình Wi-Fi AP
const char* ap_ssid = "ESP32_AP";
const char* ap_pass = "12345678";

// Cấu hình Wi-Fi STA
const char* sta_ssid = "W_I_F_I";  // SSID router
const char* sta_pass = "P_A_S_S"; // Pass router

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Bật chế độ kép: vừa AP vừa STA
  WiFi.mode(WIFI_AP_STA);

  // AP
  bool ap_ok = WiFi.softAP(ap_ssid, ap_pass);
  if (ap_ok) {
    Serial.println("[AP] Access Point đã tạo thành công!");
    Serial.print("[AP] SSID: "); Serial.println(ap_ssid);
    Serial.print("[AP] PASS: "); Serial.println(ap_pass);
    Serial.print("[AP] IP: "); Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("[AP] Tạo Access Point thất bại!");
  }

  // STA
  Serial.print("[STA] Đang kết nối tới router: ");
  Serial.println(sta_ssid);
  WiFi.begin(sta_ssid, sta_pass);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[STA] Kết nối router thành công!");
    Serial.print("[STA] IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("[STA] Gateway: ");
    Serial.println(WiFi.gatewayIP());
  } else {
    Serial.println("\n[STA] Kết nối router thất bại!");
  }

  Serial.println("========================================");
}

void loop() {
  static int lastClient = -1;
  int currentClient = WiFi.softAPgetStationNum();
  if (currentClient != lastClient) {
    Serial.printf("[AP] Số client kết nối: %d\n", currentClient);
    lastClient = currentClient;
  }
  delay(1000);
}
```

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32 (Tools > Board > ESP32 Arduino > ESP32 Dev Module).
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ap_ssid`, `ap_pass`, `sta_ssid`, `sta_pass` cho phù hợp.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi log.

## Giải thích mã nguồn

### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi, hỗ trợ chế độ AP/STA/Dual.
- `#include "esp_wifi.h"`: Thư viện nâng cao để truy cập API ESP-IDF như lấy số lượng client.

### Cấu hình Dual Mode (trong setup())

- `WiFi.mode(WIFI_AP_STA);`: Kích hoạt chế độ kép, cho phép ESP32 hoạt động đồng thời như AP và STA.
- `WiFi.softAP(ap_ssid, ap_pass);`: Khởi tạo Access Point với SSID và mật khẩu.
- `WiFi.softAPIP();`: Trả về IP của AP.
- `WiFi.begin(sta_ssid, sta_pass);`: Bắt đầu kết nối STA tới router (SSID/PASS). Sử dụng DHCP để nhận IP từ router.
- `while (WiFi.status() != WL_CONNECTED && retry < 20) { ... }`: Poll trạng thái kết nối STA.
- `WiFi.localIP();`: IP mà router cấp cho ESP32.
- `WiFi.gatewayIP();`: IP gateway của router.

### Theo dõi client (trong loop())

- `WiFi.softAPgetStationNum();`: Trả về số lượng client đang kết nối vào AP.
- `static int lastClient = -1;`: Biến tĩnh lưu số client vòng lặp trước (khởi tạo -1 để in lần đầu).
- Nếu `currentClient != lastClient`: In số lượng client và cập nhật biến để tránh spam log.

# Bài tập 5.1: ESP32 Wi-Fi Scanner

Bài tập này hướng dẫn sử dụng ESP32 với Arduino Core để quét và liệt kê các mạng Wi-Fi xung quanh. Chương trình chạy ở chế độ Station (STA), quét toàn bộ mạng trong phạm vi, và hiển thị SSID theo định dạng bảng gọn gàng. Quét tự động lặp lại mỗi 5 giây.

Qua bài tập:
- Hiểu cách ESP32 quét Wi-Fi ở chế độ STA.
- Quan sát danh sách mạng.
- Xử lý kết quả quét và giải phóng bộ nhớ hiệu quả.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>

void setup() {
    Serial.begin(115200);

    // ESP32 ở chế độ Station (chỉ quét, không phát Wi-Fi)
    WiFi.mode(WIFI_STA);

    // Ngắt kết nối nếu đang kết nối AP nào trước đó
    WiFi.disconnect();
    delay(1000);

    Serial.println("===== ESP32 Wi-Fi Scanner =====");
}

void loop() {
    Serial.println("[SCAN] Đang quét Wi-Fi xung quanh...");

    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("[SCAN] Không tìm thấy mạng nào!");
    } else {
        Serial.printf("[SCAN] Tìm thấy %d mạng:\n", n);
        Serial.println("Nr | SSID                             | RSSI (dBm)");

        for (int i = 0; i < n; ++i) {
            Serial.printf("%2d | ", i + 1);
            Serial.printf("%-32.32s | ", WiFi.SSID(i).c_str());
            Serial.printf("%4d\n", WiFi.RSSI(i));
        }
    }

    // Giải phóng bộ nhớ scan
    WiFi.scanDelete();

    // Chờ 5 giây trước lần quét tiếp theo
    delay(5000);
}
```

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32 (Tools > Board > ESP32 Arduino > ESP32 Dev Module).
3. Kết nối ESP32 qua USB.
4. Paste code vào sketch mới.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi kết quả quét.

## Giải thích mã nguồn

### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi cốt lõi cho ESP32.

### Cấu hình ban đầu (trong setup())

- `WiFi.mode(WIFI_STA);`: Đặt ESP32 ở chế độ Station (chỉ quét, không phát AP).
- `WiFi.disconnect();`: Ngắt bất kỳ kết nối cũ nào để tránh ảnh hưởng đến quét.
- `delay(1000);`: Chờ ổn định sau khi ngắt kết nối.

### Quét Wi-Fi (trong loop())

- `int n = WiFi.scanNetworks();`: Thực hiện quét toàn bộ mạng Wi-Fi xung quanh, trả về số lượng mạng tìm thấy (n).
- Nếu `n == 0`: In thông báo không tìm thấy mạng.
- Vòng lặp `for (int i = 0; i < n; ++i)`: Duyệt qua từng mạng:
  - `WiFi.SSID(i)`: Trả về tên SSID của mạng thứ i (String).
  - `WiFi.RSSI(i)`: Trả về cường độ tín hiệu (RSSI) của mạng thứ i (int, đơn vị dBm).
  - `Serial.printf("%-32.32s | ", WiFi.SSID(i).c_str());`: In SSID với độ rộng cố định 32 ký tự (cắt nếu dài hơn).
- `WiFi.scanDelete();`: Giải phóng bộ nhớ đã cấp cho kết quả quét.
- `delay(5000);`: Tạm dừng 5 giây trước lần quét tiếp theo.

**Lưu ý:** RSSI (Received Signal Strength Indicator):
- -30 dBm: Mạnh (sát router).
- -67 dBm: Ổn định.
- -90 dBm: Gần như mất sóng.

# Bài tập 5.2: ESP32 Wi-Fi Reconnect

Bài tập này hướng dẫn triển khai cơ chế tự động reconnect Wi-Fi trên ESP32 để xử lý tình huống mất kết nối tạm thời (router restart, di chuyển ra vùng phủ sóng, gián đoạn mạng). Chương trình kiểm tra trạng thái kết nối định kỳ và thử kết nối lại nếu cần, đảm bảo ESP32 không bị treo.

Qua bài tập:
- Hiểu cách kiểm tra trạng thái Wi-Fi bằng `WiFi.status()`.
- Triển khai reconnect đơn giản trong `loop()` mà không chặn chương trình.
- Quan sát log khi kết nối thành công, mất kết nối và reconnect.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>

// Thông tin mạng Wi-Fi
const char* ssid = "W_I_F_I";       // SSID của mạng
const char* password = "P_A_S_S";   // Mật khẩu

void setup() {
  Serial.begin(115200);

  // Kết nối Wi-Fi lần đầu
  WiFi.begin(ssid, password);
  Serial.print("[STA] Đang kết nối");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\n[STA] IP: ");
  Serial.println(WiFi.localIP());  // In ra IP do DHCP cấp
}

void loop() {
  // Nếu mất kết nối
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[STA] Mất kết nối! Thử reconnect...");
    WiFi.begin(ssid, password); // Thử reconnect
    delay(5000);                // Chờ 5 giây trước lần thử tiếp
  } else {
    // Nếu đang online
    Serial.println("[STA] Đang online");
    delay(3000);
  }
}
```

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32 (Tools > Board > ESP32 Arduino > ESP32 Dev Module).
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ssid` và `password` cho phù hợp với router của bạn.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi log kết nối.

## Giải thích mã nguồn

### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi cốt lõi cho ESP32.
### Kết nối ban đầu (trong setup())

- `Serial.begin(115200);`: Khởi tạo Serial Monitor với baud rate 115200.
- `WiFi.begin(ssid, password);`: Bắt đầu kết nối Wi-Fi ở chế độ Station (STA) với SSID và mật khẩu.
- `while (WiFi.status() != WL_CONNECTED) { ... }`: Poll trạng thái (blocking). In "." mỗi 500ms.
- `WiFi.status()`: Trả về trạng thái kết nối (wl_status_t):
  - `WL_CONNECTED`: Kết nối thành công.
  - `WL_NO_SSID_AVAIL`, `WL_CONNECT_FAILED`, `WL_CONNECTION_LOST`: Các trạng thái lỗi/mất kết nối.
- `WiFi.localIP();`: In địa chỉ IP do router cấp.
### Xử lý reconnect (trong loop())

- `if (WiFi.status() != WL_CONNECTED) { ... }`: Kiểm tra định kỳ trạng thái kết nối.
- `WiFi.begin(ssid, password);`: Gọi lại để thử kết nối mới.
- `delay(5000);`: Chờ 5 giây sau mỗi lần thử reconnect.
- `else { ... }`: Nếu connected, in trạng thái online và delay 3 giây.

# Bài tập 5.3: ESP32 Wi-Fi Status

Bài tập này hướng dẫn giám sát trạng thái kết nối Wi-Fi trên ESP32 sau khi kết nối thành công đến mạng có sẵn. Chương trình lấy và hiển thị thông tin cơ bản như IP (do router cấp), MAC Address (địa chỉ phần cứng), và RSSI (cường độ tín hiệu, dBm).

Qua bài tập:
- Hiểu cách lấy thông tin trạng thái Wi-Fi sau khi kết nối.
- Quan sát IP, MAC và RSSI qua Serial Monitor.
- Đánh giá chất lượng tín hiệu dựa trên giá trị RSSI.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>

const char* ssid = "VJU Student";          // SSID Wi-Fi
const char* password = "studentVJU@2022";  // Mật khẩu Wi-Fi

void setup() {
  Serial.begin(115200);

  // Bắt đầu kết nối
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[STA] Kết nối thành công!");

  // In thông tin trạng thái
  Serial.print("[STATUS] IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("[STATUS] MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.print("[STATUS] RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void loop() {
  // Không làm gì, in thông tin một lần sau khi kết nối
}
```

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32 (Tools > Board > ESP32 Arduino > ESP32 Dev Module).
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ssid` và `password` cho phù hợp với router của bạn.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi log kết nối và trạng thái.

## Giải thích mã nguồn

### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi cốt lõi cho ESP32.

### Kết nối và hiển thị trạng thái (trong setup())

- `Serial.begin(115200);`: Khởi tạo Serial Monitor với baud rate 115200.
- `WiFi.begin(ssid, password);`: Bắt đầu kết nối Wi-Fi ở chế độ Station (STA) với SSID và mật khẩu.
- `while (WiFi.status() != WL_CONNECTED) { ... }`: Poll trạng thái (blocking). In "." mỗi 500ms.
- `WiFi.status()`: Trả về trạng thái kết nối (wl_status_t). `WL_CONNECTED` xác nhận kết nối ổn định.
- `WiFi.localIP();`: Trả về địa chỉ IP của ESP32 trong mạng LAN.
- `WiFi.macAddress();`: Trả về địa chỉ MAC của ESP32.
- `WiFi.RSSI();`: Trả về cường độ tín hiệu nhận được (RSSI, đơn vị dBm)

**Lưu ý:** RSSI (Received Signal Strength Indicator):
- -30 dBm: Mạnh (sát router).
- -67 dBm: Ổn định.
- -90 dBm: Gần như mất sóng.
