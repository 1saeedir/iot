/*  ESP8266 LDR ArduinoIDE Thingspeak IoT Example code
 *
 *  This sketch sends data via HTTP GET requests to api.thingspeak.com of the value on
 *  LDR sensor on ESP8266 development board - Wittyboard. You need an api_key from Thingspeak.
 * 
 *  Based on: https://vaasa.hacklab.fi/2016/02/06/esp8266-on-nodemcu-board-ds18b20-arduinoide-thingspeak/
 *  ESP8266 Witty: http://www.schatenseite.de/en/2016/04/22/esp8266-witty-cloud-module/
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
const char* ssid = "Tenda_41EB79";
const char* pass = "Saeed@123";
// RGB or button pins are not used in LDR example

#define AOUTpin     A0
#define redalert     0
#define LDR_PIN      1
#define dataWire     2
#define sensorgas    3
#define ledok       12
//#define ledokdo     13
#define DOUTpin     14
#define ledwar      15
#define lednarenji  16


int limit;
int value;
int tmp ;
int alert ;
const int buzzer = 13; 
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire myWire(dataWire);
DallasTemperature sensors(&myWire);
char host[] = "api.thingspeak.com";
String ApiKey = "93Y2UUCMQSDX733W";
String path = "/update?key=" + ApiKey + "&field1=";
String f2="&field2=";
int ldr_value = 0;
//-------------------------------Start Setup-----------------------------------
void setup() {

//sets the pin as an input OR OUTPUT -----------------
  pinMode(LDR_PIN, INPUT);
  pinMode(redalert, OUTPUT);
    pinMode(ledwar, OUTPUT);
  pinMode(ledok, OUTPUT);
//  pinMode(ledokdo, OUTPUT);
  pinMode(lednarenji, OUTPUT);
  pinMode(tmp, INPUT);
  pinMode(sensorgas, INPUT);
  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
  pinMode(buzzer, OUTPUT);
//sets the pin as an input OR OUTPUT -----------------

  
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  WiFi.begin(ssid, pass);
  // Wait for connection
//  while (WiFi.status() != WL_CONNECTED) {
//      lcd.begin();
//  lcd.setCursor(0,0);
  // Turn on the blacklight and print a message.
//  lcd.backlight();
//  lcd.print(" Offline-Check! ");
//      delay(500);
//    Serial.print(".");
//  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

lcd.begin();
lcd.backlight();
  lcd.setCursor(0,0);
      lcd.print("CnC:");
      lcd.print(ssid);
  lcd.setCursor(0,1);
      lcd.print("IP:");
      lcd.print(WiFi.localIP());
delay(2000);
lcd.clear();
lcd.begin();
 lcd.setCursor(0,0);
lcd.print("   Connected ;) ");
delay(1000);
  
}
void loop() {
//-------------------- VARS--------------------
 value= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
  limit= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
     sensors.requestTemperatures();
   tmp = sensors.getTempCByIndex(0);
//-------------------- VARS--------------------//

//-------------------- Serial Prints --------------------
  Serial.print("CO value: ");
  Serial.println(value);//prints the CO value
  Serial.print("Limit: ");
  Serial.println(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
  Serial.println("DONE"); 
  Serial.print("Temperature for Device 1 is: ");
  Serial.println(tmp);
  Serial.println(sensorgas);
//-------------------- Serial Prints --------------------//

//-------------------- LCD1602 Prints --------------------
      lcd.clear();
      lcd.begin();
      lcd. setCursor(0,0);
      lcd.print("TEMP:");
      lcd.print(tmp);
      lcd.print("  M: 30c  ");
      lcd.setCursor(0,1);
      lcd.print("Gas :");
      lcd.print(value);
//-------------------- LCD1602 Prints --------------------//
value = 800;

  if (value > 650 ){
    digitalWrite(lednarenji, HIGH);//if threshold not reached, LED remains off
    tone(buzzer, 100);
  delay(150); 
   noTone(buzzer);
   delay(1000);
   
    digitalWrite(ledok, LOW);
     lcd.clear();
      lcd.begin();
    lcd.setCursor(0,0);
    lcd.print(" >WARNING");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print(" >Gas Detected!");
    delay(500);
  }
  else{

    //    digitalWrite(ledokdo, HIGH);
    digitalWrite(lednarenji, LOW);//if limit has been reached, LED turns on as status indicator
  }
  
 

      //--------------------------- Sharte DAMA ---------------------
  if (tmp > 30) {
  digitalWrite(ledwar, HIGH);
  digitalWrite(ledok, LOW);
  
  
   tone(buzzer, 100);
  delay(150); 
   noTone(buzzer);
   delay(1000);
  
  
  
  Serial.println("Start Alert ");
  lcd.println(" S:High  ");
  }
else {
  digitalWrite(ledwar, LOW);
  digitalWrite(ledok, HIGH);
  
     tone(buzzer, 20);
  delay(30); 
   noTone(buzzer);
   delay(30000);
  
  
  
  Serial.println("No Alert ");
  lcd.println(" S:Norm  ");
  }
    //--------------------------- Sharte DAMA ---------------------
  
  //-----------------------Send Data-------------------------------
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    
    lcd.print("WIFI Failed");
    delay(500);
    return;
  }
  client.print(String("GET ") + path + tmp + f2 + value + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: keep-alive\r\n\r\n");
         //-----------------------Send Data-------------------------------//
       
  delay(10000);
}
