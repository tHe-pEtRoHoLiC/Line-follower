#define RIGHT_PWM_PIN 9
#define LEFT_PWM_PIN 10
#define RIGHT_DIR_PIN 15
#define LEFT_DIR_PIN 16

#define FORWARD LOW
#define REVERSE HIGH

class MotorController {
  public:
    MotorController() {
      // Configure the pin modes for the motor controls
      pinMode(LEFT_PWM_PIN, OUTPUT);
      pinMode(LEFT_DIR_PIN, OUTPUT);
      pinMode(RIGHT_PWM_PIN, OUTPUT);
      pinMode(RIGHT_DIR_PIN, OUTPUT);

      // Set initial direction to forward
      digitalWrite(LEFT_DIR_PIN, FORWARD);
      digitalWrite(RIGHT_DIR_PIN, FORWARD);
    }

    void driveMotors(float leftSpeed, float rightSpeed) {
      // Determine direction for the left motor
      if (leftSpeed >= 0) {digitalWrite(LEFT_DIR_PIN, FORWARD);}
          else {digitalWrite(LEFT_DIR_PIN, REVERSE);
             leftSpeed = abs(leftSpeed); //absolute PWM
      }

      // Find direction for the right motor 
      if (rightSpeed >= 0) {
        digitalWrite(RIGHT_DIR_PIN, FORWARD);
      } else {
        digitalWrite(RIGHT_DIR_PIN, REVERSE);
        rightSpeed = abs(rightSpeed); // Use absolute value for PWM
      }

      //Set PWM for left and right motors
      analogWrite(LEFT_PWM_PIN, constrain(leftSpeed, 0, 255));
      analogWrite(RIGHT_PWM_PIN, constrain(rightSpeed, 0, 255));
    }
};
