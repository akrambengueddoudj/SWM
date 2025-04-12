#include <ESP32Servo.h>

// Define L298N connections for single stepper motor
const int enA = 13;  // PWM for Motor A (Speed control)
const int in1 = 14;  // Direction control 1
const int in2 = 27;  // Direction control 2
const int in3 = 26;  // Direction control 3
const int in4 = 25;  // Direction control 4
const int enB = 12;  // PWM for Motor B (Speed control)

const int stepDelay = 5;  // Delay between steps (ms) - controls speed

void setup() {
  // Initialize motor control pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  
  // Initialize with motor stopped
  stopStepper();
  
  Serial.begin(115200);
  Serial.println("Stepper Motor Control Ready");
}

void loop() {
  // Example usage:
  Serial.println("Rotating forward for 2000ms");
  rotateStepperBackward(3200);  // Rotate forward for 2 seconds
  delay(1000);                 // Pause for 1 second
  
  Serial.println("Rotating backward for 2000ms");
  rotateStepperForward(3400); // Rotate backward for 2 seconds
  delay(1000);                 // Pause for 1 second
}

void rotateStepperForward(int durationMS) {
  Serial.println("Starting forward rotation");
  unsigned long start = millis();
  while (millis() - start < durationMS) {
    // Set motor speed (150/255 ≈ 60% power)
    analogWrite(enA, 150);
    analogWrite(enB, 150);

    // Full-step sequence (4 steps per cycle)
    stepMotor(HIGH, LOW, HIGH, LOW); delay(stepDelay);
    stepMotor(LOW, HIGH, HIGH, LOW); delay(stepDelay);
    stepMotor(LOW, HIGH, LOW, HIGH); delay(stepDelay);
    stepMotor(HIGH, LOW, LOW, HIGH); delay(stepDelay);
  }
  stopStepper();
  Serial.println("Forward rotation complete");
}

void rotateStepperBackward(int durationMS) {
  Serial.println("Starting backward rotation");
  unsigned long start = millis();
  while (millis() - start < durationMS) {
    // Set motor speed
    analogWrite(enA, 255);
    analogWrite(enB, 255);

    // Full-step sequence in reverse
    stepMotor(HIGH, LOW, LOW, HIGH); delay(stepDelay);
    stepMotor(LOW, HIGH, LOW, HIGH); delay(stepDelay);
    stepMotor(LOW, HIGH, HIGH, LOW); delay(stepDelay);
    stepMotor(HIGH, LOW, HIGH, LOW); delay(stepDelay);
  }
  stopStepper();
  Serial.println("Backward rotation complete");
}

void stepMotor(int i1, int i2, int i3, int i4) {
  digitalWrite(in1, i1);
  digitalWrite(in2, i2);
  digitalWrite(in3, i3);
  digitalWrite(in4, i4);
}

void stopStepper() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}
