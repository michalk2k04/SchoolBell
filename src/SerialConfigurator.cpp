#include <Arduino.h> 
#include "SerialConfigurator.h"

SerialConfigurator::SerialConfigurator(SoftwareSerial& mySerial , Config& config)
{
  this->config = &config;
  this->mySerial = &mySerial;

}

void SerialConfigurator::begin()
{
  if (config->firstRun != 1)
  {
    firstRun();
    config->save();
  }
  
}

void SerialConfigurator::handleMenu(BellTimer& timer)
{
  long currentMillis = millis();

  if(currentMillis - timerMills > 100000)
  { 
    mySerial->println(" ");
    mySerial->println(" Menu :");
    mySerial->println("   ");
    mySerial->println("   1 - WiFi configuration");
    mySerial->println("   2 - User configuration");
    mySerial->println("   3 - HTTP and bell duration configuration");
    mySerial->println("   4 - Print config variables");
    mySerial->println("   5 - Print files");
    mySerial->println("   6 - Print list of files");
    mySerial->println("   7 - Print all templates");
    mySerial->println("   8 - Print all bells"); 
    mySerial->println("   9 - Save and relstart ESP");
    mySerial->println("   ");
    config->save();
    mySerial->println("   Memory: " + String(ESP.getFreeHeap()));
    mySerial->println("   ");
    mySerial->println("   Time: " + timer.getTime());
    mySerial->println("   ");

    timerMills = millis();
  }

  if (mySerial->available())
  {
    String str = mySerial->readString();
    int i = str.toInt();
    if (i>=1 && i <= 8)
    {
      menu = i;
    }
    else
    {
      menu = 0;
    }
    run();
  }
  
}

void SerialConfigurator::run()
{ 
  switch (menu)
  {
    case 1:
      menu1();
      break;
    case 2:
      menu2();
      break;
    case 3:
      menu3();
      break;
    case 4:
      menu4();
      break;
    case 5:
      menu5();
      break;
    case 6:
      menu6();
      break;
    case 7:
      menu7();
      break;
    case 8:
      menu8();
      break;
    case 9:
      menu9();
      break;
    
    default:
      break;
  }
}

void SerialConfigurator::firstRun()
{
  waitForInput("write something");
  
  menu6();
  menu1();
  menu2();
  menu3();

  config->firstRun = 1;
  config->save();
}

String SerialConfigurator::waitForInput(String inputName)
{
  yield();
  mySerial->println(" ");
  mySerial->print(" " + inputName + " >");
  
  String readStr;

  while (!mySerial->available())
  {
    yield();
  }

  readStr = mySerial->readString();
  yield();

  mySerial->println(readStr);

  return readStr;  
}

void SerialConfigurator::menu1()
{
  yield();
  String wifi_ssid = waitForInput("Input WiFi ssid (name) ");
  int wifi_ssid_size = wifi_ssid.length(); 
  wifi_ssid.toCharArray(config->wifi_ssid,wifi_ssid_size); 
  yield();
  
  String wifi_pass = waitForInput("Input WiFi password ");
  int wifi_pass_size = wifi_pass.length();
  wifi_pass.toCharArray(config->wifi_pass,wifi_pass_size);
  yield();
  menu = 0;
  config->save();
  yield();
}

void SerialConfigurator::menu2()
{
  yield();
  String www_user = waitForInput("Input new username ");
  int www_user_size = www_user.length(); 
  www_user.toCharArray(config->www_user,www_user_size); 
  yield();

  String www_pass = waitForInput("Input new user password ");
  int www_pass_size = www_pass.length();
  www_pass.toCharArray(config->www_pass,www_pass_size);
  yield();
  
  menu = 0;
  config->save();
  yield();
}

void SerialConfigurator::menu3()
{
  String port = waitForInput("Input web (www) port ");
  config->port = port.toInt();
  yield();
  String bell = waitForInput("Input bell duration (seconds) ");
  config->bellDuration = bell.toInt();
  yield();
  menu = 0;
  config->save();
  yield();
}

void SerialConfigurator::menu4()
{
  
  mySerial->println("========================");
  mySerial->print(" WiFi ssid =");
  mySerial->println(config->wifi_ssid);
  mySerial->print(" WiFi pass =");
  mySerial->println(config->wifi_pass);
  mySerial->print(" Username =");
  mySerial->println(config->www_user);
  mySerial->print(" User password =");
  mySerial->println(config->www_pass);
  mySerial->print(" HTTP port =");
  mySerial->println(config->port);
  mySerial->print(" Bell duration =");
  mySerial->println(config->bellDuration);
  mySerial->println("========================");

  menu = 0;
}

void SerialConfigurator::menu5()
{

  File root = LittleFS.open("/","r");
  File file = root.openNextFile();
  
  mySerial->println(" ");
  mySerial->println("==============================================");

  mySerial->println(" ");
  mySerial->println(" All Filess in ESP8266");
  mySerial->println(" ");
  while(file)
  {
 
    mySerial->print("FILE: ");
    mySerial->print(file.name());
    mySerial->print(" - ");
    mySerial->println(file.readString());
    file = root.openNextFile();
  }
  mySerial->println("==============================================");
  mySerial->println(" ");
 
  menu = 0;
}


void SerialConfigurator::menu6()
{
  File root = LittleFS.open("/","r");
  File file = root.openNextFile();
  
  mySerial->println(" ");
  mySerial->println("==============================================");

  mySerial->println(" ");
  mySerial->println(" All Filess in ESP8266");
  mySerial->println(" ");
  while(file)
  {
    mySerial->print("FILE: ");
    mySerial->println(file.name());
    file = root.openNextFile();
  }
  mySerial->println("==============================================");
  mySerial->println(" ");

  menu = 0;
}


void SerialConfigurator::menu7()
{
  mySerial->println("==============================================");

  mySerial->println(" ");
  mySerial->println("Template 0 :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->template0[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->template0[i][1]));
  }

    mySerial->println(" ");
  mySerial->println("Template 1 :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->template1[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->template2[i][1]));
  }

    mySerial->println(" ");
  mySerial->println("Template 2 :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->template2[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->template2[i][1]));
  }

    mySerial->println(" ");
  mySerial->println("Template 3 :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->template3[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->template3[i][1]));
  }


  mySerial->println("==============================================");
  
  menu = 0;
}


void SerialConfigurator::menu8()
{ 
  mySerial->println("==============================================");

  mySerial->println(" ");
  mySerial->println(" Monday :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->monBell[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->monBell[i][1]));
  }

    mySerial->println(" ");
  mySerial->println(" Tuesday :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->tueBell[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->tueBell[i][1]));
  }

    mySerial->println(" ");
  mySerial->println("Wednesday :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->wedBell[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->wedBell[i][1]));
  }

    mySerial->println(" ");
  mySerial->println("Thursday :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->thuBell[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->thuBell[i][1]));
  }

    mySerial->println(" ");
  mySerial->println("Friday :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->friBell[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->friBell[i][1]));
  }

    mySerial->println(" ");
  mySerial->println("Saturday :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->satBell[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->satBell[i][1]));
  }

    mySerial->println(" ");
  mySerial->println("Sunday :");
  mySerial->println(" ");

  for (int i = 0; i < 24; i++)
  {
    mySerial->println("["+String(i)+"][0] ="+String(config->sunBell[i][0]));
    mySerial->println("["+String(i)+"][1] ="+String(config->sunBell[i][1]));
  } 

  mySerial->println("==============================================");

  menu = 0;
}


void SerialConfigurator::menu9()
{
  config->save();
  mySerial->println("=====================================");
  mySerial->println(" ");
  mySerial->println(" ESP reload !");
  mySerial->println(" ");
  mySerial->println("=====================================");
  ESP.restart(); 
}