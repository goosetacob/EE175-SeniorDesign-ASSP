#define leftMotor A0
#define rightMotor A1
#define encoder0PinA 2
#define encoder0PinB 3

int pin = 13;

volatile int state = LOW;
volatile int encoder_tick = 0;
volatile signed int encoder0Pos = 0;

void setup() {
  pinMode (encoder0PinA,INPUT);
  pinMode (encoder0PinB,INPUT);
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  attachInterrupt(0, doEncoderA, CHANGE);
}

void moveForward() {
  analogWrite(leftMotor, 255);
  analogWrite(rightMotor, 255);
  delay(500);
}

void moveBackword() {
  
}

void stopMoving() {
  analogWrite(leftMotor, 0);
  analogWrite(rightMotor, 0); 
  delay(500);
}

void turnLeft() {
  analogWrite(leftMotor, 255);
  analogWrite(rightMotor, 0);
  delay(500);
}

void turnRight() {
  analogWrite(leftMotor, 0);
  analogWrite(rightMotor, 255);
  delay(500);
}

void Encoder()
{
  state = !state;
  encoder_tick++;
}

void doEncoderA(){

  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {  
      encoder0Pos = encoder0Pos + 1;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinB) == HIGH) {   
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
  //Serial.println (encoder0Pos, DEC);          
  // use for debugging - remember to comment out
}

void loop() {
  moveForward();
  stopMoving();
  turnRight();
  stopMoving();
  turnLeft();
  stopMoving();
  
  //Serial.println(encoder0Pos);
  
}
