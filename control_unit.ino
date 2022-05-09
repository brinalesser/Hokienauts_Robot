#include "BluetoothSerial.h"
#include <Wire.h>
const int x_axis = 27;
const int y_axis = 14;
const int sen_button = 34;
const int mode_button = 35;
const int cam_button = 32;
const int micro_button = 33;
const int select_button = 25;
const int back_button = 26;
int sen_b_value, mode_b_value, cam_b_value, micro_b_value, sel_b_value, back_b_value;
int get_x, get_y;
String s;
BluetoothSerial SerialBT;
bool rover = true;
String m = "RoverMode: ";

void setup() {
  Wire.begin();
  Serial.begin(115200);

   SerialBT.begin("ESP32test");
    if(SerialBT){
    Serial.println("Serial Bluetooth Started");
  }
  else{
    Serial.println("Serial Bluetooth NOT Started");
  }
  pinMode(sen_button, INPUT);
  digitalWrite(sen_button, LOW);
  pinMode(mode_button, INPUT);
  digitalWrite(mode_button, LOW);
  pinMode(cam_button, INPUT);
  digitalWrite(cam_button, LOW);
  pinMode(micro_button, INPUT);
  digitalWrite(micro_button, LOW);
  pinMode(select_button, INPUT);
  digitalWrite(select_button, LOW);
  pinMode(back_button, INPUT);
  digitalWrite(back_button, LOW);
  pinMode(x_axis, OUTPUT);
  pinMode(y_axis, OUTPUT);
}

void loop() {

  sen_b_value = digitalRead(sen_button);
  mode_b_value = digitalRead(mode_button);
  cam_b_value = digitalRead(cam_button);
  micro_b_value = digitalRead(micro_button);
  sel_b_value = digitalRead(select_button);
  back_b_value = digitalRead(back_button);
  get_x = analogRead(x_axis);
  get_y = analogRead(y_axis);
  if(sen_b_value == HIGH)
  {
    s = "S_ON";
  }
  else if(cam_b_value == HIGH)
  {
    s = "CAM_ON";
  }
  else if(micro_b_value == HIGH)
  {
    s = "MICRO_ON";
  }
  else if(sel_b_value == HIGH && !rover)
  {
    s = "SEL_ON";
  }
  else if(back_b_value == HIGH && !rover)
  {
    s = "BACK_ON";
  }
  else if(mode_b_value == HIGH)
  {
    rover = !rover;
    if(rover)
    {
      m = "RoverMode";
    }
   else
    {
      m = "NavMode";
    }
        s = ("Mode:" + m);
  }
  else if(get_x < 50)
  {
      s = (m + ":" + "L");
  }
  else if(get_x > 4000)
  {
     s = (m + ":" + "R");
  }
  else if(get_y < 50)
  {
      s = (m + ":" + "B");
  }
  else if(get_y > 4000)
  {
      s = (m + ":" + "F");
  }
  if (s != "")
  {
    Serial.println(s); 
  }
  s = "";
    delay(500);
}
