#include <SdFat.h>
#include "HalfDuplexSimpleSerial.h"
#include "GPSSmartModule.h"
#include "gps_type_define.h"
#include "dht.h"

//GPS DEFINES
#define GPS_SIO 9
#define GPS_BAUDRATE 9600
#define SERIAL_MONITOR_BR 9600

//SPI DEFINES
#define MOSI 11
#define MISO 12
#define CLK 13 

//SD DEFINES
#define SD_CS 10
#define LINE_DIM 100
#define BLU_DIM 10
SdFat sd;
SdFile currentFile;

//SENSOR DEFINES
#define CO_SENSOR A0
#define METHANE_SENSOR A1
#define TEMPHUMPIN 5

//BUTTON DEFINES
#define SENDDATAPIN 8
#define LOGDATAPIN 7

void gps_send_cmd(uint8_t cmd_b, uint8_t cmd_s) {
  HDSS_write(0xA5);
  HDSS_write(cmd_b);
  HDSS_write(cmd_s);
  HDSS_write(0x5A);
}

void dec0_to_string(int32_t dec_value, uint8_t bits, int8_t * str_buffer) {
    uint8_t j;
    uint8_t bits_ = bits;
    int32_t i;
    // For a 32-bit int, 11 bits will be maximum.
    if(bits < 11)
    {
        if (dec_value < 0)
        {
            dec_value = 0 - dec_value;
            *str_buffer = '-';
            str_buffer++;
        }

        i = 1;
        while((--bits)>0)
        {i *=10;}
        dec_value %= (i*10);

        for(j = 0; j < bits_;  j++)
        {
            *str_buffer = ((dec_value / i) + '0');
            dec_value %= i;
            str_buffer++;
            i /= 10;
        }
    }
    // put the string ended indicator to the end.
    *str_buffer = '\0';
}


void sdStart() {
  if (!sd.begin(SD_CS, SPI_HALF_SPEED)){
    sd.initErrorHalt();
  }
  //Serial.println(F("initialization done."));
}

void logData() {    
  char dataFileName[] = "agvdata.txt";

  if (sd.exists(dataFileName)) {
    //Serial.println(F("  found file"));
  } else {
    //Serial.println(F("  created file"));
  }

  if (!currentFile.open(dataFileName , O_WRITE | O_CREAT | O_APPEND)) {
    sd.errorHalt("opening agvdata.txt failed");
  }

  //SETUP GPS
  uint8_t i=0;
  uint8_t temp8; // 8-bit temperate
  uint16_t temp16; // 16-bit temperate
  uint32_t temp32; // 32-bit temperate
  uint32_t rxdata = 0; // rxdata from module (32-bit), when error occur, will get 32-bit value, otherwise will get a 8-bit value(LSB)
  int8_t convert_buff[16];
  utc_t utc;// UTC time, union, hour, minute, second
  date_t date; // date, union, day, month, year
  latitude_t latitude; // latitude, union, degree, minute, second(the module will return degree(8-bit), minute(8-bit), mill-minute(16-bit))
  longitude_t longitude; //longitude, union, degree, minute, second(the module will return degree(8-bit), minute(8-bit), mill-minute(16-bit))

   /* Get Latitude */
  gps_send_cmd(CMD_INFO_BASIC, CMD_GET_LATITUDE);
  // The module will return 5 bytes, after received the command
  // Degree, minute, second(16-bit, MSB first), N/S
  for(i = 0; i < 5; i++)
  {
      rxdata = HDSS_read_time(8);
      if(rxdata == ERROR_RX_TIMEOUT)
      {
         Serial.println("ERROR_RX_TIMEOUT");
          //return ERROR_RX_TIMEOUT;
      }
      latitude.buff[i] = rxdata;
  }
  currentFile.print("Latitude: ");
  dec0_to_string(latitude.degree, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print('`');
  dec0_to_string(latitude.minute, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print('\'');
  temp16 = latitude.buff[2] << 8 | latitude.buff[3];
  latitude.second = temp16 * 6 / 10;
  dec0_to_string(latitude.second / 100, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print(".");
  dec0_to_string(latitude.second % 100  , 2, convert_buff);
  currentFile.print((char *)convert_buff);
  if(latitude.indicator == 0)
  {
      currentFile.print("\" N ");
  }
  else
  {
      currentFile.print("\" S ");
  }
  
      /* Get Longitude */
    gps_send_cmd(CMD_INFO_BASIC, CMD_GET_LONGITUDE);
    // The module will return 5 bytes, after received the command
    // Degree, minute, second(16-bit, MSB first), E/W
    
    for(i = 0; i < 5; i++)
    {
        rxdata = HDSS_read_time(8);
        if(rxdata == ERROR_RX_TIMEOUT)
        {
          Serial.print("ERROR_RX_TIMEOUT");
            //return ERROR_RX_TIMEOUT;
        }
        longitude.buff[i] = rxdata;
    }
    currentFile.print("Longitude: ");
    dec0_to_string(longitude.degree, 3, convert_buff);
    currentFile.print((char *)convert_buff);
    currentFile.print('`');
    dec0_to_string(longitude.minute, 2, convert_buff);
    currentFile.print((char *)convert_buff);
    currentFile.print('\'');
    temp16 = longitude.buff[2] << 8 | longitude.buff[3];
    longitude.second = temp16 * 6 / 10;
    dec0_to_string(longitude.second / 100, 2, convert_buff);
    currentFile.print((char *)convert_buff);
    currentFile.print(".");
    dec0_to_string(longitude.second % 100, 2, convert_buff);
    currentFile.print((char *)convert_buff);
    if(longitude.indicator == 0)
    {
        currentFile.print("\" E ");
    }
    else
    {
        currentFile.print("\" W ");
    }

  /* Get UTC time */
  gps_send_cmd(CMD_INFO_BASIC, CMD_GET_TIME);
  // The module will return 3 bytes, after received the command
  // Hour, Minute, Second
  for(i = 0; i < 3; i++)
  {
      rxdata = HDSS_read_time(8);
      if(rxdata == ERROR_RX_TIMEOUT)
      {
          Serial.println(ERROR_RX_TIMEOUT);
          //return ERROR_RX_TIMEOUT;
      }
      utc.buff[i] = rxdata;
  }

  currentFile.print("Time: ");
  dec0_to_string(utc.hour, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print(':');
  dec0_to_string(utc.minute, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print(':');
  dec0_to_string(utc.second, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print(" ");

  /* Get Date */
  gps_send_cmd(CMD_INFO_BASIC, CMD_GET_DATE);
  // The module will return 3 bytes, after received the command
  // Day, Month, Year(00~99)
  for(i = 0; i < 3; i++)
  {
      rxdata = HDSS_read_time(8);
      if(rxdata == ERROR_RX_TIMEOUT)
      {
          Serial.println(ERROR_RX_TIMEOUT);
          //return ERROR_RX_TIMEOUT;
      }
      date.buff[i] = rxdata;
  }

  currentFile.print("Date: ");
  dec0_to_string(date.day, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print('/');
  dec0_to_string(date.month, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print("/20");
  dec0_to_string(date.year, 2, convert_buff);
  currentFile.print((char *)convert_buff);
  currentFile.print(" ");

  // HUMIDITY AND TEMP SENSOR
  dht DHT;
  int chk = DHT.read22(TEMPHUMPIN);

  //WRITE SENSOR DATA TO FILE
  currentFile.print("humidity: ");
  currentFile.print(DHT.humidity);
  currentFile.print("% ");
  currentFile.print("temp: ");
  currentFile.print(DHT.humidity);
  currentFile.print("C ");
  currentFile.print("CO: ");
  currentFile.print(analogRead(CO_SENSOR));
  currentFile.print("ppm ");
  currentFile.print("CH4: ");
  currentFile.print(analogRead(METHANE_SENSOR));
  currentFile.println("ppm");
  
  // Force data to SD and update the directory entry to avoid data loss.
  if (!currentFile.sync() || currentFile.getWriteError()) {
    Serial.println("write error"); 
  }
  
  if (!currentFile.close()) {
    Serial.println("error closing");
  }
  delay(1000);
  
}

void sendData() {
  char dataFileName[] = "agvdata.txt";

  if (sd.exists(dataFileName)) {
    //Serial.println(F("  found file"));
  } else {
    //Serial.println(F("  created file"));
  }

  if (!currentFile.open(dataFileName , O_READ)) {
    sd.errorHalt("opening agvdata.txt failed");
  }
  
  size_t n;
  char line[19];
  while ((n = currentFile.fgets(line, sizeof(line))) > 0) {
    //only send 20 bytes at a time
    int start = 0;
    int last = 19;
    do {
      char sending[19];
      strncpy(sending, line+start, 19);
      
      Serial.print(sending);
      delay(300);
      
      start = last;
      last += 19;
    } while(last < sizeof(line));
  }  
  
  // Force data to SD and update the directory entry to avoid data loss.
  if (!currentFile.sync() || currentFile.getWriteError()) {
    Serial.println("write error"); 
  }
  
  if (!currentFile.close()) {
    Serial.println("failed close");
  }
}

void setup(void) {
  //setup trigger pins
  pinMode(SENDDATAPIN,INPUT);
  pinMode(LOGDATAPIN, INPUT);
  
  // Open serial communications and wait for port to open:
  Serial.begin(SERIAL_MONITOR_BR);
  HDSS_start(GPS_SIO, 0, GPS_BAUDRATE);
  
  //wait for Serial port to connect
  while (!Serial) { ;} 
  delay(1000);
  
  sdStart();
}

void loop() { 
  if (!digitalRead(SENDDATAPIN)) {
    sendData();
  }
  if (!digitalRead(LOGDATAPIN)) {
    logData();
  }
}

