void tick_speed(){
   current_time = millis();
   if ( current_time - int_time >= sample_time){
     //Get time interval
     sum_tick_left = delta_left - initial_tick_left;
     sum_tick_right = delta_right - initial_tick_right;

     // Get Tics/ms
     left_tick_sec = sum_tick_left/(current_time - int_time);
     right_tick_sec = sum_tick_right/(current_time - int_time);
      Serial.println(current_time - int_time);
     //Redifne Intervals
     initial_tick_left = delta_left;
     initial_tick_right = delta_right;
     int_time = millis();
   }
}

void doEncoderA(){

  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinLeft_int) == HIGH) {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinLeft) == LOW) {
      encoder0PosLeft = encoder0PosLeft - 1;         // CW
      delta_left = delta_left + 1;
    }
    else {
      encoder0PosLeft = encoder0PosLeft + 1;         // CCW
      delta_left = delta_left + 1;
    }
  }
  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinLeft) == HIGH) {
      encoder0PosLeft = encoder0PosLeft - 1;          // CW
      delta_left = delta_left + 1;
    }
    else {
      encoder0PosLeft = encoder0PosLeft + 1;          // CCW
      delta_left = delta_left + 1;
    }
  }
  //Serial.println ('interupt');
}

void doEncoderB(){

  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinRight_int) == HIGH) {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinRight) == LOW) {
      encoder0PosRight = encoder0PosRight + 1;         // CW
      delta_right = delta_right + 1;
    }
    else {
      encoder0PosRight = encoder0PosRight- 1;         // CCW
      delta_right = delta_right + 1;
    }
  }
  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinRight) == HIGH) {
      encoder0PosRight = encoder0PosRight + 1;          // CW
      delta_right = delta_right + 1;
    }
    else {
      encoder0PosRight = encoder0PosRight - 1;          // CCW
      delta_right = delta_right + 1;
    }
  }





}
