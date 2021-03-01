#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Arduino.h>
#include <LittleFS.h>
#include <FS.h>
#include <Config.h>
#include <SoftwareSerial.h>

Config config = Config();

SoftwareSerial mySerial(D4,D5);

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
 
}

void loop()
{
  if (mySerial.available())
  {
    Serial.write(mySerial.readString().c_str());
  }  
  delay(10);
}