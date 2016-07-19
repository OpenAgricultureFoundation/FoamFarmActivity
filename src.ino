#include <Wire.h>
#include "Adafruit_AM2315.h"

int LIGHT = 3;
int FAN = 4;
int HUMIDIFIER = 5;
int PUMP = 6;

int lightOnPeriod = 10000;
int lightOffPeriod = 5000;

bool isLightOn = false;

int lightTurnedOn = 0;
int lightTurnedOff = 0;

int desiredTemperature = 22;
int desiredHumidity = 47;

Adafruit_AM2315 am2315;

float currentTemperature;
float currentHumidity;

void setup() {
  Serial.begin(9600);

  pinMode(FAN, OUTPUT);
  pinMode(HUMIDIFIER, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(PUMP, OUTPUT);

  if (!am2315.begin()) {
    Serial.println("Failed to initialize am2315");
    while (true);
  }
}

void loop() {
  int ms = millis();

  // If the light has been on for too long, turn it off
  if (isLightOn) {
    if (ms - lightTurnedOn > lightOnPeriod) {
      lightTurnedOff = lightTurnedOn + lightOnPeriod;
      isLightOn = false;
      digitalWrite(LIGHT, LOW);
    }
  }
  // If the light has been off for too long, turn it on
  else {
    if (ms - lightTurnedOff > lightOffPeriod) {
      lightTurnedOn = lightTurnedOff + lightOffPeriod;
      isLightOn = true;
      digitalWrite(LIGHT, HIGH);
    }
  }

  if (!am2315.readTemperatureAndHumidity(currentTemperature, currentHumidity)) {
    Serial.println("Failed to read from am2315");
  }
  else {
    Serial.print("Air Temperature: ");
    Serial.println(currentTemperature);
    Serial.print("Air Humidity: ");
    Serial.println(currentHumidity);
    if (desiredHumidity > currentHumidity) {
      digitalWrite(HUMIDIFIER, HIGH);
    }
    else {
      digitalWrite(HUMIDIFIER, LOW);
    }
    if (desiredTemperature < currentTemperature) {
      digitalWrite(FAN, HIGH);
    }
    else {
      digitalWrite(FAN, LOW);
    }
  }
  delay(500);
}
