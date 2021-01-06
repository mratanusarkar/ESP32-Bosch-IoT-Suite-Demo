#include "DHT.h"

// ESP32 In-Built Sensor Values
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

int Temp_val = 0;
int Hall_val = 0;

// define LED pins
const int LED_RED = 2;

// Touch Sensor
const int TOUCH_0 = 4;
int T0_val;

// DHT22
const int DHTPIN = 5;
const int DHTTYPE = 11;

float Humidity_val = 0.0;
float TempC_val = 0.0;
float TempF_val = 0.0;
float HeatIndexC = 0.0;
float HeatIndexF = 0.0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // LEDs
  pinMode(LED_RED, OUTPUT);

  // Touch Sensors
  pinMode(TOUCH_0, INPUT);
  Serial.begin(115200);

  // DHT
  dht.begin();
}

void loop() {
  // Read Sensor Values from ESP32
  T0_val = touchRead(TOUCH_0);
  Hall_val = hallRead();
  Temp_val = (temprature_sens_read() - 32) / 1.8;

  // Read from DHT11
  Humidity_val = dht.readHumidity();
  TempC_val = dht.readTemperature(false);
  TempF_val = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(Humidity_val) || isnan(TempC_val) || isnan(TempF_val)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Heat Index
  HeatIndexC = dht.computeHeatIndex(TempC_val, Humidity_val, false);
  HeatIndexF = dht.computeHeatIndex(TempF_val, Humidity_val, true);

  // Red led toggle according to T0 value
  if (T0_val < 40){
    digitalWrite(LED_RED, HIGH);
  }
  else {
    digitalWrite(LED_RED, LOW);
  }

  // print sensor values
  Serial.print("Touch Sensor Value: ");
  Serial.println(T0_val);
  Serial.print("Internal Hall Sensor Value: ");
  Serial.println(Hall_val);
  Serial.print("Internal Temperature: ");
  Serial.print(Temp_val);
  Serial.println("°C ");

  Serial.print("DHT11 Humidity: ");
  Serial.print(Humidity_val);
  Serial.println("% ");
  Serial.print("DHT11 Temperature: ");
  Serial.print(TempC_val);
  Serial.print("°C / ");
  Serial.print(TempF_val);
  Serial.println("°F ");

  Serial.print("Heat Index: ");
  Serial.print(HeatIndexC);
  Serial.print("°C / ");
  Serial.print(HeatIndexF);
  Serial.println("°F ");

  Serial.println();
  
  // delay
  delay(1000);
}
