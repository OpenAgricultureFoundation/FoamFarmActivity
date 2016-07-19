#include <Wire.h>
#include "Adafruit_AM2315.h"
#include "rgb_lcd.h"

const int LIGHT = 3;
const int  FAN = 4;
const int HUMIDIFIER = 5;
const int PUMP = 6;

rgb_lcd lcd;

const int lightOnPeriod = 10000;
const int lightOffPeriod = 5000;

bool isLightOn = false;

int lightTurnedOn = 0;
int lightTurnedOff = 0;

const int desiredTemperature = 22;
const int desiredHumidity = 50;

Adafruit_AM2315 am2315;

float currentTemperature;
float currentHumidity;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setRGB(65,105,225);

  pinMode(FAN, OUTPUT);
  pinMode(HUMIDIFIER, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(PUMP, OUTPUT);
  
  while (!am2315.begin()) {
    delay(1000);
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
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.setCursor(11, 0);
    lcd.print(currentTemperature);
    Serial.print("Air Temperature: ");
    Serial.println(currentTemperature);
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.setCursor(11, 1);
    lcd.print(currentHumidity);
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
