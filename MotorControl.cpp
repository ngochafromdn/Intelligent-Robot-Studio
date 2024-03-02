#include "MotorControl.h"

// Define your global variables here
int right_wheel_pulse = 0;
int left_wheel_pulse = 0;

// Define pins for Motor A control
#define MotFwdA 7  // Motor A Forward pin right
#define MotRevA  6 // Motor A Reverse pin right
#define enA 5      // Motor A Enable pin right

// Define pins for Motor B control
#define MotFwdB 8   // Motor B Forward pin left
#define MotRevB  9     // Motor B Reverse pin left
#define enB 10       // Motor B Enable pin left

// Encoder pins for Motor A
int encoderPin1A = 19;

// Encoder pins for Motor B
int encoderPin1B = 18;

// PID Constants
double Kp = 40.0;  // Proportional constant
double Ki = 0.001;  // Integral constant
double Kd = 20;  // Derivative constant

// Variables for PID control
double P = 0;
double I = 0;
double D = 0;
double error = 0;
double previous_error = 0;
double PID_value = 0;

void motorControlSetup() {
  // Set up Motor A and Motor B pins as OUTPUT
  pinMode(MotFwdA, OUTPUT);
  pinMode(MotRevA, OUTPUT);
  pinMode(MotFwdB, OUTPUT);
  pinMode(MotRevB, OUTPUT);

  // Set up encoder pins for Motor A and Motor B
  pinMode(encoderPin1A, INPUT);
  pinMode(encoderPin1B, INPUT);

  // Set up enA and enB as OUTPUT
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  // Initialize serial communication
  Serial.begin(115200);

  // Set initial motor directions
  digitalWrite(MotFwdA, LOW);
  digitalWrite(MotRevA, HIGH);
  digitalWrite(MotFwdB, LOW);
  digitalWrite(MotRevB, HIGH);

  // Attach interrupts for wheel pulse counting
  attachInterrupt(digitalPinToInterrupt(encoderPin1A), rightWheelPulseCount, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPin1B), leftWheelPulseCount, RISING);
}

void rightWheelPulseCount() {
  right_wheel_pulse++;
}

void leftWheelPulseCount() {
  left_wheel_pulse++;
}

void calculatePID() {
  P = error;
  I += error;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
}

void motorControl() {
  // Calculating the effective motor speed:
  int A_motor_speed = initial_motor_speed_right - PID_value;
  int B_motor_speed = initial_motor_speed_left + PID_value;

  // The motor speed should not exceed the max PWM value
  A_motor_speed = constrain(A_motor_speed, 0, 255);
  B_motor_speed = constrain(B_motor_speed, 0, 255);

  analogWrite(enB, B_motor_speed);  // Left Motor Speed
  analogWrite(enA, A_motor_speed);  // Right Motor Speed

  // Motor direction control
  digitalWrite(MotFwdA, LOW);
  digitalWrite(MotRevA, HIGH);
  digitalWrite(MotFwdB, LOW);
  digitalWrite(MotRevB, HIGH);
}

void test_motor(){
  double time = millis();
  digitalWrite(MotFwdA, LOW);
  digitalWrite(MotRevA, HIGH);
  digitalWrite(MotFwdB, LOW);
  digitalWrite(MotRevB, HIGH);
  analogWrite(enB, 140);  // Left Motor Speed
  analogWrite(enA, 120);  // Right Motor Speed
  delay(3000);
  Serial.print("RIGHT-LEFT");
  Serial.println((right_wheel_pulse-left_wheel_pulse)/(time-millis()));
  Serial.print("RIGHT/LEFT");
  Serial.println((right_wheel_pulse/left_wheel_pulse)*1000);
}

void stop() {
  analogWrite(enB, 0);  // Left Motor Speed
  analogWrite(enA, 0);  // Right Motor Speed
  digitalWrite(MotFwdA, LOW);
  digitalWrite(MotRevA, LOW);
  digitalWrite(MotFwdB, LOW);
  digitalWrite(MotRevB, LOW);
}
