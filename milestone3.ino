// Motor Control Pins
const int MOTOR_LEFT_PWM     = 2;    // Motor B ENABLE pin (PB5)
const int MOTOR_RIGHT_PWM    = 23;   // Motor A ENABLE pin (Pd0)
const int MOTOR_LEFT_IN1     = 3;    // Motor B IN1 pin (Pb0)
const int MOTOR_LEFT_IN2     = 4;    // Motor B IN2 pin (Pb1)
const int MOTOR_RIGHT_IN3    = 24;   // Motor A IN1 pin (Pd1)
const int MOTOR_RIGHT_IN4    = 25;   // Motor A IN2 pin (Pd2)

// Ultrasonic Sensor Pins
const int SENSOR_FRONT_TRIG  = 8;    // Previously Front
const int SENSOR_FRONT_ECHO  = 27;
const int SENSOR_LEFT_TRIG   = 9;    // Previously Left
const int SENSOR_LEFT_ECHO   = 28;
const int SENSOR_RIGHT_TRIG  = 10;   // Previously Right
const int SENSOR_RIGHT_ECHO  = 29;

int SAFE_STOP_DISTANCE = 15;
int SAFE_WALL_DISTANCE = 10;

long distanceFront, distanceLeft, distanceRight;

void setup() {
    Serial.begin(9600);

    pinMode(MOTOR_LEFT_IN1, OUTPUT);
    pinMode(MOTOR_LEFT_IN2, OUTPUT);
    pinMode(MOTOR_RIGHT_IN3, OUTPUT);
    pinMode(MOTOR_RIGHT_IN4, OUTPUT);
    pinMode(MOTOR_LEFT_PWM, OUTPUT);
    pinMode(MOTOR_RIGHT_PWM, OUTPUT);

    pinMode(SENSOR_FRONT_TRIG, OUTPUT);
    pinMode(SENSOR_FRONT_ECHO, INPUT);
    pinMode(SENSOR_LEFT_TRIG, OUTPUT);
    pinMode(SENSOR_LEFT_ECHO, INPUT);
    pinMode(SENSOR_RIGHT_TRIG, OUTPUT);
    pinMode(SENSOR_RIGHT_ECHO, INPUT);
}

void loop() {
    distanceFront = getDistance(SENSOR_FRONT_TRIG, SENSOR_FRONT_ECHO);
    distanceLeft = getDistance(SENSOR_LEFT_TRIG, SENSOR_LEFT_ECHO);
    distanceRight = getDistance(SENSOR_RIGHT_TRIG, SENSOR_RIGHT_ECHO);

    Serial.print("Front: ");
    Serial.print(distanceFront);
    Serial.print(" cm, Left: ");
    Serial.print(distanceLeft);
    Serial.print(" cm, Right: ");
    Serial.print(distanceRight);
    Serial.println(" cm");

    if (distanceFront < SAFE_STOP_DISTANCE) {
        Turn_Right();
    }
    else if (distanceLeft < SAFE_WALL_DISTANCE) {
        Correct_Right();
    }
    else if (distanceRight < SAFE_WALL_DISTANCE) {
        Correct_Left();
    }
    else {
        Move_Forward();
    }

    delay(5);
}

void Turn_Right() {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
    digitalWrite(MOTOR_RIGHT_IN3, LOW);
    digitalWrite(MOTOR_RIGHT_IN4, HIGH);
    analogWrite(MOTOR_LEFT_PWM, 200);
    analogWrite(MOTOR_RIGHT_PWM, 50);
    delay(900);
}

void Correct_Right() {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
    digitalWrite(MOTOR_RIGHT_IN3, LOW);
    digitalWrite(MOTOR_RIGHT_IN4, HIGH);
    analogWrite(MOTOR_LEFT_PWM, 180);
    analogWrite(MOTOR_RIGHT_PWM, 130);
}

void Correct_Left() {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
    digitalWrite(MOTOR_RIGHT_IN3, LOW);
    digitalWrite(MOTOR_RIGHT_IN4, HIGH);
    analogWrite(MOTOR_LEFT_PWM, 130);
    analogWrite(MOTOR_RIGHT_PWM, 180);
}

void Move_Forward() {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
    digitalWrite(MOTOR_RIGHT_IN3, LOW);
    digitalWrite(MOTOR_RIGHT_IN4, HIGH);
    analogWrite(MOTOR_LEFT_PWM, 150);
    analogWrite(MOTOR_RIGHT_PWM, 150);
}

void Stop_Motors() {
    analogWrite(MOTOR_LEFT_PWM, 0);
    analogWrite(MOTOR_RIGHT_PWM, 0);
}

long microsecondsToCentimeters(long microseconds) {
    return (microseconds * 0.0343) / 2;
}

long getDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 10000);
    long distance = microsecondsToCentimeters(duration);

    return distance;
}
