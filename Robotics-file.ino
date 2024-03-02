#include "MotorControl.h"
#include "SensorControl.h"
#include "PID_Control.h"
#include "GoalController.h"

int sensor[5];
int s1 = 30;
int s2 = 32;
int s3 = 34;
int s4 = 31;
int s5 = 33;


int initial_motor_speed_left = 140;
int initial_motor_speed_right = 100;



const int lidarPin = ;  // Assuming Lidar data pin is connected to digital pin 

void setup()
{
  Serial.begin(25600);

}

void loop()
{

  test_motor();


}