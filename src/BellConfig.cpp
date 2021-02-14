#include "BellConfig.h"

/**
 * 0 - firstRun
 * 1 - httpPort
 * 
 * 
 *  
 * 
    const char* wifi_ssid;
    const char* wifi_pass;
    
    const char* www_username;
    const char* www_pass; 
 * 
 */

BellConfig::BellConfig()
{
    firstRun = 0;
    httpPort = 80;

    wifi_ssid = (char *) "zmienna_1zmienna_1zmienna_1";
    wifi_pass = (char *) "zmienna_2zmienna_2zmienna_2";

    www_username = (char *) "zmienna_3zmienna_3zmienna_3";
    www_pass = (char *) "zmienna_4zmienna_4zmienna_4";


}

std::string myReadSerialStr() {
  std::string str = "";
  while (Serial.available () > 0) {
    str += char(Serial.read ());
  }
  return str;
}

void BellConfig::runSerialConfiguration()
{
    bool end = false;

    int x = 0;


    
    Serial.print("Wprowadz ssid wifi : "); 
    while (!end)
    {
        if (Serial.available())
        {
            String str = Serial.readString();
            int dlugosc = str.length();
 
            Serial.println(str);
            switch (x)
            {
            case 0:
                str.toCharArray(wifi_ssid, dlugosc);

                Serial.print("Wprowadz hasło wifi : "); 
                break;
            case 1:
                str.toCharArray(wifi_pass, dlugosc);  

                Serial.print("Wprowadz nową nazwe użytkownika : ");
                break;
            case 2:
                str.toCharArray(www_username, dlugosc); 

                Serial.print("Wprowadz nowe hasło www : ");
                break;
            case 3:
                str.toCharArray(www_pass, dlugosc); 

                Serial.print("Wprowadz port http : ");
                break;
            case 4:
                httpPort = str.toInt();
                end = true;
                EEPROM.write(0,1);
                firstRun = 1;
                break;   
            default:
                break;
            }
            x++;
        }
        
     }

     Serial.println("=========================================");
     Serial.print("wifi_ssid = ");Serial.println(wifi_ssid);
     Serial.print("wifi_pass = ");Serial.println(wifi_pass);
     Serial.print("www_username = ");Serial.println(www_username);
     Serial.print("www_pass = ");Serial.println(www_pass);
     Serial.print("httpPort = ");Serial.println(httpPort);
     Serial.println("=========================================");
     
}

void BellConfig::begin()
{ 

    EEPROM.begin(512);
    firstRun = EEPROM.read(0);
    httpPort = EEPROM.read(1);

    if (firstRun != 1)
    {
        firstRun = 1; 

        runSerialConfiguration(); 
 
        EEPROM.write(0,1);

        save();
    } 

    EEPROM.end();
    load();

}

void BellConfig::load()
{
    EEPROM.begin(512);
    firstRun = EEPROM.read(0);
    httpPort = EEPROM.read(1);
    loadConfig();
    loadBells();
    EEPROM.end();
}

void BellConfig::save()
{

    EEPROM.begin(512);
    EEPROM.write(0,firstRun);
    EEPROM.write(1,httpPort);
    saveConfig();
    saveBells();
  EEPROM.end();
}

void BellConfig::loadConfig()
{   
    File configFile = LittleFS.open(fileConfig, "r");
    StaticJsonDocument<512> doc;

    DeserializationError error = deserializeJson(doc, configFile);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
    
    strlcpy(wifi_ssid,                 
          doc["wifi_ssid"] | "wifi_ssid",  
          50);
    strlcpy(wifi_pass,                 
          doc["wifi_pass"] | "wifi_pass",  
          50);
    strlcpy(www_username,                 
          doc["www_username"] | "www_username",  
          50);
    strlcpy(www_pass,                 
          doc["www_pass"] | "www_pass",  
          50);
    
    configFile.close();

}

void BellConfig::saveConfig()
{
    StaticJsonDocument<200> doc;
    doc["wifi_ssid"] = wifi_ssid;
    doc["wifi_pass"] = wifi_pass;
    doc["www_username"] = www_username;
    doc["www_pass"] = www_pass; 

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
 * BELLSSSSSSS.....................
 * 
 */

void BellConfig::loadBells()
{
    loadMon();
    loadTue();
    loadWed();
    loadThu();
    loadFri();
    loadSat();
    loadSun();
}

void BellConfig::saveBells()
{
    saveMon();
    saveTue();
    saveWed();
    saveThu();
    saveFri();
    saveSat();
    saveSun();
}

void BellConfig::loadMon()
{
    File monFile = LittleFS.open(fileMon, "r");


    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, monFile);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));


    timeBellMon[0][0] = int(doc["Bell_0_0"]);
    timeBellMon[0][1] = int(doc["Bell_0_1"]);
    timeBellMon[1][0] = int(doc["Bell_1_0"]);
    timeBellMon[1][1] = int(doc["Bell_1_1"]);
    timeBellMon[2][0] = int(doc["Bell_2_0"]);
    timeBellMon[2][1] = int(doc["Bell_2_1"]);
    timeBellMon[3][0] = int(doc["Bell_3_0"]);
    timeBellMon[3][1] = int(doc["Bell_3_1"]);
    timeBellMon[4][0] = int(doc["Bell_4_0"]);
    timeBellMon[4][1] = int(doc["Bell_4_1"]);
    timeBellMon[5][0] = int(doc["Bell_5_0"]);
    timeBellMon[5][1] = int(doc["Bell_5_1"]);
    timeBellMon[6][0] = int(doc["Bell_6_0"]);
    timeBellMon[6][1] = int(doc["Bell_6_1"]);
    timeBellMon[7][0] = int(doc["Bell_7_0"]);
    timeBellMon[7][1] = int(doc["Bell_7_1"]);
    timeBellMon[8][0] = int(doc["Bell_8_0"]);
    timeBellMon[8][1] = int(doc["Bell_8_1"]);
    timeBellMon[9][0] = int(doc["Bell_9_0"]);
    timeBellMon[9][1] = int(doc["Bell_9_1"]);
    timeBellMon[10][0] = int(doc["Bell_10_0"]);
    timeBellMon[10][1] = int(doc["Bell_10_1"]);
    timeBellMon[11][0] = int(doc["Bell_11_0"]);
    timeBellMon[11][1] = int(doc["Bell_11_1"]);
    timeBellMon[12][0] = int(doc["Bell_12_0"]);
    timeBellMon[12][1] = int(doc["Bell_12_1"]);
    timeBellMon[13][0] = int(doc["Bell_13_0"]);
    timeBellMon[13][1] = int(doc["Bell_13_1"]);
    timeBellMon[14][0] = int(doc["Bell_14_0"]);
    timeBellMon[14][1] = int(doc["Bell_14_1"]);
    timeBellMon[15][0] = int(doc["Bell_15_0"]);
    timeBellMon[15][1] = int(doc["Bell_15_1"]);
    timeBellMon[16][0] = int(doc["Bell_16_0"]);
    timeBellMon[16][1] = int(doc["Bell_16_1"]);
    timeBellMon[17][0] = int(doc["Bell_17_0"]);
    timeBellMon[17][1] = int(doc["Bell_17_1"]);
    timeBellMon[18][0] = int(doc["Bell_18_0"]);
    timeBellMon[18][1] = int(doc["Bell_18_1"]);
    timeBellMon[19][0] = int(doc["Bell_19_0"]);
    timeBellMon[19][1] = int(doc["Bell_19_1"]);
    timeBellMon[20][0] = int(doc["Bell_20_0"]);
    timeBellMon[20][1] = int(doc["Bell_20_1"]);
    timeBellMon[21][0] = int(doc["Bell_21_0"]);
    timeBellMon[21][1] = int(doc["Bell_21_1"]);

    monFile.close();
}

void BellConfig::loadTue()
{
    File tueFile = LittleFS.open(fileTue, "r");



    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, tueFile);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));


    timeBellTue[0][0] = int(doc["Bell_0_0"]);
    timeBellTue[0][1] = int(doc["Bell_0_1"]);
    timeBellTue[1][0] = int(doc["Bell_1_0"]);
    timeBellTue[1][1] = int(doc["Bell_1_1"]);
    timeBellTue[2][0] = int(doc["Bell_2_0"]);
    timeBellTue[2][1] = int(doc["Bell_2_1"]);
    timeBellTue[3][0] = int(doc["Bell_3_0"]);
    timeBellTue[3][1] = int(doc["Bell_3_1"]);
    timeBellTue[4][0] = int(doc["Bell_4_0"]);
    timeBellTue[4][1] = int(doc["Bell_4_1"]);
    timeBellTue[5][0] = int(doc["Bell_5_0"]);
    timeBellTue[5][1] = int(doc["Bell_5_1"]);
    timeBellTue[6][0] = int(doc["Bell_6_0"]);
    timeBellTue[6][1] = int(doc["Bell_6_1"]);
    timeBellTue[7][0] = int(doc["Bell_7_0"]);
    timeBellTue[7][1] = int(doc["Bell_7_1"]);
    timeBellTue[8][0] = int(doc["Bell_8_0"]);
    timeBellTue[8][1] = int(doc["Bell_8_1"]);
    timeBellTue[9][0] = int(doc["Bell_9_0"]);
    timeBellTue[9][1] = int(doc["Bell_9_1"]);
    timeBellTue[10][0] = int(doc["Bell_10_0"]);
    timeBellTue[10][1] = int(doc["Bell_10_1"]);
    timeBellTue[11][0] = int(doc["Bell_11_0"]);
    timeBellTue[11][1] = int(doc["Bell_11_1"]);
    timeBellTue[12][0] = int(doc["Bell_12_0"]);
    timeBellTue[12][1] = int(doc["Bell_12_1"]);
    timeBellTue[13][0] = int(doc["Bell_13_0"]);
    timeBellTue[13][1] = int(doc["Bell_13_1"]);
    timeBellTue[14][0] = int(doc["Bell_14_0"]);
    timeBellTue[14][1] = int(doc["Bell_14_1"]);
    timeBellTue[15][0] = int(doc["Bell_15_0"]);
    timeBellTue[15][1] = int(doc["Bell_15_1"]);
    timeBellTue[16][0] = int(doc["Bell_16_0"]);
    timeBellTue[16][1] = int(doc["Bell_16_1"]);
    timeBellTue[17][0] = int(doc["Bell_17_0"]);
    timeBellTue[17][1] = int(doc["Bell_17_1"]);
    timeBellTue[18][0] = int(doc["Bell_18_0"]);
    timeBellTue[18][1] = int(doc["Bell_18_1"]);
    timeBellTue[19][0] = int(doc["Bell_19_0"]);
    timeBellTue[19][1] = int(doc["Bell_19_1"]);
    timeBellTue[20][0] = int(doc["Bell_20_0"]);
    timeBellTue[20][1] = int(doc["Bell_20_1"]);
    timeBellTue[21][0] = int(doc["Bell_21_0"]);
    timeBellTue[21][1] = int(doc["Bell_21_1"]);

    tueFile.close();
}

void BellConfig::loadWed()
{
    File wedFile = LittleFS.open(fileWed, "r");


    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, wedFile);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));



    timeBellWed[0][0] = int(doc["Bell_0_0"]);
    timeBellWed[0][1] = int(doc["Bell_0_1"]);
    timeBellWed[1][0] = int(doc["Bell_1_0"]);
    timeBellWed[1][1] = int(doc["Bell_1_1"]);
    timeBellWed[2][0] = int(doc["Bell_2_0"]);
    timeBellWed[2][1] = int(doc["Bell_2_1"]);
    timeBellWed[3][0] = int(doc["Bell_3_0"]);
    timeBellWed[3][1] = int(doc["Bell_3_1"]);
    timeBellWed[4][0] = int(doc["Bell_4_0"]);
    timeBellWed[4][1] = int(doc["Bell_4_1"]);
    timeBellWed[5][0] = int(doc["Bell_5_0"]);
    timeBellWed[5][1] = int(doc["Bell_5_1"]);
    timeBellWed[6][0] = int(doc["Bell_6_0"]);
    timeBellWed[6][1] = int(doc["Bell_6_1"]);
    timeBellWed[7][0] = int(doc["Bell_7_0"]);
    timeBellWed[7][1] = int(doc["Bell_7_1"]);
    timeBellWed[8][0] = int(doc["Bell_8_0"]);
    timeBellWed[8][1] = int(doc["Bell_8_1"]);
    timeBellWed[9][0] = int(doc["Bell_9_0"]);
    timeBellWed[9][1] = int(doc["Bell_9_1"]);
    timeBellWed[10][0] = int(doc["Bell_10_0"]);
    timeBellWed[10][1] = int(doc["Bell_10_1"]);
    timeBellWed[11][0] = int(doc["Bell_11_0"]);
    timeBellWed[11][1] = int(doc["Bell_11_1"]);
    timeBellWed[12][0] = int(doc["Bell_12_0"]);
    timeBellWed[12][1] = int(doc["Bell_12_1"]);
    timeBellWed[13][0] = int(doc["Bell_13_0"]);
    timeBellWed[13][1] = int(doc["Bell_13_1"]);
    timeBellWed[14][0] = int(doc["Bell_14_0"]);
    timeBellWed[14][1] = int(doc["Bell_14_1"]);
    timeBellWed[15][0] = int(doc["Bell_15_0"]);
    timeBellWed[15][1] = int(doc["Bell_15_1"]);
    timeBellWed[16][0] = int(doc["Bell_16_0"]);
    timeBellWed[16][1] = int(doc["Bell_16_1"]);
    timeBellWed[17][0] = int(doc["Bell_17_0"]);
    timeBellWed[17][1] = int(doc["Bell_17_1"]);
    timeBellWed[18][0] = int(doc["Bell_18_0"]);
    timeBellWed[18][1] = int(doc["Bell_18_1"]);
    timeBellWed[19][0] = int(doc["Bell_19_0"]);
    timeBellWed[19][1] = int(doc["Bell_19_1"]);
    timeBellWed[20][0] = int(doc["Bell_20_0"]);
    timeBellWed[20][1] = int(doc["Bell_20_1"]);
    timeBellWed[21][0] = int(doc["Bell_21_0"]);
    timeBellWed[21][1] = int(doc["Bell_21_1"]);

    wedFile.close();
}


void BellConfig::loadThu()
{
    File thuFile = LittleFS.open(fileThu, "r");

 

    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, thuFile);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));


    timeBellThu[0][0] = int(doc["Bell_0_0"]);
    timeBellThu[0][1] = int(doc["Bell_0_1"]);
    timeBellThu[1][0] = int(doc["Bell_1_0"]);
    timeBellThu[1][1] = int(doc["Bell_1_1"]);
    timeBellThu[2][0] = int(doc["Bell_2_0"]);
    timeBellThu[2][1] = int(doc["Bell_2_1"]);
    timeBellThu[3][0] = int(doc["Bell_3_0"]);
    timeBellThu[3][1] = int(doc["Bell_3_1"]);
    timeBellThu[4][0] = int(doc["Bell_4_0"]);
    timeBellThu[4][1] = int(doc["Bell_4_1"]);
    timeBellThu[5][0] = int(doc["Bell_5_0"]);
    timeBellThu[5][1] = int(doc["Bell_5_1"]);
    timeBellThu[6][0] = int(doc["Bell_6_0"]);
    timeBellThu[6][1] = int(doc["Bell_6_1"]);
    timeBellThu[7][0] = int(doc["Bell_7_0"]);
    timeBellThu[7][1] = int(doc["Bell_7_1"]);
    timeBellThu[8][0] = int(doc["Bell_8_0"]);
    timeBellThu[8][1] = int(doc["Bell_8_1"]);
    timeBellThu[9][0] = int(doc["Bell_9_0"]);
    timeBellThu[9][1] = int(doc["Bell_9_1"]);
    timeBellThu[10][0] = int(doc["Bell_10_0"]);
    timeBellThu[10][1] = int(doc["Bell_10_1"]);
    timeBellThu[11][0] = int(doc["Bell_11_0"]);
    timeBellThu[11][1] = int(doc["Bell_11_1"]);
    timeBellThu[12][0] = int(doc["Bell_12_0"]);
    timeBellThu[12][1] = int(doc["Bell_12_1"]);
    timeBellThu[13][0] = int(doc["Bell_13_0"]);
    timeBellThu[13][1] = int(doc["Bell_13_1"]);
    timeBellThu[14][0] = int(doc["Bell_14_0"]);
    timeBellThu[14][1] = int(doc["Bell_14_1"]);
    timeBellThu[15][0] = int(doc["Bell_15_0"]);
    timeBellThu[15][1] = int(doc["Bell_15_1"]);
    timeBellThu[16][0] = int(doc["Bell_16_0"]);
    timeBellThu[16][1] = int(doc["Bell_16_1"]);
    timeBellThu[17][0] = int(doc["Bell_17_0"]);
    timeBellThu[17][1] = int(doc["Bell_17_1"]);
    timeBellThu[18][0] = int(doc["Bell_18_0"]);
    timeBellThu[18][1] = int(doc["Bell_18_1"]);
    timeBellThu[19][0] = int(doc["Bell_19_0"]);
    timeBellThu[19][1] = int(doc["Bell_19_1"]);
    timeBellThu[20][0] = int(doc["Bell_20_0"]);
    timeBellThu[20][1] = int(doc["Bell_20_1"]);
    timeBellThu[21][0] = int(doc["Bell_21_0"]);
    timeBellThu[21][1] = int(doc["Bell_21_1"]);

    thuFile.close();
}


void BellConfig::loadFri()
{
    File friFile = LittleFS.open(fileFri, "r");

    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, friFile);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));
 
    
    timeBellFri[0][0] = int(doc["Bell_0_0"]);
    timeBellFri[0][1] = int(doc["Bell_0_1"]);       

    timeBellFri[1][0] = int(doc["Bell_1_0"]);
    timeBellFri[1][1] = int(doc["Bell_1_1"]);
    timeBellFri[2][0] = int(doc["Bell_2_0"]);
    timeBellFri[2][1] = int(doc["Bell_2_1"]);
    timeBellFri[3][0] = int(doc["Bell_3_0"]);
    timeBellFri[3][1] = int(doc["Bell_3_1"]);
    timeBellFri[4][0] = int(doc["Bell_4_0"]);
    timeBellFri[4][1] = int(doc["Bell_4_1"]);
    timeBellFri[5][0] = int(doc["Bell_5_0"]);
    timeBellFri[5][1] = int(doc["Bell_5_1"]);
    timeBellFri[6][0] = int(doc["Bell_6_0"]);
    timeBellFri[6][1] = int(doc["Bell_6_1"]);
    timeBellFri[7][0] = int(doc["Bell_7_0"]);
    timeBellFri[7][1] = int(doc["Bell_7_1"]);
    timeBellFri[8][0] = int(doc["Bell_8_0"]);
    timeBellFri[8][1] = int(doc["Bell_8_1"]);
    timeBellFri[9][0] = int(doc["Bell_9_0"]);
    timeBellFri[9][1] = int(doc["Bell_9_1"]);
    timeBellFri[10][0] = int(doc["Bell_10_0"]);
    timeBellFri[10][1] = int(doc["Bell_10_1"]);
    timeBellFri[11][0] = int(doc["Bell_11_0"]);
    timeBellFri[11][1] = int(doc["Bell_11_1"]);
    timeBellFri[12][0] = int(doc["Bell_12_0"]);
    timeBellFri[12][1] = int(doc["Bell_12_1"]);
    timeBellFri[13][0] = int(doc["Bell_13_0"]);
    timeBellFri[13][1] = int(doc["Bell_13_1"]);
    timeBellFri[14][0] = int(doc["Bell_14_0"]);
    timeBellFri[14][1] = int(doc["Bell_14_1"]);
    timeBellFri[15][0] = int(doc["Bell_15_0"]);
    timeBellFri[15][1] = int(doc["Bell_15_1"]);
    timeBellFri[16][0] = int(doc["Bell_16_0"]);
    timeBellFri[16][1] = int(doc["Bell_16_1"]);
    timeBellFri[17][0] = int(doc["Bell_17_0"]);
    timeBellFri[17][1] = int(doc["Bell_17_1"]);
    timeBellFri[18][0] = int(doc["Bell_18_0"]);
    timeBellFri[18][1] = int(doc["Bell_18_1"]);
    timeBellFri[19][0] = int(doc["Bell_19_0"]);
    timeBellFri[19][1] = int(doc["Bell_19_1"]);
    timeBellFri[20][0] = int(doc["Bell_20_0"]);
    timeBellFri[20][1] = int(doc["Bell_20_1"]);
    timeBellFri[21][0] = int(doc["Bell_21_0"]);
    timeBellFri[21][1] = int(doc["Bell_21_1"]);

    friFile.close();
}


void BellConfig::loadSat()
{
    File satFile = LittleFS.open(fileSat, "r");


    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, satFile);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));


    timeBellSat[0][0] = int(doc["Bell_0_0"]);
    timeBellSat[0][1] = int(doc["Bell_0_1"]);
    timeBellSat[1][0] = int(doc["Bell_1_0"]);
    timeBellSat[1][1] = int(doc["Bell_1_1"]);
    timeBellSat[2][0] = int(doc["Bell_2_0"]);
    timeBellSat[2][1] = int(doc["Bell_2_1"]);
    timeBellSat[3][0] = int(doc["Bell_3_0"]);
    timeBellSat[3][1] = int(doc["Bell_3_1"]);
    timeBellSat[4][0] = int(doc["Bell_4_0"]);
    timeBellSat[4][1] = int(doc["Bell_4_1"]);
    timeBellSat[5][0] = int(doc["Bell_5_0"]);
    timeBellSat[5][1] = int(doc["Bell_5_1"]);
    timeBellSat[6][0] = int(doc["Bell_6_0"]);
    timeBellSat[6][1] = int(doc["Bell_6_1"]);
    timeBellSat[7][0] = int(doc["Bell_7_0"]);
    timeBellSat[7][1] = int(doc["Bell_7_1"]);
    timeBellSat[8][0] = int(doc["Bell_8_0"]);
    timeBellSat[8][1] = int(doc["Bell_8_1"]);
    timeBellSat[9][0] = int(doc["Bell_9_0"]);
    timeBellSat[9][1] = int(doc["Bell_9_1"]);
    timeBellSat[10][0] = int(doc["Bell_10_0"]);
    timeBellSat[10][1] = int(doc["Bell_10_1"]);
    timeBellSat[11][0] = int(doc["Bell_11_0"]);
    timeBellSat[11][1] = int(doc["Bell_11_1"]);
    timeBellSat[12][0] = int(doc["Bell_12_0"]);
    timeBellSat[12][1] = int(doc["Bell_12_1"]);
    timeBellSat[13][0] = int(doc["Bell_13_0"]);
    timeBellSat[13][1] = int(doc["Bell_13_1"]);
    timeBellSat[14][0] = int(doc["Bell_14_0"]);
    timeBellSat[14][1] = int(doc["Bell_14_1"]);
    timeBellSat[15][0] = int(doc["Bell_15_0"]);
    timeBellSat[15][1] = int(doc["Bell_15_1"]);
    timeBellSat[16][0] = int(doc["Bell_16_0"]);
    timeBellSat[16][1] = int(doc["Bell_16_1"]);
    timeBellSat[17][0] = int(doc["Bell_17_0"]);
    timeBellSat[17][1] = int(doc["Bell_17_1"]);
    timeBellSat[18][0] = int(doc["Bell_18_0"]);
    timeBellSat[18][1] = int(doc["Bell_18_1"]);
    timeBellSat[19][0] = int(doc["Bell_19_0"]);
    timeBellSat[19][1] = int(doc["Bell_19_1"]);
    timeBellSat[20][0] = int(doc["Bell_20_0"]);
    timeBellSat[20][1] = int(doc["Bell_20_1"]);
    timeBellSat[21][0] = int(doc["Bell_21_0"]);
    timeBellSat[21][1] = int(doc["Bell_21_1"]);

    satFile.close();
}


void BellConfig::loadSun()
{
    File sunFile = LittleFS.open(fileSun, "r");


    StaticJsonDocument<2048> doc;

    DeserializationError error = deserializeJson(doc, sunFile);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));


    timeBellSun[0][0] = int(doc["Bell_0_0"]);
    timeBellSun[0][1] = int(doc["Bell_0_1"]);
    timeBellSun[1][0] = int(doc["Bell_1_0"]);
    timeBellSun[1][1] = int(doc["Bell_1_1"]);
    timeBellSun[2][0] = int(doc["Bell_2_0"]);
    timeBellSun[2][1] = int(doc["Bell_2_1"]);
    timeBellSun[3][0] = int(doc["Bell_3_0"]);
    timeBellSun[3][1] = int(doc["Bell_3_1"]);
    timeBellSun[4][0] = int(doc["Bell_4_0"]);
    timeBellSun[4][1] = int(doc["Bell_4_1"]);
    timeBellSun[5][0] = int(doc["Bell_5_0"]);
    timeBellSun[5][1] = int(doc["Bell_5_1"]);
    timeBellSun[6][0] = int(doc["Bell_6_0"]);
    timeBellSun[6][1] = int(doc["Bell_6_1"]);
    timeBellSun[7][0] = int(doc["Bell_7_0"]);
    timeBellSun[7][1] = int(doc["Bell_7_1"]);
    timeBellSun[8][0] = int(doc["Bell_8_0"]);
    timeBellSun[8][1] = int(doc["Bell_8_1"]);
    timeBellSun[9][0] = int(doc["Bell_9_0"]);
    timeBellSun[9][1] = int(doc["Bell_9_1"]);
    timeBellSun[10][0] = int(doc["Bell_10_0"]);
    timeBellSun[10][1] = int(doc["Bell_10_1"]);
    timeBellSun[11][0] = int(doc["Bell_11_0"]);
    timeBellSun[11][1] = int(doc["Bell_11_1"]);
    timeBellSun[12][0] = int(doc["Bell_12_0"]);
    timeBellSun[12][1] = int(doc["Bell_12_1"]);
    timeBellSun[13][0] = int(doc["Bell_13_0"]);
    timeBellSun[13][1] = int(doc["Bell_13_1"]);
    timeBellSun[14][0] = int(doc["Bell_14_0"]);
    timeBellSun[14][1] = int(doc["Bell_14_1"]);
    timeBellSun[15][0] = int(doc["Bell_15_0"]);
    timeBellSun[15][1] = int(doc["Bell_15_1"]);
    timeBellSun[16][0] = int(doc["Bell_16_0"]);
    timeBellSun[16][1] = int(doc["Bell_16_1"]);
    timeBellSun[17][0] = int(doc["Bell_17_0"]);
    timeBellSun[17][1] = int(doc["Bell_17_1"]);
    timeBellSun[18][0] = int(doc["Bell_18_0"]);
    timeBellSun[18][1] = int(doc["Bell_18_1"]);
    timeBellSun[19][0] = int(doc["Bell_19_0"]);
    timeBellSun[19][1] = int(doc["Bell_19_1"]);
    timeBellSun[20][0] = int(doc["Bell_20_0"]);
    timeBellSun[20][1] = int(doc["Bell_20_1"]);
    timeBellSun[21][0] = int(doc["Bell_21_0"]);
    timeBellSun[21][1] = int(doc["Bell_21_1"]);

    sunFile.close();
}




void BellConfig::saveMon()
{
    StaticJsonDocument<2048> doc;

    doc["Bell_0_0"] = timeBellMon[0][0];
    doc["Bell_0_1"] = timeBellMon[0][1];
    doc["Bell_1_0"] = timeBellMon[1][0];
    doc["Bell_1_1"] = timeBellMon[1][1];
    doc["Bell_2_0"] = timeBellMon[2][0];
    doc["Bell_2_1"] = timeBellMon[2][1];
    doc["Bell_3_0"] = timeBellMon[3][0];
    doc["Bell_3_1"] = timeBellMon[3][1];
    doc["Bell_4_0"] = timeBellMon[4][0];
    doc["Bell_4_1"] = timeBellMon[4][1];
    doc["Bell_5_0"] = timeBellMon[5][0];
    doc["Bell_5_1"] = timeBellMon[5][1];
    doc["Bell_6_0"] = timeBellMon[6][0];
    doc["Bell_6_1"] = timeBellMon[6][1];
    doc["Bell_7_0"] = timeBellMon[7][0];
    doc["Bell_7_1"] = timeBellMon[7][1];
    doc["Bell_8_0"] = timeBellMon[8][0];
    doc["Bell_8_1"] = timeBellMon[8][1];
    doc["Bell_9_0"] = timeBellMon[9][0];
    doc["Bell_9_1"] = timeBellMon[9][1];
    doc["Bell_10_0"] = timeBellMon[10][0];
    doc["Bell_10_1"] = timeBellMon[10][1];
    doc["Bell_11_0"] = timeBellMon[11][0];
    doc["Bell_11_1"] = timeBellMon[11][1];
    doc["Bell_12_0"] = timeBellMon[12][0];
    doc["Bell_12_1"] = timeBellMon[12][1];
    doc["Bell_13_0"] = timeBellMon[13][0];
    doc["Bell_13_1"] = timeBellMon[13][1];
    doc["Bell_14_0"] = timeBellMon[14][0];
    doc["Bell_14_1"] = timeBellMon[14][1];
    doc["Bell_15_0"] = timeBellMon[15][0];
    doc["Bell_15_1"] = timeBellMon[15][1];
    doc["Bell_16_0"] = timeBellMon[16][0];
    doc["Bell_16_1"] = timeBellMon[16][1];
    doc["Bell_17_0"] = timeBellMon[17][0];
    doc["Bell_17_1"] = timeBellMon[17][1];
    doc["Bell_18_0"] = timeBellMon[18][0];
    doc["Bell_18_1"] = timeBellMon[18][1];
    doc["Bell_19_0"] = timeBellMon[19][0];
    doc["Bell_19_1"] = timeBellMon[19][1];
    doc["Bell_20_0"] = timeBellMon[20][0];
    doc["Bell_20_1"] =timeBellMon[20][1];
    doc["Bell_21_0"] = timeBellMon[21][0];
    doc["Bell_21_1"] = timeBellMon[21][1];


    File configFile = LittleFS.open(fileMon, "w");
    if (!configFile) 
    {
        Serial.println("Failed to open config file for writing");
        return;
    }

  serializeJson(doc, configFile);

  configFile.close();
}

void BellConfig::saveTue()
{
    StaticJsonDocument<2048> doc;

    doc["Bell_0_0"] = timeBellTue[0][0];
    doc["Bell_0_1"] = timeBellTue[0][1];
    doc["Bell_1_0"] = timeBellTue[1][0];
    doc["Bell_1_1"] = timeBellTue[1][1];
    doc["Bell_2_0"] = timeBellTue[2][0];
    doc["Bell_2_1"] = timeBellTue[2][1];
    doc["Bell_3_0"] = timeBellTue[3][0];
    doc["Bell_3_1"] = timeBellTue[3][1];
    doc["Bell_4_0"] = timeBellTue[4][0];
    doc["Bell_4_1"] = timeBellTue[4][1];
    doc["Bell_5_0"] = timeBellTue[5][0];
    doc["Bell_5_1"] = timeBellTue[5][1];
    doc["Bell_6_0"] = timeBellTue[6][0];
    doc["Bell_6_1"] = timeBellTue[6][1];
    doc["Bell_7_0"] = timeBellTue[7][0];
    doc["Bell_7_1"] = timeBellTue[7][1];
    doc["Bell_8_0"] = timeBellTue[8][0];
    doc["Bell_8_1"] = timeBellTue[8][1];
    doc["Bell_9_0"] = timeBellTue[9][0];
    doc["Bell_9_1"] = timeBellTue[9][1];
    doc["Bell_10_0"] = timeBellTue[10][0];
    doc["Bell_10_1"] = timeBellTue[10][1];
    doc["Bell_11_0"] = timeBellTue[11][0];
    doc["Bell_11_1"] = timeBellTue[11][1];
    doc["Bell_12_0"] = timeBellTue[12][0];
    doc["Bell_12_1"] = timeBellTue[12][1];
    doc["Bell_13_0"] = timeBellTue[13][0];
    doc["Bell_13_1"] = timeBellTue[13][1];
    doc["Bell_14_0"] = timeBellTue[14][0];
    doc["Bell_14_1"] = timeBellTue[14][1];
    doc["Bell_15_0"] = timeBellTue[15][0];
    doc["Bell_15_1"] = timeBellTue[15][1];
    doc["Bell_16_0"] = timeBellTue[16][0];
    doc["Bell_16_1"] = timeBellTue[16][1];
    doc["Bell_17_0"] = timeBellTue[17][0];
    doc["Bell_17_1"] = timeBellTue[17][1];
    doc["Bell_18_0"] = timeBellTue[18][0];
    doc["Bell_18_1"] = timeBellTue[18][1];
    doc["Bell_19_0"] = timeBellTue[19][0];
    doc["Bell_19_1"] = timeBellTue[19][1];
    doc["Bell_20_0"] = timeBellTue[20][0];
    doc["Bell_20_1"] = timeBellTue[20][1];
    doc["Bell_21_0"] = timeBellTue[21][0];
    doc["Bell_21_1"] = timeBellTue[21][1];


    File configFile = LittleFS.open(fileTue, "w");
    if (!configFile) 
    {
        Serial.println("Failed to open config file for writing");
        return;
    }

  serializeJson(doc, configFile);

  configFile.close();
}

void BellConfig::saveWed()
{
    StaticJsonDocument<2048> doc; 
    doc["Bell_1_1"] = timeBellWed[0][0];
    doc["Bell_1_1"] = timeBellWed[0][1];
    doc["Bell_1_1"] = timeBellWed[1][0];
    doc["Bell_1_1"] = timeBellWed[1][1];
    doc["Bell_2_0"] = timeBellWed[2][0];
    doc["Bell_2_1"] = timeBellWed[2][1];
    doc["Bell_3_0"] = timeBellWed[3][0];
    doc["Bell_3_1"] = timeBellWed[3][1];
    doc["Bell_4_0"] = timeBellWed[4][0];
    doc["Bell_4_1"] = timeBellWed[4][1];
    doc["Bell_5_0"] = timeBellWed[5][0];
    doc["Bell_5_1"] = timeBellWed[5][1];
    doc["Bell_6_0"] = timeBellWed[6][0];
    doc["Bell_6_1"] = timeBellWed[6][1];
    doc["Bell_7_0"] = timeBellWed[7][0];
    doc["Bell_7_1"] = timeBellWed[7][1];
    doc["Bell_8_0"] = timeBellWed[8][0];
    doc["Bell_8_1"] = timeBellWed[8][1];
    doc["Bell_9_0"] = timeBellWed[9][0];
    doc["Bell_9_1"] = timeBellWed[9][1];
    doc["Bell_10_0"] = timeBellWed[10][0];
    doc["Bell_10_1"] = timeBellWed[10][1];
    doc["Bell_11_0"] = timeBellWed[11][0];
    doc["Bell_11_1"] = timeBellWed[11][1];
    doc["Bell_12_0"] = timeBellWed[12][0];
    doc["Bell_12_1"] = timeBellWed[12][1];
    doc["Bell_13_0"] = timeBellWed[13][0];
    doc["Bell_13_1"] = timeBellWed[13][1];
    doc["Bell_14_0"] = timeBellWed[14][0];
    doc["Bell_14_1"] = timeBellWed[14][1];
    doc["Bell_15_0"] = timeBellWed[15][0];
    doc["Bell_15_1"] = timeBellWed[15][1];
    doc["Bell_16_0"] = timeBellWed[16][0];
    doc["Bell_16_1"] = timeBellWed[16][1];
    doc["Bell_17_0"] = timeBellWed[17][0];
    doc["Bell_17_1"] = timeBellWed[17][1];
    doc["Bell_18_0"] = timeBellWed[18][0];
    doc["Bell_18_1"] = timeBellWed[18][1];
    doc["Bell_19_0"] = timeBellWed[19][0];
    doc["Bell_19_1"] = timeBellWed[19][1];
    doc["Bell_20_0"] = timeBellWed[20][0];
    doc["Bell_20_1"] = timeBellWed[20][1];
    doc["Bell_21_0"] = timeBellWed[21][0];
    doc["Bell_21_1"] = timeBellWed[21][1];


    File configFile = LittleFS.open(fileWed, "w");
    if (!configFile) 
    {
        Serial.println("Failed to open config file for writing");
        return;
    }

  serializeJson(doc, configFile);

  configFile.close();
}

void BellConfig::saveThu()
{
    StaticJsonDocument<2048> doc;

    doc["Bell_0_0"] = timeBellThu[0][0];
    doc["Bell_0_1"] = timeBellThu[0][1];
    doc["Bell_1_0"] = timeBellThu[1][0];
    doc["Bell_1_1"] = timeBellThu[1][1];
    doc["Bell_2_0"] = timeBellThu[2][0];
    doc["Bell_2_1"] = timeBellThu[2][1];
    doc["Bell_3_0"] = timeBellThu[3][0];
    doc["Bell_3_1"] = timeBellThu[3][1];
    doc["Bell_4_0"] = timeBellThu[4][0];
    doc["Bell_4_1"] = timeBellThu[4][1];
    doc["Bell_5_0"] = timeBellThu[5][0];
    doc["Bell_5_1"] = timeBellThu[5][1];
    doc["Bell_6_0"] = timeBellThu[6][0];
    doc["Bell_6_1"] = timeBellThu[6][1];
    doc["Bell_7_0"] = timeBellThu[7][0];
    doc["Bell_7_1"] = timeBellThu[7][1];
    doc["Bell_8_0"] = timeBellThu[8][0];
    doc["Bell_8_1"] = timeBellThu[8][1];
    doc["Bell_9_0"] = timeBellThu[9][0];
    doc["Bell_9_1"] = timeBellThu[9][1];
    doc["Bell_10_0"] = timeBellThu[10][0];
    doc["Bell_10_1"] = timeBellThu[10][1];
    doc["Bell_11_0"] = timeBellThu[11][0];
    doc["Bell_11_1"] = timeBellThu[11][1];
    doc["Bell_12_0"] = timeBellThu[12][0];
    doc["Bell_12_1"] = timeBellThu[12][1];
    doc["Bell_13_0"] = timeBellThu[13][0];
    doc["Bell_13_1"] = timeBellThu[13][1];
    doc["Bell_14_0"] = timeBellThu[14][0];
    doc["Bell_14_1"] = timeBellThu[14][1];
    doc["Bell_15_0"] = timeBellThu[15][0];
    doc["Bell_15_1"] = timeBellThu[15][1];
    doc["Bell_16_0"] = timeBellThu[16][0];
    doc["Bell_16_1"] = timeBellThu[16][1];
    doc["Bell_17_0"] = timeBellThu[17][0];
    doc["Bell_17_1"] = timeBellThu[17][1];
    doc["Bell_18_0"] = timeBellThu[18][0];
    doc["Bell_18_1"] = timeBellThu[18][1];
    doc["Bell_19_0"] = timeBellThu[19][0];
    doc["Bell_19_1"] = timeBellThu[19][1];
    doc["Bell_20_0"] = timeBellThu[20][0];
    doc["Bell_20_1"] = timeBellThu[20][1];
    doc["Bell_21_0"] = timeBellThu[21][0];
    doc["Bell_21_1"] = timeBellThu[21][1];


    File configFile = LittleFS.open(fileThu, "w");
    if (!configFile) 
    {
        Serial.println("Failed to open config file for writing");
        return;
    }

  serializeJson(doc, configFile);

  configFile.close();
}

void BellConfig::saveFri()
{
    StaticJsonDocument<2048> doc;

    doc["Bell_0_0"] = timeBellFri[0][0];
    doc["Bell_0_1"] = timeBellFri[0][1];
    doc["Bell_1_0"] = timeBellFri[1][0];
    doc["Bell_1_1"] = timeBellFri[1][1];
    doc["Bell_2_0"] = timeBellFri[2][0];
    doc["Bell_2_1"] = timeBellFri[2][1];
    doc["Bell_3_0"] = timeBellFri[3][0];
    doc["Bell_3_1"] = timeBellFri[3][1];
    doc["Bell_4_0"] = timeBellFri[4][0];
    doc["Bell_4_1"] = timeBellFri[4][1];
    doc["Bell_5_0"] = timeBellFri[5][0];
    doc["Bell_5_1"] = timeBellFri[5][1];
    doc["Bell_6_0"] = timeBellFri[6][0];
    doc["Bell_6_1"] = timeBellFri[6][1];
    doc["Bell_7_0"] = timeBellFri[7][0];
    doc["Bell_7_1"] = timeBellFri[7][1];
    doc["Bell_8_0"] = timeBellFri[8][0];
    doc["Bell_8_1"] = timeBellFri[8][1];
    doc["Bell_9_0"] = timeBellFri[9][0];
    doc["Bell_9_1"] = timeBellFri[9][1];
    doc["Bell_10_0"] = timeBellFri[10][0];
    doc["Bell_10_1"] = timeBellFri[10][1];
    doc["Bell_11_0"] = timeBellFri[11][0];
    doc["Bell_11_1"] = timeBellFri[11][1];
    doc["Bell_12_0"] = timeBellFri[12][0];
    doc["Bell_12_1"] = timeBellFri[12][1];
    doc["Bell_13_0"] = timeBellFri[13][0];
    doc["Bell_13_1"] = timeBellFri[13][1];
    doc["Bell_14_0"] = timeBellFri[14][0];
    doc["Bell_14_1"] = timeBellFri[14][1];
    doc["Bell_15_0"] = timeBellFri[15][0];
    doc["Bell_15_1"] = timeBellFri[15][1];
    doc["Bell_16_0"] = timeBellFri[16][0];
    doc["Bell_16_1"] = timeBellFri[16][1];
    doc["Bell_17_0"] = timeBellFri[17][0];
    doc["Bell_17_1"] = timeBellFri[17][1];
    doc["Bell_18_0"] = timeBellFri[18][0];
    doc["Bell_18_1"] = timeBellFri[18][1];
    doc["Bell_19_0"] = timeBellFri[19][0];
    doc["Bell_19_1"] = timeBellFri[19][1];
    doc["Bell_20_0"] = timeBellFri[20][0];
    doc["Bell_20_1"] = timeBellFri[20][1];
    doc["Bell_21_0"] = timeBellFri[21][0];
    doc["Bell_21_1"] = timeBellFri[21][1];


    File configFile = LittleFS.open(fileFri, "w");
    if (!configFile) 
    {
        Serial.println("Failed to open config file for writing");
        return;
    }

  serializeJson(doc, configFile);

  configFile.close();
}

void BellConfig::saveSat()
{
    StaticJsonDocument<2048> doc;

    doc["Bell_0_0"] = timeBellSat[0][0];
    doc["Bell_0_1"] = timeBellSat[0][1];
    doc["Bell_1_0"] = timeBellSat[1][0];
    doc["Bell_1_1"] = timeBellSat[1][1];
    doc["Bell_2_0"] = timeBellSat[2][0];
    doc["Bell_2_1"] = timeBellSat[2][1];
    doc["Bell_3_0"] = timeBellSat[3][0];
    doc["Bell_3_1"] = timeBellSat[3][1];
    doc["Bell_4_0"] = timeBellSat[4][0];
    doc["Bell_4_1"] = timeBellSat[4][1];
    doc["Bell_5_0"] = timeBellSat[5][0];
    doc["Bell_5_1"] = timeBellSat[5][1];
    doc["Bell_6_0"] = timeBellSat[6][0];
    doc["Bell_6_1"] = timeBellSat[6][1];
    doc["Bell_7_0"] = timeBellSat[7][0];
    doc["Bell_7_1"] = timeBellSat[7][1];
    doc["Bell_8_0"] = timeBellSat[8][0];
    doc["Bell_8_1"] = timeBellSat[8][1];
    doc["Bell_9_0"] = timeBellSat[9][0];
    doc["Bell_9_1"] = timeBellSat[9][1];
    doc["Bell_10_0"] = timeBellSat[10][0];
    doc["Bell_10_1"] = timeBellSat[10][1];
    doc["Bell_11_0"] = timeBellSat[11][0];
    doc["Bell_11_1"] = timeBellSat[11][1];
    doc["Bell_12_0"] = timeBellSat[12][0];
    doc["Bell_12_1"] = timeBellSat[12][1];
    doc["Bell_13_0"] = timeBellSat[13][0];
    doc["Bell_13_1"] = timeBellSat[13][1];
    doc["Bell_14_0"] = timeBellSat[14][0];
    doc["Bell_14_1"] = timeBellSat[14][1];
    doc["Bell_15_0"] = timeBellSat[15][0];
    doc["Bell_15_1"] = timeBellSat[15][1];
    doc["Bell_16_0"] = timeBellSat[16][0];
    doc["Bell_16_1"] = timeBellSat[16][1];
    doc["Bell_17_0"] = timeBellSat[17][0];
    doc["Bell_17_1"] = timeBellSat[17][1];
    doc["Bell_18_0"] = timeBellSat[18][0];
    doc["Bell_18_1"] = timeBellSat[18][1];
    doc["Bell_19_0"] = timeBellSat[19][0];
    doc["Bell_19_1"] = timeBellSat[19][1];
    doc["Bell_20_0"] = timeBellSat[20][0];
    doc["Bell_20_1"] = timeBellSat[20][1];
    doc["Bell_21_0"] = timeBellSat[21][0];
    doc["Bell_21_1"] = timeBellSat[21][1];


    File configFile = LittleFS.open(fileSat, "w");
    if (!configFile) 
    {
        Serial.println("Failed to open config file for writing");
        return;
    }

  serializeJson(doc, configFile);

  configFile.close();
}

void BellConfig::saveSun()
{
    StaticJsonDocument<2048> doc;

    doc["Bell_0_0"] = timeBellSun[0][0];
    doc["Bell_0_1"] = timeBellSun[0][1];
    doc["Bell_1_0"] = timeBellSun[1][0];
    doc["Bell_1_1"] = timeBellSun[1][1];
    doc["Bell_2_0"] = timeBellSun[2][0];
    doc["Bell_2_1"] = timeBellSun[2][1];
    doc["Bell_3_0"] = timeBellSun[3][0];
    doc["Bell_3_1"] = timeBellSun[3][1];
    doc["Bell_4_0"] = timeBellSun[4][0];
    doc["Bell_4_1"] = timeBellSun[4][1];
    doc["Bell_5_0"] = timeBellSun[5][0];
    doc["Bell_5_1"] = timeBellSun[5][1];
    doc["Bell_6_0"] = timeBellSun[6][0];
    doc["Bell_6_1"] = timeBellSun[6][1];
    doc["Bell_7_0"] = timeBellSun[7][0];
    doc["Bell_7_1"] = timeBellSun[7][1];
    doc["Bell_8_0"] = timeBellSun[8][0];
    doc["Bell_8_1"] = timeBellSun[8][1];
    doc["Bell_9_0"] = timeBellSun[9][0];
    doc["Bell_9_1"] = timeBellSun[9][1];
    doc["Bell_10_0"] = timeBellSun[10][0];
    doc["Bell_10_1"] = timeBellSun[10][1];
    doc["Bell_11_0"] = timeBellSun[11][0];
    doc["Bell_11_1"] = timeBellSun[11][1];
    doc["Bell_12_0"] = timeBellSun[12][0];
    doc["Bell_12_1"] = timeBellSun[12][1];
    doc["Bell_13_0"] = timeBellSun[13][0];
    doc["Bell_13_1"] = timeBellSun[13][1];
    doc["Bell_14_0"] = timeBellSun[14][0];
    doc["Bell_14_1"] = timeBellSun[14][1];
    doc["Bell_15_0"] = timeBellSun[15][0];
    doc["Bell_15_1"] = timeBellSun[15][1];
    doc["Bell_16_0"] = timeBellSun[16][0];
    doc["Bell_16_1"] = timeBellSun[16][1];
    doc["Bell_17_0"] = timeBellSun[17][0];
    doc["Bell_17_1"] = timeBellSun[17][1];
    doc["Bell_18_0"] = timeBellSun[18][0];
    doc["Bell_18_1"] = timeBellSun[18][1];
    doc["Bell_19_0"] = timeBellSun[19][0];
    doc["Bell_19_1"] = timeBellSun[19][1];
    doc["Bell_20_0"] = timeBellSun[20][0];
    doc["Bell_20_1"] = timeBellSun[20][1];
    doc["Bell_21_0"] = timeBellSun[21][0];
    doc["Bell_21_1"] = timeBellSun[21][1];


    File configFile = LittleFS.open(fileSun, "w");
    if (!configFile) 
    {
        Serial.println("Failed to open config file for writing");
        return;
    }

  serializeJson(doc, configFile);

  configFile.close();
}

