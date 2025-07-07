#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

// moze se mijenjati (u mom slucaju philips daljinski)
enum philips_buttons {
  button_0 = 65536, 
  button_1, 
  button_2, 
  button_3, 
  button_4, 
  button_5, 
  button_6, 
  button_7, 
  button_8, 
  button_9, 
  red_button = 65645, 
  green_button, 
  yellow_button,
  blue_button,
  volume_up = 65552,
  volume_down,
  stop_button = 65584,
  pause_button,
  record_button = 65591,
  fast_rewind = 65579,
  play_button,
  fast_forward = 65576
};

int SDA_PIN = A4;
int SCL_PIN = A5;
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

const int ledGreen = 4;
const int ledRed = 5;
const int IRdiode = 3;

// led varijable
int ledRed_max_brightness = 255;
int brightness_increment = 10; //moze se mijenjati
int current_brightness = 0;
bool ledState = false; 

// nfc varijable
bool nfcEnabled = false;
unsigned long lastNfcReadTime = 0;
const unsigned long nfcReadInterval = 500; // 500ms između pokušaja čitanja
unsigned long lastNfcTagTime = 0;
bool nfcTagRecentlyRead = false;
unsigned long nfcStartTime = 0;
const unsigned long maxNfcDuration = 10000; // 10 sekundi timeout

IRrecv irrecv(IRdiode);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 nije pronađen");
    while (1);
  }
  nfc.SAMConfig();
  Serial.println("PN532 spreman."); 
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == 0xFFFFFFFFFF) {
      Serial.println("Ponovljen signal");
      irrecv.resume();
      return;
    }
    switch (results.value) {
    //ovo je testni case
    /*case button_1:
        Serial.println("Button 1 pressed");
        ledState = !ledState;
        digitalWrite(ledGreen, ledState);
        Serial.println(ledState ? "On" : "Off");
        delay(300);
        break;*/

      case volume_up:
        //Serial.println("Button volume up pressed");
        current_brightness = min(ledRed_max_brightness, current_brightness + brightness_increment);
        analogWrite(ledRed, current_brightness);
        Serial.print("Current brightness: ");
        Serial.println(current_brightness);
        break;

      case volume_down:
        //Serial.println("Button volume down pressed");
        current_brightness = max(0, current_brightness - brightness_increment);
        analogWrite(ledRed, current_brightness);
        Serial.print("Current brightness: ");
        Serial.println(current_brightness);
        break;

      case green_button:
        Serial.println("Green button pressed - NFC ENABLED");
        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledRed, LOW);
        lastNfcReadTime = 0;
        nfcStartTime = millis();
        nfcEnabled = true;
        break;

      case red_button:
        Serial.println("Red button pressed - NFC DISABLED");
        digitalWrite(ledRed, HIGH);
        digitalWrite(ledGreen, LOW);
        nfcEnabled = false;
        break;

      case stop_button:
        Serial.println("Stop button pressed - RESETTING SYSTEM");
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledRed, LOW);
        ledState = false;
        current_brightness = 0;
        analogWrite(ledRed, current_brightness);
        nfcEnabled = false;
        break;
    }
    irrecv.resume();
    delay(300); 
  }
  // automatsko gasenje nfc nakon  neaktivnosti
  if (nfcEnabled && (millis() - nfcStartTime > maxNfcDuration)) {
    Serial.println("NFC timeout - disabling");
    nfcEnabled = false;
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
  }

  if (nfcEnabled && (millis() - lastNfcReadTime > nfcReadInterval)) {
    lastNfcReadTime = millis();
    if (nfcTagRecentlyRead && (millis() - lastNfcTagTime < 1000)) {
      return;
    }
    uint8_t uid[7];
    uint8_t uidLength;

    if (nfc.inListPassiveTarget()) {
      if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
        Serial.print("NFC Tag detected. UID: ");
        for (uint8_t i = 0; i < uidLength; i++) {
          Serial.print(uid[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
        nfcTagRecentlyRead = true;
        lastNfcTagTime = millis();
      }
    } else {
      nfcTagRecentlyRead = false;
    }
  }
}