#include <Arduino.h>
#include <SPI.h>


//SHT31 Libraries
#include <Wire.h>
#include "Adafruit_SHT31.h"

//DS18B20 Libraries
#include <OneWire.h>
#include <DallasTemperature.h>


//SHT31
Adafruit_SHT31 sht31 = Adafruit_SHT31();        //Object Creation

//DS18B20
#define ONE_WIRE_BUS 27
OneWire oneWire(ONE_WIRE_BUS);                  //Object Creation
DallasTemperature sensors(&oneWire);            //

//Temp conversion from C to F
float celsiustoFahrenheit(float input){
    float result = (input * (9.0/5.0) + 32);    
    return result;
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


}

//Loop is used for the actual firmware code, this is what runs!
void loop(){
    //Serial.println("Hello World!"); //<-- this shit worked!!

    //================================= SHT31 ============================================
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();

    if (! isnan(t)) {  // check if 'is not a number'
        Serial.print("Temp *F = "); Serial.print(celsiustoFahrenheit(t)); Serial.print("\t\t");
    } else { 
        Serial.println("Failed to read temperature");
    }
    
    if (! isnan(h)) {  // check if 'is not a number'
        Serial.print("Hum. % = "); Serial.println(h);
    } else { 
        Serial.println("Failed to read humidity");
    }

    delay(1000);



    //============================= Temperature Sensor (DS18B20) ==========================================
    sensors.requestTemperatures();
    for (int i = 0; i < 5; i++){
        float tempC = sensors.getTempCByIndex(i);
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(celsiustoFahrenheit(tempC));
    }
}