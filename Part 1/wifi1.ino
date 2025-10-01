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
