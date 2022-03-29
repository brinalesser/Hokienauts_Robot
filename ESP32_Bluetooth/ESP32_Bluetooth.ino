#include "BluetoothSerial.h"
    
BluetoothSerial SerialBT;

String btInput;
String serialInput;

int num_bytes;
byte buf[1024];
     
void setup()
{
  num_bytes = 0;
  
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
  num_bytes = Serial.available();
  if (num_bytes > 0) {
    Serial.readBytes(buf, num_bytes);
    SerialBT.write(buf, num_bytes);
  }
  
  num_bytes = SerialBT.available();
  if (num_bytes > 0) {
    SerialBT.readBytes(buf, num_bytes);
    Serial.write(buf, num_bytes);
  }
}
