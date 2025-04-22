// ----- Motor Control Pins -----
const int LEFT_PWM  = 4;   // PWM pin for left motor speed control
const int RIGHT_PWM = 14;  // PWM pin for right motor speed control
const int LEFT_IN1  = 5;   // Direction control pin 1 for left motor
const int LEFT_IN2  = 6;   // Direction control pin 2 for left motor
const int RIGHT_IN3 = 13;  // Direction control pin 1 for right motor
const int RIGHT_IN4 = 12;  // Direction control pin 2 for right motor

// ----- Ultrasonic Sensor Pins -----
const int trigFront = 9, echoFront = 28;  // Front sensor: trigger and echo
const int trigLeft = 8, echoLeft = 27;    // Left sensor: trigger and echo
const int trigRight = 10, echoRight = 29; // Right sensor: trigger and echo

// ----- Variables -----
long cmFront, cmLeft, cmRight; // Distance measurements from ultrasonic sensors

// Movement and detection configuration
int straight_speed = 200;       // Speed when moving straight
int stop_distance = 30;         // Distance to stop if obstacle is detected in front
int turn_speed2 = 200;          // Speed during hard turn
int reverse_speed = 100;        // Speed if reversing is needed (not used in current code)
int turn_speed = 140;           // Speed for adjustment turns
int correction_speed = 85;      // Lower speed for smoother adjustments
int wall_distance = 7;          // Distance from wall to start correcting direction

bool carStarted = false;        // Flag to check if car should be running

void setup() {
  Serial.begin(115200);         // Serial monitor for debugging
  Serial3.begin(9600);          // Bluetooth module (HC-05) on Serial3

  // Set motor control pins as outputs
  pinMode(LEFT_IN1, OUTPUT); 
  pinMode(LEFT_IN2, OUTPUT); 
  pinMode(RIGHT_IN3, OUTPUT); 
  pinMode(RIGHT_IN4, OUTPUT); 
  pinMode(LEFT_PWM, OUTPUT); 
  pinMode(RIGHT_PWM, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);

  Serial.println("Waiting for Bluetooth commands: 's' = start, 'x' = stop");
}

void loop() {
  // --- Bluetooth Command Handling ---
  if (Serial3.available() > 0) {
    char colorDetect = Serial3.read();  // Read single character command
    Serial.print("Received: "); Serial.println(colorDetect);

    if (colorDetect == 's') {
      carStarted = true;  // Start the car
      Serial.println("Start command received");
    } 
    else if (colorDetect == 'x') {
      carStarted = false; // Stop the car
      Stop_Motors();
      Serial.println("Stop command received");
    }
  }

  // --- If car is not started, keep motors off ---
  if (!carStarted) {
    Stop_Motors();
    return;
  }

  // --- Read distance from sensors ---
  cmRight = getDistance(trigFront, echoFront); // Misnamed: this is actually front sensor
  cmFront = getDistance(trigLeft, echoLeft);   // Misnamed: this is actually left sensor
  cmLeft  = getDistance(trigRight, echoRight); // Misnamed: this is actually right sensor

  // Debug print of distances
  Serial.print("Right: "); Serial.println(cmRight);
  Serial.print("Left: "); Serial.println(cmLeft);
  Serial.print("Front: "); Serial.println(cmFront);

  // --- Movement Logic based on distance readings ---
  if (cmFront > 0 && cmFront < stop_distance) {
    Move_Right();  // Turn to avoid obstacle in front
  } else if (cmLeft > 0 && cmLeft < wall_distance) {
    Adjust_Left(); // Getting too close to left wall
  } else if (cmRight > 0 && cmRight < wall_distance) {
    Adjust_Right(); // Getting too close to right wall
  } else {
    Move_Forward(); // Safe to keep moving forward
  }

  delay(4); // Small delay for smoother performance
}

// ----- Movement Functions -----
void Move_Right() { 
  // Perform a hard right turn
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW); 
  digitalWrite(RIGHT_IN3, HIGH); digitalWrite(RIGHT_IN4, LOW);
  analogWrite(LEFT_PWM, turn_speed2);  
  analogWrite(RIGHT_PWM, 45);  
  delay(755); // Delay for turning approx 90 degrees
}

void Adjust_Right() { 
  // Slight turn to left (right side too close)
  Serial.println(" adjust right");
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW); 
  digitalWrite(RIGHT_IN3, HIGH); digitalWrite(RIGHT_IN4, LOW); 
  analogWrite(LEFT_PWM, turn_speed);  
  analogWrite(RIGHT_PWM, correction_speed); 
}

void Adjust_Left() { 
  // Slight turn to right (left side too close)
  Serial.println(" adjust left");
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW); 
  digitalWrite(RIGHT_IN3, HIGH); digitalWrite(RIGHT_IN4, LOW);  
  analogWrite(LEFT_PWM, correction_speed);  
  analogWrite(RIGHT_PWM, turn_speed);  
}

void Move_Forward() { 
  // Move both wheels forward
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW); 
  digitalWrite(RIGHT_IN3, HIGH); digitalWrite(RIGHT_IN4, LOW); 
  analogWrite(LEFT_PWM, 90);  
  analogWrite(RIGHT_PWM, 100); 
}

void Stop_Motors() { 
  // Set motor speeds to zero
  analogWrite(LEFT_PWM, 0);  
  analogWrite(RIGHT_PWM, 0); 
}

// ----- Distance Calculation -----
long microsecondsToCentimeters(long microseconds) {
  // Convert echo time to distance in cm
  return (microseconds * 0.0343) / 2;
}

long getDistance(int trigPin, int echoPin) {
  // Trigger ultrasonic pulse and read the distance
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 10000); // 10ms timeout
  long distance = microsecondsToCentimeters(duration);
  return (distance > 1) ? distance : 0; // Ignore invalid (very short) readings
}
