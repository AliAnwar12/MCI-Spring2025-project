// Define motor control pins
#define EnA 23  // PD3 (PWM for Left Motor)
#define EnB 2   // PD2 (PWM for Right Motor)
#define IN1 24  // PB0 (Left Motor Forward)
#define IN2 25  // PB1 (Left Motor Backward)
#define IN3 3   // PB2 (Right Motor Forward)
#define IN4 4   // PB3 (Right Motor Backward)

void setup() {
  Serial.begin(9600); // 
  
  // Set motor control pins as OUTPUT
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Set motor speed control as OUTPUT
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  
  // Set motors to full speed
  analogWrite(EnA, 255);
  analogWrite(EnB, 255);
}

void loop() {
  // Move forward for 3 seconds
  Serial.println("Moving Forward..."); //
  Move_Forward();
  delay(3000); // 

  // Stop briefly before turning
  Serial.println("Stopping...");
  Motor_Stop();
  delay(500);

  // Turn left 90 degrees
  Serial.println("Turning Left 90°...");
  Turn_Left_90();
  delay(600);  // 

  // Stop after turn
  Serial.println("Turn Completed. Robot Stopped.");
  Motor_Stop();
  while (1); // 
}

// Function to move the robot forward
void Move_Forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to turn left 90°
void Turn_Left_90() {
  digitalWrite(IN1, LOW);  // Left Motor Backward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); // Right Motor Forward
  digitalWrite(IN4, LOW);
  delay(600);  //
}

// Function to stop the motors
void Motor_Stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
