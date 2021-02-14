#include <Arduino.h> 
  
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h> 
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <BellConfig.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LittleFS.h>

AsyncWebServer server(80);

BellConfig bellConfig = BellConfig();
  
const long utcOffsetInSeconds = 3600;

const long updateInMs = 3600000;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds,updateInMs);

int lastUpdate = 0;

bool bellAlert = false;

bool bell = false;

String processor(const String& var)
{
  if (var.equalsIgnoreCase("ssid"))
  {
    return bellConfig.wifi_ssid;
  }
  if (var.equalsIgnoreCase("HTTP_port"))
  {
    return String(bellConfig.httpPort);
  }
  if (var.equalsIgnoreCase("username"))
  {
    return bellConfig.www_username;
  }
  
  for (int i = 0; i < 22; i++)
  {
    if (var.equalsIgnoreCase("MonBellSettings_h"+String(i)))
    {
      return String(bellConfig.timeBellMon[i][0]);
    }

    if (var.equalsIgnoreCase("MonBellSettings_m"+String(i)))
    {
      return String(bellConfig.timeBellMon[i][1]);
    } 
    if (var.equalsIgnoreCase("TueBellSettings_h"+String(i)))
    {
      return String(bellConfig.timeBellTue[i][0]);
    }

    if (var.equalsIgnoreCase("TueBellSettings_m"+String(i)))
    {
      return String(bellConfig.timeBellTue[i][1]);
    } 
    if (var.equalsIgnoreCase("WedBellSettings_h"+String(i)))
    {
      return String(bellConfig.timeBellWed[i][0]);
    }

    if (var.equalsIgnoreCase("WedBellSettings_m"+String(i)))
    {
      return String(bellConfig.timeBellWed[i][1]);
    } 
    if (var.equalsIgnoreCase("ThuBellSettings_h"+String(i)))
    {
      return String(bellConfig.timeBellThu[i][0]);
    }

    if (var.equalsIgnoreCase("ThuBellSettings_m"+String(i)))
    {
      return String(bellConfig.timeBellThu[i][1]);
    } 
    if (var.equalsIgnoreCase("FriBellSettings_h"+String(i)))
    {
      return String(bellConfig.timeBellFri[i][0]);
    }

    if (var.equalsIgnoreCase("FriBellSettings_m"+String(i)))
    {
      return String(bellConfig.timeBellFri[i][1]);
    } 
    if (var.equalsIgnoreCase("SatBellSettings_h"+String(i)))
    {
      return String(bellConfig.timeBellSat[i][0]);
    }

    if (var.equalsIgnoreCase("SatBellSettings_m"+String(i)))
    {
      return String(bellConfig.timeBellSat[i][1]);
    } 

    if (var.equalsIgnoreCase("SunBellSettings_h"+String(i)))
    {
      return String(bellConfig.timeBellSun[i][0]);
    }

    if (var.equalsIgnoreCase("SunBellSettings_m"+String(i)))
    {
      return String(bellConfig.timeBellSun[i][1]);
    }
  }

  
   

  return "";
}


void dzwon()
{ 
  digitalWrite(D8,HIGH);
  Serial.println("Dzwonek zaczoł dzwonić");
  delay(3000);
  Serial.println("Dzwonek przestał dzwonić"); 
  digitalWrite(D8,LOW);
}

void alert()
{
  dzwon();
  delay(3000);
  dzwon();
  delay(3000);
  dzwon();
  delay(15000);

  
  dzwon();
  delay(3000);
  dzwon();
  delay(3000);
  dzwon(); 

   
}

bool isTime(int day,int hour,int minute)
{
  if (timeClient.getDay() != day)
  {
    return false;
  }
  
   if(timeClient.getHours() != hour)
   {
    return false;
   }
   
   if(timeClient.getMinutes() != minute)
   {
    return false;
   }

   if(timeClient.getSeconds() >= 5)
   {
      return false;
   }

   return true;
}

void dzwonek()
{ 

 
  
  for (int i = 0; i < 22; i++)
  {
    if (isTime(1,bellConfig.timeBellMon[i][0],bellConfig.timeBellMon[i][1]))
    {
      bell = true;
    }
    
    if (isTime(2,bellConfig.timeBellTue[i][0],bellConfig.timeBellTue[i][1]))
    {
      bell = true;
    }
    
    if (isTime(3,bellConfig.timeBellWed[i][0],bellConfig.timeBellWed[i][1]))
    {
      bell = true;
    }
    
    if (isTime(4,bellConfig.timeBellThu[i][0],bellConfig.timeBellThu[i][1]))
    {
      bell = true;
    } 
    
    if (isTime(5,bellConfig.timeBellFri[i][0],bellConfig.timeBellFri[i][1]))
    {
      bell = true;
    } 
    if (isTime(6,bellConfig.timeBellSat[i][0],bellConfig.timeBellSat[i][1]))
    {
      bell = true;
    }
    
    if (isTime(0,bellConfig.timeBellSun[i][0],bellConfig.timeBellSun[i][1]))
    {
      bell = true;
    }
    
  } 
   
}

bool is_authenticated(AsyncWebServerRequest *request) 
{ 
  if (request -> hasHeader("Cookie")) { 
    String cookie = request -> header("Cookie"); 
    if (cookie.indexOf("ESPSESSIONID=1") != -1) { 
      return true;
    }
  } 
  return false;
}

/**
 * 
 * 
 * WEB HANDLERS Files
 * 
 * 
 */

//CSS

void handle404CSS(AsyncWebServerRequest *request)
{
  request->send(LittleFS, "/404.css",String() , false);
} 

void handleStyleCSS(AsyncWebServerRequest *request)
{
  request->send(LittleFS, "/style.css",String() , false);
} 

void handleWelcomeCSS(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
  request->send(LittleFS, "/welcome.css",String() , false);}
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  } 
} 

//JS

void handleSettingsJS(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
  request->send(LittleFS, "/Settings.js",String() , false);
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  } 
} 

//HTML

void handleIndexHTML(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
    request->send(LittleFS, "/welcome.html",String() , false);
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  } 
}

void handleSettingsHTML(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
  request->send(LittleFS, "/Settings.html",String() , false , processor);
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  } 
}

void handleAboutHTML(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
  request->send(LittleFS, "/About.html",String() , false);
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  } 
}


void handleWelcomeHTML(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
  request->send(LittleFS, "/welcome.html",String() , false );
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  } 
}


void handleFunctionsHTML(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
  request->send(LittleFS, "/Functions.html",String() , false);
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  } 
}

// Not found


void handleNotFound(AsyncWebServerRequest *request)
{
  request->send(LittleFS, "/FileNotFound.html",String() , false);
}

void handleFavicon(AsyncWebServerRequest *request)
{
  request->send(LittleFS, "/favicon.ico","image/png");
}


/**
 * 
 * 
 * WEB HANDLERS Functions
 * 
 * 
 */

void handleBell(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
    request->send(LittleFS, "/Functions.html",String() , false);
    bell = true;
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleAlert(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
    request->send(LittleFS, "/Functions.html",String() , false);
    bellAlert = true;
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleRestart(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
    request->send(LittleFS, "/Functions.html",String() , false);
      Serial.println("ESP restart");
      ESP.restart(); 

  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}


void handleLogin(AsyncWebServerRequest *request) 
{  
  if (request -> hasArg("username") && request -> hasArg("password")) {
    if (request -> arg("username") == bellConfig.www_username &&  request -> arg("password") == bellConfig.www_pass) {
      AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/welcome.html",String() ); 
  
      response->addHeader("Cache-Control","no-cache");
      response->addHeader("Set-Cookie", "ESPSESSIONID=1");
      
      request -> send(response);
      return;
    } 
    else
    {
      AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html",String() ); 
  
      response->addHeader("Cache-Control","no-cache");
      response->addHeader("Set-Cookie", "ESPSESSIONID=0");
      
      request -> send(response);
      return;
    }
  }
  else
  {
    AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html",String() ); 

    response->addHeader("Cache-Control","no-cache");
    response->addHeader("Set-Cookie", "ESPSESSIONID=0");
      
      request -> send(response);
    return;
  }
}

void handleLogout(AsyncWebServerRequest *request)
{  

  AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html",String() ); 
  response->addHeader("Cache-Control","no-cache");
  response->addHeader("Set-Cookie", "ESPSESSIONID=0");
  
  request->send(response);  
  return;
}

void setup()
{

  pinMode(D8,OUTPUT); 

    Serial.begin(115200);

    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
 
    if(!LittleFS.begin())
    {
      Serial.println("An Error has occurred while mounting LittleFS");
      return;
    }

    if (!LittleFS.exists("/index.html"))
    {
      Serial.println("Waiting for html files !");

      Serial.println("Upload files and restart esp !");

      while (true)
      {
        delay(1000);
      }
      
    }
    

    FSInfo fs_info;
  LittleFS.info(fs_info);

    float fileTotalKB = (float)fs_info.totalBytes / 1024.0;
  float fileUsedKB = (float)fs_info.usedBytes / 1024.0;
 
    Serial.println("===== File system info =====");
 
    Serial.print("Total space:      ");
    Serial.print(fileTotalKB);
    Serial.println("byte");
 
    Serial.print("Total space used: ");
    Serial.print(fileUsedKB);
    Serial.println("byte");
 
    Serial.println();

    Dir dir = LittleFS.openDir("/");
    Serial.println("LittleFS directory {/} :");
    bool filesExist=false;
    while (dir.next())
    {
    filesExist=true;
    Serial.print("File Name "); Serial.println(dir.fileName());
    Serial.print("File Length "); Serial.println(dir.fileSize());
    }
    if (!filesExist) {
    Serial.println("No files in LittleFS");
    }

    Serial.println("============================");

    bellConfig.begin();

    Serial.println("===== File system info =====");

    File file2 = LittleFS.open("/Config.json", "r");
 
    if (!file2) 
    {
      Serial.println("Failed to open file for reading");
      return;
    }
  
    Serial.println("File Content:");
  
    while (file2.available()) {
  
      Serial.write(file2.read());
    }
    Serial.println(" ");

    Serial.println("===== File system info =====");
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(bellConfig.wifi_ssid,bellConfig.wifi_pass);

    Serial.println(bellConfig.wifi_ssid);
    Serial.println(bellConfig.wifi_pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) 
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  
    server.on("/style.css" , HTTP_GET , handleStyleCSS);
    server.on("/welcome.css" , HTTP_GET , handleWelcomeCSS);
    server.on("/404.css" , HTTP_GET , handle404CSS);

    server.on("/Settings.js" , HTTP_GET , handleSettingsJS);

    server.on("/" , HTTP_GET , handleIndexHTML);
    server.on("/index.html" , HTTP_GET , handleIndexHTML);
    server.on("/Settings.html" , HTTP_GET , handleSettingsHTML);
    server.on("/welcome.html" , HTTP_GET , handleWelcomeHTML);
    server.on("/About.html" , HTTP_GET , handleAboutHTML);
    server.on("/Functions.html" , HTTP_GET , handleFunctionsHTML);


    server.on("/login" , HTTP_GET , handleLogin);
    server.on("/logout" , HTTP_GET , handleLogout);
    server.on("/bell" , HTTP_GET , handleBell);
    server.on("/alert" , HTTP_GET , handleAlert);
    
    
    server.on("/favicon.ico" , HTTP_GET , handleFavicon);

    server.onNotFound(handleNotFound);
  
    server.begin();

    if (!MDNS.begin("schoolbell_1")) 
    {
      Serial.println("Error setting up MDNS responder!");
    }


    File file3 = LittleFS.open("/Fri.json", "r");
 
    if (!file3) 
    {
      Serial.println("Failed to open file for reading");
      return;
    }
  
    Serial.println("File Content:");
  
    while (file3.available()) {
  
      Serial.write(file3.read());
    }

    file3.close();
    Serial.println(" ");

    timeClient.begin();
    timeClient.forceUpdate();

    
}



void loop() 
{
  if(Serial.available())
  {
    String command = Serial.readStringUntil('\n');
  
    if (command.equalsIgnoreCase("config"))
    {
      Serial.println("Uruchamiam konfigurator !");

      bellConfig.runSerialConfiguration();

      bellConfig.save();
      Serial.println("ESP restart");
      ESP.restart(); 
    }

    if (command.equalsIgnoreCase("clear"))
    {
      EEPROM.begin(512);
      for (int i = 0; i < 512; i++) 
      {
        EEPROM.write(i, 0);
      }
      EEPROM.end();

      LittleFS.remove(bellConfig.fileConfig);
      LittleFS.remove(bellConfig.fileMon);
      LittleFS.remove(bellConfig.fileTue);
      LittleFS.remove(bellConfig.fileWed);
      LittleFS.remove(bellConfig.fileThu);
      LittleFS.remove(bellConfig.fileFri);
      LittleFS.remove(bellConfig.fileSat);
      LittleFS.remove(bellConfig.fileSun);

      Serial.println("LittleFS formatign ... ");
      LittleFS.format();
      Serial.println("ESP restart");
      ESP.restart(); 
    }
    
    if (command.equalsIgnoreCase("info"))
    {
      Serial.println("====================================================");

      for (int i = 0; i < 22; i++)
      {
        Serial.print("Mon ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(bellConfig.timeBellMon[i][0]);
        Serial.print(":");
        Serial.println(bellConfig.timeBellMon[i][1]);
      }
      Serial.println("----------------------------------------------------");
      for (int i = 0; i < 22; i++)
      {
        Serial.print("Tue ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(bellConfig.timeBellTue[i][0]);
        Serial.print(":");
        Serial.println(bellConfig.timeBellTue[i][1]);
      }
      Serial.println("----------------------------------------------------");
      for (int i = 0; i < 22; i++)
      {
        Serial.print("Wed ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(bellConfig.timeBellWed[i][0]);
        Serial.print(":");
        Serial.println(bellConfig.timeBellWed[i][1]);
      }
      Serial.println("----------------------------------------------------");
      for (int i = 0; i < 22; i++)
      {
        Serial.print("Thu ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(bellConfig.timeBellThu[i][0]);
        Serial.print(":");
        Serial.println(bellConfig.timeBellThu[i][1]);
      }
      Serial.println("----------------------------------------------------");
      for (int i = 0; i < 22; i++)
      {
        Serial.print("Fri ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(bellConfig.timeBellFri[i][0]);
        Serial.print(":");
        Serial.println(bellConfig.timeBellFri[i][1]);
      }
      Serial.println("----------------------------------------------------");
      for (int i = 0; i < 22; i++)
      {
        Serial.print("Sat ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(bellConfig.timeBellSat[i][0]);
        Serial.print(":");
        Serial.println(bellConfig.timeBellSat[i][1]);
      }
      Serial.println("----------------------------------------------------");
      for (int i = 0; i < 22; i++)
      {
        Serial.print("Sun ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(bellConfig.timeBellSun[i][0]);
        Serial.print(":");
        Serial.println(bellConfig.timeBellSun[i][1]);
      }
 
  
      Serial.println("===================================================="); 

    }
    
  }

  dzwonek();

  if(lastUpdate != timeClient.getHours())
  {
    lastUpdate = timeClient.getHours();
    timeClient.forceUpdate(); 
  }


  if (bellAlert)
  {
    alert();
    bellAlert = false;
    delay(3000);
  }

  if (bell)
  {
    dzwon();
    bell = false;
    delay(3000);
  }
  
  delay(1000);
  
}

