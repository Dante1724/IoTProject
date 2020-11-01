#include <Blynk.h>
#define BLYNK_PRINT Serial

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Albpq_5D7QuTM_fGPoDS44wSSh0EBulf";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "GLADYS";
char pass[] = "87654321";

float cahaya, batasCahaya;
float suhu, batasSuhu;

int pinLDR = 34;
int pinLedSensorCahaya = 33;
int pinLedSensorSuhu = 25;

bool aktif = false;

DHT dht(35, DHT11);

WidgetLED ledCahaya(V6);
WidgetLED ledSuhu(V7);


void sensorWarnaLed(){
  ledCahaya.on();
  if(cahaya > batasCahaya){
    Blynk.setProperty(V6, "color", "#D3435C");
    Serial.println("Status Led Cahaya: Red, Cahaya melebihi batas");
    digitalWrite(pinLedSensorCahaya, HIGH);
  }else{
    Blynk.setProperty(V6, "color", "#23C48E");
    Serial.println("Led Cahaya: Green, Cahaya normal");
    digitalWrite(pinLedSensorCahaya, LOW);
  }

  ledSuhu.on();
  if(suhu > batasSuhu){
    Blynk.setProperty(V7, "color", "#D3435C");
    Serial.print("Temp: "); Serial.println(suhu);
    Serial.println("Status Led Suhu: Red, Suhu melebihi batas");
    digitalWrite(pinLedSensorSuhu, HIGH);
  }else{
    Blynk.setProperty(V7, "color", "#23C48E");
    Serial.print("Cahaya: "); Serial.println(cahaya);
    Serial.println("Status Led Suhu: Green, Suhu normal");
    digitalWrite(pinLedSensorSuhu, LOW);
  }
}

void sensorOn(){
  delay(2000);
  cahaya = analogRead(pinLDR);
  float lux = 0.009768*cahaya+10;
  if(isnan(lux)){
    Serial.println("Gagal membaca LDR sensor");
  }
  Blynk.virtualWrite(V0, lux);
  Blynk.virtualWrite(V2, lux);

  int temp = dht.readTemperature();
  suhu = temp;
  if(isnan(temp)){
    Serial.println("Gagal membaca DHT sensor");
  }
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V3, temp);
}

BLYNK_WRITE(V4){ //slider cahaya
  int pinSlider = param.asInt();
  batasCahaya = pinSlider;
  Serial.print("Nilai threshold cahaya: "); Serial.println(batasCahaya);
}

BLYNK_WRITE(V5){ //slider suhu
  int pinSlider = param.asInt();
  batasSuhu = pinSlider;
  Serial.println("Nilai threshold suhu: "); Serial.println(batasSuhu);
}

BLYNK_WRITE(V8){ //on off
  int pinOnOff = param.asInt();
  if(pinOnOff == 0){
    Serial.println("Sistem mati");
    aktif = false;
  }else if(pinOnOff == 1){
    Serial.println("Sistem aktif");
    aktif = true;
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  
  ledCahaya.on();
  pinMode(pinLedSensorCahaya, OUTPUT);

  ledSuhu.on();
  pinMode(pinLedSensorSuhu, OUTPUT);
  dht.begin();
  

//  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,21), 8080);
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  if(!aktif){
    ledCahaya.off();
    ledSuhu.off();
    digitalWrite(pinLedSensorCahaya, LOW);
    digitalWrite(pinLedSensorSuhu, LOW);
  }else{
    sensorOn();
    sensorWarnaLed();
  }
  
  
}
