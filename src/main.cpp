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

String processorTemplates(const String& var)
{
  for (int i = 0; i < 22; i++)
  {
    if (var.equalsIgnoreCase("Template_0_h"+String(i)))
    {
      return String(bellConfig.template0[i][0]);
    }

    if (var.equalsIgnoreCase("Template_0_m"+String(i)))
    {
      return String(bellConfig.template0[i][1]);
    }

    if (var.equalsIgnoreCase("Template_1_h"+String(i)))
    {
      return String(bellConfig.template1[i][0]);
    }

    if (var.equalsIgnoreCase("Template_1_m"+String(i)))
    {
      return String(bellConfig.template1[i][1]);
    }

    if (var.equalsIgnoreCase("Template_2_h"+String(i)))
    {
      return String(bellConfig.template2[i][0]);
    }

    if (var.equalsIgnoreCase("Template_2_m"+String(i)))
    {
      return String(bellConfig.template2[i][1]);
    }
  }

  return "";
}

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
  if (var.equalsIgnoreCase("BELL_TIME"))
  {
    return String(bellConfig.bell_time);
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
  delay(bellConfig.bell_time*1000);
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

void handleTemplatesHTML(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
  request->send(LittleFS, "/Templates.html",String() , false , processorTemplates);
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
  request->send(LittleFS, "/favicon.png","image/png");
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

void handleSaveNetworkSettings(AsyncWebServerRequest *request)
{
    if (is_authenticated(request))
  { 
      
    String wifi_ssid = request->arg("ssid");
    String wifi_password = request->arg("password");
    int httpPort = request->arg("HTTP_port").toInt();
    int bell_time = request->arg("BELL_TIME").toInt();


    wifi_ssid.toCharArray(bellConfig.wifi_ssid,wifi_ssid.length()+1);
    wifi_password.toCharArray(bellConfig.wifi_pass, wifi_password.length()+1);
    
    bellConfig.httpPort = httpPort;

    bellConfig.bell_time = bell_time;

    bellConfig.save();

    request->send(LittleFS, "/Settings.html",String() , false , processor);
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveAccountSettings(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
      
    String www_username = request->arg("login");
    String www_pass = request->arg("password");


    www_username.toCharArray(bellConfig.www_username,www_username.length()+1);
    www_pass.toCharArray(bellConfig.www_pass, www_pass.length()+1);

    bellConfig.save();

    request->send(LittleFS, "/Settings.html",String() , false , processor);
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveMon(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("MonBellSettings_h"+String(i)).toInt();
      int b = request -> arg("MonBellSettings_m"+String(i)).toInt();

      bellConfig.timeBellMon[i][0] = a;
      bellConfig.timeBellMon[i][1] = b;
    }

    request->send(LittleFS, "/Settings.html",String() , false , processor);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveTue(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("TueBellSettings_h"+String(i)).toInt();
      int b = request -> arg("TueBellSettings_m"+String(i)).toInt();

      bellConfig.timeBellTue[i][0] = a;
      bellConfig.timeBellTue[i][1] = b;
    }

    request->send(LittleFS, "/Settings.html",String() , false , processor);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveWed(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("WedBellSettings_h"+String(i)).toInt();
      int b = request -> arg("WedBellSettings_m"+String(i)).toInt();

      bellConfig.timeBellWed[i][0] = a;
      bellConfig.timeBellWed[i][1] = b;
    }

    request->send(LittleFS, "/Settings.html",String() , false , processor);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveThu(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("ThuBellSettings_h"+String(i)).toInt();
      int b = request -> arg("ThuBellSettings_m"+String(i)).toInt();

      bellConfig.timeBellThu[i][0] = a;
      bellConfig.timeBellThu[i][1] = b;
    }

    request->send(LittleFS, "/Settings.html",String() , false , processor);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveFri(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("FriBellSettings_h"+String(i)).toInt();
      int b = request -> arg("FriBellSettings_m"+String(i)).toInt();

      bellConfig.timeBellFri[i][0] = a;
      bellConfig.timeBellFri[i][1] = b;
    }

    request->send(LittleFS, "/Settings.html",String() , false , processor);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveSat(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("SatBellSettings_h"+String(i)).toInt();
      int b = request -> arg("SatBellSettings_m"+String(i)).toInt();

      bellConfig.timeBellSat[i][0] = a;
      bellConfig.timeBellSat[i][1] = b;
    }

    request->send(LittleFS, "/Settings.html",String() , false , processor);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveSun(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("MonBellSettings_h"+String(i)).toInt();
      int b = request -> arg("MonBellSettings_m"+String(i)).toInt();

      bellConfig.timeBellSun[i][0] = a;
      bellConfig.timeBellSun[i][1] = b;
    }

    request->send(LittleFS, "/Settings.html",String() , false , processor);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveTemplate0(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("Template_0_h"+String(i)).toInt();
      int b = request -> arg("Template_0_m"+String(i)).toInt();

      bellConfig.template0[i][0] = a;
      bellConfig.template0[i][1] = b;
    }

    request->send(LittleFS, "/Templates.html",String() , false , processorTemplates);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveTemplate1(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("Template_1_h"+String(i)).toInt();
      int b = request -> arg("Template_1_m"+String(i)).toInt();

      bellConfig.template1[i][0] = a;
      bellConfig.template1[i][1] = b;
    }

    request->send(LittleFS, "/Templates.html",String() , false , processorTemplates);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleSaveTemplate2(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 22; i++)
    {
      int a = request -> arg("Template_2_h"+String(i)).toInt();
      int b = request -> arg("Template_2_m"+String(i)).toInt();

      bellConfig.template2[i][0] = a;
      bellConfig.template2[i][1] = b;
    }

    request->send(LittleFS, "/Templates.html",String() , false , processorTemplates);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void handleLoadTemplate(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    
    String templateID = request -> arg("template");
    String dayOfWeek = request -> arg("dayOfWeek");

    if(dayOfWeek.equalsIgnoreCase("mon"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(bellConfig.timeBellMon,bellConfig.template0,sizeof(bellConfig.timeBellMon));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(bellConfig.timeBellMon,bellConfig.template1,sizeof(bellConfig.timeBellMon));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(bellConfig.timeBellMon,bellConfig.template2,sizeof(bellConfig.timeBellMon));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("tue"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(bellConfig.timeBellTue,bellConfig.template0,sizeof(bellConfig.timeBellTue));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(bellConfig.timeBellTue,bellConfig.template1,sizeof(bellConfig.timeBellTue));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(bellConfig.timeBellTue,bellConfig.template2,sizeof(bellConfig.timeBellTue));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("wed"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(bellConfig.timeBellWed,bellConfig.template0,sizeof(bellConfig.timeBellWed));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(bellConfig.timeBellWed,bellConfig.template1,sizeof(bellConfig.timeBellWed));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(bellConfig.timeBellWed,bellConfig.template2,sizeof(bellConfig.timeBellWed));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("thu"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(bellConfig.timeBellThu,bellConfig.template0,sizeof(bellConfig.timeBellThu));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(bellConfig.timeBellThu,bellConfig.template1,sizeof(bellConfig.timeBellThu));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(bellConfig.timeBellThu,bellConfig.template2,sizeof(bellConfig.timeBellThu));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("fri"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(bellConfig.timeBellFri,bellConfig.template0,sizeof(bellConfig.timeBellFri));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(bellConfig.timeBellFri,bellConfig.template1,sizeof(bellConfig.timeBellFri));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(bellConfig.timeBellFri,bellConfig.template2,sizeof(bellConfig.timeBellFri));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("sat"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(bellConfig.timeBellSat,bellConfig.template0,sizeof(bellConfig.timeBellSat));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(bellConfig.timeBellSat,bellConfig.template1,sizeof(bellConfig.timeBellSat));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(bellConfig.timeBellSat,bellConfig.template2,sizeof(bellConfig.timeBellSat));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("sun"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(bellConfig.timeBellSun,bellConfig.template0,sizeof(bellConfig.timeBellSun));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(bellConfig.timeBellSun,bellConfig.template1,sizeof(bellConfig.timeBellSun));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(bellConfig.timeBellSun,bellConfig.template2,sizeof(bellConfig.timeBellSun));
      }
    }
    
    
    
    
    

    request->send(LittleFS, "/Templates.html",String() , false , processorTemplates);

    bellConfig.save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
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
  server.on("/Templates.html" , HTTP_GET , handleTemplatesHTML);
  server.on("/welcome.html" , HTTP_GET , handleWelcomeHTML);
  server.on("/About.html" , HTTP_GET , handleAboutHTML);
  server.on("/Functions.html" , HTTP_GET , handleFunctionsHTML);


  server.on("/login" , HTTP_GET , handleLogin);
  server.on("/logout" , HTTP_GET , handleLogout);
  server.on("/bell" , HTTP_GET , handleBell);
  server.on("/alert" , HTTP_GET , handleAlert);
  server.on("/saveAccountSettings" , HTTP_GET , handleSaveAccountSettings);
  server.on("/saveNetworkSettings" , HTTP_GET , handleSaveNetworkSettings);

  server.on("/saveMon" , HTTP_GET , handleSaveMon);
  server.on("/saveTue" , HTTP_GET , handleSaveTue);
  server.on("/saveWed" , HTTP_GET , handleSaveWed);
  server.on("/saveThu" , HTTP_GET , handleSaveThu);
  server.on("/saveFri" , HTTP_GET , handleSaveFri);
  server.on("/saveSat" , HTTP_GET , handleSaveSat);
  server.on("/saveSun" , HTTP_GET , handleSaveSun);
  server.on("/saveTemplate0" , HTTP_GET , handleSaveTemplate0);
  server.on("/saveTemplate1" , HTTP_GET , handleSaveTemplate1);
  server.on("/saveTemplate2" , HTTP_GET , handleSaveTemplate2);
  server.on("/loadTemplate" , HTTP_GET , handleLoadTemplate);


    
    

  server.on("/favicon.ico" , HTTP_GET , handleFavicon );
  server.on("/favicon.png" , HTTP_GET , handleFavicon );

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

