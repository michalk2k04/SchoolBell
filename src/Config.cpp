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
    przestawienieCzasu = int(doc["przestawienie"]);
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
    doc["przestawienie"] = przestawienieCzasu;

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
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileTemplate0, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    template0[i][0] = doc["bell"+String(i)+"h"] | -2;
    template0[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadTemplate1()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileTemplate1, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    template1[i][0] = doc["bell"+String(i)+"h"] | -2;
    template1[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadTemplate2()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileTemplate2, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    template2[i][0] = doc["bell"+String(i)+"h"] | -2;
    template2[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadTemplate3()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileTemplate3, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    template3[i][0] = doc["bell"+String(i)+"h"] | -2;
    template3[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
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
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileTemplate0);

  File file = LittleFS.open(fileTemplate0, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = template0[i][0];
    doc["bell"+String(i)+"m"] = template0[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveTemplate1()
{
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileTemplate1);

  File file = LittleFS.open(fileTemplate1, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = template1[i][0];
    doc["bell"+String(i)+"m"] = template1[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveTemplate2()
{
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileTemplate2);

  File file = LittleFS.open(fileTemplate2, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = template2[i][0];
    doc["bell"+String(i)+"m"] = template2[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveTemplate3()
{
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileTemplate3);

  File file = LittleFS.open(fileTemplate3, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = template3[i][0];
    doc["bell"+String(i)+"m"] = template3[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
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
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileMon, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    monBell[i][0] = doc["bell"+String(i)+"h"] | -2;
    monBell[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadTue()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileTue, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    tueBell[i][0] = doc["bell"+String(i)+"h"] | -2;
    tueBell[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadWed()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileWed, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    wedBell[i][0] = doc["bell"+String(i)+"h"] | -2;
    wedBell[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadThu()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileThu, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    thuBell[i][0] = doc["bell"+String(i)+"h"] | -2;
    thuBell[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadFri()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileFri, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    friBell[i][0] = doc["bell"+String(i)+"h"] | -2;
    friBell[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadSat()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileSat, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    satBell[i][0] = doc["bell"+String(i)+"h"] | -2;
    satBell[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
}

void Config::loadSun()
{
  DynamicJsonDocument doc(1536);

  File file = LittleFS.open(fileSun, "r");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    sunBell[i][0] = doc["bell"+String(i)+"h"] | -2;
    sunBell[i][1] = doc["bell"+String(i)+"m"] | -2;
  } 

  file.close();
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
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileMon);

  File file = LittleFS.open(fileMon, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = monBell[i][0];
    doc["bell"+String(i)+"m"] = monBell[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveTue()
{
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileTue);

  File file = LittleFS.open(fileTue, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = tueBell[i][0];
    doc["bell"+String(i)+"m"] = tueBell[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveWed()
{
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileWed);

  File file = LittleFS.open(fileWed, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = wedBell[i][0];
    doc["bell"+String(i)+"m"] = wedBell[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveThu()
{

  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileThu);

  File file = LittleFS.open(fileThu, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = thuBell[i][0];
    doc["bell"+String(i)+"m"] = thuBell[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveFri()
{
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileFri);

  File file = LittleFS.open(fileFri, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = friBell[i][0];
    doc["bell"+String(i)+"m"] = friBell[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveSat()
{
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileSat);

  File file = LittleFS.open(fileSat, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = satBell[i][0];
    doc["bell"+String(i)+"m"] = satBell[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void Config::saveSun()
{
  DynamicJsonDocument doc(1536);
  
  LittleFS.remove(fileSun);

  File file = LittleFS.open(fileSun, "w");

  if (!file) 
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  for (int i = 0; i < 24; i++)
  {
    doc["bell"+String(i)+"h"] = sunBell[i][0];
    doc["bell"+String(i)+"m"] = sunBell[i][1];
  } 

  if (serializeJson(doc, file) == 0) 
  {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}
