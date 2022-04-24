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

SoftwareSerial gpsSerial(ATGM332D_RXD, ATGM332D_TXD);  // rx,tx

// struct {
//     char GPS_Buffer[80];
//     bool isGetData;      //是否获取到GPS数据
//     bool isParseData;    //是否解析完成
//     char UTCTime[11];    // UTC时间
//     char latitude[11];   //纬度
//     char N_S[2];         // N/S
//     char longitude[12];  //经度
//     char E_W[2];         // E/W
//     bool isUsefull;      //定位信息是否有效
// } currentGPSData;

// struct {
//     String title;
//     String line1;
//     String line2;
//     String line3;
//     String line4;
//     int TX_RX;
// } currentDisplayData;

// const unsigned int gpsRxBufferLength = 600;
// char gpsRxBuffer[gpsRxBufferLength];
// unsigned int ii = 0;

// unsigned int loopTimeStamp = 0;

// void setDisplay() {
//     debugSerial.println(currentDisplayData.title);
//     debugSerial.println(currentDisplayData.line1);
//     debugSerial.println(currentDisplayData.line2);
//     debugSerial.println(currentDisplayData.line3);
//     debugSerial.println("");
//     debugSerial.println("");
//     debugSerial.println("");
//     debugSerial.println(" Menu ");
// }

// void onGPSError(int num) {
//     debugSerial.print("ERROR");
//     debugSerial.println(num);
//     gpsSerial.flush();
// }

// void onGPSDataChange() {
//     if (currentGPSData.isParseData) {
//         currentGPSData.isParseData = false;
//         if (currentGPSData.isUsefull) {
//             currentGPSData.isUsefull = false;
//             String loaction =
//                 "QTH: " + String(atof(currentGPSData.latitude) / 100, 3) +
//                 currentGPSData.N_S + " " +
//                 String(atof(currentGPSData.longitude) / 100, 3) +
//                 currentGPSData.E_W;
//             currentDisplayData.line1 = loaction;
//             debugSerial.println(loaction);
//         } else {
//             debugSerial.println("GPS DATA is not usefull!");
//             currentDisplayData.line1 = "Loss GPS Info";
//         }

//         if (loopTimeStamp == 5) {
//             debugSerial.println("ARPS_TX...");
//             currentDisplayData.TX_RX = 1;
//             loopTimeStamp = 0;
//         } else {
//             currentDisplayData.TX_RX = 0;
//             debugSerial.println("ARPS_RX...");
//             loopTimeStamp++;
//         }
//         String currentTime =
//             "Time: " + String(atol(currentGPSData.UTCTime) + 80000);
//         debugSerial.println(currentTime);
//     }
// }

// void parseGPSData() {
//     char* subString;
//     char* subStringNext;
//     if (currentGPSData.isGetData) {
//         currentGPSData.isGetData = false;
//         debugSerial.println("**************");
//         debugSerial.println(currentGPSData.GPS_Buffer);

//         for (int i = 0; i <= 6; i++) {
//             if (i == 0) {
//                 if ((subString = strstr(currentGPSData.GPS_Buffer, ",")) ==
//                     NULL)
//                     onGPSError(1);  //解析错误
//             } else {
//                 subString++;
//                 if ((subStringNext = strstr(subString, ",")) != NULL) {
//                     char usefullBuffer[2];
//                     switch (i) {
//                         case 1:
//                             memcpy(currentGPSData.UTCTime, subString,
//                                    subStringNext - subString);
//                             break;  //获取UTC时间
//                         case 2:
//                             memcpy(usefullBuffer, subString,
//                                    subStringNext - subString);
//                             break;  //获取UTC时间
//                         case 3:
//                             memcpy(currentGPSData.latitude, subString,
//                                    subStringNext - subString);
//                             break;  //获取纬度信息
//                         case 4:
//                             memcpy(currentGPSData.N_S, subString,
//                                    subStringNext - subString);
//                             break;  //获取N/S
//                         case 5:
//                             memcpy(currentGPSData.longitude, subString,
//                                    subStringNext - subString);
//                             break;  //获取纬度信息
//                         case 6:
//                             memcpy(currentGPSData.E_W, subString,
//                                    subStringNext - subString);
//                             break;  //获取E/W

//                         default:
//                             break;
//                     }

//                     subString = subStringNext;
//                     currentGPSData.isParseData = true;
//                     if (usefullBuffer[0] == 'A')
//                         currentGPSData.isUsefull = true;
//                     else if (usefullBuffer[0] == 'V')
//                         currentGPSData.isUsefull = false;
//                 } else {
//                     onGPSError(2);  //解析错误
//                 }
//             }
//         }
//     }
// }

// void getGPSData() {
//     while (gpsSerial.available()) {
//         gpsRxBuffer[ii++] = gpsSerial.read();
//         if (ii == gpsRxBufferLength)
//             clrGpsRxBuffer();
//     }

//     char* GPS_BufferHead;
//     char* GPS_BufferTail;
//     if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL ||
//         (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL) {
//         if (((GPS_BufferTail = strstr(GPS_BufferHead, "\r\n")) != NULL) &&
//             (GPS_BufferTail > GPS_BufferHead)) {
//             memcpy(currentGPSData.GPS_Buffer, GPS_BufferHead,
//                    GPS_BufferTail - GPS_BufferHead);
//             currentGPSData.isGetData = true;

//             clrGpsRxBuffer();
//         }
//     }
// }

// void clrGpsRxBuffer(void) {
//     memset(gpsRxBuffer, 0, gpsRxBufferLength);  //清空
//     ii = 0;
// }

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

// void initData() {
//     currentDisplayData.title = "  " + USER_CALLSING + " APRS SYSTEM";
//     currentDisplayData.line1 = "Initializing...";
//     currentDisplayData.line2 = "";
//     currentDisplayData.line3 = "";
//     currentDisplayData.line4 = " SET MODE    ";
//     currentDisplayData.TX_RX = 0;

//     currentGPSData.isGetData = false;
//     currentGPSData.isParseData = false;
//     currentGPSData.isUsefull = false;
// }

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
    }
}
