#include <WiFi.h>
#include "esp_wifi.h"   // để dùng esp_wifi_ap_get_sta_list

const char* ap_ssid = "ESP32_AP";
const char* ap_pass = "12345678";

int lastClientCount = -1; // lưu số client lần trước

void setup() {
  Serial.begin(115200);
  delay(1000);

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

  // Lấy danh sách client hiện tại
  esp_wifi_ap_get_sta_list(&stationList);
  tcpip_adapter_get_sta_list(&stationList, &adapter_sta_list);

  int currentCount = adapter_sta_list.num;

  // Chỉ in nếu số lượng thay đổi
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

    lastClientCount = currentCount; // update
  }

  delay(1000);
}
