#include <IRremote.h>

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


const int ledB = 4;
const int ledR = 5;

//int ledR_min_brightness = 0;
int ledR_max_brightness = 255;
int brightness_increment = 1;
int current_brightness = 0;

const int recv = 2;
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
    Serial.println(results.value); //za ocitanje kodova sa daljinskog
    
    if (results.value == 0xFFFFFFFFFF) {  
        Serial.println("Ponovljen signal");
        irrecv.resume();
        return;
    }

    switch (results.value){
      case button_1:
        //Serial.println("Button 1 pressed");
        if(results.value == button_1){
          ledState=!ledState;
          digitalWrite(ledB, ledState);
          Serial.println(ledState ? "On" : "Off");
          delay(300);
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

      case volume_up:
        //Serial.println("Button volume up pressed");
        current_brightness = min(ledR_max_brightness, current_brightness + brightness_increment);
        analogWrite(ledR, current_brightness);
        Serial.print("Current brightness: ");
        Serial.println(current_brightness);
        break;

      case volume_down:
        //Serial.println("Button volume down pressed");
        current_brightness = max(0, current_brightness - brightness_increment);
        analogWrite(ledR, current_brightness);
        Serial.print("Current brightness: ");
        Serial.println(current_brightness);
        break;
      /*case motor_speed_up:
      //uln2003 
        break;
      case motor_speed_down:
      //uln2003
        break;*/
    }
    irrecv.resume();
    delay(300);
  }
}