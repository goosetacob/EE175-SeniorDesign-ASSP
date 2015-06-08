#define left_motor_pin 6
#define right_motor_pin 5
#define encoder0PinLeft_int 2
#define encoder0PinLeft 10
#define encoder0PinRight_int 3
#define encoder0PinRight 11
#define right_echo_pin 4
#define right_trig_pin 12

#define left_echo_pin A6
#define left_trig_pin A7
wire
#define cent_trig_pin 8
#define cent_echo_pin 9
#define left_enable_A A0
#define left_enable_B A1

#define right_enable_A A2

#define right_enable_B A3

#define right_motorA  A0
#define right_motorB  A1
#define left_motorB  A2mov
#define left_motorA  A3

#define test_pin 13

#define bumper_pin 13

int bumper_state = 1;

volatile int pin = 13;
volatile int state = LOW;
volatile int encoder_tick = 0;
volatile signed int encoder0PosLeft = 0;
volatile signed int encoder0PosRight = 0;
unsigned long right_pulse = 0;

unsigned int duration_left, inches_left, duration_cent, duration_right, inches_right;
unsigned int  inches_cent = 14;

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000; // interval at which to blink (milliseconds)

volatile long delta_right = 0;
volatile long delta_left = 0;

int num_points = 5;
int current_point = 1;
double deg_per_sec_speed_left = 360/3235;

// PI Varailbes
int base_speed = 200;
int motorPWN_right = 200;
int error_right = 0;
int error_left = 0;
int motorPWN_left = 200;
double set_point_right = 335;
double set_point_left = 0;
double Kp = 2;
double Ki = .0005;
double int_error = 0;
double dt = 0;
long int_time_motor = 0;
long current_time_motor = 0;
int heading_val = 0;
double kp_adjust = 0;


unsigned long int_time = millis();
unsigned long sample_time = 1;
unsigned long sum_tick_right = 0;
unsigned long sum_tick_left = 0;
unsigned long current_time = 0;
unsigned long left_tick_sec = 0;
unsigned long right_tick_sec = 0;
unsigned long initial_tick_left = 0;
unsigned long initial_tick_right   = 0;
int hit_flag = false;


float heading = 0;
float pre_heading = 0;

double foward_delay_offset = 0;

double distance[7] = { 60 ,60 ,60 ,60};
double headings[7] = {90 ,90 ,90 ,90};

///////////////////////
// Example I2C Setup //
///////////////////////
// Comment out this section if you're using SPI
// SDO_XM and SDO_G are both grounded, so our addresses are:
#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW
// Create an instance of the LSM9DS0 library called `dof` the
// parameters for this constructor are:
// [SPI or I2C Mode declaration],[gyro I2C address],[xm I2C add.]
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);
#define PRINT_CALCULATED
//#define PRINT_RAW

#define PRINT_SPEED 500 // 500 ms between prints

// Object Avoidance

long current_stop_time = 0;
long stop_interval = 5000;
long current_turn_time_left = 0;
long current_turn_time_right = 0;
long turn_interval = 500;
long current_move_past_time = 0;
long move_past_interval = 1;
long move_foward_interval = 0;
long move_foward_interval_start = 0;
long move_foward_start_2 = 0;

int speed = 255;

//Testing
int loop_flag = 1;
