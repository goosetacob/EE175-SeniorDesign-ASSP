//written by: Gustavo Blanco
//http://simplytronics.com/products/ST-00059

#ifndef ASSPGPS_H
#define ASSPGPS_H

unsigned char startCondition = 0xA5;
unsigned char endCondition = 0x5A;


// Basic Information Class Values
// 0x00	GetInfo	GPS Smart Module Version	2	Hardware, Firmware
// 0x01	GetValid	Check validity of data string	1	0 = Not Valid, 1 = Valid
// 0x02	GetTime	Time(UTC/Greenwich Mean Time)	3	Hours, Minutes, Seconds
// 0x03	GetDate	Date(UTC/Greenwich Mean Time)	3	Month, Day, Year
// 0x04	GetLat	Latitude	5	Degree, Minutes, Fractional Minute(World),
// Direction(0 = N, 1 = S)
// 0x05	GetLong	Longitude	5	Degree, Minutes, Fractional Minute(World),
// Direction(0 = E, 1 = W)
// 0x06	GetAlt	Altitude above mean-sea level(in tenths of meters),
// 6553.5 m max	2	Altitude(word)
// 0x07	GetSpeedKmh	Speed(in tenths of KM/h), 999 Km/h	2	Speed(word) Km/h
// 0x08	GetSpeedKn	Speed(in tenths of knots), 999.9 knots max	2	Speed(word)Knots/h
// 0x09	GetHead	Heading/Direction of travel 0~359.9
// (in tenths of degree)	2	Heading(word)
// 0x0A	StaticNaviOn	Turn off Static Navigate mode	1	0x00, received the command
// 0x0B	StaticNaviOff	Turn on Static Navigate mode	1	0x00, received the command
// 0x0D	Reset
unsgined char commandClass Value = 0x00;
unsigned char basicInformation{12} = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0D}

//
void gpsSendCmd(unsigned char cmdClass, unsigned char cmdValue) {  
  HDSS_write(0xA5);
  HDSS_write(cmd_b);
  HDSS_write(cmd_s);
  HDSS_write(0x5A);
}

//getLatitude
unsigned char[5] getLatitude(unsigned char port, unsigned char number) {

}

//getLongitude
unsigned char[5] getLongitude(unsigned char port, unsigned char number) {

}

#endif
