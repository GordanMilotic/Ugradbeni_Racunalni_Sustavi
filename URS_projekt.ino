#include <IRremote.h>

const int button_0 = 65536;
const int button_1 = 65537;
const int button_2 = 65538;
const int button_3 = 65539;
const int button_4 = 65540;
const int button_5 = 65541;
const int button_6 = 65542;
const int button_7 = 65543;
const int button_8 = 65544;
const int button_9 = 65545;
const int button_subtitle = 65611;
const int button_text = 65596;

const int ledB = 4;
const int ledR = 5;

int recv = 2;
IRrecv irrecv(recv);
bool ledState = false; 
decode_results results;

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(ledB, OUTPUT);
  pinMode(ledR, OUTPUT);
}

void loop(){
  if (irrecv.decode(&results)){
    //Serial.println("Recieved: ");  
    //Serial.println(results.value); //za ocitanje kodova sa daljinskog
    switch (results.value){
      case button_1:
        Serial.println("Button 1 pressed");
        if(results.value == button_1){
          ledState=!ledState;
          digitalWrite(ledB, ledState);
          Serial.println(ledState ? "On" : "Off");
          //delay(300);
          //digitalWrite(ledR, !ledState);
        }
        break;
      case button_2:
        Serial.println("Button 2 pressed");
        break;
      case button_3:
        Serial.println("Button 3 pressed");
        break;
      case button_4:
        Serial.println("Button 4 pressed");
        break;
      case button_5:
        Serial.println("Button 5 pressed");
        break;
      case button_6:
        Serial.println("Button 6 pressed");
        break;
      case button_7:
        Serial.println("Button 7 pressed");
        break;
      case button_8:
        Serial.println("Button 8 pressed");
        break;
      case button_9:
        Serial.println("Button 9 pressed");
        break;
      case button_0:
        Serial.println("Button 0 pressed");
        break;
      case button_subtitle:
        Serial.println("Button subtitle pressed");
        break;
      case button_text:
        Serial.println("Button text pressed");
        break;
    }
    irrecv.resume();
  }
}