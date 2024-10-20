#include <Servo.h>
#include <PID_v1_bc.h>

// Pin Definitions
const int trigPin = 5;   // Ultrasonic sensor trigger pin
const int echoPin = 6;   // Ultrasonic sensor echo pin
const int servoPin = 9;  // Servo motor pin

// Ultrasonic Sensor Variables
long duration;
int distance;  // Measured distance from the ball (in meters)
const float beamLength = 25;  // Length of the beam (in meters)

// PID control variables
double setpoint, ballPosition, servoOutput;
double kp = 3.0, ki =  1.5, kd = 0.5;  // PID constants (tune these)

// Create Servo object and PID controller
Servo servoMotor;
PID myPID(&ballPosition, &servoOutput, &setpoint, kp, ki, kd, DIRECT);

// Function to measure distance using ultrasonic sensor
float measureDistance() {
  // Trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the echo time
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in meters (speed of sound is ~343 m/s)
  distance = (duration*0.034)/2;
  

}

void setup() {
  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach the servo motor
  servoMotor.attach(servoPin);

  // Set initial servo position (middle)
  servoMotor.write(0);

  // Set the desired position (setpoint) in meters
  setpoint = beamLength / 2;  // Target is to keep the ball at the center of the beam

  // Initialize the PID controller
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 180);  // Servo can move between 0 and 180 degrees

  Serial.begin(9600);  // For debugging
}

void loop() {
  // Measure the ball's position using the ultrasonic sensor
  ballPosition = measureDistance();
  
  // Compute the PID control signal
  myPID.Compute();
  
  // Map the PID output (0-180) to servo position and move the servo
  servoMotor.write(servoOutput);
  
  // Debugging: Print the current distance and servo position
  Serial.print("Ball Position: ");
  Serial.print(ballPosition);
  Serial.print(" m, Servo Output: ");
  Serial.println(servoOutput);

  // Short delay for smoother operation
  delay(50);
}
