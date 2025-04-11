// ----- Motor Control Pins -----
const int LEFT_PWM  = 4;
const int RIGHT_PWM = 14;
const int LEFT_IN1  = 5;
const int LEFT_IN2  = 6;
const int RIGHT_IN3 = 13;
const int RIGHT_IN4 = 12;

// ----- Ultrasonic Sensor Pins -----
const int trigFront = 9, echoFront = 28;
const int trigLeft = 8, echoLeft = 27;
const int trigRight = 10, echoRight = 29;

// ----- Variables -----
long cmFront, cmLeft, cmRight;

int straight_speed = 255;
int stop_distance = 30;
int turn_speed2 = 200;
int reverse_speed = 100;
int turn_speed = 180;
int correction_speed = 130;
int wall_distance = 7;

bool carStarted = false;

void setup() {
  Serial.begin(115200);         // USB Serial 
  Serial3.begin(9600);          // Bluetooth HC-05 via UART3

  pinMode(LEFT_IN1, OUTPUT); 
  pinMode(LEFT_IN2, OUTPUT); 
  pinMode(RIGHT_IN3, OUTPUT); 
  pinMode(RIGHT_IN4, OUTPUT); 
  pinMode(LEFT_PWM, OUTPUT); 
  pinMode(RIGHT_PWM, OUTPUT);

  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);

  Serial.println("Waiting for Bluetooth commands: 's' = start, 'x' = stop");
}

void loop() {
  // --- Read Bluetooth Single Character Command ---
  if (Serial3.available() > 0) {
    char colorDetect = Serial3.read();
    Serial.print("Received: "); Serial.println(colorDetect);

    if (colorDetect == 's') {
      carStarted = true;
      Serial.println("Start command received");
    } 
    else if (colorDetect == 'x') {
      carStarted = false;
      Stop_Motors();
      Serial.println(" Stop command received");
    }
  }

  // --- Check if car is started ---
  if (!carStarted) {
    Stop_Motors();
    return;
  }

  // --- Distance Readings ---
  cmRight = getDistance(trigFront, echoFront);
  cmFront = getDistance(trigLeft, echoLeft);
  cmLeft  = getDistance(trigRight, echoRight);

  Serial.print("Right: "); Serial.println(cmRight);
  Serial.print("Left: "); Serial.println(cmLeft);
  Serial.print("Front: "); Serial.println(cmFront);

  if (cmFront > 0 && cmFront < stop_distance) {
    Move_Right();
  } else if (cmLeft > 0 && cmLeft < wall_distance) {
    Adjust_Left();
  } else if (cmRight > 0 && cmRight < wall_distance) {
    Adjust_Right();
  } else {
    Move_Forward();
  }

  delay(4);
}

// ----- Movement Functions -----
void Move_Right() { 
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW); 
  digitalWrite(RIGHT_IN3, HIGH); digitalWrite(RIGHT_IN4, LOW);
  analogWrite(LEFT_PWM, turn_speed2);  
  analogWrite(RIGHT_PWM, 0);  
  delay(770);  //perf 90 turn
}
void Adjust_Right() { 
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW); 
  digitalWrite(RIGHT_IN3, HIGH); digitalWrite(RIGHT_IN4, LOW); 
  analogWrite(LEFT_PWM, turn_speed);  
  analogWrite(RIGHT_PWM, correction_speed); 
}
void Adjust_Left() { 
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW); 
  digitalWrite(RIGHT_IN3, HIGH); digitalWrite(RIGHT_IN4, LOW);  
  analogWrite(LEFT_PWM, correction_speed);  
  analogWrite(RIGHT_PWM, turn_speed);  
}
void Move_Forward() { 
  digitalWrite(LEFT_IN1, HIGH); digitalWrite(LEFT_IN2, LOW); 
  digitalWrite(RIGHT_IN3, HIGH); digitalWrite(RIGHT_IN4, LOW); 
  analogWrite(LEFT_PWM, 150);  
  analogWrite(RIGHT_PWM, 170); 
}
void Stop_Motors() { 
  analogWrite(LEFT_PWM, 0);  
  analogWrite(RIGHT_PWM, 0); 
}

// ----- Distance Calculation -----
long microsecondsToCentimeters(long microseconds) {
  return (microseconds * 0.0343) / 2;
}
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 10000);
  long distance = microsecondsToCentimeters(duration);
  return (distance > 1) ? distance : 0;
}
