// ref: Examples > BluetoothSerial > SerialToSerialBT

#include "BluetoothSerial.h"
#include "esp_bt_device.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void printDeviceAddress() {
  const uint8_t* point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++) {
    char str[3];
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
    if (i < 5){
      Serial.print(":");
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  if(SerialBT.begin("HokienautsESP32")){
    Serial.println("The device started, now you can pair it with bluetooth!");
    Serial.println("Device Name: HokienautsESP32");
    Serial.print("BT MAC Address: ");
    printDeviceAddress();
    Serial.println("\n");
  }
  else{
    Serial.println("Error starting Bluetooth");
  }
}

void loop() {
  if (Serial.available()) { //send
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) { //receive
    Serial.println(SerialBT.read());
  }
  delay(20);
}
