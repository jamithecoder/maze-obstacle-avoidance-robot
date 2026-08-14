/*
  Maze / Obstacle Avoidance Robot — Arduino Uno build
  BRAC University, CSE-350 (Project Demo-1)

  A single HC-SR04 ultrasonic sensor is mounted on a servo. The robot
  drives forward and checks the distance ahead every `interval` ms.
  When an obstacle is closer than the threshold, it stops, scans right
  and left with the servo, and records the clearance on each side.

  Full write-up: ../../docs/DESIGN_DOCUMENT.md

  NOTE: moveForward(), stopMotor(), moveRight() and moveLeft() below
  were reconstructed from the pin-logic described in the design
  document's "Motor Control and Differential Drive" section — the
  original sketch in the report was pasted in truncated. The decision
  logic that compares lval/rval and actually steers the robot was
  never completed in the original project (see "Challenge 4:
  Incomplete Avoidance Logic" in the design doc); it's left as a TODO
  below rather than invented, so this file matches the real state of
  the project.
*/

#include <Servo.h>

// ---- Motor driver (L298N) pins ----
const int in1 = 7; // IN1 - Motor 1 (left) direction pin
const int in2 = 6; // IN2 - Motor 1 (left) direction pin
const int in3 = 5; // IN3 - Motor 2 (right) direction pin
const int in4 = 4; // IN4 - Motor 2 (right) direction pin

// ---- Ultrasonic sensor (HC-SR04) pins ----
const int trigPin = 8; // Trigger pin
const int echoPin = 9; // Echo pin

// ---- Servo scan positions ----
int forwardAngle = 90;
int rightAngle    = 0;
int leftAngle     = 180;
Servo servo;

// ---- Last scan results (cm) ----
int lval = 999;
int rval = 999;

// ---- Non-blocking timing ----
unsigned long previousMillis = 0;
const long interval = 500; // distance check interval (ms)

long duration; // echo pulse duration (us)
int distance;  // last measured distance (cm)

void setup() {
  // Motor control pins as outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  servo.attach(10);
  servo.write(forwardAngle);

  stopMotor(); // Start at rest
}

void loop() {
  unsigned long currentMillis = millis();

  // Check distance every `interval` ms without blocking
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    distance = checkDistance();

    Serial.print("Distance: ");
    Serial.println(distance);

    // Obstacle detected within threshold
    if (distance < 15) {
      stopMotor();
      Serial.println("Obstacle detected! Stopping.");

      servo.write(rightAngle);
      delay(700);
      rval = checkDistance();
      Serial.println(rval);

      servo.write(forwardAngle);
      delay(700);

      servo.write(leftAngle);
      delay(700);
      lval = checkDistance();
      Serial.println(lval);

      // TODO: compare lval and rval and call moveLeft() / moveRight()
      // toward whichever side has greater clearance, with an escape
      // case for when both sides are blocked. See "Challenge 4" and
      // "Future Improvements" in docs/DESIGN_DOCUMENT.md.
    }
  }

  // Keep moving forward while checking the distance
  moveForward();
}

// ---- Distance measurement ----
int checkDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int cm = duration * 0.034 / 2;
  return cm;
}

// ---- Motor control ----
// Reconstructed from the pin logic in the design document:
//   IN1=HIGH, IN2=LOW  -> Motor 1 (left)  forward
//   IN3=LOW,  IN4=HIGH -> Motor 2 (right) forward
//   Reversing each pair reverses that motor.

void moveForward() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);  // left forward
  digitalWrite(in3, LOW);  digitalWrite(in4, HIGH); // right forward
}

void stopMotor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void moveRight() {
  // Clockwise rotation: left forward, right reverse
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);  // left forward
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);  // right reverse
}

void moveLeft() {
  // Counter-clockwise rotation: left reverse, right forward
  digitalWrite(in1, LOW);  digitalWrite(in2, HIGH); // left reverse
  digitalWrite(in3, LOW);  digitalWrite(in4, HIGH); // right forward
}
