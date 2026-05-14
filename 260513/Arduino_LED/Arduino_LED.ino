int currentLED = 8;  

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);

  Serial.begin(9600);
  Serial.println("Serial Test!!");
}

void loop() {
  char data;

  if (Serial.available() > 0)
  {
    data = Serial.read();

    if (data == '\n' || data == '\r')
    {
      return;
    }

    Serial.print("ECHO >> ");
    Serial.write(data);
    Serial.println();

    if (data == 'o')
    {
      digitalWrite(currentLED, LOW);
    }
    else if (data == 'x')
    {
      digitalWrite(currentLED, HIGH);
    }
    else if (data == 'r')   // 빨간색 LED 켜기
    {
      digitalWrite(currentLED, HIGH);
      currentLED = 8;
      digitalWrite(currentLED, LOW);
    }
    else if (data == 'g')   // 초록색 LED 켜기
    {
      digitalWrite(currentLED, HIGH);
      currentLED = 9;
      digitalWrite(currentLED, LOW);
    }
    else if (data == 'b')   // 파란색 LED 켜기
    {
      digitalWrite(currentLED, HIGH);
      currentLED = 10;
      digitalWrite(currentLED, LOW);
    }
  }
}