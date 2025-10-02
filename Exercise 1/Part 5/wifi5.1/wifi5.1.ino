#include <WiFi.h>

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1000);
    Serial.println("Setup done");
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

  // Free mem
  WiFi.scanDelete();
  delay(5000);
}
