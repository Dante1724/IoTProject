#include <ESP32Servo.h>

#include <WiFi.h>
#include <MQTT.h>

const char ssid[] = "GLADYS";
const char pass[] = "87654321";

Servo servo; //initialize a servo object
int pinLdr = 35;
int lux;
int stats=0;
int angle = 0; 
WiFiClient net;
MQTTClient client;


void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.print("connecting...");
  while (!client.connect("ESP32s", "baaec812", "cc5190e9eee6e40f")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("connected!");

  client.subscribe("/lux");
  client.subscribe("/status");
}

void sensorCahaya(){
  if(stats == 1){
    float light = analogRead(pinLdr);
    lux = (0.009768*light)+10;
    if (isnan(lux)) {
        Serial.println("Failed to read from LDR sensor!");
        return;
     }else{
        Serial.print("LUX: ");
        Serial.println(lux);
     }
    String luxString = String(lux);
    delay(1000);
    client.publish("/lux",luxString);
    if(lux >= 20){
      delay(2000);
      openCurtain();
    }else{
      delay(2000);
      closeCurtain();
    }
  }else if(stats == 0){
    client.publish("/ldr", "0");
  }
  
}

void openCurtain(){
  for(angle = 0; angle < 180; angle += 1)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo.write(angle);                 //command to rotate the servo to the specified angle
    delay(15);                       
  } 
 
  delay(1000);
}

void closeCurtain(){
  for(angle = 180; angle>=1; angle-=5)     // command to move from 180 degrees to 0 degrees 
  {                                
    servo.write(angle);              //command to rotate the servo to the specified angle
    delay(5);                       
  } 

    delay(1000);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(topic == "/status"){
      if(payload == "1"){
        stats = 1;
        Serial.println("Sistem Hidup");
      }else if(payload == "0"){
        stats = 0;
        Serial.println("Sistem Mati");
      }
  }
}

void setup() {
  servo.attach(36);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
 
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  connect();
}

void loop() {
  client.loop();
  delay(10);  
  if (!client.connected()) {
    connect();
  }
  sensorCahaya();
}
