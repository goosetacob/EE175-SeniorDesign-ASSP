//#include <motor_control.h>
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h>
#include <SFE_LSM9DS0.h>
#include <TaskScheduler.h>


#include "define_assp.h"


#include "imu_assp.h"
#include "encoder_assp.h"



#include "motor_control_assp.h"
#include "sonic_assp.h"





void setup()
{
  //----------------------IMU Set Up -------------------
    Serial.begin(9600); // Start serial at 115200 bps
  // Use the begin() function to initialize the LSM9DS0 library.
  // You can either call it with no parameters (the easy way):
  uint16_t status = dof.begin();
  // Or call it with declarations for sensor scales and data rates:  
  //uint16_t status = dof.begin(dof.G_SCALE_2000DPS, 
  //                            dof.A_SCALE_6G, dof.M_SCALE_2GS);
  
  // begin() returns a 16-bit value which includes both the gyro 
  // and accelerometers WHO_AM_I response. You can check this to
  // make sure communication was successful.
  Serial.print("LSM9DS0 WHO_AM_I's returned: 0x");
  Serial.println(status, HEX);
  Serial.println("Should be 0x49D4");
  Serial.println();
  //------------------End - IMU Set Up -------------------
  
  // Attatch Encoder Interupts
  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);
  //Define Input and Outputs
  pinMode (encoder0PinLeft_int,INPUT);
  pinMode (encoder0PinLeft,INPUT);
  pinMode (encoder0PinRight_int,INPUT);
  pinMode (encoder0PinRight,INPUT);
  pinMode(left_motor_pin, OUTPUT);
  pinMode(right_motor_pin, OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(bumper_pin,OUTPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  // Intialize Foward Movement for Motors
  digitalWrite(right_enable_A,LOW);
  digitalWrite(right_enable_B,HIGH);
  digitalWrite(left_enable_A,LOW);
  digitalWrite(left_enable_B,HIGH);
//  Sch.init();  // Initialize task scheduler
//
//    /*
//     * use Sch.addTask(task, start_time, period, priority) to add tasks
//     * task - tasks to be scheduled
//     * start_time - when the task starts (ms)
//     * period - repeat period of the task (ms)
//     * priority - 1: mormal priority, 0: high priority
//     */
//    //Sch.addTask(Cent_sonic,0,1,1);
//    //Sch.addTask(Left_sonic,0,1,1);  //从第 0 毫秒开始闪烁 LED，每隔 1s, LED 状态改变一次
//    //Sch.addTask(Right_sonic,0,1,1);  //从第 0 毫秒开始闪烁 LED，每隔 1s, LED 状态改变一次
//    Sch.addTask(Motor_tick,0,1,1);  //从第 0 毫秒开始闪烁 LED，每隔 1s, LED 状态改变一次
//    Sch.start();  // Start the task scheduler
//    pre_heading = get_heading();
//    analogWrite(left_motor_pin,255);
//    analogWrite(right_motor_pin,255);
}

void loop()
{
//     Sch.dispatchTasks();
//  path_plan();
//  heading = get_heading();
//  Serial.println(heading);
//  delay(10000);

//  Serial.print(digitalRead(A7));
//  Serial.println();
    detect_hit();
//Serial.println(1111111111);
   
}

enum object_avoid_states {move_foward_state, stop_state_one, turn_left_one, turn_stop_one, move_left_one, move_past_one, move_past_stop_one, turn_right_one, turn_stop_two, move_foward_delay,  move_foward_state_two, move_past_two, move_past_stop_two, turn_right_two, turn_stop_three, move_right_one, move_right_stop_one,  turn_left_two, turn_stop_four  } object_avoid_state;

void Motor_tick() {
  switch(object_avoid_state) { // Transitions
      case -1:
          object_avoid_state = move_foward_state;
          break;
      case move_foward_state:
          Cent_sonic();
          if(inches_cent < 2)
          {
            inches_cent = 20;
          }
          if( inches_cent < 11){
            object_avoid_state = stop_state_one;
            current_stop_time = millis();
            Serial.println("inches Centtran: ");
            Serial.println(inches_cent);
          }
          else{
            object_avoid_state = move_foward_state;
            Serial.println("inches Cent: ");
            Serial.println(inches_cent);
          }
          break;
          
      case stop_state_one:
          if( millis() - current_stop_time > stop_interval){
            object_avoid_state = turn_left_one;
             current_turn_time_left = millis(); 
          }
          else{
            object_avoid_state = stop_state_one;
          }
          break;
          
      case turn_left_one:
          if( millis() - current_turn_time_left > turn_interval){
            object_avoid_state = turn_stop_one;
             current_stop_time = millis();
          }
          else{
            object_avoid_state = turn_left_one;
          }
          break;
          
       case turn_stop_one:
          if( millis() - current_stop_time > stop_interval){
            object_avoid_state = move_left_one;
             move_foward_interval_start = millis(); 
          }
          else{
            object_avoid_state = turn_stop_one;
          }
          break;
          
      case move_left_one:
          Right_sonic();
          if( inches_right > 12){
            object_avoid_state = move_past_one;
            current_move_past_time = millis();          
          }
          else{
            object_avoid_state = move_left_one;
          }
          break;
          
      case move_past_one:
            
          if( millis() - current_move_past_time > move_past_interval){
            object_avoid_state = move_past_stop_one;
            move_foward_interval = millis() - move_foward_interval_start;
            current_stop_time = millis();
            Serial.print("Millis: ");
            Serial.print(millis());
            Serial.print("Move past interval: ");
            Serial.print( current_move_past_time);
            Serial.print("Current Move Past Time: ") ;           
            Serial.print(move_past_interval);
          }
          else{
            object_avoid_state = move_past_one;
          }
          break;
           
       case move_past_stop_one:
          if( millis() - current_stop_time > stop_interval){
            object_avoid_state = turn_right_one;
            
            current_turn_time_right = millis(); 
          }
          else{
            object_avoid_state = move_past_stop_one;
          }
          break;
          
      case turn_right_one:
          if( millis() - current_turn_time_right > turn_interval){
            object_avoid_state = turn_stop_two;
            current_stop_time = millis();
          }
          else{
            object_avoid_state = turn_right_one;
          }
          break;
          
      case turn_stop_two:
          if( millis() - current_stop_time > stop_interval){
            object_avoid_state =  move_foward_delay;
             current_turn_time_left = millis(); 
          }
          else{
            object_avoid_state = turn_stop_two;
          }
          break;
          
       case move_foward_delay:
          if( millis() - current_turn_time_left> 750){
            object_avoid_state =  move_foward_state_two;
             current_turn_time_left = millis(); 
             move_foward_interval_start = millis();
          }
          else{
            object_avoid_state = move_foward_delay;
          }
          break; 
          
      case move_foward_state_two:
          Right_sonic();
          if( inches_right > 10){
            object_avoid_state = move_past_two;
            current_move_past_time = millis();
          }
          else{
            object_avoid_state = move_foward_state_two;
          }
          break;   
          
       case move_past_two:
          if( millis() - current_move_past_time > move_past_interval){
            Serial.print("Millis: ");
            Serial.print(millis());
            Serial.print("Move past interval: ");
            Serial.print( move_past_interval);
            Serial.print("Current Move Past Time: ") ;           
            Serial.print(current_move_past_time);
            object_avoid_state = move_past_stop_two;
            current_stop_time = millis();
          }
          else{
            object_avoid_state = move_past_two;
          }
          break; 
          
       case move_past_stop_two:
          if( millis() - current_stop_time > stop_interval){
            object_avoid_state = turn_right_two;
            current_turn_time_right = millis(); 
          }
          else{
            object_avoid_state = move_past_stop_two;
          }
          break;
        
       case turn_right_two:
          if( millis() - current_turn_time_right > turn_interval){
            object_avoid_state = turn_stop_three;
            current_stop_time = millis();
          }
          else{
            object_avoid_state = turn_right_two;
          }
          break;
                
      case turn_stop_three:
          if( millis() - current_stop_time > stop_interval){
            object_avoid_state =  move_right_one;
             move_foward_start_2 = millis();
          }
          else{
            object_avoid_state = turn_stop_three;
          }
          break;
          
      case move_right_one:
          if( millis() - move_foward_start_2 > move_foward_interval){
            object_avoid_state = move_right_stop_one;
            current_stop_time = millis();
          }
          else{
            object_avoid_state = move_right_one;
          }
          break; 
          
       case move_right_stop_one:
          if( millis() - current_stop_time > stop_interval){
            object_avoid_state =  turn_left_two;
            current_turn_time_left = millis();
          }
          else{
            object_avoid_state = move_right_stop_one;
          }
          break; 
      case turn_left_two:
          if( millis() - current_turn_time_left > turn_interval){
            object_avoid_state = turn_stop_four;
             current_stop_time = millis();
          }
          else{
            object_avoid_state = turn_left_two;
          }
          break; 
       case turn_stop_four:
          if( millis() - current_stop_time > stop_interval){
            object_avoid_state =  move_foward_state;
          }
          else{
            object_avoid_state = turn_stop_four;
          }
          break; 
          
             
      default:
          object_avoid_state = move_foward_state;
          break;
      } // Transitions
  switch(object_avoid_state) { // State actions
      case -1:
          break;
      case move_foward_state:
          object_avoid_state = move_foward_state;
          Serial.println("Move_foward_state");
          analogWrite(left_motor_pin,speed);
          analogWrite(right_motor_pin,speed);
          break;
      case stop_state_one:
          stop_motor();
          Serial.println("stop_state_one");
          break;
      case turn_left_one:
          Serial.println("turn_left_one");
          analogWrite(right_motor_pin,speed);
          break;
      case turn_stop_one:
          Serial.println("turn_stop_omne"); 
          stop_motor();
          break;
      case move_left_one:
          Serial.println("move_left_one");
          analogWrite(left_motor_pin,speed);
          analogWrite(right_motor_pin,speed);
          break;
      case move_past_one:
          Serial.println("move_past_one");
          break;
      case move_past_stop_one:
          Serial.println("move_past_stop_one");
          stop_motor();
          break;
      case turn_right_one:
          analogWrite(left_motor_pin,speed);
          Serial.println("turn_right_one");
          break;
      case turn_stop_two:
          Serial.println("turn_stop_two");
          stop_motor();
          break;
      case move_foward_delay:
          Serial.println("move_foward_delay");
          analogWrite(left_motor_pin,speed);
          analogWrite(right_motor_pin,speed);
          break;
      case move_foward_state_two:
          Serial.println("move_foward_state_two");
          analogWrite(left_motor_pin,speed);
          analogWrite(right_motor_pin,speed);
          break;   
       case move_past_two:
          Serial.println("move_past_two");
          break;         
      case move_past_stop_two:
          Serial.println("move_past_stop_two");
          stop_motor();
          break;
      case turn_right_two:
          Serial.println("turn_right_two");
          analogWrite(left_motor_pin,speed);
          break; 
      case turn_stop_three:
          Serial.println("turn_stop_three");
          stop_motor();
          break;
       case move_right_one:
          Serial.println("move_right_one");
          analogWrite(left_motor_pin,speed);
          analogWrite(right_motor_pin,speed);
          break; 
       case move_right_stop_one:
           Serial.println("mover_right_stop_one");
          stop_motor();
          break; 
        case turn_left_two:
          Serial.println("turn_left_two");
          analogWrite(right_motor_pin,speed);
          break;
       case turn_stop_four:
          Serial.println("turn_stop_four");
          stop_motor();
          break;   
      default:
          object_avoid_state = move_foward_state;
          break;
  } // State actions
}



















