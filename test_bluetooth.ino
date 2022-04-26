#include "BluetoothSerial.h"
#include <Wire.h>
#include "SparkFun_AS7341X_Arduino_Library.h"

// Main AS7341L object
SparkFun_AS7341X as7341L; 
BluetoothSerial SerialBT;
#define led 2
//#define I2C_SDA_PIN 21
//#define I2C_SCL_PIN 22
String s;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32test");
    if(SerialBT){
    Serial.println("Serial Bluetooth Started");
  }
  else{
    Serial.println("Serial Bluetooth NOT Started");
  }
  //delay(1000);
  //Wire.begin( I2C_SDA_PIN, I2C_SCL_PIN );
  Wire.begin();
  pinMode(21, INPUT);
  pinMode(22, INPUT);
  analogWrite(21, LOW);
  analogWrite(22, LOW);
  delay(500);
  as7341L.begin();
  // If the board did not properly initialize print an error message and halt the system

  // Bring AS7341L to the powered up state
  as7341L.enable_AS7341X();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int channelReadings[12] = { 0 };  
    if (SerialBT.available()) {
       s = SerialBT.readString();
  }
      if (Serial.available()) {
  SerialBT.write(Serial.read());
  }

  // Check if the read operation was successful and print out results
  if (s == "S_ON")
  {
    // Set white LED drive current to 60 mA
    //as7341L.disablePowerLed();
    as7341L.setLedDrive(2);
    // Turn on the white LED
    as7341L.enableWhiteLed();
    // Turn on the IR LED
    as7341L.enableIRLed();
    as7341L.readAllChannels(channelReadings);
    delay(5000);
    as7341L.readAllChannels(channelReadings);
    SerialBT.print("(415nm): ");
    SerialBT.print(channelReadings[0]);
    SerialBT.print(";(445nm): ");
    SerialBT.print(channelReadings[1]);
    SerialBT.print(";(480nm): ");
    SerialBT.print(channelReadings[2]);
    SerialBT.print(";(515nm): ");
    SerialBT.print(channelReadings[3]);
    SerialBT.print(";(555nm): ");
    SerialBT.print(channelReadings[6]);
    SerialBT.print(";(590nm): ");
    SerialBT.print(channelReadings[7]);
    SerialBT.print(";(630nm): ");
    SerialBT.print(channelReadings[8]);
    SerialBT.print(";(680nm): ");
    SerialBT.print(channelReadings[9]);
    SerialBT.print(";(Clear): ");
    SerialBT.print(channelReadings[10]);
    SerialBT.print(";(NIR): ");
    SerialBT.print(channelReadings[11]);
    SerialBT.println();
    s = "";
    //as7341L.disableWhiteLed();
    //as7341L.disableIRLed();
  }
  if(s == "C_ON")
  {
    //What we want to do if Camera button has been pressed
    s = "";
  }
  if(s == "M_ON")
  {
    //What we want to do if Microscope button has been pressed
    s = "";
  }
  if(s == "Mode:Nav")
  {
    //What we want to do if mode is changed to navigation mode
    s = "";
  }
  if(s == "Mode:Rover")
  {
    //What we want to do if mode is changed to rover mode
    s = "";
  }
}
