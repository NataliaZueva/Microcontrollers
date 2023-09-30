#define led_pin 2
int sensor_pin = A0;
bool streaming = false;
bool send_one_value = false;
long previous_send_time = 0;
long send_count = 0;
bool manual_control_on = false; 
bool manual_control_off = false; 
bool running = false;

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long current_time = millis();
  data_reading();
  if (!manual_control_on || manual_control_off) {diod();}
  if (streaming == true and current_time / 100 != send_count) {
    send_photo_data();
    previous_send_time = previous_send_time + 100;
    send_count = current_time / 100;
    Serial.print("Time in millis:");
    Serial.println(millis());
  }
  if (send_one_value == true) {
    send_photo_data();
    send_one_value = false;
  }
  if (manual_control_on == true) {
    int val = analogRead(sensor_pin);
    digitalWrite(led_pin, HIGH);
    while (abs(val - analogRead(sensor_pin)) <= 150) {
      char command = Serial.read();
      if (command == 'f') {val = 2000;}
    }
    manual_control_on = false;
  }
  if (manual_control_off == true) {
    int val = analogRead(sensor_pin);
    digitalWrite(led_pin, LOW);
    while (abs(val - analogRead(sensor_pin)) <= 150) {
      char command = Serial.read();
      if (command == 'o') {val = 2000;}
    }
    manual_control_off = false;
  }
  if (running == true) {
    while (running) {
      char command = Serial.read();
      if (command == 'x') {
        running = false; 
      }
      digitalWrite(led_pin, HIGH);
      digitalWrite(led_pin, LOW);
      ne_delay(500);
      digitalWrite(led_pin, LOW);
      digitalWrite(led_pin, HIGH);
      ne_delay(500);
    }
  }
}

void ne_delay(int del) {
  unsigned long current_time = millis();
  while (millis() - current_time <= del);
}

void diod() {
  int val = analogRead(sensor_pin);
  if (val <= 500 ) { 
    digitalWrite(led_pin, HIGH);
    digitalWrite(led_pin, LOW);
  } else {
    digitalWrite(led_pin, LOW);
    digitalWrite(led_pin, HIGH);
  }
}

void send_photo_data() {
  int val = analogRead(sensor_pin);
  Serial.print("Sensor value:");
  Serial.println(val);
}

void data_reading() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'p') { // p для включения режима считывания фотодатчика
      streaming = true;
    } else if (command == 's') { // s для отправки одного значения
      send_one_value = true;
    } else if (command == 'o') { // o для включения светодиода (ручное управление)
      manual_control_on = true;
    } else if (command == 'f') { // f для выключения светодиода (ручное управление)
      manual_control_off = true;
    } else if (command == 'z') { // z для включения сигнализации 
      running = true;
    } else {
      streaming = false;
    }
  }
}
