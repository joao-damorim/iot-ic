#define LED1  5

void setup() {
  pinMode(LED1, OUTPUT);
  Serial.begin(9600); 
}

void loop() { 
  //digitalWrite(LED1, HIGH);
  if(Serial.available() > 1){
    long input = Serial.parseInt();
    Serial.println(input);  
    if(input == 987654321) {
      digitalWrite(LED1, HIGH);  
    } 
    if(input == 123456789) {
      digitalWrite(LED1, LOW);
    }
  }
  delay(20);
}
