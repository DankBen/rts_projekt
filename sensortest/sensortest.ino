#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>


void setup(){
  Serial.begin(9600);
  while(!Serial);

  if (!HTS.begin()){
    Serial.println("Failed to initialize humidity temperature sensor");
    while(1);
  }

  if(!APDS.begin()){
    Serial.println("Failed to initialize ambient light sensor");
    while(1);  
  }

  Serial.println("Setup complete.");
}

void loop(){
   while (! APDS.colorAvailable()) {
    delay(5);
  }
  int r, b, g, a;
  APDS.readColor(r,g,b,a);
  Serial.println(a);
  Serial.print(HTS.readTemperature());
  Serial.println(" °C");
  delay(1000);
}
