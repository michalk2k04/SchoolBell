#include <Arduino.h>
#include <Config.h>
#include <Console.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <FS.h>

Config config = Config();

Console console = Console();

WebServer webServer = WebServer();

void listDir(const char * dirname) {
  Serial.printf("Listing directory: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) 
  {
    File file = root.openFile("r");
    Serial.print("  FILE: ");
    Serial.println(root.fileName());
    file.close();
  }
}

void setup() 
{

    Serial.begin(9600); 
    
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");

    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }

    Serial.println("==============================");
    listDir("/");
    Serial.println("==============================");
    
    config.begin();


    Serial.println("==============================");

    for (int i = 0; i < 24; i++)
    {
      Serial.print("Template0 [");
      Serial.print(i);
      Serial.print("][0] = ");
      Serial.print(config.template0[i][0]);
      Serial.print(" ["+String(i)+"][1]");
      Serial.println(";");
    }
    

    Serial.println("==============================");

    console.begin(config); 
    webServer.begin(config);
    
}
    
void loop() 
{
  delay(10); 
    console.handleConsole(); 
}



