const int hallPin = 3;
boolean lastRevolutionPin = LOW;
boolean currentRevolutionPin = LOW;
unsigned long Count = 0;
    
void setup() {
  pinMode (hallPin, INPUT);
  Serial.begin(9600);
}
    
void loop() {
  Serial.println(pleasecountaccurate());
  Count = 0;
}
    
int pleasecountaccurate() {
  while (Count < 20) { // I want to return only when it completed 20 rotations 
    currentRevolutionPin = check(lastRevolutionPin, hallPin);
    if (lastRevolutionPin == HIGH && currentRevolutionPin == LOW) {
      Count++;
    }
    lastRevolutionPin = currentRevolutionPin;
  }
  return Count;
}
    
boolean check(boolean last, int pin) {
  boolean current = digitalRead(pin);
  if (last != current) {
    delay(5);
    current = digitalRead(pin);
  }
  return current;
}
