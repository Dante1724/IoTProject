int ledPin1 = 23;
int ledPin2 = 22;
int ledPin3 = 1;
int ledState = LOW;
int count = 3;
unsigned long currentMillis = millis();

unsigned long previousMillis = 0;
const long interval = 1000; 

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 
  while(true){
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      
      previousMillis = currentMillis;
  
      
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      
      if(count == 3){
        digitalWrite(ledPin1, ledState);
      }
      else if(count == 2){
        digitalWrite(ledPin2, ledState);
      }
      else if(count == 1){
        digitalWrite(ledPin3, ledState);
      }
      if(ledState == LOW){
        if(count != 1){
        count--;
        } else if(count == 1){
          count+=2;
        }
        break;
      }
    }
  }
  
}
