
#include "DHT.h"
#define DHTPIN 12     // на какой пин подключен датчик

// релюшки
#define RELE_COOLER 4
#define RELE_UVLAJNITEL 5
#define RELE_SVET 6
// тип включения реле
#define ON 0
#define OFF 1


#define DHTTYPE DHT11   // тип датчика
DHT dht(DHTPIN, DHTTYPE);

// границы влажности для работы увлажнителя
float hum_max = 60.00;
float hum_min = 40.00;

//порог срабатывания кулера доп проветривания бокса.
float deltaHumCooler = 10.00;


void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  pinMode(RELE_UVLAJNITEL, OUTPUT); // реле на 5 пине
  pinMode(RELE_SVET, OUTPUT); // реле на 6 пине
  pinMode(RELE_COOLER, OUTPUT); // реле на 4 пине
  digitalWrite(RELE_UVLAJNITEL, OFF);
  digitalWrite(RELE_COOLER, OFF);
  digitalWrite(RELE_SVET, OFF);
  dht.begin();
}//setup



void loop() {

  delay(2000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // проверка, что датчик считался
  if (isnan(h) || isnan(t)) {
    Serial.println("Error DHT sensor!");
    return;
  }// конец  проверки

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.println();

// зададим включение влажности до 60% 
// пока влажность не опустится до 40%, включать не будем
  if (h <= hum_min) {
    digitalWrite(RELE_UVLAJNITEL, ON);
    Serial.print(" Uvlajnitel Vkl. Vl = "); 
    Serial.println(h);
  }//if h
  if (h >= hum_max) {
    digitalWrite(RELE_UVLAJNITEL, ON);
    Serial.println("    Uvlajnitel Otkl. Vl =");
    Serial.println(h);
  }//if h

// зададим включение дополнительной вентиляции бокса при влажности на 5% больше чем максимальная для увлажнителя
// выключение кулера при влажности на 10% меньшей, чем максимальная для увлажнителя
if(h <= (hum_max - deltaHumCooler)){
    digitalWrite(RELE_COOLER, OFF);
    Serial.println("    Cooler OFF. Vl =");
    Serial.println(h);
  
}
if(h >= (hum_max + deltaHumCooler)){
    digitalWrite(RELE_COOLER, ON);
    Serial.println("    Cooler ON. Vl =");
    Serial.println(h);
}


}//loop
