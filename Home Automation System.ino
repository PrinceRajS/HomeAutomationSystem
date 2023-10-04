#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define relay1 D1
#define relay2 D2

#define BLYNK_TEMPLATE_ID "TMPL3eDI3NMR1"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

char auth[] = "sSSROq4mm_gbRv5JP0Dg-o46qALT-fbW";
char ssid[] = "Garvit's Galaxy A52s 5G";
char pass[] = "qjqu276f";

BlynkTimer timer;
String auto_on_slot = "1200-1201";
String auto_off_slot = "1201-1200";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void check_auto_on_off()
{
  timeClient.update(); // Update the time from the NTP server
  String cur_time = String(timeClient.getHours() * 100 + timeClient.getMinutes());

  if (cur_time >= auto_on_slot && cur_time < auto_off_slot) {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  } else {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
  }
}

BLYNK_WRITE(V1)
{
  int value1 = param.asInt();
  if (value1 == 1) {
    digitalWrite(relay1, LOW);
  } else {
    digitalWrite(relay1, HIGH);
  }
}

BLYNK_WRITE(V2)
{
  int value2 = param.asInt();
  if   (value2 == 1) {
    digitalWrite(relay2, LOW);
  } else {
    digitalWrite(relay2, HIGH);
  }
}

void setup()
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  timer.setInterval(2000L, []() {
    // Check if Blynk server is connected every 2 seconds
    if (!Blynk.connected()) {
      // Implement your code to handle the disconnection here
    }
  });

  timer.setInterval(1000L, []() {
    // Insert your code here to read sensor data and send it to Blynk
  });

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timeClient.begin(); // Start NTP time synchronization
}

void loop()
{
  Blynk.run();

  // Check auto-on/off slots every minute
  static unsigned long prev_check_time = 0;
  if (millis() - prev_check_time >= 60000) {
    check_auto_on_off();
    prev_check_time = millis();
  }

  timer.run();
}
