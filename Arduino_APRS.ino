/**
 * @file Arduino_APRS.ino
 * @author YuYan (sakura@alleysakura.com)
 * @brief
 * @version 0.1
 * @date 2022-04-24
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
 * MyGPS Lib
 * https://github.com/handiko/MyGPS
 */
#include <MyGPS.h>
#include <SoftwareSerial.h>
#include <stdlib.h>

#define debugSerial Serial

/* CONIFG START */

#define SA818_TXD 10
#define SA818_RXD 11

#define ATGM332D_TXD 8
#define ATGM332D_RXD 9

String USER_CALLSING = "BG5ABL";
String APRS_FREQ = "144.640";
/* CONIFG END */

/* CONSTANT START */
const char GPS_VALID_TAG = 'A';

SoftwareSerial gpsSerial(ATGM332D_RXD, ATGM332D_TXD);  // rx,tx

void initSA818(SoftwareSerial& ser) {
    ser.println("AT+DMOCONNECT");
}

void resetSA818(SoftwareSerial& ser) {
    for (char i = 0; i < 3; i++) {
        ser.println("AT+DMOCONNECT");
    }
}

void setSA818Freq(SoftwareSerial& ser) {
    ser.println("AT+DMOSETGROUP=0," + APRS_FREQ + "," + APRS_FREQ +
                ",0000,0,0000");
}

void setSA818Filter(SoftwareSerial& ser) {
    ser.println("AT+SETFILTER=0,0,0");
}

void onSA818Readback(SoftwareSerial& ser) {
    String d;

    while (ser.available() < 1)
        ;
    if (ser.available() > 0) {
        d = ser.readString();
        Serial.print(d);
    }
}

void SA818_close(SoftwareSerial& ser) {
    ser.end();
}

void setup() {
    debugSerial.begin(9600);
    gpsSerial.begin(9600);
    debugSerial.println("test");
}

void loop() {
    char buff[250];

    GPSresults GPS = gps_parse(gpsSerial);

    if (GPS.gps_success) {
        sprintf(buff, "\n%s", GPS.gps_rmc);

        sprintf(buff, "%s \n GPS Time (HHMMSS): %s", buff, GPS.gps_time);
        sprintf(buff, "%s \n GPS Valid: %c", buff, GPS.gps_valid);
        sprintf(buff, "%s \n GPS Latitude: %s", buff, GPS.gps_lat);
        sprintf(buff, "%s \n GPS N/S: %c", buff, GPS.gps_ns);
        sprintf(buff, "%s \n GPS Longitude: %s", buff, GPS.gps_lon);
        sprintf(buff, "%s \n GPS E/W: %c", buff, GPS.gps_ew);
        sprintf(buff, "%s \n GPS Speed (knots): %s", buff, GPS.gps_spd);
        sprintf(buff, "%s \n GPS Course (degree): %s", buff, GPS.gps_cse);
        sprintf(buff, "%s \n GPS Date (DDMMYY): %s \n", buff, GPS.gps_date);

        debugSerial.println(buff);
        if (GPS.gps_valid == GPS_VALID_TAG) {
            debugSerial.println("success");
        } else {
            debugSerial.println("fail");
        }
    } else {
        debugSerial.println("wait next data");
    }
}
