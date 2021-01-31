#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>  
#include <Arduino_APDS9960.h>

#define RED 22
#define GREEN 23
#define BLUE 24
#define LED_PWR 25

void setup(){
  // initialization´
  Serial.begin(9600);
  while(!Serial);
  
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  
  if (!BLE.begin()){
    Serial.println("starting BLE failed!");

    while(1);
  }

    if (!HTS.begin()){  
    Serial.println("Failed to initialize humidity temperature sensor"); 
    while(1); 
  } 

  if(!APDS.begin()){  
    Serial.println("Failed to initialize ambient light sensor");  
    while(1);   
  }
  
  Serial.println("starting BLE as central");
}

int soil_1, soil_2 = 0;
int stamp = millis();

void loop(){

  if(soil_1 == 0 || soil_2 == 0){
    if(soil_1 == 0){
      BLEDevice peripheral1 = connectToUuid("1000");
      delay(1000);
      soil_1 = readSoilMoisture(peripheral1, 1);
      return;
    }
    delay(1000);
    if(soil_2 == 0){
      BLEDevice peripheral2 = connectToUuid("2000");
      delay(1000);
      soil_2 = readSoilMoisture(peripheral2, 2);
      return;
    }
  }

  while(millis() > stamp + 5000 ){
    if(soil_1 == 0 || soil_2 == 0){
      return;  
    }
    true;
  }
  delay(2000);
  float value = calcNeededWater(soil_1, soil_2);
  Serial.println(value);

  soil_1 = 0;
  soil_2 = 0;
  
}

BLEDevice connectToUuid(char Uuid[]){
  Serial.print("Connecting to BLE Device with UUID: ");
  Serial.println(Uuid);
  BLE.scanForUuid(Uuid);
  BLEDevice peripheral = BLE.available();

  
  if (peripheral){
    BLE.stopScan();
    Serial.print("found device: ");
    Serial.println(peripheral.localName());
    // check for connection
    if (!peripheral.connect()) {
      Serial.print("Failed to connect to ");
      Serial.println(Uuid);
      peripheral.disconnect();
      return peripheral;
    }

    // check if the peripheral device is setup correctly
    if (!peripheral.discoverAttributes()) {
      Serial.println("Attribute discovery failed!");
      peripheral.disconnect();
      return peripheral;
    }
    Serial.print("setup finished of ");
    Serial.println(Uuid);
    return peripheral;
  }  
}


int readSoilMoisture(BLEDevice peripheral, int device){
  if (peripheral.connected()){
    BLECharacteristic soilCharacteristic = peripheral.characteristic(device == 1 ? "1001" : "2001");
    byte val = 0;
    soilCharacteristic.readValue(val);
    Serial.print(device == 1 ? "Soil Monitor 1 reads: " : "Soil Monitor 2 reads: ");
    Serial.println(val);
    Serial.println((int) val);  
  }

  
  peripheral.disconnect();    
}


void indicate(float moisture){
  if(moisture < 3){
    
  }
  if(moisture < 7){
    
  }
  else{
    
  }
}

// returns representation of how soon water is needed on a scale from 0 to 10
float calcNeededWater(int soil1, int soil2){
  // if the soil is below this threshol+d, the plants need to be watered immediately
  if (soil1 < 280 || soil2 < 280){
    return 10.0;
  }

  float total = 0.0;

  while (! APDS.colorAvailable()) {
    delay(5);
  }
  
  int r, g, b, a;
  APDS.readColor(r,g,b,a);
  
  // if its too dark, we never water 
  if ( a < 300){
    return 0.0;  
  }

  
  float temp = HTS.readTemperature();

  // low temperature -> lesss water evaporates
  if (temp < 22.0 && temp > 10.0){
    total +=  (22.0 - temp) * 0.33;
  }

  // base formula 

  total += ((500-soil1) * 0.1818 ) + ((500-soil2) * 0.1818 );

  return total;
}
