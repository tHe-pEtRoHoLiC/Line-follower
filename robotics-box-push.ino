
#define EMIT_PIN 11
#define BR 5
#define BL 4
#define RBS 800 //252blacktapped
#define LBS 820

#include "motors.h"
#include "bump_sensor.h"

float misalignment_rate = 0.0;
long startTime = millis();

RoboMotors motor;
BumpSensor sensor;

void setup() {

  sensor.initialise();
  Serial.begin(9600);
  delay(1000);
  Serial.println("****RESET****");
  //  moveMotorForward();
}

void loop() {
  // Move towards the object for detection
  moveMotorForward();
  delay(1000);
  // Make a read of the sensor.
  sensor.readBumpSensor();
  float rbs = sensor.bump_sensor_reading[0];
  float lbs = sensor.bump_sensor_reading[1];
  //  for (int i = 0; i < 2; i++) {
  //    Serial.print(sensor.bump_sensor_reading[i]);
  //    Serial.print(",");
  //  }
  //  Serial.println("");
  sensor.weighted_measure();

  if (sensor.bump_sensor_reading[0] > RBS && sensor.bump_sensor_reading[1] < LBS) {
    misalignment_rate += 1.0;
    moveMotorRight();
  } else if (sensor.bump_sensor_reading[0] < RBS && sensor.bump_sensor_reading[1] > LBS) {
    misalignment_rate += 1.0;
    moveMotorLeft();

  } else if (sensor.bump_sensor_reading[0] >= RBS && sensor.bump_sensor_reading[1] >= LBS) {
    moveMotorForward();
  } else {
    stopMotor();
    delay(1000);
  }
  //Calculate the misalignment
  Serial.print(misalignment_rate);
  Serial.print(",");
  long currentTime = millis();
  float elapsedTime = (currentTime - startTime) / 1000.0;
  Serial.print(elapsedTime);
  Serial.println("");
  delay(100);

}

void stopMotor() {
  motor.setMotorPower(0, 0);
}

void moveMotorForward() {
  float turn = sensor.weighted_measure();
  float left_wheel_speed = forwardBias - turn;
  float right_wheel_speed = forwardBias + turn;
  motor.setMotorPower(left_wheel_speed, right_wheel_speed);
}

void moveMotorBack() {
  float turn = sensor.weighted_measure();
  float left_wheel_speed = forwardBias - turn;
  float right_wheel_speed = forwardBias + turn;
  motor.setMotorPower(-left_wheel_speed, -right_wheel_speed);

}
void moveMotorLeft() {
  float turn = sensor.weighted_measure();
  float left_wheel_speed = forwardBias - turn;
  float right_wheel_speed = forwardBias + turn;
  motor.setMotorPower(-left_wheel_speed, right_wheel_speed);
}

void moveMotorRight() {
  float turn = sensor.weighted_measure();
  float left_wheel_speed = forwardBias - turn;
  float right_wheel_speed = forwardBias + turn;
  motor.setMotorPower(left_wheel_speed, -right_wheel_speed);
}
