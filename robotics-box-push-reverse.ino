#define EMIT_PIN 11
#define BR 5
#define BL 4
#define RBS 748 //252blacktapped
#define LBS 808
#include "motors.h"
#include "bump_sensor.h"
float misalignment_rate = 0.0;
long startTime = millis();
//#define RBS 720
//#define LBS 840

RoboMotors motor;
BumpSensor sensor;

void setup() {
  sensor.initialise();
  Serial.begin(9600);
  delay(1000);
  Serial.println("****RESET****");
  moveMotorBackward();
}

void loop() {
  moveMotorForward();
  //moveMotorBackward();
  delay(2000);
  sensor.readBumpSensor();
  float rbs = sensor.bump_sensor_reading[0];
  float lbs = sensor.bump_sensor_reading[1];

  //  for (int i = 0; i < 2; i++) {
  //    Serial.print(sensor.bump_sensor_reading[i]);
  //    Serial.print(",");
  //
  //  }
  //  Serial.println("");


  if (sensor.bump_sensor_reading[0] > RBS && sensor.bump_sensor_reading[1] < LBS) {
    //    Serial.println("RB: Activated, LB:Not Activated");
    //    Serial.print(sensor.bump_sensor_reading[0]);
    //    Serial.print(",");
    //    Serial.print(sensor.bump_sensor_reading[1]);
    //Serial.println("Object is in Right of Robot");
    misalignment_rate += 1.0;
    moveMotorRight();

  } else if (sensor.bump_sensor_reading[0] < RBS && sensor.bump_sensor_reading[1] > LBS) {
    //    Serial.println("RB:Not Activated, LB:Activated");
    //    Serial.print(sensor.bump_sensor_reading[0]);
    //    Serial.print(",");
    //    Serial.print(sensor.bump_sensor_reading[1]);
    // Serial.println("Object is in left of Robot");

    misalignment_rate += 1.0;
    moveMotorLeft();

  }  else if (sensor.bump_sensor_reading[0] >= RBS && sensor.bump_sensor_reading[1] >= LBS) {
    //    Serial.println("RB:Activated, LB:Activated");
    //    Serial.print(sensor.bump_sensor_reading[0]);
    //    Serial.print(",");
    //    Serial.print(sensor.bump_sensor_reading[1]);
    // Serial.println("Object is in centre");
    moveMotorBackward();
    // delay(100);
  }
  else  {
    //   Serial.println("Object not detected");
    moveMotorForward();
    //stopMotor();
    delay(100);
  }
  long currentTime = millis();
  float elapsedTime = (currentTime - startTime) / 1000.0;
  Serial.print(misalignment_rate);
  Serial.print(",");
  Serial.println(elapsedTime);
  delay(100);
}
void stopMotor() {
  motor.setMotorPower(0, 0);
}
void moveMotorBackward() {
  float turn = sensor.weighted_measure();
  float left_wheel_speed = backwardBias - turn;
  float right_wheel_speed = backwardBias + turn;
  motor.setMotorPower(left_wheel_speed, right_wheel_speed);
}

void moveMotorLeft() {
  float turn = sensor.weighted_measure();
  float left_wheel_speed = forwardBias + turn;
  float right_wheel_speed = forwardBias - turn;
  motor.setMotorPower( left_wheel_speed, -right_wheel_speed);
}

void moveMotorRight() {
  float turn = sensor.weighted_measure();
  float left_wheel_speed = forwardBias + turn;
  float right_wheel_speed = forwardBias - turn;
  motor.setMotorPower(-left_wheel_speed, right_wheel_speed);
}

void moveMotorForward() {
  float turn = sensor.weighted_measure();
  float left_wheel_speed = forwardBias - turn;
  float right_wheel_speed = forwardBias + turn;
  motor.setMotorPower(-left_wheel_speed, -right_wheel_speed);
}
