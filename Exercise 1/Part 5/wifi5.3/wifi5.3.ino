#include <WiFi.h>

const char* ssid = "W_I_F_I";
const char* password = "P_A_S_S";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[STA] Kết nối thành công!");
  Serial.print("[STATUS] IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("[STATUS] MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.print("[STATUS] RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void loop() {

}
