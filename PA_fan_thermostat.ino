/* 
This code was written to control the temperature of an LDMOS SSPA module
by measuring the temperature directly at the heat spreader with a DS18B20 temperature probe and
outputting a signal with the Arduino Nano to control the fan of the cooler assembly.

DC1RDB
Ver. 1.2
Apr 19, 2026
*/


#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define fanPin 10

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const unsigned long interval = 3000;
unsigned long lastUpdate = 0;

const float tempLow = 25.0;
const float tempHigh = 26.0;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  sensors.setWaitForConversion(false); 

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(fanPin, OUTPUT);

  // Self-test
  digitalWrite(fanPin, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000); 
  digitalWrite(fanPin, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= interval) {
    lastUpdate = currentMillis;

    // Request new temp and read the previous one immediately
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);

    // Filter out sensor errors (-127 occurs if disconnected)
    if (temp > -50) { 
      Serial.print(temp);
      Serial.print("°C, Fan: ");
      Serial.println(digitalRead(fanPin));

      if (temp >= tempHigh) {
        digitalWrite(fanPin, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
      } else if (temp <= tempLow) {
        digitalWrite(fanPin, LOW);
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }
}
