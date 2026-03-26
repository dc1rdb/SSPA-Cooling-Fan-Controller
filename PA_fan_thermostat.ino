/* 
This code was written to control the temperature of an LDMOS SSPA module
by measuring the temperature directly at the heat spreader with a DS18B20 temperature probe and
outputting a signal with the Arduino Nano to control the fan of the cooler assembly.

DC1RDB
Ver. 1.1
Feb 20, 2026
*/


#include <OneWire.h>
#include <DallasTemperature.h>

// Digital pin of temperature sensor
#define ONE_WIRE_BUS 2  //4.7K pullup resistor reqd

// Setup a oneWire instance
OneWire oneWire(ONE_WIRE_BUS);  

// Setup temperature sensor library
DallasTemperature sensors(&oneWire);

// output pin to MOSFET switch
#define fanPin 10

// how frequently the main loop runs
const int tempSetInterval = 3000;  //temperature is measured at this interval


// temperature settings
const float tempLow = 20;  //at and below this temperature the fan shuts off
const float tempHigh = 24; //at and above this temperature the fan is on


void setup() {
	
	// start serial port 
	Serial.begin(9600); 
	
	// Start up the temperature library 
	sensors.begin();
	sensors.requestTemperatures();

	// configure pins
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(fanPin, OUTPUT);

	// fan power-on test for 2 sec
	digitalWrite(fanPin, HIGH);
	digitalWrite(LED_BUILTIN, HIGH);
  delay (2000);
  digitalWrite(fanPin, LOW);
	digitalWrite(LED_BUILTIN, LOW);

}


// main loop ##############################################
void loop() {
	
	sensors.requestTemperatures();
	float temp = sensors.getTempCByIndex(0);

	Serial.print(temp);
	Serial.print("°C, ");
	Serial.println(digitalRead (fanPin));

	if (temp >= tempHigh) {
    digitalWrite(fanPin, HIGH);
		digitalWrite(LED_BUILTIN, HIGH);
  } 
	else if (temp <= tempLow) {
    digitalWrite(fanPin, LOW);
		digitalWrite(LED_BUILTIN, LOW);
  }

	delay(tempSetInterval);
}