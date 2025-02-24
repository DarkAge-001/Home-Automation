#define PIR_PIN 4
#define BUZZER_PIN 5
#define LED_PIN 3

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    Serial.println("Motion Detected! Triggering Alarm...");
    digitalWrite(BUZZER_PIN, HIGH); // Turn On Alarm
    digitalWrite(LED_PIN, HIGH);
    delay(1000); // Alarm stays on for 1 second
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Turn off Alarm
    digitalWrite(LED_PIN, LOW);
    Serial.println("No Motion detected.");
  }
  delay(500); // Small delays before the next check
}
