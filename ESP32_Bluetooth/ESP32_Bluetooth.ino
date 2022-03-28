#include "BluetoothSerial.h"
    
BluetoothSerial SerialBT;

String btInput;
String serialInput;

bool recv_pic;
     
void setup()
{
  Serial.begin(115200);

  SerialBT.begin("ESP32test");
  if(SerialBT){
    Serial.println("Serial Bluetooth Started");
  }
  else{
    Serial.println("Serial Bluetooth NOT Started");
  }
  
  recv_pic = false;
  delay(1000);
}
     
void loop()
{
  if(recv_pic){
    serialInput = Serial.readString();
    SerialBT.print(serialInput);
    if(serialInput.endsWith("END")){
      recv_pic = false;
    }
    return;
  }
  
  if (Serial.available() > 0) {
    serialInput = Serial.readStringUntil('\n');
    SerialBT.println(serialInput);
  }

    if(serialInput == "Hello from robot"){
      Serial.println("Hello from ESP32");
    }
    if(serialInput == "START"){
      recv_pic = true;
    }
  }
  
  if (SerialBT.available()) {
    btInput = SerialBT.readStringUntil('\n');
    Serial.println(btInput);

    if(btInput == "Hello from PC"){
      SerialBT.println("Hello from ESP32");
    }
  }
}
