#include <ArduinoBLE.h>

BLEService soilService("2000");
BLEIntCharacteristic soilHydration("2001", BLERead | BLENotify); 

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

  
  while(central.connected()){
    // code to be executed while peripheral is connected to master
    int moist = readMoisture();
    Serial.println(moist);
    soilHydration.writeValue(moist);
    delay(1);
    
  }

  delay(1);
  
  Serial.println("Disconnected from central");
  
  
}



int readMoisture(){
  //int moisture = analogRead(A0);
  int avg = 0;
  for(int i = 0; i < 5; i++){
    avg += random(300,600);
    delay(3);
  }

  avg /= 5;
  
  return avg;
}
