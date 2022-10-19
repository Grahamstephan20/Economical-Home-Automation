#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif 

#include "SinricPro.h"
#include "SinricProSwitch.h"
#include "SinricProFanUS.h"
#include <LiquidCrystal_I2C.h>

#ifdef ESP32   
       #include <WiFi.h>
#endif
#define WIFI_SSID         "graham"    
#define WIFI_PASS         "12345678g"
#define APP_KEY           "e9b9c7af-2f6d-4eb6-a800-5a3c21633d00"     
#define APP_SECRET        "ffc5df7e-ec04-4940-8b29-75f9084d6302-300dd879-3f30-4de5-a359-7b641ffc1bcc"   
#define SWITCH_ID1         "62827593a6872f36392918de"  
#define SWITCH_ID2        "6283eddd5322d5576ad0dd7e"
#define FAN_ID1           "62c7d8230aec232058f7d49b"
#define BAUD_RATE         115200                // Change baudrate to your need

#define RELAY_PIN1         2    //warm light               // Pin where the relay is connected (D5 = GPIO 14 on ESP8266)
#define RELAY_PIN2         4    //lamp shade  
#define RELAY_PIN3         5    //fan

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
String wifidis = "WiFi disconnect";
String wificon = "WiFi connected";
String bulb1off = "Room light OFF";
String bulb1on = "Room light ON";
String bulb2off = "Lamp is OFF";
String bulb2on = "Lamp is ON";
String fanoff = "Fan is OFF";
String fanon = "Fan is ON";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print(wifidis);
  }
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(wificon);
  digitalWrite(4,HIGH);
  delay(1000);
  digitalWrite(4,LOW);
  delay(1000); 
}

bool onPowerState1(const String &deviceId1, bool &state1) {
  digitalWrite(RELAY_PIN1, state1);             // set pin state
  if (state1 == false){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(bulb1off);
    }else{ lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(bulb1on);}
  return true;                                  // request handled properly
}
bool onPowerState2(const String &deviceId2, bool &state2){
  digitalWrite(RELAY_PIN2, state2);
  if (state2 == false){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(bulb2off);
    }else{ lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(bulb2on);}
  return true;
}

bool onPowerState3(const String &deviceID3, bool &state3){
  digitalWrite(RELAY_PIN3, state3);
  if (state3 == false){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(fanoff);
    }else{ lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(fanon);}
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN1, OUTPUT);                 // set relay-pin to output mode
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  lcd.init();                   
  lcd.backlight();
  initWiFi();
  
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID1];   // create new switch device
  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID2];
  SinricProFanUS& myFan1 = SinricPro[FAN_ID1];
  mySwitch1.onPowerState(onPowerState1); // apply onPowerState callback
  mySwitch2.onPowerState(onPowerState2);
  myFan1.onPowerState(onPowerState3);
  SinricPro.begin(APP_KEY, APP_SECRET);               // start SinricPro
}

void loop() {
  SinricPro.handle();                         // handle SinricPro commands
}
