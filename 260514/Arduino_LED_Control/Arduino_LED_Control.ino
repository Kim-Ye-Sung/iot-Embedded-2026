String cmd = "";
const int LED_PIN = 8; 


void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(115200);
}

void loop() {
  while(Serial.available()>0)
  {
    cmd = Serial.readStringUntil('\n');
      if(cmd == "ON")
      {
        digitalWrite(LED_PIN, LOW);
      }
      else if(cmd == "OFF")
      {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED OFF");
      }
  }
}
