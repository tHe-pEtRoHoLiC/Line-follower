#include "motors.h"
#include "line_sensors.h"

#define BUZZER_PIN 6      // Buzzer pin number
#define THRESHOLD 1250    // Threshold value for line sensors
#define TIME_LIMIT 28000  // Time limit for the RUN in milliseconds
LineSensor sensors;
MotorController motors;


// Buzzer beep function
String currentState;
unsigned long startTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("***INITIALIZING***");
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize encoders and sensors
 // initializeLeftEncoder();
  //initializeRightEncoder();
  sensors.setup();
  startTime = millis(); // Timer start
  currentState = "BEGIN";
}

void loop() {
  unsigned long now = millis();
  unsigned long timeElapsed = now - startTime; //elepsed time during the run

  sensors.readAllSensors();

  if (currentState == "BEGIN") {
    driveForward();
    delay(325);  // Temporary delay for initial movement
    if (sensors.sensorReadings[2] > THRESHOLD) {
      motors.driveMotors(0, 0);
      delay(500);
      currentState = "LINE_TRACKING"; //finding starting line
    }
  }

  if (currentState == "LINE_TRACKING") {
    trackLine();
  }

//stopping when lost
  if (currentState == "LINE_SEARCH") {
    if (timeElapsed > TIME_LIMIT && isEndOfLine()) {
      currentState = "STOP";
    } else {
      if (searchLine()) {
        currentState = "LINE_TRACKING";
      }
    }
  }

//Right turn
  if (currentState == "TURN_RIGHT_CORNER") {
    if (navigateRightCorner()) {
      currentState = "LINE_TRACKING";
    }
  }

//navigating T- Junction
  if (currentState == "HANDLE_T_JUNCTION") {
    if (handleTJunction()) {
      currentState = "LINE_TRACKING";
    }
  }

// navigating Y- Junction
  if (currentState == "HANDLE_Y_JUNCTION") {
    if (handleYJunction()) {
      currentState = "LINE_TRACKING";
    }
  }

// Arrival at Destination
  if (currentState == "REACH_DESTINATION") {
    haltMotors();
    delay(900); // delay to denote stoppage
    currentState = "RETURN_HOME";
  }

// turn around and return
  if (currentState == "RETURN_HOME") {
    haltMotors();
    // TODO: Implement return_to_home();
  }

//stopping due to end of time or end of line
  if (currentState == "STOP") {
    haltMotors();
    Serial.println("Robot has halted due to elapsed time and end of line.");
    beepBuzzer();
    while (true) {
      // Infinite loop to keep the robot stopped
    }
  }
}

//Beep at stop
void beepBuzzer() {
  tone(BUZZER_PIN, 1000, 250);  // Beep at 1000Hz for 250ms
}

//Robot at Halt
void haltMotors() {
  motors.driveMotors(0, 0);
}

//Forward movement
void driveForward() {
  float adjustment = sensors.calculateWeightedMeasure();
  float leftSpeed = baseSpeed - adjustment;
  float rightSpeed = baseSpeed + adjustment;
  motors.driveMotors(leftSpeed, rightSpeed);
}

//Left turn
void driveLeft() {
  float adjustment = sensors.calculateWeightedMeasure();
  float leftSpeed = baseSpeed - adjustment;
  float rightSpeed = baseSpeed + adjustment;
  motors.driveMotors(-leftSpeed, rightSpeed);
}

//Right turn
void driveRight() {
  float adjustment = sensors.calculateWeightedMeasure();
  float leftSpeed = baseSpeed - adjustment;
  float rightSpeed = baseSpeed + adjustment;
  motors.driveMotors(leftSpeed, -rightSpeed);
}

//Line tracking
void trackLine() {
  if (sensors.sensorReadings[1] > THRESHOLD && 
      sensors.sensorReadings[2] > THRESHOLD && 
      sensors.sensorReadings[3] > THRESHOLD) {
    currentState = "HANDLE_Y_JUNCTION";
  } else if (sensors.sensorReadings[2] > THRESHOLD) {
    driveForward();
  } else if (sensors.sensorReadings[2] >= THRESHOLD && 
             sensors.sensorReadings[0] >= THRESHOLD && 
             sensors.sensorReadings[4] >= THRESHOLD) {
    currentState = "HANDLE_T_JUNCTION";
  } else if ((sensors.sensorReadings[3] > THRESHOLD && sensors.sensorReadings[4] < THRESHOLD) || 
             (sensors.sensorReadings[1] > THRESHOLD && sensors.sensorReadings[2] < THRESHOLD)) {
    driveLeft();
  } else if (sensors.sensorReadings[4] > THRESHOLD || 
             sensors.sensorReadings[3] > THRESHOLD) {
    currentState = "TURN_RIGHT_CORNER";
  } else if (isEndOfLine()) {
    currentState = "LINE_SEARCH";
  } else {
    Serial.println("****OFF LINE - STATE UNKNOWN*****");
  }
}


bool navigateRightCorner() {
  driveRight();
  if (sensors.sensorReadings[2] > THRESHOLD) {
    return true;
  }
  return false;
}

bool searchLine() {
  driveLeft();
  if (sensors.sensorReadings[2] >= THRESHOLD) {
    return true;
  }
  return false;
}

bool handleTJunction() {
  driveLeft();
  if (sensors.sensorReadings[2] >= THRESHOLD &&
      sensors.sensorReadings[4] <= THRESHOLD &&
      sensors.sensorReadings[0] <= THRESHOLD)
  {
    return true;
  }
  return false;
}

bool handleYJunction() {
  driveRight();
  if (sensors.sensorReadings[2] >= THRESHOLD) {
    return true;
  }
  return false;
}

bool isEndOfLine() {
  return (sensors.sensorReadings[0] <= THRESHOLD &&
          sensors.sensorReadings[1] <= THRESHOLD &&
          sensors.sensorReadings[2] <= THRESHOLD &&
          sensors.sensorReadings[3] <= THRESHOLD &&
          sensors.sensorReadings[4] <= THRESHOLD);
}
