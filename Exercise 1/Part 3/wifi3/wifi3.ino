#include <WiFi.h>
#include "esp_wifi.h"

// Cấu hình Wi-Fi AP
const char* ap_ssid = "ESP32_AP";
const char* ap_pass = "12345678";

// Cấu hình Wi-Fi STA
const char* sta_ssid = "W_I_F_I";        // SSID router
const char* sta_pass = "P_A_S_S"; // Pass router

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP_STA);

  // 2. AP
  bool ap_ok = WiFi.softAP(ap_ssid, ap_pass);
  if (ap_ok) {
    Serial.println("[AP] Access Point đã tạo thành công!");
    Serial.print("[AP] SSID: "); Serial.println(ap_ssid);
    Serial.print("[AP] PASS: "); Serial.println(ap_pass);
    Serial.print("[AP] IP: "); Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("[AP] Tạo Access Point thất bại!");
  }

  // 3. STA
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
