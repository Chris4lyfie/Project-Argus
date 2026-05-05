

#include <Arduino.h>
#include <SPI.h>


//SHT31 Libraries
#include <Wire.h>
#include "Adafruit_SHT31.h"

//DS18B20 Libraries
#include <OneWire.h>
#include <DallasTemperature.h>

//W5500 Libraries
#include <Ethernet.h>
#include "webpage.h"

// Wrapper to satisfy ESP32 core's Server::begin(uint16_t) pure virtual
class EthernetServerFixed : public EthernetServer {
public:
    EthernetServerFixed(uint16_t port) : EthernetServer(port) {}
    void begin(uint16_t port = 0) { EthernetServer::begin(); }
};


//SHT31
Adafruit_SHT31 sht31 = Adafruit_SHT31();        //Object Creation

//DS18B20
#define ONE_WIRE_BUS 27
OneWire oneWire(ONE_WIRE_BUS);                  //Object Creation
DallasTemperature sensors(&oneWire);            //

//W5500
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 10, 150);

EthernetServerFixed server(80);

// Global sensor readings - written by claude
float shtTemp     = 0.0;
float shtHumidity = 0.0;
float dsTemps[5]  = {0.0, 0.0, 0.0, 0.0, 0.0};

// Sensor read timer - written by claude
unsigned long lastSensorRead = 0;
const unsigned long SENSOR_INTERVAL = 2000;


//Temp conversion from C to F
float celsiustoFahrenheit(float input){
    float result = (input * (9.0/5.0) + 32);
    return result;
}

// Reads all sensors into globals - written by claude
void readSensors() {
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    if (!isnan(t)) shtTemp     = celsiustoFahrenheit(t);
    if (!isnan(h)) shtHumidity = h;

    sensors.requestTemperatures();
    for (int i = 0; i < 5; i++) {
        float c = sensors.getTempCByIndex(i);
        if (c != DEVICE_DISCONNECTED_C) dsTemps[i] = celsiustoFahrenheit(c);
    }

    Serial.print("SHT31: "); Serial.print(shtTemp); Serial.print("F  Hum: "); Serial.print(shtHumidity); Serial.println("%");
    for (int i = 0; i < 5; i++) {
        Serial.print("DS"); Serial.print(i); Serial.print(": "); Serial.print(dsTemps[i]); Serial.println("F");
    }
}

// Handles an incoming HTTP client, routes / and /data - written by claude
void handleClient(EthernetClient &client) {
    String requestLine = "";
    bool requestLineRead = false;
    bool currentLineIsBlank = true;

    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            if (!requestLineRead) requestLine += c;
            if (c == '\n' && !requestLineRead) requestLineRead = true;

            if (c == '\n' && currentLineIsBlank) {
                if (requestLine.indexOf("GET /data") >= 0) {
                    String json = "{";
                    json += "\"sht_temp\":"  + String(shtTemp, 1)     + ",";
                    json += "\"humidity\":"  + String(shtHumidity, 1) + ",";
                    json += "\"ds0\":"       + String(dsTemps[0], 1)  + ",";
                    json += "\"ds1\":"       + String(dsTemps[1], 1)  + ",";
                    json += "\"ds2\":"       + String(dsTemps[2], 1)  + ",";
                    json += "\"ds3\":"       + String(dsTemps[3], 1)  + ",";
                    json += "\"ds4\":"       + String(dsTemps[4], 1);
                    json += "}";
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: application/json");
                    client.println("Connection: close");
                    client.println();
                    client.println(json);
                } else {
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    size_t len = strlen(htmlPage);
                    size_t sent = 0;
                    while (sent < len && client.connected()) {
                        size_t written = client.write((const uint8_t*)htmlPage + sent, min(len - sent, (size_t)512));
                        if (written > 0) sent += written;
                        else delay(1);
                    }
                }
                break;
            }
            if (c == '\n') currentLineIsBlank = true;
            else if (c != '\r') currentLineIsBlank = false;
        }
    }
    delay(1);
    client.stop();
}

//Setup is used for initializing the actual ESP32
//Would eventually setup all the other sensors and components in the build
void setup(){
    Serial.begin(460800); //needs baud rate as the arugment, assuming 115200?

    //============================== Humidity Sensor Setup (SHT31, uses I2C!!!) ============================
    //Initialize the sensor itself since the arduino library handles SCL/SDA initialization already
    //https://registry.platformio.org/libraries/adafruit/Adafruit%20SHT31%20Library/examples/SHT31test/SHT31test.ino
    Serial.println("SHT31 test");
    if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
        Serial.println("Couldn't find SHT31");
        while (1) delay(1);
    }

    /* Unneeded code
    Serial.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
        Serial.println("ENABLED");
    else
        Serial.println("DISABLED");
    */

    //============================= Temperature Sensor (DS18B20) ==========================================
    sensors.begin();    //start the temperature sensors

    //============================= Ethernet (W5500) ====================================================
    Ethernet.init(5);
    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("Server at ");
    Serial.println(Ethernet.localIP());
}

//Loop is used for the actual firmware code, this is what runs!
void loop(){
    //Serial.println("Hello World!"); //<-- this shit worked!!

    //================================= SHT31 ============================================
    // float t = sht31.readTemperature();
    // float h = sht31.readHumidity();

    // if (! isnan(t)) {  // check if 'is not a number'
    //     Serial.print("Temp *F = "); Serial.print(celsiustoFahrenheit(t)); Serial.print("\t\t");
    // } else {
    //     Serial.println("Failed to read temperature");
    // }

    // if (! isnan(h)) {  // check if 'is not a number'
    //     Serial.print("Hum. % = "); Serial.println(h);
    // } else {
    //     Serial.println("Failed to read humidity");
    // }

    //============================= Temperature Sensor (DS18B20) ==========================================
    // sensors.requestTemperatures();
    // for (int i = 0; i < 5; i++){
    //     float tempC = sensors.getTempCByIndex(i);
    //     Serial.print("Sensor ");
    //     Serial.print(i);
    //     Serial.print(": ");
    //     Serial.println(celsiustoFahrenheit(tempC));
    // }

    // Non-blocking sensor read every SENSOR_INTERVAL ms - written by claude
    if (millis() - lastSensorRead >= SENSOR_INTERVAL) {
        lastSensorRead = millis();
        readSensors();
    }

    //============================== Ethernet (W5500) =========================================
    // Largely written by claude since it talks to front end.
    // Old blocking handler commented out below, replaced with handleClient() - written by claude
    EthernetClient client = server.available();
    if (client) {
        Serial.println("new client");
        // bool currentLineIsBlank = true;
        // while (client.connected()) {
        //     if (client.available()) {
        //         char c = client.read();
        //         Serial.write(c);
        //         if (c == '\n' && currentLineIsBlank) {
        //             client.println("HTTP/1.1 200 OK");
        //             client.println("Content-Type: text/html");
        //             client.println("Connection: close");
        //             client.println("Refresh: 5");
        //             client.println();
        //             client.println("<!DOCTYPE HTML>");
        //             client.println("<html>");
        //             client.println("Temp: %02f F", t);
        //             client.println("</html>");
        //             break;
        //         }
        //         if (c == '\n') currentLineIsBlank = true;
        //         else if (c != '\r') currentLineIsBlank = false;
        //     }
        // }
        // delay(1);
        // client.stop();
        handleClient(client);
        Serial.println("client disconnected");
    }
}
