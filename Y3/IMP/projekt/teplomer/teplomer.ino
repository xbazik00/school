/*
 * Application for esp8266 microcontroller that creates WiFi AP with http server. This
 * application is connected by OneWire to the thermometer module. It returns temperature
 * data using json.
 * 
 * Author: Martin Bazik, xbazik00
 * Date: 9.12.2017
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <time.h>

/* OneWire port */
#define ONE_WIRE_BUS 5  // DS18B20 pin

/* size of used EEPROM */
#define USED_EEPROM 0x28

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
ESP8266WebServer server(80);

/* AP settings*/
#define AP_SSID "Thermo"
#define AP_PASS "tpass"

/* Current Temperature */
float temp = 0.0;

/* Current EEPROM address */
int addr = 0;

/* Second counter */
long counter = 0;

/*
 * Handles http request for temperature
 */
void handleRoot(){
  float f = 0.0;

  /* current temperature */
  String json = "{ \"temperature\" : " + String(temp);
  
  /* history of temperatures from EEPROM memory */
  json += ", \"old\":[";
  for(int i = 0; i < 10; i++){
    json += String(EEPROM.get(i*sizeof(f),f)); 
    json += (i == 9) ? "" : ",";
  }
  json +=  "], ";

  /* timestamp */
  json += "\"time\":" + String(counter) + " }";
  server.send(200, "application/json", json);
}

void setup() {
  /* Sets up EEPROM */
  EEPROM.begin(USED_EEPROM);

  /* Starts WiFi AP */
  WiFi.softAP(AP_SSID,AP_PASS);

  /* sets up HTTP server */
  server.on("/",handleRoot);
  server.begin();
}

void loop() {

  /* Waits for temperature*/
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
  } while (temp == 85.0 || temp == (-127.0));

  /* Every minute, a temperature is saved into EEPROM */
  if((counter % 60) == 0){
    /* If EEPROM section is full, the data are shifted */
    if(addr == USED_EEPROM ){
      for (int i = 0 ; i < USED_EEPROM ; i++) {
        EEPROM.write(i, EEPROM.read(i+sizeof(temp)));
      }

      /* resets addrs to the top */
      addr -= sizeof(temp); 
    }
  

    /* Saves data */
    EEPROM.put(addr, temp);

    /* Moves address of the used EEPROM locations */
    addr += sizeof(temp);
  }
  
  /* Handles request */
  server.handleClient();

  /* Waits for a second. */
  delay(1000);

  /* Increments second counter */
  counter += 1;

}
