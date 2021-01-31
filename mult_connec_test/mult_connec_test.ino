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


}

void loop() {
  BLEDevice peripheral1 = connectToUuid("1000");
  delay(100);
  BLEDevice peripheral2 = connectToUuid("2000");
  BLECharacteristic soilCharacteristic_p1;
  BLECharacteristic soilCharacteristic_p2;
  delay(100);
  
  if(peripheral1.connect()){
    soilCharacteristic_p1 = peripheral1.characteristic("1001");
    Serial.println("AAA");

  }

  if(peripheral2.connect()){
    soilCharacteristic_p2 = peripheral2.characteristic("2001");
    Serial.println("BBB");
  }
  
  int stamp = millis();
  while(peripheral1.connected() || peripheral2.connected()){
    Serial.println("CCC");
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
    
    if (millis() > stamp + 5000){
      if(!(peripheral1.connected() && peripheral2.connected())){
              Serial.println("brrr");
              break;
      }
      else{
        stamp = millis();
        Serial.println("stopping");
        BLE.stopScan();
      }
    }
    delay(100);
   }
  Serial.println("broken");
}


BLEDevice connectToUuid(char Uuid[]){
  Serial.print("Connecting to BLE Device with UUID: ");
  Serial.println(Uuid);
  BLE.scanForUuid(Uuid);
  BLEDevice peripheral = BLE.available();
  if (peripheral){
    Serial.print("found device: ");
    Serial.println(peripheral.localName());
    if (!peripheral.connect()) {
      Serial.print("Failed to connect to ");
      Serial.println(Uuid);
      peripheral.disconnect();
      return peripheral;
    }
      Serial.print("setup finished of ");
      Serial.println(Uuid);
      return peripheral;
  }
  //  BLE.stopScan();

}
