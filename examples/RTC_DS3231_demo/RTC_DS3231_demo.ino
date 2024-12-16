#include <RTClib.h>
RTC_DS3231 rtc;


void setup() {
  // put your setup code here, to run once:
  Wire.begin(38,39);
  Serial.begin(115200);
  if (! rtc.begin()) {
    Serial.println("RTC module is NOT found");
    while (1);  
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
  Serial.print("ESP32 RTC Date Time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(now.dayOfTheWeek());
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

}
