#include <WiFi.h>

const char* ssid = "VJU Student";
const char* password = "studentVJU@2022";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("[STA] Đang kết nối");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n[STA] IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[STA] Mất kết nối! Thử reconnect...");
    WiFi.begin(ssid, password);
    delay(5000);
  } else {
    Serial.println("[STA] Online");
    delay(3000);
  }
}
