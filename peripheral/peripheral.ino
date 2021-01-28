#include <ArduinoBLE.h>

BLEService soilService("1000");
BLEIntCharacteristic soilHydration("1001", BLERead | BLENotify); 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  Serial.println("Test");
  
  if(!BLE.begin()){
    Serial.println("starting BLE failed.");
    while(1);
  }

  BLE.setLocalName("SoilMonitor");
  BLE.setAdvertisedService(soilService);
  soilService.addCharacteristic(soilHydration);
  BLE.addService(soilService);

  BLE.advertise();

  Serial.println("Bluetooth active...");
  
}

void loop() {
  // put your main code here, to run repeatedly:

  BLEDevice central = BLE.central();

  if(central){
    Serial.print("Connected to central:");  
    Serial.println(central.address());
  }

  int fakesoil = 0;
  
  while(central.connected()){
    // code to be executed while peripheral is connected to master
    fakesoil++;
    soilHydration.writeValue(fakesoil);
    delay(200);
    Serial.println(fakesoil);
  }

  delay(400);
  
  Serial.println("Disconnected from central");
  
  
}



int readMoisture(){
  int moisture = analogRead(A0);
  Serial.print("read Value from Moisture Sensor: ");
  Serial.println(moisture);
  return moisture;
}
