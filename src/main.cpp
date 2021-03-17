#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Arduino.h>
#include <LittleFS.h>
#include <FS.h>
#include <Config.h>
#include "SoftwareSerial.h"
#include <SerialConfigurator.h>
#include <HTTPServer.h>
#include <BellTimer.h>

Config config = Config();

SoftwareSerial mySerial(D4,D5);

SerialConfigurator serialConfigurator = SerialConfigurator(mySerial,config);

BellTimer timer = BellTimer(mySerial,config);

HTTPServer server = HTTPServer(mySerial,config,timer);

void listFiles()
{
  File root = LittleFS.open("/","r");
  File file = root.openNextFile();
  
  mySerial.println(" ");
  mySerial.println("==============================================");

  mySerial.println(" ");
  mySerial.println(" All Filess in ESP8266");
  mySerial.println(" ");
  while(file)
  {
 
    mySerial.print("FILE: ");
    mySerial.println(file.name());
    file = root.openNextFile();
  }
  mySerial.println("==============================================");
  mySerial.println(" ");

 
}

void begin()
{
  if (!LittleFS.exists("/index.html"))
  {
    mySerial.println("Upload html files with LittleFS !");

    while (true)
    {
      delay(10);
    }
    
  }
  
}

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);
  //mySerial.begin(9600,SWSERIAL_8N1,D4,D5, false, 64);  

  mySerial.println(" ");
  mySerial.println("==============================================");
  mySerial.println(" ");
  mySerial.println(" ESP8266 has started.");
  mySerial.println(" ");
  mySerial.println("  Project - SchoolBell");
  mySerial.println(" ");
  mySerial.println("==============================================");
  mySerial.println(" ");
 
  if (!LittleFS.begin())
  {
    mySerial.println("LittleFs load failed !");
    return;
  }
  
  listFiles();
  
  begin();

  config.begin();
 
  serialConfigurator.begin();

  server.begin();

  timer.begin();
}

void loop()
{ 

  if(config.mainSave())
  {
    Serial.println("Config saved !");
  }
  
  if (timer.bellAlert)
  {
    timer.alert();
    timer.bellAlert = false;
    delay(3000);
  }

  if (timer.bell)
  {
    timer.dzwon();
    timer.bell = false;
    delay(3000);
  }
  
  delay(1000);

  if (Serial.available())
  {
    mySerial.write(Serial.readString().c_str());
  }  

  timer.handleBell();
  serialConfigurator.handleMenu(timer);
}