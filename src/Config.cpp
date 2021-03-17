#include <Arduino.h> 
#include "Config.h"

Config::Config()
{
  firstRun = 0;
  port = 80;

  bellDuration = 6;

    wifi_ssid = (char *) "zmienna_1zmienna_1zmienna_1";
    wifi_pass = (char *) "zmienna_2zmienna_2zmienna_2";

    www_user = (char *) "zmienna_3zmienna_3zmienna_3";
    www_pass = (char *) "zmienna_4zmienna_4zmienna_4";
}

void Config::begin()
{  
  if (!LittleFS.exists(fileConfig))
  {
    save();
  }

  load();
  
}

bool toSave = false;

void Config::save()
{ 
  toSave = true;
}

bool Config::mainSave()
{
  if (toSave)
  {
    toSave = false;
    saveConfig();
    saveTemplates();
    saveBells();
    return true;
  }

  return false;
  
}

void Config::load()
{  
  if (!LittleFS.exists(fileConfig) || !LittleFS.exists(fileTemplate0) || !LittleFS.exists(fileMon))
  {
    mainSave();
  }
  loadConfig();
  loadTemplates();
  loadBells();
}

/**
 * 
 * 
 * Config
 * 
 * 
 */

void Config::loadConfig()
{    
      File configFile = LittleFS.open(fileConfig, "r");
    StaticJsonDocument<512> doc;

    DeserializationError error = deserializeJson(doc, configFile);

    if (error)
        Serial.println(F("Failed to read file, using default configuration"));
    
    strlcpy(wifi_ssid,                 
          doc["wifi_ssid"] | "wifi_ssid",  
          50);
    strlcpy(wifi_pass,                 
          doc["wifi_pass"] | "wifi_pass",  
          50);
    strlcpy(www_user,                 
          doc["www_user"] | "www_user",  
          50);
    strlcpy(www_pass,                 
          doc["www_pass"] | "www_pass",  
          50);

    bellDuration = int(doc["duration"]);
    port = int(doc["port"]);
    firstRun = int(doc["run"]);
    
    configFile.flush();
    configFile.close();
}

void Config::saveConfig()
{ 
    StaticJsonDocument<512> doc;
    doc["wifi_ssid"] = wifi_ssid;
    doc["wifi_pass"] = wifi_pass;
    doc["www_user"] = www_user;
    doc["www_pass"] = www_pass; 
    doc["port"] = port;
    doc["duration"] = bellDuration; 
    doc["run"] = firstRun; 

    File configFile = LittleFS.open(fileConfig, "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return;
    }

    serializeJson(doc, configFile);
    configFile.flush();
    configFile.close();
    return;
}

/**
 * 
 * 
 * Templates
 * 
 * 
 */
void Config::loadTemplates()
{ 
  loadTemplate0();
  loadTemplate1();
  loadTemplate2();
  loadTemplate3();
}

void Config::loadTemplate0()
{
    File template0File = LittleFS.open(fileTemplate0, "r");


    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, template0File);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));

    for (int i = 0; i < 22; i++)
    {
        
        template0[i][0] = int(doc["Bell_"+String(i)+"_0"]);
    
        template0[i][1] = int(doc["Bell_"+String(i)+"_1"]);   
    }

    template0File.close();
}

void Config::loadTemplate1()
{
  File configFile = LittleFS.open(fileTemplate1, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);

  if (error)
      Serial.println(F("Failed to read file, using default configuration"));
  
  for (int i = 0; i < 24; i++)
  {
    template1[i][0] = doc[("h_"+String(i))];
    template1[i][1] = doc[("m_"+String(i))];
  }
  
  
  configFile.flush();
  configFile.close();

}

void Config::loadTemplate2()
{
  File configFile = LittleFS.open(fileTemplate2, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);

  if (error)
      Serial.println(F("Failed to read file, using default configuration"));
  
  for (int i = 0; i < 24; i++)
  {
    template2[i][0] = doc[("h_"+String(i))];
    template2[i][1] = doc[("m_"+String(i))];
  }
  
  
  configFile.flush();
  configFile.close();

}

void Config::loadTemplate3()
{
  File configFile = LittleFS.open(fileTemplate3, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);

  if (error)
      Serial.println(F("Failed to read file, using default configuration"));
  
  for (int i = 0; i < 24; i++)
  {
    template3[i][0] = doc[("h_"+String(i))];
    template3[i][1] = doc[("m_"+String(i))];
  }
  
  
  configFile.flush();
  configFile.close();

}

void Config::saveTemplates()
{
  yield();
  saveTemplate0();
  yield();
  saveTemplate1();
  yield();
  saveTemplate2();
  yield();
  saveTemplate3();
  yield();
}

void Config::saveTemplate0()
{
    StaticJsonDocument<2048> doc;
    
    for (int i = 0; i < 22; i++)
    {
        doc["Bell_"+String(i)+"_0"] = template0[i][0];
        doc["Bell_"+String(i)+"_1"] = template0[i][1];
    }
    
    File configFile = LittleFS.open(fileTemplate0, "w");

    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return;
    }

    serializeJson(doc, configFile);
    configFile.close();
    return;
}

void Config::saveTemplate1()
{
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = template1[i][0];
    doc[("m_"+String(i))] = template1[i][1];
  }
  
  File configFile = LittleFS.open(fileTemplate1, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return; 
}

void Config::saveTemplate2()
{
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = template2[i][0];
    doc[("m_"+String(i))] = template2[i][1];
  }
  
  File configFile = LittleFS.open(fileTemplate2, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;
}

void Config::saveTemplate3()
{
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = template3[i][0];
    doc[("m_"+String(i))] = template3[i][1];
  }
  
  File configFile = LittleFS.open(fileTemplate3, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;

}

/**
 * 
 * 
 * Bells
 * 
 * 
 */

void Config::loadBells()
{

}

void Config::loadMon()
{

}

void Config::loadTue()
{

}

void Config::loadWed()
{

}

void Config::loadThu()
{

}

void Config::loadFri()
{

}

void Config::loadSat()
{

}

void Config::loadSun()
{ 

}


void Config::saveBells()
{
  yield();
  saveMon();
  yield();
  saveTue();
  yield();
  saveWed();
  yield();
  saveThu();
  yield();
  saveFri();
  yield();
  saveSat();
  yield();
  saveSun();
  yield();
}

void Config::saveMon()
{ 
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = monBell[i][0];
    doc[("m_"+String(i))] = monBell[i][1];
  }
  
  File configFile = LittleFS.open(fileMon, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;

}

void Config::saveTue()
{ 
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = tueBell[i][0];
    doc[("m_"+String(i))] = tueBell[i][1];
  }
  
  File configFile = LittleFS.open(fileTue, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;

}

void Config::saveWed()
{
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = wedBell[i][0];
    doc[("m_"+String(i))] = wedBell[i][1];
  }
  
  File configFile = LittleFS.open(fileWed, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;
 
}

void Config::saveThu()
{ 
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = thuBell[i][0];
    doc[("m_"+String(i))] = thuBell[i][1];
  }
  
  File configFile = LittleFS.open(fileThu, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;

}

void Config::saveFri()
{ 
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = friBell[i][0];
    doc[("m_"+String(i))] = friBell[i][1];
  }
  
  File configFile = LittleFS.open(fileFri, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;

}

void Config::saveSat()
{ 
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = satBell[i][0];
    doc[("m_"+String(i))] = satBell[i][1];
  }
  
  File configFile = LittleFS.open(fileSat, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;

}

void Config::saveSun()
{ 
  StaticJsonDocument<2048> doc;
  
  for (int i = 0; i < 24; i++)
  {
    doc[("h_"+String(i))] = sunBell[i][0];
    doc[("m_"+String(i))] = sunBell[i][1];
  }
  
  File configFile = LittleFS.open(fileSun, "w");

  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.flush();
  configFile.close();
  return;
}
