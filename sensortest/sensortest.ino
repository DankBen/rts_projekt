#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>

#define RED 22
#define GREEN 23
#define BLUE 24
#define LED_PWR 25

void setup(){
  Serial.begin(9600);
  while(!Serial);

  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  
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
  digitalWrite(RED, HIGH);
  delay(1000);
  digitalWrite(GREEN,HIGH);
  delay(1000);
  digitalWrite(BLUE, HIGH);
  digitalWrite(LED_PWR, HIGH);

}
