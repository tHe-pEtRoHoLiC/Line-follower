#define EMITTER_PIN 11
#define SENSOR_LEFT_PIN 12     // left sensor pin
#define SENSOR_MIDLEFT_PIN A0  // middle-left sensor pin 
#define SENSOR_MIDDLE_PIN A2   // middle sensor pin 
#define SENSOR_MIDRIGHT_PIN A3 // middle-right sensor pin 
#define SENSOR_RIGHT_PIN A4    // right sensor pin 

// Array for Line sensor pins
int sensorPins[5] = {
  SENSOR_LEFT_PIN,
  SENSOR_MIDLEFT_PIN,
  SENSOR_MIDDLE_PIN,
  SENSOR_MIDRIGHT_PIN,
  SENSOR_RIGHT_PIN
};

#define SENSOR_COUNT 5 // total number of line sensors


float leftWeight = 0, rightWeight = 0, lineError = 0, controlOutput = 0;
float baseSpeed = 30;

class LineSensor {

  public:
    int sensorPinArray[SENSOR_COUNT] = {SENSOR_LEFT_PIN, SENSOR_MIDLEFT_PIN, SENSOR_MIDDLE_PIN, SENSOR_MIDRIGHT_PIN, SENSOR_RIGHT_PIN};
    int sensorReadings[SENSOR_COUNT];
    unsigned long sensorEndTimes[SENSOR_COUNT];
    float leftMotorSpeed;
    float rightMotorSpeed;
    String currentRobotState = "UNKNOWN";

    LineSensor() {}

    void setup() {
      pinMode(EMITTER_PIN, INPUT);
      digitalWrite(EMITTER_PIN, HIGH);
      turnEmitterOff();
    }

    void turnEmitterOn() {
      pinMode(EMITTER_PIN, OUTPUT);
      digitalWrite(EMITTER_PIN, HIGH);
    }

    void turnEmitterOff() {
      pinMode(EMITTER_PIN, INPUT);
    }

//Read each line sensor
    float readIndividualSensor(int index) {
      if (index < 0 || index >= SENSOR_COUNT) {
        Serial.println("****ERROR: Invalid Sensor Index*****");
        return 0;
      }

      pinMode(sensorPins[index], INPUT);
      turnEmitterOn();
      pinMode(sensorPins[index], OUTPUT);
      digitalWrite(sensorPins[index], HIGH);

      delayMicroseconds(10);
      pinMode(sensorPins[index], INPUT);

      unsigned long startTime = micros();

      while (digitalRead(sensorPins[index]) == HIGH) {
        // Wait until the pin reads LOW
      }

      unsigned long endTime = micros();
      turnEmitterOff();
      unsigned long duration = endTime - startTime;
      return static_cast<float>(duration);
    }

//Read all the line sensors
    void readAllSensors() {
      bool allSensorsDone = false;
      turnEmitterOn();

      for (int i = 0; i < SENSOR_COUNT; i++) {
        pinMode(sensorPinArray[i], OUTPUT);
        digitalWrite(sensorPinArray[i], HIGH);
      }

      delayMicroseconds(10);

      for (int i = 0; i < SENSOR_COUNT; i++) {
        sensorEndTimes[i] = 0; // Initialize sensorEndTimes
      }

      for (int i = 0; i < SENSOR_COUNT; i++) {
        pinMode(sensorPinArray[i], INPUT);
      }

      unsigned long startTime = micros();
      int remainingSensors = SENSOR_COUNT;

      while (!allSensorsDone) {
        for (int i = 0; i < SENSOR_COUNT; i++) {
          if (sensorEndTimes[i] == 0 && digitalRead(sensorPinArray[i]) == LOW) {
            sensorEndTimes[i] = micros();
            remainingSensors--;
          }
        }
        if (remainingSensors == 0) {
          allSensorsDone = true;
        }
      }

      for (int i = 0; i < SENSOR_COUNT; i++) {
        unsigned long elapsedTime = sensorEndTimes[i] - startTime;
        sensorReadings[i] = static_cast<int>(elapsedTime);
      }

      turnEmitterOff();
    }

// Weighted measurement calculation for Line navigation
    float calculateWeightedMeasure() {
      float leftSensor, middleSensor, rightSensor, totalValue;

      leftSensor = sensorReadings[1];
      middleSensor = sensorReadings[2];
      rightSensor = sensorReadings[3];

      totalValue = leftSensor + middleSensor + rightSensor;
      leftSensor /= totalValue;
      middleSensor /= totalValue;
      rightSensor /= totalValue;

      leftWeight = leftSensor + (middleSensor * 0.5);
      rightWeight = rightSensor + (middleSensor * 0.5);

      lineError = leftWeight - rightWeight;

      float proportionalGain = 1.0;
      controlOutput = lineError * proportionalGain;

      float maxTurnValue = 50;

      maxTurnValue *= controlOutput;
      Serial.print("maxTurnValue: ");
      Serial.println(maxTurnValue);

      return maxTurnValue;
    }
};
