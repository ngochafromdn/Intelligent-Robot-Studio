#include "PID_Control.h"

// Global variables for PID control
double Kp = 40.0;    // Proportional constant
double Ki = 0.001;   // Integral constant
double Kd = 20;      // Derivative constant

double P = 0;
double I = 0;
double D = 0;
double error = 0;
double previous_error = 0;
double PID_value = 0;

int initial_motor_speed_left = 140;
int initial_motor_speed_right = 100;

void calculatePID() {
  P = error;
  I += error;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
}

void resetPID() {
  // Reset PID variables
  P = 0;
  I = 0;
  D = 0;
  error = 0;
  previous_error = 0;
  PID_value = 0;
}
