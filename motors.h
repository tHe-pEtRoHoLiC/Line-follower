# define R_PWM_PIN 9
# define L_PWM_PIN 10
# define R_DIR_PIN 15
# define L_DIR_PIN 16

// REVERSE DIRECTION

# define FWD LOW
# define REV HIGH

class RoboMotors {
  public:

    RoboMotors() {
      //setup pin mode for left and right motors
      pinMode(L_PWM_PIN, OUTPUT);
      pinMode(L_DIR_PIN, OUTPUT);
      pinMode(R_PWM_PIN, OUTPUT);
      pinMode(R_DIR_PIN, OUTPUT);

      // setup the direction
      digitalWrite(L_DIR_PIN, FWD);
      digitalWrite(R_DIR_PIN, FWD);
    }

    void setMotorPower(float left_pwm, float right_pwm) {
      if (left_pwm >= 0) {
        digitalWrite(L_DIR_PIN, REV);

      } else if ( left_pwm < 0) {
        digitalWrite(L_DIR_PIN, FWD);
        left_pwm = abs(left_pwm);
      }

      if (right_pwm >= 0) {
        digitalWrite(R_DIR_PIN, REV);

      } else if ( right_pwm < 0) {
        digitalWrite(R_DIR_PIN, FWD);
        right_pwm = abs(right_pwm);
      }
      analogWrite(L_PWM_PIN, left_pwm);
      analogWrite(R_PWM_PIN, right_pwm);

    }
};
