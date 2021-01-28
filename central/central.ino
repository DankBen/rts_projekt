#include <ArduinoBLE.h>

void setup(){
  Serial.begin(9600);
  while(!Serial);

  // initialization
  if (!BLE.begin()){
    Serial.println("starting BLE failed!");

    while(1);
  }

  Serial.println("starting BLE as central");
  BLE.scanForName("SoilMonitor");
}

void loop(){
  BLEDevice peripheral = BLE.available();
  BLECharacteristic soilCharacteristic;
  if (peripheral) {
        BLE.stopScan();
        Serial.print("found device");
        Serial.println(peripheral.localName());
        if (!peripheral.connect()) {
          Serial.println("Failed to connect!");
          return;
        }
  
        if (!peripheral.discoverAttributes()) {
          Serial.println("Attribute discovery failed!");
          peripheral.disconnect();
          return;
        }
        soilCharacteristic = peripheral.characteristic("1001");
        Serial.println("setup finished");
   }
   while(peripheral.connected()){
    int valueread = 0;
    Serial.println("aaa");
    byte val = 0;
    soilCharacteristic.readValue(val);
    valueread = (int)val;
    Serial.println(val);
    
   }
}
