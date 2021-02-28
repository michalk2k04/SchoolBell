#include <Arduino.h> 
#include "Console.h"

bool logged = false;

int menu = -1;

long timee = 0;

Console::Console()
{

}

void Console::begin(Config& Config)
{
    this->config = config; 

  if(!LittleFS.exists("/index.html"))
  {
    Serial.println("Upload files !!!");
    while (true)
    {
      delay(2000);  
    }
    
  }

    Serial.println("=========================================");
    Serial.print("wifi_ssid = ");Serial.println(config.wifi_ssid);
    Serial.print("wifi_pass = ");Serial.println(config.wifi_pass);
    Serial.print("www_username = ");Serial.println(config.www_user);
    Serial.print("www_pass = ");Serial.println(config.www_pass);
    Serial.print("httpPort = ");Serial.println(config.port);
    Serial.println("========================================="); 
    

    EEPROM.begin(512);

    int i = EEPROM.read(0);

    EEPROM.end();

    if (i != 1)
    {
        logged = true;

        while (!Serial.available()) 
        {
            Serial.println("Write something !"); 
            delay(2000);
        }

        handleConsole();

        config.save();


        EEPROM.begin(512);

        EEPROM.write(0,1);

        EEPROM.end();

        
    }
    
}

void printHelp()
{
    Serial.println("=============================");
    Serial.println(" ");
    Serial.println("To login to the console, enter login");
    Serial.println(" ");
    Serial.println("=============================");
}

void Console::handleConsole()
{ 
  if (!(millis() >= long(timee-(1000*60)) || millis() <= long(timee+(1000*60))))
  {
    printHelp();

    timee = millis();
  }

  if (Serial.available())
  {
    String com = Serial.readString();

    if (com.startsWith("login"))
    {
        bool end = true;
        int mu = 0;
        Serial.println("Enter username : ");
        Serial.print(" > ");
        
        char * username = (char *) "adsfasdsafdssafdasdfdasddfas";

        char *  password = (char *) "adsfasdsagfdsafdasdfdasddfas";
        
        while (end)
        {
            if (Serial.available())
            {
                String str = Serial.readString();
                int size = sizeof(str); 
                
                if (mu == 0)
                {
                    str.toCharArray(username,size);
                    Serial.println(username);
                    Serial.println("Enter password : ");
                    Serial.print(" > ");

                    mu = 1;
                }
                else if (mu == 1)
                {
                    str.toCharArray(password,size);
                    Serial.println(password);
                    end = false;


                    logged = false;
                    if (strcmp(username,config.www_user))
                    {
                        if (strcmp(password,config.www_pass))
                        {
                            logged = true;
                        }
                    }

                    Serial.println("=============================");
                    if (logged)
                    {
                        Serial.println("Logged in");
                    }
                    else
                    {
                        Serial.println("Bad username or password");
                    }
                    Serial.println("=============================");
                    
                    
                }
                
                
            }

            delay(10);
            
        }
        
    }

    if (com.startsWith("clear"))
    {
        EEPROM.begin(512);
        
        for (int i = 0; i < 512; i++) {
            EEPROM.write(i, 0);
        }

      EEPROM.end();

      LittleFS.format();

      ESP.restart();
    }
    
  }

  if (logged)
  {

    int x = 0;

    Serial.println("=========================================");

    Serial.print("Enter wifi ssid : ");  
    while (logged)
    { 
      if (Serial.available())
      {
        String str = Serial.readString();
        int dlugosc = str.length();

        Serial.println(str);
        switch (x)
        {
        case 0:
            str.toCharArray(config.wifi_ssid, dlugosc);

            Serial.print("Ender wifi password : "); 
            break;
        case 1:
            str.toCharArray(config.wifi_pass, dlugosc);  

            Serial.print("Enter new username : ");
            break;
        case 2:
            str.toCharArray(config.www_user, dlugosc); 

            Serial.print("Enter new password : ");
            break;
        case 3:
            str.toCharArray(config.www_pass, dlugosc); 

            Serial.print("Enter http port : ");
            break;
        case 4:
            config.port = str.toInt();
            logged = false;
            config.firstRun = 1;
            config.save();

            break;   
        default:
            break;
        }
          x++;
    
      }
    }

    Serial.println("=========================================");
    Serial.print("wifi_ssid = ");Serial.println(config.wifi_ssid);
    Serial.print("wifi_pass = ");Serial.println(config.wifi_pass);
    Serial.print("www_username = ");Serial.println(config.www_user);
    Serial.print("www_pass = ");Serial.println(config.www_pass);
    Serial.print("httpPort = ");Serial.println(config.port);
    Serial.println("========================================="); 
  }    
}
 