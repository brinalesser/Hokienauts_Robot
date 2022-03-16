#include "BluetoothSerial.h"
    
BluetoothSerial SerialBT;

String btInput;
String serialInput;
     
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
  
  delay(1000);
}
     
void loop()
{
  if (Serial.available() > 0) {
    serialInput = Serial.readStringUntil('\n') + "\n";
    SerialBT.print(serialInput);
  }
  
  
  if (SerialBT.available()) {
    btInput = SerialBT.readStringUntil('\n');
    Serial.println(btInput);

    if(btInput == "Hello from PC"){
      SerialBT.println("Hello from ESP32");
    }
  }
}
