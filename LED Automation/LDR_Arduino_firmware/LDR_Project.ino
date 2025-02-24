#define LDR_PIN A0 // LDR sensor connected to A0
#define LED_PIN 9 // PWM pin for LED strip (via transistor)

int lightThreshold = 300; //Adjust based on Testing

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  if (lightLevel < lightThreshold) {
    analogWrite(LED_PIN, 255); //Turn LED strip ON (Full brightness)
    Serial.println("LED Strip ON");
  } else {
    analogWrite(LED_PIN, 0); // Turn LED Strip OFF
    Serial.println("LED Strip OFF");
  }

  delay(500); // Small delay to avoid flickering
}
