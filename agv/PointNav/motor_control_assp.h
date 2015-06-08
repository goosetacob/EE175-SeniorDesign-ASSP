double distance_trav(long ticks){

  //12.5663796 is Circumference of wheel
  return 12.5663706/1533*ticks;

}

void stop_motor() {
    analogWrite(left_motor_pin,0);
    analogWrite(right_motor_pin,0);
}

void soft_stop_motor(int PWM_val) {
    for(int i = PWM_val; i >= 0 ; i--)
   {
      analogWrite(left_motor_pin,i);
      analogWrite(right_motor_pin,i);   
   } 
    analogWrite(left_motor_pin,0);
    analogWrite(right_motor_pin,0);

}


void turn_right(double heading){
  analogWrite(left_motor_pin,base_speed);
  analogWrite(right_motor_pin,0);
  delay((heading*3235)/360);
  stop_motor();


}

void turn_left(double heading){
  analogWrite(right_motor_pin,base_speed);
  analogWrite(left_motor_pin,0);
  delay(500);
  //delay(heading/deg_per_sec_speed_right);
  analogWrite(right_motor_pin,0);

}

void move_foward(double distance){
  //P Calculations





//  Serial.print(distance);
//  Serial.println(distance_trav(delta_right));
  delta_right = 0;
  delta_left = 0;
  encoder0PosRight = 0;
  encoder0PosLeft = 0;
  current_time_motor = millis();
  int_error = 0;
  int counter = 0;
  
    pre_heading = heading;
    set_point_right = sample_heading(pre_heading);
    Serial.print(pre_heading);
    loop_flag = 1;
    while (distance_trav(delta_right) < distance - foward_delay_offset){
      
          heading = sample_heading(pre_heading);
          error_right = set_point_right - heading;
          
//          dt =  int_time_motor - current_time_motor;
//          int_error = (int_error + error_right) * dt /1000;
           kp_adjust = Kp*error_right;
          if( kp_adjust > 4){
              kp_adjust = 4;
           }
           else if ( kp_adjust < -4){
              kp_adjust = -4;
           }    
          motorPWN_right = base_speed + (kp_adjust); //+ (Ki * int_error );
          motorPWN_left = base_speed + (kp_adjust); //+ (Ki * int_error );      

          int_time_motor = millis();
          Serial.print(motorPWN_right);
          Serial.print(" motorPWN_right: ");
          Serial.print(motorPWN_right);
          Serial.print(" motorPWN_left: ");
          Serial.print(motorPWN_left);          
          Serial.print(" Adjustment kp: ");
          Serial.print((Kp*error_right));
          
//          Serial.print(" Adjustment ki: ");
//          Serial.print((Ki * int_error ));
//          
//          Serial.print(" dt: ");
//          Serial.print(( dt));
//  
//          Serial.print(" SUM error: ");
//          Serial.print(int_error );        

          Serial.print(" Heading: ");
          Serial.println(heading, 2);
          //Start Full Power
          if(loop_flag == 1){
               analogWrite(left_motor_pin,255);
               analogWrite(right_motor_pin,255);
               loop_flag = 0;
               delay(1000);
               Serial.print("start");
               digitalWrite(test_pin,HIGH);
               delta_right = 0;
               delta_left = 0;
               encoder0PosRight = 0;
               encoder0PosLeft = 0;
           }
          analogWrite(left_motor_pin,motorPWN_left);
          analogWrite(right_motor_pin,motorPWN_right);
    }
       stop_motor();
       digitalWrite(test_pin,LOW);



}

void path_plan(){
  Serial.println("Start");
  for( int i = 0 ; i < num_points ; i++){
    move_foward(distance[i]);
    turn_right(headings[i]);
    
  }
  Serial.println("END");
  stop_motor();
}

void detect_hit(){
  bumper_state = digitalRead(bumper_pin);
  if( hit_flag == false && bumper_state == 0){
    analogWrite(left_motor_pin,255);
    analogWrite(right_motor_pin,255);
//    digitalWrite(test_pin,LOW);
    //Serial.println("no Hit");
    Serial.println(digitalRead(bumper_pin));
  }
  else  {//( bumper_state == 1 || hit_flag == false ){
    analogWrite(left_motor_pin,0);
    analogWrite(right_motor_pin,0);
    digitalWrite(test_pin,HIGH);
    //Serial.println(1);
   // Serial.println("Hit");
    hit_flag = true; 
     Serial.println(1111111111);  
  }


}

