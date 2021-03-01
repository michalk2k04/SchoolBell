#ifndef SerialConfigurator_H
#define SerialConfigurator_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Config.h>

class SerialConfigurator
{
  
  public:
    SerialConfigurator(SoftwareSerial& mySerial , Config& config);

    void begin();

    void handleMenu(); 

  private:

    void run();

    long timerMills = 0;

    int menu = 0;

    void firstRun();
    
    String waitForInput(String inputName);

    void menu1();
    void menu2();
    void menu3();
    void menu4();
    void menu5();
    void menu6();
    void menu7();
    void menu8();

    Config *config;
    SoftwareSerial *mySerial;
};

#endif