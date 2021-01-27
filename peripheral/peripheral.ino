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

  while(central.connected()){
    // code to be executed while peripheral is connected to master
  }

  Serial.println("Disconnected from central");
  
  
}


/*
#include <ArduinoBLE.h>
BLEService batteryService("1101");
BLEUnsignedCharCharacteristic batteryLevelChar("2101", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
  if (!BLE.begin()) 
  {
    Serial.println("starting BLE failed!");
    while (1);
  }
  
  BLE.setLocalName("BatteryMonitor");
  BLE.setAdvertisedService(batteryService);
  batteryService.addCharacteristic(batteryLevelChar);
  BLE.addService(batteryService);
  
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() 
{
  BLEDevice central = BLE.central();
  
  if (central) 
  {
  Serial.print("Connected to central: ");
  Serial.println(central.address());
  digitalWrite(LED_BUILTIN, HIGH);
  
  while (central.connected()) {
  
        int battery = analogRead(A0);
        int batteryLevel = map(battery, 0, 1023, 0, 100);
        Serial.print("Battery Level % is now: ");
        Serial.println(batteryLevel);
        batteryLevelChar.writeValue(batteryLevel);
        delay(200);
  
  }
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}*/ 
