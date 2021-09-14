/**
 * @file main.cpp
 * @author Mickael Lehoux (mickael@lehoux.net)
 * @brief this is the Water Tank Level firmware
 * @version 0.1
 * @date 2021-09-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "main.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "arduino_secrets.h"

char apSsid[127];
extern char apSsid[127];
char hostname[127];
extern char hostname[127];
int potentiometerPin = 35; //IO35 on board
extern int potentiometerPin;

IPAddress ipAddress;
String currentSsid;
long rssi;
int level;
int analogValue;
float voltageValue;

String jsonStr;
extern String jsonStr;
DynamicJsonDocument data(1024);

unsigned long lastLevelReadTime, lastLevelSendTime;
int readDelay = 10, sendDelay = 1800;

float floatMap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void buildJsonString()
{
  jsonStr = ""; // vaccum data before serialize avoid concat data
  data["water_level"]["value"] = level;
  data["water_level"]["analog_value"] = analogValue;
  data["water_level"]["voltage"] = voltageValue;
  data["network"]["ip_address"] = ipAddress;
  data["network"]["ssid"] = currentSsid;
  data["network"]["rssi"] = String(rssi);
  data["network"]["mdns"] = String(hostname) + ".local";
  serializeJson(data, jsonStr);
  // Serial.print("json file: ");
  // serializeJsonPretty(data, Serial);
}

void setup()
{
  Serial.begin(115200);
  pinMode(potentiometerPin, INPUT);

  getBoardInfos();
  initEeprom();

  const char *deviceName = "water_tank_level";
  strncat(apSsid, deviceName, strlen(deviceName));
  strncat(hostname, deviceName, strlen(deviceName));

  Serial.print("my apSsid is: ");
  Serial.println(apSsid);

  Serial.print("my hostname is: ");
  Serial.println(hostname);

  ap_start();
  _ota_start();
  mdns_start();
  webserver_start();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  ipAddress = WiFi.localIP();
  currentSsid = WiFi.SSID();
  rssi = WiFi.RSSI();
}

void loop()
{
  if ((millis() - lastLevelReadTime) > (readDelay * 1000))
  {
    //read level value each 10 sec
    Serial.println("low level stored: "+String(EEPROM.read(EEPROM_ADDRESSES.LOW_LEVEL)));
    Serial.println("high level stored: "+String(EEPROM.read(EEPROM_ADDRESSES.HIGH_LEVEL)));
    analogValue = analogRead(potentiometerPin);
    Serial.println("raw analog value: " + String(analogValue));
    voltageValue = floatMap(analogValue, 0, 4095, 0, 5);
    Serial.println("voltage value: " + String(voltageValue));

    if (voltageValue < EEPROM.read(EEPROM_ADDRESSES.LOW_LEVEL))
    {
      updateEeprom(EEPROM_ADDRESSES.LOW_LEVEL, voltageValue);
    }
    if (voltageValue > EEPROM.read(EEPROM_ADDRESSES.HIGH_LEVEL))
    {
      updateEeprom(EEPROM_ADDRESSES.HIGH_LEVEL, voltageValue);
    }

    if (voltageValue <= EEPROM.read(EEPROM_ADDRESSES.LOW_LEVEL))
    {
      level = 1;
    }
    else if (voltageValue >= EEPROM.read(EEPROM_ADDRESSES.HIGH_LEVEL))
    {
      level = 2;
    }
    else
    {
      level = 0;
    }

    buildJsonString();

    lastLevelReadTime = millis();
  }

  if ((millis() - lastLevelSendTime) > (sendDelay * 1000))
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      //send level value each 30 min
      HTTPClient http;
      http.begin("http://jsonplaceholder.typicode.com/posts"); //Specify destination for HTTP request
      http.addHeader("Content-Type", "text/plain");            //Specify content-type header
      int httpResponseCode = http.POST("POSTING from ESP32");  //Send the actual POST request
      if (httpResponseCode > 0)
      {
        String response = http.getString(); //Get the response to the request
        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);           //Print request answer
      }
      else
      {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }
      http.end(); //Free resources

      lastLevelSendTime = millis();
    }
  }
  // delay(2000);
}