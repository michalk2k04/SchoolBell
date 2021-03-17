#ifndef BELLTIMER_H
#define BELLTIMER_H

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Config.h>
#include <SoftwareSerial.h>

class BellTimer
{
  public:
    BellTimer(SoftwareSerial& mySerial,Config& config);

    void begin();
    void handleBell();

    String getTime();
    String toNextBell();

    bool bellAlert = false;

    bool bell = false;

    void dzwon(); 
    
    void alert();
    
  private:

      const long utcOffsetInSeconds = 3600;

      const long updateInMs = 3600000;

      int lastUpdate = 0;

      bool isTime(int day,int hour,int minute);

      Config *config;
      WiFiUDP *ntpUDP;
      NTPClient *timeClient;
      SoftwareSerial *mySerial;
};
#endif