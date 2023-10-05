int led_pin = LED_BUILTIN;

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'u') {
      digitalWrite(led_pin, HIGH);
      Serial.print("led on");
    } else if (cmd == 'd') {
      digitalWrite(led_pin, LOW);
      Serial.print("led off");
    }
  }
}