#include <ArduinoBLE.h>

void setup() {
 Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  Serial.println("BLE Central multi connect test");

  BLE.scanForName("SoilMonitor", true);

}

void loop() {
  BLEDevice peripheral1 = BLE.available();
  BLEDevice peripheral2 = BLE.available();
  BLECharacteristic soilCharacteristic_p1;
  BLECharacteristic soilCharacteristic_p2;


  if(peripheral1){
    Serial.print("found device 1");
    Serial.println(peripheral1.localName());
    if (!peripheral1.connect()) {
      Serial.println("Failed to connect!");
      return;
    }
      soilCharacteristic_p1 = peripheral1.characteristic("1001");
      Serial.println("setup of p1 finished");
  }

  if(peripheral2){
    Serial.print("found device 2");
    Serial.println(peripheral2.localName());
    if (!peripheral2.connect()) {
      Serial.println("Failed to connect!");
      return;
    }
    soilCharacteristic_p2 = peripheral2.characteristic("1001");
    Serial.println("setup of p2 finished");
  }

  while(peripheral1.connected() || peripheral2.connected()){
      
    if (peripheral1.connected()){
      byte val = 0;
      soilCharacteristic_p1.readValue(val);
      Serial.print("Soil Monitor 1 reads: ");
      Serial.println(val);      
    }

    if (peripheral2.connected()){
      byte val = 0;
      soilCharacteristic_p2.readValue(val);
      Serial.print("Soil Monitor 2 reads: ");
      Serial.println(val);      
    }

    delay(100);
   }

}
