long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void Right_sonic(){
  pinMode(right_echo_pin, OUTPUT);// attach pin 4 to Trig
  digitalWrite(right_echo_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(right_echo_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(right_echo_pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode (right_trig_pin, INPUT);//attach pin 5 to Echo
//  right_micros()
//  if(digitalRead(right_trig_pin) == High)
  duration_right = pulseIn(right_trig_pin, HIGH);
  inches_right = microsecondsToInches(duration_right);

//  Serial.print(" Left:");
//  Serial.print(inches_left);
//  Serial.print("in, ");
//  Serial.print("|||");
//  Serial.print(" Center:");
//  Serial.print(inches_cent);
//  Serial.print("in, ");
//  Serial.print("|||");
  Serial.print(" Right:");
  Serial.print(inches_right);
  Serial.print("in, ");
//  Serial.print("|||");
//  Serial.print(" Enocder TestA: ");
//  Serial.print(encoder0PosLeft);
//  Serial.print("|||");
//   Serial.print(" Enocder TestB: ");
//  Serial.print(encoder0PosRight);
//  Serial.println();


}

void Left_sonic(){
  pinMode(left_echo_pin, OUTPUT);// attach pin 4 to Trig
  digitalWrite(left_echo_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(left_echo_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(left_echo_pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode (left_trig_pin, INPUT);//attach pin 5 to Echo
  duration_left = pulseIn(left_trig_pin, HIGH);
  inches_left = microsecondsToInches(duration_left);

//  Serial.print(" Left:");
//  Serial.print(inches_left);
//  Serial.print("in, ");
//  Serial.print("|||");
//  Serial.print(" Center:");
//  Serial.print(inches_cent);
//  Serial.print("in, ");
//  Serial.print("|||");
//  Serial.print(" Right:");
//  Serial.print(inches_right);
//  Serial.print("in, ");
//  Serial.print("|||");
//   Serial.print(" Enocder TestA: ");
//  Serial.print(encoder0PosLeft);
//  Serial.print("|||");
//   Serial.print(" Enocder TestB: ");
//  Serial.print(encoder0PosRight);
//  Serial.println();



}

void Cent_sonic()
{
  pinMode(cent_echo_pin, OUTPUT);// attach pin 4 to Trig
  digitalWrite(cent_echo_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(cent_echo_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(cent_echo_pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode (cent_trig_pin, INPUT);//attach pin 5 to Echo
//  right_micros()
//  if(digitalRead(right_trig_pin) == High)
  duration_cent = pulseIn(cent_trig_pin, HIGH);
  inches_cent = microsecondsToInches(duration_cent);
//
//  Serial.print(" Left:");
//  Serial.print(inches_left);
//  Serial.print("in, ");
//  Serial.print("|||");
  Serial.print(" Center:");
  Serial.print(inches_cent);
  Serial.print("in, ");
//  Serial.print("|||");
//  Serial.print(" Right:");
//  Serial.print(inches_right);
//  Serial.print("in, ");
//  Serial.print("|||");
//  Serial.print(" Enocder TestA: ");
//  Serial.print(encoder0PosLeft);
//  Serial.print("|||");
//   Serial.print(" Enocder TestB: ");
//  Serial.print(encoder0PosRight);
//  Serial.println();



}
