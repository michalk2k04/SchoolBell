#include <Arduino.h> 
#include "Config.h"

Config::Config()
{
  firstRun = 0;
  port = 80;

  bellDuration = 6;

  wifi_ssid = (char *) "aaaaaaaaaaaaaaaaaaaaaaaaaaa1";
  wifi_pass = (char *) "aaaaaaaaaaaaaaaaaaaaaaaaaaa2";

  www_user = (char *) "aaaaaaaaaaaaaaaaaaaaaaaaaaa3";
  www_pass = (char *) "aaaaaaaaaaaaaaaaaaaaaaaaaaa4";
}

void Config::begin()
{ 
  load();
}

void Config::save()
{
  EEPROM.begin(512);

  EEPROM.write(0,firstRun);
  
  saveConfig();

  saveTemplates();

  saveBells();

  EEPROM.commit();
  EEPROM.end();

  Serial.println("Config saved ! -"+firstRun); 
}

void Config::load()
{
  if (!LittleFS.exists(fileConfig))
  {
    save();

    EEPROM.begin(512);
    EEPROM.write(0,0);
    EEPROM.end();  
  }
  

  EEPROM.begin(512); 
  firstRun = int(EEPROM.read(0)); 
  EEPROM.end(); 
  
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

bellDuration = int(doc["bellDuration"]);
port = int(doc["port"]);
  
  configFile.close();

}

void Config::saveConfig()
{
  StaticJsonDocument<512> doc;

  doc["wifi_ssid"] = wifi_ssid;
  doc["wifi_pass"] = wifi_pass;
  doc["www_user"] = www_user;
  doc["www_pass"] = www_pass; 
  doc["bellDuration"] = bellDuration; 
  doc["port"] = port; 

  File configFile = LittleFS.open(fileConfig, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
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
  File configFile = LittleFS.open(fileTemplate0, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  
  if (error)
      Serial.println(F("Failed to read file, using default configuration"));

  for (int i = 0; i < 24; i++)
  {
      template0[i][0] = doc["temp_"+String(i)+"_0"];
      template0[i][1] = doc["temp_"+String(i)+"_1"];
  }
    
  configFile.close();
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
      template1[i][0] = doc["temp_"+String(i)+"_0"];
      template1[i][1] = doc["temp_"+String(i)+"_1"];
  }
    
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
      template2[i][0] = doc["temp_"+String(i)+"_0"];
      template2[i][1] = doc["temp_"+String(i)+"_1"];
  }
    
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
      template3[i][0] = doc["temp_"+String(i)+"_0"];
      template3[i][1] = doc["temp_"+String(i)+"_1"];
  }
    
  configFile.close();
}

void Config::saveTemplates()
{
  saveTemplate0();
  saveTemplate1();
  saveTemplate2();
  saveTemplate3();
}
void Config::saveTemplate0()
{
  StaticJsonDocument<4096> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["temp0_"+String(i)+"_0"] = template0[i][0];
    doc["temp0_"+String(i)+"_1"] = template0[i][1];
  }
    

  File configFile = LittleFS.open(fileTemplate0, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 
}

void Config::saveTemplate1()
{
  StaticJsonDocument<4096> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["temp1_"+String(i)+"_0"] = template1[i][0];
    doc["temp1_"+String(i)+"_1"] = template1[i][1];
  }
    

  File configFile = LittleFS.open(fileTemplate1, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 
}

void Config::saveTemplate2()
{
  StaticJsonDocument<4096> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["temp2_"+String(i)+"_0"] = template2[i][0];
    doc["temp2_"+String(i)+"_1"] = template2[i][1];
  }
    

  File configFile = LittleFS.open(fileTemplate2, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 
}

void Config::saveTemplate3()
{
  StaticJsonDocument<4096> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["temp3_"+String(i)+"_0"] = template3[i][0];
    doc["temp3_"+String(i)+"_1"] = template3[i][1];
  }
    

  File configFile = LittleFS.open(fileTemplate3, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 
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
  loadMon();
  loadTue();
  loadWed();
  loadThu();
  loadFri();
  loadSat();
  loadSun();
}

void Config::loadMon()
{
  File configFile = LittleFS.open(fileMon, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  
  if (error)
      Serial.println(F("Failed to read file, using default configuration"));

  for (int i = 0; i < 24; i++)
  {
      monBell[i][0] = doc["mon_"+String(i)+"_0"];
      monBell[i][1] = doc["mon_"+String(i)+"_1"];
  }
    
  configFile.close();
}

void Config::loadTue()
{
  File configFile = LittleFS.open(fileTue, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  
  if (error)
      Serial.println(F("Failed to read file, using default configuration"));

  for (int i = 0; i < 24; i++)
  {
      tueBell[i][0] = doc["tue_"+String(i)+"_0"];
      tueBell[i][1] = doc["tue_"+String(i)+"_1"];
  }
    
  configFile.close();
}

void Config::loadWed()
{
  File configFile = LittleFS.open(fileWed, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  
  if (error)
      Serial.println(F("Failed to read file, using default configuration"));

  for (int i = 0; i < 24; i++)
  {
      wedBell[i][0] = doc["wed_"+String(i)+"_0"];
      wedBell[i][1] = doc["wed_"+String(i)+"_1"];
  }
    
  configFile.close();
}

void Config::loadThu()
{
  File configFile = LittleFS.open(fileThu, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  
  if (error)
      Serial.println(F("Failed to read file, using default configuration"));

  for (int i = 0; i < 24; i++)
  {
      thuBell[i][0] = doc["thu_"+String(i)+"_0"];
      thuBell[i][1] = doc["thu_"+String(i)+"_1"];
  }
    
  configFile.close();
}

void Config::loadFri()
{
  File configFile = LittleFS.open(fileFri, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  
  if (error)
      Serial.println(F("Failed to read file, using default configuration"));

  for (int i = 0; i < 24; i++)
  {
      friBell[i][0] = doc["fri_"+String(i)+"_0"];
      friBell[i][1] = doc["fri_"+String(i)+"_1"];
  }
    
  configFile.close();
}

void Config::loadSat()
{
  File configFile = LittleFS.open(fileSat, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  
  if (error)
      Serial.println(F("Failed to read file, using default configuration"));

  for (int i = 0; i < 24; i++)
  {
      satBell[i][0] = doc["sat_"+String(i)+"_0"];
      satBell[i][1] = doc["sat_"+String(i)+"_1"];
  }
    
  configFile.close();
}

void Config::loadSun()
{
  File configFile = LittleFS.open(fileSun, "r");
  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  
  if (error)
      Serial.println(F("Failed to read file, using default configuration"));

  for (int i = 0; i < 24; i++)
  {
      sunBell[i][0] = doc["sun_"+String(i)+"_0"];
      sunBell[i][1] = doc["sun_"+String(i)+"_1"];
  }
    
  configFile.close();
}


void Config::saveBells()
{
  saveMon();
  saveTue();
  saveWed();
  saveThu();
  saveFri();
  saveSat();
  saveSun();
}

void Config::saveMon()
{
  StaticJsonDocument<2048> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["mon_"+String(i)+"_0"] = monBell[i][0];
    doc["mon_"+String(i)+"_1"] = monBell[i][1];
  }
    

  File configFile = LittleFS.open(fileMon, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 
}

void Config::saveTue()
{
  StaticJsonDocument<2048> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["tue_"+String(i)+"_0"] = tueBell[i][0];
    doc["tue_"+String(i)+"_1"] = tueBell[i][1];
  }
    

  File configFile = LittleFS.open(fileTue, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 
}

void Config::saveWed()
{
  StaticJsonDocument<2048> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["wed_"+String(i)+"_0"] = wedBell[i][0];
    doc["wed_"+String(i)+"_1"] = wedBell[i][1];
  }
    

  File configFile = LittleFS.open(fileWed, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 

}

void Config::saveThu()
{
  StaticJsonDocument<2048> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["thu_"+String(i)+"_0"] = thuBell[i][0];
    doc["thu_"+String(i)+"_1"] = thuBell[i][1];
  }
    

  File configFile = LittleFS.open(fileThu, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 

}

void Config::saveFri()
{
  StaticJsonDocument<2048> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["fri_"+String(i)+"_0"] = friBell[i][0];
    doc["fri_"+String(i)+"_1"] = friBell[i][1];
  }
    

  File configFile = LittleFS.open(fileFri, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 

}

void Config::saveSat()
{
  StaticJsonDocument<2048> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["sat_"+String(i)+"_0"] = satBell[i][0];
    doc["sat_"+String(i)+"_1"] = satBell[i][1];
  }
    

  File configFile = LittleFS.open(fileSat, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 

}

void Config::saveSun()
{
  StaticJsonDocument<2048> doc;

  for (int i = 0; i < 24; i++)
  {
    doc["sun_"+String(i)+"_0"] = sunBell[i][0];
    doc["sun_"+String(i)+"_1"] = sunBell[i][1];
  }
    

  File configFile = LittleFS.open(fileSun, "w");
  if (!configFile) {
      Serial.println("Failed to open config file for writing");
      return;
  }

  serializeJson(doc, configFile);
  configFile.close(); 

}
