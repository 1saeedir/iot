/*
   ------------------------------------------------------------------------
   Example sketch/program showing how to read UID from a RC522 to serial.
   If UID is in our dataset we turn on access status LED.
   هنگام مواجهه با کارت درست ، جریان عبور کرده و لامپ روشن میشود
   ------------------------------------------------------------------------
  +-----------------------+
  |            |  Arduino |
  | Signal     |  Uno Pin |
  +-----------------------+
  | RST/Reset  |   9      |
  +-----------------------+
  | SPI SS-SDA |   10     |
  +-----------------------+
  | SPI MOSI   |   11     |
  +-----------------------+
  | SPI MISO   |   12     |
  +-----------------------+
  | SPI SCK    |   13     |
  ------------------------
  | LED +      |   A0     |
  ------------------------
  | LED -      |   GND    |
  +-----------------------+
*/
#include <SPI.h>
#include <MFRC522.h>
#define LED_PIN A0
#define LED2_PIN A1
#define LED3_PIN 4
#define LEDwarning_PIN 3
#define LED_wait 2
//defie rc552 pins:
#define SS_PIN 10
#define RST_PIN 9
#include <Servo.h>
Servo servo1;
int degree, serial_data = 0 , read_degree = 0;
MFRC522 rc552(SS_PIN, RST_PIN); // Instance of the class
//change here the UID of the card/cards that you want to give access//آبی تگ کوچک / خط دوم
byte storedUID[] = {86, 29, 90, 165}; //blue tag
byte stored2UID[] = {165, 122, 27, 16}; // white card
byte stored3UID[] = {41, 143, 82, 162}; // Sekkeii sefid 1
void setup() {
  Serial.begin(115200);
  pinMode(LED3_PIN, OUTPUT);
    pinMode(LEDwarning_PIN, OUTPUT);
        pinMode(LED_wait, OUTPUT);
   servo1.attach(5); // تعیین این که سرو موتورمون به کدوم پایه وصل هستش
  pinMode(LED_PIN, OUTPUT);  // initialize LED_PIN as an output.
    pinMode(LED2_PIN, OUTPUT);  // initialize LED2_PIN as an output.
  SPI.begin(); // Init SPI bus
  rc552.PCD_Init(); // Init MFRC522
  Serial.println("Approximate your card to the reader...");
}
void loop() {
    digitalWrite(LED_wait, HIGH);   // sets the LED on
  delay(1000);                  // waits for a second
  digitalWrite(LED_wait, LOW);
  delay(1000);
  // Look for new cards
  if ( ! rc552.PICC_IsNewCardPresent())
    return;
  // Verify if the UID has been readed
  if ( ! rc552.PICC_ReadCardSerial())
    return;
  Serial.println(F("The UID tag is:"));
  Serial.print(F("In hex: "));
  printHex(rc552.uid.uidByte, rc552.uid.size);
  Serial.println();
  Serial.print(F("In dec: "));
  printDec(rc552.uid.uidByte, rc552.uid.size);
  Serial.println();
  if (rc552.uid.uidByte[0] == storedUID[0] ||
      rc552.uid.uidByte[1] == storedUID[1] ||
      rc552.uid.uidByte[2] == storedUID[2] ||
      rc552.uid.uidByte[3] == storedUID[3] ||
      rc552.uid.uidByte[0] == stored2UID[0] ||
      rc552.uid.uidByte[1] == stored2UID[1] ||
      rc552.uid.uidByte[2] == stored2UID[2] ||
      rc552.uid.uidByte[3] == stored2UID[3] ||
      rc552.uid.uidByte[0] == stored3UID[0] ||
      rc552.uid.uidByte[1] == stored3UID[1] ||
      rc552.uid.uidByte[2] == stored3UID[2] ||
      rc552.uid.uidByte[3] == stored3UID[3] ) {
    Serial.println(F("Welcome! - Blue Small Tag Detected !"));
    
 Serial.println("Cad ok , Unlocking ... ");
      digitalWrite(LED3_PIN, HIGH);
  delay(1000);
  servo1.write(40);
delay(500);
//servo1.write(-150);
//delay(2000);
servo1.write(0);
delay(15000);
servo1.write(40);
delay(500);
digitalWrite(LED3_PIN, LOW);
//servo1.write(-50);

  }

  else {
    Serial.println("Access denied");
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LEDwarning_PIN, HIGH);
    delay(5000);
    digitalWrite(LEDwarning_PIN, LOW);
  }
  // Halt PICC
  rc552.PICC_HaltA();
  // Stop encryption on PCD
  rc552.PCD_StopCrypto1();
}
/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
