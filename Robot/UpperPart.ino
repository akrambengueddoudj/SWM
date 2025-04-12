#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 21
MFRC522 mfrc522(SS_PIN, RST_PIN);

// DC Motors (L298N) - Using safe ESP32 pins
const int LEFT_SPEED = 13;  // PWM capable (was Arduino 5)
const int LEFT_FWD = 14;     // (was Arduino 4)
const int LEFT_REV = 27;     // (was Arduino 2)

const int RIGHT_SPEED = 12; // PWM capable (was Arduino 6)
const int RIGHT_FWD = 26;    // (was Arduino 3)
const int RIGHT_REV = 25;    // (was Arduino 7)

// IR Sensors (Using ESP32 analog pins)
const int SENSOR_RIGHT_Extreme = 36; // VP (Analog-capable)
const int SENSOR_RIGHT = 39;         // VN (Analog-capable)
const int SENSOR_LEFT = 34;          // Analog-capable
const int SENSOR_LEFT_extreme = 35;  // Analog-capable

int correctioncoef = 170; // Your correction factor

  int leftSensor ;
  int rightSensorExtreme; 
  int leftSensorExtreme ;
  int rightSensor  ;
 int c = 1 ;
  int d = 1 ;
const byte authorizedUID[4] = {0xD3, 0x2D, 0x56, 0xFE};
bool programExecuted = false;


// Stepper Motor Pins (L298N)
const int enA = 13;  // PWM for Motor A
const int in1 = 14;  // Direction control 1
const int in2 = 27;  // Direction control 2
const int in3 = 26;  // Direction control 3
const int in4 = 25;  // Direction control 4
const int enB = 12;  // PWM for Motor B

// Ultrasonic Pins
const int trigPin = 17;
const int echoPin = 16;

// Servo
const int servoPin = 22;
Servo myservo;

// Timing
const int stepDelay = 5;  // Stepper speed control (ms)
int pos = 0;            // Initial servo position

void setup() {
  Serial.begin(115200);

  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Ready - Show RFID card");
  // Stepper motor setup
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Critical GPIO 2 setup:
  pinMode(2, OUTPUT);        // Must declare as output
  digitalWrite(2, HIGH);     // Must set HIGH after boot
  delay(100)
  
  stopStepper();
  // Ultrasonic setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Servo setup
  myservo.attach(servoPin, 1000, 2000);  // Standard 50Hz servo
  myservo.write(pos);
}

void loop() {
  if (!programExecuted) {
    runMainProgram();
    
    if (checkRFID()) {
      runSecondaryProgram();
      programExecuted = true; // Stop after execution
    }
  }
}
void runMainProgram() {
  // // Your main continuous code here
  // Serial.println("Running main tasks...");
  // delay(500); // Example delay
  // put your main code here, to run repeatedly:
 leftSensor   = digitalRead(SENSOR_LEFT);
  rightSensorExtreme = digitalRead(SENSOR_RIGHT_Extreme );
  leftSensorExtreme = digitalRead(SENSOR_LEFT_extreme );
  rightSensor  = digitalRead(SENSOR_RIGHT);
  
  Serial.print(leftSensorExtreme);
    Serial.print("    ");
  Serial.println(rightSensorExtreme);

 
  if (leftSensor == 0  && rightSensor == 0) {
    goForward();
  }
  if (leftSensorExtreme == 1  || leftSensor == 1 ) {
      turnLeftCorrection();
 }
  if (rightSensorExtreme == 1  || rightSensor == 1 ) {
      turnRightCorrection();
 }

}
void runSecondaryProgram()
{
  // 1. Stepper forward
  rotateStepperBackward(3200);
  stopStepper();
  delay(500);

  // 2. Check distance
  float distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  // 3. Servo action if object detected (2-12cm)
  if (distance >= 2.0 && distance <= 12.0) {
    sweepServo();
    delay(1000);
  }

  // 4. Stepper backward
  rotateStepperForward(3400);
  stopStepper();
  delay(5000);
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


// Servo control
void sweepServo() {
  // Sweep down
  for (pos = 0; pos <= 130; pos += +10) {
    myservo.write(pos);
    delay(20);
  }
  // Sweep up
  for (pos = 130; pos >= 0; pos -= 5) {
    myservo.write(pos);
    delay(20);
  }
}

// Ultrasonic function
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30ms
  return duration * 0.034 / 2;  // Distance in cm
}

bool checkRFID() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    bool authorized = true;
    for (byte i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] != authorizedUID[i]) {
        authorized = false;
        break;
      }
    }
    mfrc522.PICC_HaltA();
    return authorized;
  }
  return false;
}

void goForward() {
     analogWrite(LEFT_SPEED, 170); 
  analogWrite(RIGHT_SPEED, 170); 
  // Left motor: forward.
  digitalWrite(LEFT_FWD, HIGH);
  digitalWrite(LEFT_REV, LOW);
  
  // Right motor: forward.
  digitalWrite(RIGHT_FWD, HIGH);
  digitalWrite(RIGHT_REV, LOW);
}
// Turns the robot to the left.
void turnLeftCorrection() {
  // Left motor: continue forward.
  digitalWrite(LEFT_FWD, LOW);
  digitalWrite(LEFT_REV, HIGH);
  
  // Right motor: stop (or continue slow forward for sharper turn).
  digitalWrite(RIGHT_FWD, HIGH);
  digitalWrite(RIGHT_REV, LOW);

   analogWrite(LEFT_SPEED, correctoncoef); 
  analogWrite(RIGHT_SPEED, correctoncoef);
}


// Turns the robot to the right.
void turnRightCorrection() {
 // Left motor: stop (or continue slow forward for sharper turn).
  digitalWrite(LEFT_FWD, HIGH);
  digitalWrite(LEFT_REV, LOW);
  
  // Right motor: forward.
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(RIGHT_REV, HIGH);

    analogWrite(LEFT_SPEED, correctoncoef); 
  analogWrite(RIGHT_SPEED, correctoncoef);
}

// Stops both motors.
void stopMotors() {
        analogWrite(LEFT_SPEED, 0); 
  analogWrite(RIGHT_SPEED, 0);
  digitalWrite(LEFT_FWD, LOW);
  digitalWrite(LEFT_REV, LOW);
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(RIGHT_REV, LOW);

}

