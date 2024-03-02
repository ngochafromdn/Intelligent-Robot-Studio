#include "GoalController.h"

// Global variables for goal control
double vA_actual = 0;
double vB_actual = 0;

float x_now = 0;
float y_now = 0;
float target_x;
float target_y;
float r = 43 / 2; // Assuming r is the radius
double theta = 0; // Current orientation (radians)
const float target_head_angle = 3.14 / 2;
const double d = 171; // distance between two wheels mm
const double gamma = 0.1;
const double lamda = 0.1;
const double h = 3;

void controller_goal(float target_x, float target_y) {
  double right_wheel_pulse_NOW = right_wheel_pulse;
  double left_wheel_pulse_NOW = left_wheel_pulse;
  float goal_threshold = 10;

  while ((abs(target_y - y_now) > goal_threshold)) {
    vA_actual = ((right_wheel_pulse - right_wheel_pulse_NOW) / 1050) * (2 * PI * r); // Calculate actual speed of motor1
    vB_actual = ((left_wheel_pulse - left_wheel_pulse_NOW) / 1050) * (2 * PI * r);    // Calculate actual speed of motor2
    right_wheel_pulse_NOW = right_wheel_pulse;                                        // Previous count for motor_right pulses
    left_wheel_pulse_NOW = left_wheel_pulse;                                          // Previous count for motor_left pulses

    right_wheel_pulse_NOW = right_wheel_pulse; // Previous count for motor_right pulses
    left_wheel_pulse_NOW = left_wheel_pulse;   // Previous count for motor_left pulses
    x_now += (vA_actual + vB_actual) / 2 * cos(theta);
    y_now += (vA_actual + vB_actual) / 2 * sin(theta);
    theta += (vA_actual - vB_actual) / d;
    theta = atan2(sin(theta), cos(theta)); // Normalize the angle

    float deltaX = target_x - x_now; //(mm) //(distance to goal by x)
    float deltaY = target_y - y_now; //(distance to goal by y )
    float rho = sqrt(pow(deltaX, 2) + pow(deltaY, 2)); // distance from current position to goal
    float phi = atan2(deltaY, deltaX) - target_head_angle;
    float alpha = atan2(deltaY, deltaX) - theta;
    float average_v = gamma * cos(alpha) * rho;
    float w = lamda * alpha + gamma * ((alpha + h * phi) / alpha) * cos(alpha) * sin(alpha);

    float v_A = average_v + d * w / 2;
    float v_B = average_v - d * w / 2;
    float speed_A = abs(v_A * 0.8);
    float speed_B = abs(v_B * 0.8);

    // Convert to PWM
    speed_A = constrain(speed_A, 0, 255);
    speed_B = constrain(speed_B, 0, 255);
    analogWrite(enB, speed_B); // Left Motor Speed
    analogWrite(enA, speed_A); // Right Motor Speed
    digitalWrite(MotFwdA, LOW);
    digitalWrite(MotRevA, HIGH);
    digitalWrite(MotFwdB, LOW);
    digitalWrite(MotRevB, HIGH);
  }
  stop();
}
