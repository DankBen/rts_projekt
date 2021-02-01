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
  // set pins OFF, HIGH and LOW are flipped...
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(GREEN, HIGH);  
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
      peripheral1.disconnect(); 
      return;
    }
    delay(1000);
    if(soil_2 == 0){
      BLEDevice peripheral2 = connectToUuid("2000");
      delay(1000);
      soil_2 = readSoilMoisture(peripheral2, 2);
      peripheral2.disconnect();
      return;
    }
  }

  delay(10000);
  stamp = millis();  
  float val = calcNeededWater(soil_1, soil_2);
  Serial.println(val);
  indicate(val);
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
    //byte val = 0;
    int val = 0;
    soilCharacteristic.readValue((char*)&val, sizeof(val));
    Serial.print(device == 1 ? "Soil Monitor 1 reads: " : "Soil Monitor 2 reads: ");
    Serial.println(val);
    //Serial.println((int) val);  
    return val;
  }
  Serial.println("couldnt read because of missing conneciton");
  delay(1000);
  return 0; 
}


void indicate(float moisture){
  if(moisture < 3.0){
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, LOW);
  }
  else if(moisture < 7.0){
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
  }
  else{
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, HIGH);
  }
}

// returns representation of how soon water is needed on a scale from 0 to 10
float calcNeededWater(int soil1, int soil2){
  // if the soil is below this threshol+d, the plants need to be watered immediately

  float total = 0.0;

  while (! APDS.colorAvailable()) {
    delay(5);
  }
  
  int r, g, b, a;
  APDS.readColor(r,g,b,a);
  
  // if its too dark, we never water 

  Serial.print("Ambient Light reading: ");
  Serial.println(a);
  
  if ( a < 300){
    Serial.println("Too Dark to water.");
    return 0.0;  
  }

  
  float temp = HTS.readTemperature();
  
  Serial.print(temp);
  Serial.println(" °C");
  
  // low temperature -> lesss water evaporates
  if (temp < 22.0 && temp > 10.0){
    total +=  (22.0 - temp) * 0.33;
    Serial.print("Temperature is in Optimal Range");
  }


  // base formula 

  total += ((500-soil1) * 0.1818 ) + ((500-soil2) * 0.1818 );

  return total;
}
