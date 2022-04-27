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
#define SERVICE_UUID              "739b6a1b-52d1-4d30-9fe7-93694d3bc463"
#define CHARACTERISTIC_CTRL_UUID  "02c15c78-8d07-4540-a32d-aee6053033bc"
#define CHARACTERISTIC_415NM_UUID "c6011e60-226f-4580-ac56-c08a5baec46a"
#define CHARACTERISTIC_445NM_UUID "d675e7ca-42f6-4c82-b4bd-8288172183af"
#define CHARACTERISTIC_480NM_UUID "a45a0cb5-335a-400e-b24d-6335ba8b4c91"
#define CHARACTERISTIC_515NM_UUID "89288a78-ae93-42fe-9d9b-5534f0245c40"
#define CHARACTERISTIC_555NM_UUID "b25f8dfb-bf69-4184-9298-f1ea015fe25d"
#define CHARACTERISTIC_590NM_UUID "90cc81c3-ba0a-4a21-8683-744b89669aed"
#define CHARACTERISTIC_630NM_UUID "eaeca0e5-679b-44b0-b994-b83be7b83bf6"
#define CHARACTERISTIC_680NM_UUID "5024e963-2055-4fc7-b472-9bd0bddf1dcd"
#define CHARACTERISTIC_CLEAR_UUID "531553de-e595-4d79-9275-945505e712ac"
#define CHARACTERISTIC_NIR_UUID   "181d6340-15bb-4766-941f-7bdb2727ced8"

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
bool collect_data = false;
BLECharacteristic *pCtrlCharacteristic;
BLECharacteristic *p415Characteristic;
BLECharacteristic *p445Characteristic;
BLECharacteristic *p480Characteristic;
BLECharacteristic *p515Characteristic;
BLECharacteristic *p555Characteristic;
BLECharacteristic *p590Characteristic;
BLECharacteristic *p630Characteristic;
BLECharacteristic *p680Characteristic;
BLECharacteristic *pClearCharacteristic;
BLECharacteristic *pNirCharacteristic;

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
  pCtrlCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_CTRL_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCtrlCharacteristic->setValue("Initialized Value");
  pCtrlCharacteristic->setCallbacks(new MyCallbacks());
  p415Characteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_415NM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  p415Characteristic->setValue("0");
  p445Characteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_445NM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  p445Characteristic->setValue("0");
  p480Characteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_480NM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  p480Characteristic->setValue("0");
  p515Characteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_515NM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  p515Characteristic->setValue("0");
  p555Characteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_555NM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  p555Characteristic->setValue("0");
  p590Characteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_590NM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  p590Characteristic->setValue("0");
  p630Characteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_630NM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  p630Characteristic->setValue("0");
  p680Characteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_680NM_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  p680Characteristic->setValue("0");
  pClearCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_CLEAR_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pClearCharacteristic->setValue("0");
  pNirCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_NIR_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pNirCharacteristic->setValue("0");
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
  if(buf[0] == 0x44){ //ASCII D
    collect_data = true;
  }
  if(buf[0] == 0x46){ //ASCII F
    collect_data = false;
  }

  if(collect_data) {
    as7341L.setLedDrive(LED_PIN);
    as7341L.enableWhiteLed();
    as7341L.enableIRLed();

    //get sensor data
    as7341L.readAllChannels(channelReadings);

    //send sensor data
    //p415Characteristic->writeValue((byte)channelReadings[0]);
    //p445Characteristic->writeValue((byte)channelReadings[1]);
    //p480Characteristic->writeValue((byte)channelReadings[2]);
    //p515Characteristic->writeValue((byte)channelReadings[3]);
    //p555Characteristic->writeValue((byte)channelReadings[6]);
    //p590Characteristic->writeValue((byte)channelReadings[7]);
    //p630Characteristic->writeValue((byte)channelReadings[8]);
    //p680Characteristic->writeValue((byte)channelReadings[9]);
    //pClearCharacteristic->writeValue((byte)channelReadings[10]);
    //pNitCharacteristic->writeValue((byte)channelReading[11]);
    
    as7341L.disableWhiteLed();
    as7341L.disableIRLed();
    
  }
}
