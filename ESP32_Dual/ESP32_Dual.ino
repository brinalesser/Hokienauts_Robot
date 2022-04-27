/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

/* Includes ------------------------------------------------------------------*/

//Serial Bluetooth
#include "BluetoothSerial.h"

//BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

//Sensor
#include <Wire.h>
#include "SparkFun_AS7341X_Arduino_Library.h"

/* Defines -------------------------------------------------------------------*/

// BLE UUIDs generated with https://www.uuidgenerator.net
#define SERVICE_UUID        "739b6a1b-52d1-4d30-9fe7-93694d3bc463"
#define CHARACTERISTIC_UUID "02c15c78-8d07-4540-a32d-aee6053033bc"

//Sensor Pins
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define LED_PIN 2

/* Global Variables ----------------------------------------------------------*/

//Serial Bluetooth
BluetoothSerial SerialBT;

String btInput;
String serialInput;
int num_bytes;
byte buf[1024];

// Sensors
SparkFun_AS7341X as7341L; 

/* Callbacks -----------------------------------------------------------------*/

class MyCallbacks : public BLECharacteristicCallbacks
{
  //forward changes of BLE characteristic to robot Pi via serial connection
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();
  
    if (rxValue.length() > 0)
    {
      for (int i = 0; i < rxValue.length(); i++){
        Serial.print(rxValue[i]);
      }
      Serial.println();
    }
  }
};

/* Setup ----------------------------------------------------------------------*/

void setup() {

  //Serial Bluetooth
  num_bytes = 0;
  SerialBT.begin("Hokienauts Robot BT");
  if(SerialBT){
    Serial.println("Serial Bluetooth Started");
  }
  else{
    Serial.println("Serial Bluetooth NOT Started");
  }
  delay(1000);

  //BLE Server with Characteristic
  Serial.begin(115200);
  Serial.println("Starting BLE...");
  BLEDevice::init("Hokienauts Robot BLE");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setValue("Initialized Value");
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();

  //BLE Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE Advertising Started.");

  //Sensor
  Wire.begin();
  pinMode(LED_PIN, OUTPUT);
  pinMode(I2C_SDA_PIN , INPUT);
  pinMode(I2C_SCL_PIN , INPUT);
  analogWrite(I2C_SDA_PIN, LOW);
  analogWrite(I2C_SCL_PIN, LOW);
  delay(500);
  as7341L.begin();
  as7341L.enable_AS7341X();  
}

/* Loop -----------------------------------------------------------------------*/

void loop() {

  //forward serial input from robot Pi to Hololens using serial bluetooth connection
  num_bytes = Serial.available();
  if (num_bytes > 0) {
    Serial.readBytes(buf, num_bytes);
    SerialBT.write(buf, num_bytes);
  }

  //forward serial bluetooth input from Hololens to robot Pi using serial connection
  num_bytes = SerialBT.available();
  if (num_bytes > 0) {
    SerialBT.readBytes(buf, num_bytes);
    Serial.write(buf, num_bytes);
  }

  //Sensor control
  unsigned int channelReadings[12] = { 0 };  
  if(buf[0] == 82){ //ASCII R
    
    as7341L.setLedDrive(LED_PIN);
    as7341L.enableWhiteLed();
    as7341L.enableIRLed();

    //get sensor data
    as7341L.readAllChannels(channelReadings);

    //send sensor data
    Serial.print("(415nm): ");
    Serial.print(channelReadings[0]);
    Serial.print(";(445nm): ");
    Serial.print(channelReadings[1]);
    Serial.print(";(480nm): ");
    Serial.print(channelReadings[2]);
    Serial.print(";(515nm): ");
    Serial.print(channelReadings[3]);
    Serial.print(";(555nm): ");
    Serial.print(channelReadings[6]);
    Serial.print(";(590nm): ");
    Serial.print(channelReadings[7]);
    Serial.print(";(630nm): ");
    Serial.print(channelReadings[8]);
    Serial.print(";(680nm): ");
    Serial.print(channelReadings[9]);
    Serial.print(";(Clear): ");
    Serial.print(channelReadings[10]);
    Serial.print(";(NIR): ");
    Serial.print(channelReadings[11]);
    Serial.println();

    as7341L.disableWhiteLed();
    as7341L.disableIRLed();
    
  }
}
