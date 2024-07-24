// A simple NTP based clock for ESP32
// Original project uses two displays to show the time and data.
// This project uses one display, for time just,date could be simply added by software
// Decimal points are used to separate hours, minutes and seconds.
// It takes around five seconds for the time to sync with the ntp server
// ESP32 D1 mini and MAX7219, (io16-load, io17-DIN, io21-clk)
// Sequence of digits DIG6, DIG5, DIG4, DIG3, DIG2, DIG1 !!  from hour (left) to sec (right)
// Tested, OK, 14.8.2021 OkClockNtp7segEsp32Ver1
// Display intensity is set by surrounding light
// Hardware is prepared for temperature measurement

#include <Arduino.h>
#include <MAX72XX.h>
#include <time.h>                 // C Standard Library time() ctime()
#include <Ticker.h>               // Ticker Library - call function in interval
#include <WiFiMulti.h>

int wistat = 0;                   //0 not connected, 3 connected, 4 failed, 5 lost, 6 disconnected 
int lightPin = 34;                //light sensor pin
int lightValue = 0;               //light sensor value
int dispIntensity = 0;            //intensity of 7 segment Led display
const int senMin = 500;           //minimum intensity
const int senMax = 4000;          //maximum intensity
const int ledPin = 32;            //LED indicator ERROR
constexpr uint8_t latchPin = 16;                // change to reflect your latchPin
constexpr uint8_t numDevices = 1;               
constexpr uint8_t numDigits = 6;
constexpr uint8_t dataPin = 17;                 // change to reflect your dataPin
constexpr uint8_t clockPin = 21;                // change to reflect your clockPin

constexpr auto STASSID = "STASSID";
constexpr auto STAPSK = "STAPSK";

WiFiMulti wifiMulti;                            // create WiFi object
                                                // change to your timezone or use a value from TZ.h (ESP8266)
#define TZ_Europe_Bratislava  PSTR("CET-1CEST,M3.5.0,M10.5.0/3")        
                                                // create the object for the device chain
MAX72XX dispChain(dataPin, clockPin, latchPin, numDevices, numDigits); 
MAX72XXDisplay  hourDisp(&dispChain, 4, 2);     // create the time displays, from right to left 4(fifth digit) 2 digits
MAX72XXDisplay  minDisp(&dispChain, 2, 2);
MAX72XXDisplay  secDisp(&dispChain, 0, 2);

Ticker updateTicker;                            // create the ticker to update the displays

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);                      // Led output
  wifiMulti.addAP(STASSID, STAPSK);             // add the ssid and secret  
  WiFi.mode(WIFI_STA);                          // set the wifi type to station
                                                  
  configTzTime(TZ_Europe_Bratislava, "pool.ntp.org"); 
  dispChain.setIntensity(0);                    // set the intensity for all the devices

  // this ticker is called every 500ms to update the time and date
  updateTicker.attach_ms(500, []() {
    // get the time
    auto now = time(nullptr);                   //now - seconds
    const tm* tm = localtime(&now);

     // hour
    static int currHour = 1;
    if (currHour != tm->tm_hour) {
      currHour = tm->tm_hour;
      int32_t hour = currHour;                    //((tm->tm_hour % 12) != 0) ? tm->tm_hour % 12 : 12;
      hourDisp.writeNumber(hour, MAX72XX::Character::BLANK, 0);      
    }

     static int currMin = -1;                     //minute
    if (currMin != tm->tm_min) {
      currMin = tm->tm_min;
      minDisp.writeNumber(tm->tm_min, MAX72XX::Character::ZERO, 0);
    }
    
    secDisp.writeNumber(tm->tm_sec, MAX72XX::Character::ZERO);    //seconds   
  }); 
}


void loop() {
  wistat = WiFi.status();
  lightValue = analogRead(lightPin);
   dispIntensity = (map(lightValue, senMin, senMax, 5, 0) - 1);
   dispChain.setIntensity(dispIntensity);

  wifiMulti.run();                                  // conenct to wifi with reconnect if required          
   delay(2000);                                     //more reliable starting time
                                                    //without delay, time is starting in 50% after switch on
                                                    //error LED "wifi not connected"  
  if (wistat != 3)  {
  digitalWrite(ledPin, HIGH);
  }
  else {
  digitalWrite(ledPin, LOW);
  }               
  Serial.println(wistat);
 }

