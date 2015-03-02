#include <SD.h>
#include <Time.h>
#include "dht.h"

dht DHT;

//GLOBAL CONSTANTS
String DATADIR = "data/";

//PINS
#define TEMPHUMPIN 5
#define MOSI 11
#define MISO 12
#define CLK 13
#define CS 4

String currentFileName = "";
File currentFile;

void sdInit() {
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void fileInit(String file) {
  String writeToFile = DATADIR;
  writeToFile.concat(file);
  
  //annoying string to char array conv
  char fn[writeToFile.length()+1];
  writeToFile.toCharArray(fn, sizeof(fn));
  
  if (SD.exists(fn)) {
    Serial.println(writeToFile);
  }
  else {
    Serial.println(writeToFile);
  }
  
  //annoying string to char array conv
  char fn1[currentFileName.length()+1];
  currentFileName.toCharArray(fn1, sizeof(fn1));
  currentFile = SD.open(fn1, FILE_WRITE);
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  
  sdInit();
  currentFileName = "DATA20150225";
  fileInit(currentFileName);
}

void loop() {
  int chk = DHT.read22(TEMPHUMPIN);
  switch (chk) {
    case DHTLIB_OK: 
      Serial.print("OK,\t"); 
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.print("Checksum error,\t"); 
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.print("Time out error,\t"); 
      break;
    default: 
      Serial.print("Unknown error,\t"); 
      break;
  }
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  
  //WRITE TO FILE
  currentFile.write(DHT.humidity);
  currentFile.write(DHT.humidity);
  delay(1000);
  
}



