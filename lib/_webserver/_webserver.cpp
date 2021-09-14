/**
 * @file _webserver.cpp
 * @author Mickael Lehoux (mickael@lehoux.net)
 * @brief this is the webserver module to allow html interface rendering
 * @version 0.1
 * @date 2021-01-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "_webserver.h"

// from main.cpp
// extern int analogValue;
// extern float voltageValue;
// extern char hostname[127];
// extern IPAddress ipAddress;
// extern String currentSsid;
// extern long rssi;

extern String jsonStr;

//private
const char header_html[] PROGMEM = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width,initial-scale=1.0'><title>Water Tank Level</title></head><body>";
const char footer_html[] PROGMEM = "</body></html>";
const char menu_html[] PROGMEM = "<nav><a href='/'>Home</a> |<a href='/data'>Data</a> |<a href='/update'>Update firmware</a> |</nav>";

//public
bool shouldReboot = false;
extern bool shouldReboot;

/* index View */
// const char index_html[] PROGMEM = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Water Tank Level</title></head><body><h1>Water Tank Level</h1><a href='/update'>Update firmware</a></body></html>";
/* update View */
// const char update_html[] PROGMEM = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Water Tank Level</title></head><body><h1>Firmware Update</h1><h2>Only .bin file</h2><form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update' required><input type='submit' value='Run Update'></form></body></html>";

String renderHtmlView(String content)
{
    return String(header_html) + String(menu_html) + content + String(footer_html);
}

AsyncWebServer server_http(80);

void notFound(AsyncWebServerRequest *request)
{
    Serial.println("Not Found");
    request->send(404, "text/html", "<strong>not found!</strong>");
}

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        Serial.printf("UploadStart: %s\n", filename.c_str());
    }
    for (size_t i = 0; i < len; i++)
    {
        Serial.write(data[i]);
    }
    if (final)
    {
        Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
    }
}

void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (!index)
    {
        Serial.printf("BodyStart: %u B\n", total);
    }
    for (size_t i = 0; i < len; i++)
    {
        Serial.write(data[i]);
    }
    if (index + len == total)
    {
        Serial.printf("BodyEnd: %u B\n", total);
    }
}

void webserver_start()
{
    server_http.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                   {
                       DynamicJsonDocument data(1024);
                       deserializeJson(data, jsonStr);
                       String index_html = "<h1>Water Tank Level</h1><p>Level: " + data["water_level"]["value"].as<String>() + "</p><p>Analog Value: " + data["water_level"]["analog_value"].as<String>() + "</p><p>Voltage Value: " + data["water_level"]["voltage"].as<String>() + "</p><h1>Water Tank Network</h1><p>Ip Address: " + data["network"]["ip_address"].as<String>() + "</p><p>ssid: " + data["network"]["ssid"].as<String>() + "</p><p>rssi: " + data["network"]["rssi"].as<String>() + "</p><p>mdns: " + data["network"]["mdns"].as<String>() + "</p>";
                       request->send(200, "text/html", renderHtmlView(index_html));
                   });
    server_http.on("/data", HTTP_GET, [](AsyncWebServerRequest *request)
                   {
                       request->send(200, "application/json", jsonStr);
                   });
    server_http.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
                   {
                       String update_html = "<h1>Firmware Update</h1><h2>Only .bin file</h2><form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update' required><input type='submit' value='Run Update'></form>";
                       request->send(200, "text/html", renderHtmlView(update_html));
                   });
    server_http.on(
        "/update", HTTP_POST, [](AsyncWebServerRequest *request)
        {
            shouldReboot = !Update.hasError();
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", shouldReboot ? "<h1><strong>Update DONE</strong></h1>" : "<h1><strong>Update FAILED</strong></h1>");
            response->addHeader("Connection", "close");
            request->send(response);
        },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            if (!index)
            {
                Serial.printf("Update Start: %s\n", filename.c_str());
                if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000))
                {
                    Update.printError(Serial);
                }
            }
            if (!Update.hasError())
            {
                if (Update.write(data, len) != len)
                {
                    Update.printError(Serial);
                }
            }
            if (final)
            {
                if (Update.end(true))
                {
                    Serial.printf("Update Success: %uB\n", index + len);
                }
                else
                {
                    Update.printError(Serial);
                }
            }
        });

    server_http.onNotFound(notFound);
    server_http.onFileUpload(handleUpload);
    server_http.onRequestBody(handleBody);
    server_http.begin();
}

void webserver_update()
{
    if (shouldReboot)
    {
        Serial.println("webserver Rebooting ESP32...");
        delay(2000);
        ESP.restart();
    }
}