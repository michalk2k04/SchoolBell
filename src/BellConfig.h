#ifndef BellConfig_H
#define BellConfig_H
#include <Arduino.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>

class BellConfig
{
  public:
    BellConfig();

    void runSerialConfiguration();

    void begin();
    void save();  
     
    int firstRun;
    int httpPort;
    
     char* wifi_ssid;
     char* wifi_pass;
    
     char* www_username;
     char* www_pass;

    int timeBellMon[22][2] =  
    { 
        {7,10}, {7, 55},
        {8, 00}, {8, 45},
        {8,55}, {9, 40},
        {9,50}, {10,35},
        {10,45}, {11,30},
        {11,45}, {12,30},
        {12,35}, {13,20},
        {13,25}, {14,10},
        {14,15}, {15,00},
        {15,5}, {15,50},
        {15,55}, {16,40}, 
    };
    int timeBellTue[22][2] =  
    { 
        {7,10}, {7, 55},
        {8, 00}, {8, 45},
        {8,55}, {9, 40},
        {9,50}, {10,35},
        {10,45}, {11,30},
        {11,45}, {12,30},
        {12,35}, {13,20},
        {13,25}, {14,10},
        {14,15}, {15,00},
        {15,5}, {15,50},
        {15,55}, {16,40}, 
    };
    int timeBellWed[22][2] =  
    { 
        {7,10}, {7, 55},
        {8, 00}, {8, 45},
        {8,55}, {9, 40},
        {9,50}, {10,35},
        {10,45}, {11,30},
        {11,45}, {12,30},
        {12,35}, {13,20},
        {13,25}, {14,10},
        {14,15}, {15,00},
        {15,5}, {15,50},
        {15,55}, {16,40}, 
    };
    int timeBellThu[22][2] =  
    { 
        {7,10}, {7, 55},
        {8, 00}, {8, 45},
        {8,55}, {9, 40},
        {9,50}, {10,35},
        {10,45}, {11,30},
        {11,45}, {12,30},
        {12,35}, {13,20},
        {13,25}, {14,10},
        {14,15}, {15,00},
        {15,5}, {15,50},
        {15,55}, {16,40}, 
    };
    int timeBellFri[22][2] =  
    { 
        {7,10}, {7, 55},
        {8, 00}, {8, 45},
        {8,55}, {9, 40},
        {9,50}, {10,35},
        {10,45}, {11,30},
        {11,45}, {12,30},
        {12,35}, {13,20},
        {13,25}, {14,10},
        {14,15}, {15,00},
        {15,5}, {15,50},
        {15,55}, {16,40}, 
    };
    int timeBellSat[22][2] =  
    { 
        {7,10}, {7, 55},
        {8, 00}, {8, 45},
        {8,55}, {9, 40},
        {9,50}, {10,35},
        {10,45}, {11,30},
        {11,45}, {12,30},
        {12,35}, {13,20},
        {13,25}, {14,10},
        {14,15}, {15,00},
        {15,5}, {15,50},
        {15,55}, {16,40}, 
    };
    int timeBellSun[22][2] =  
    { 
        {7,10}, {7, 55},
        {8, 00}, {8, 45},
        {8,55}, {9, 40},
        {9,50}, {10,35},
        {10,45}, {11,30},
        {11,45}, {12,30},
        {12,35}, {13,20},
        {13,25}, {14,10},
        {14,15}, {15,00},
        {15,5}, {15,50},
        {15,55}, {16,40}, 
    };


    const char *fileConfig = "/Config.json";

    const char *fileMon = "/Mon.json";
    const char *fileTue = "/Tue.json";
    const char *fileWed = "/Wed.json";
    const char *fileThu = "/Thu.json";
    const char *fileFri = "/Fri.json";
    const char *fileSat = "/Sat.json";
    const char *fileSun = "/Sun.json";

  private:
    void load();
    void loadBells();
    void saveBells();

    void loadConfig();
    void loadMon();
    void loadTue();
    void loadWed();
    void loadThu();
    void loadFri();
    void loadSat();
    void loadSun();

    void saveConfig();
    void saveMon();
    void saveTue();
    void saveWed();
    void saveThu();
    void saveFri();
    void saveSat();
    void saveSun();

};

#endif