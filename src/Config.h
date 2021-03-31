#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h> 
#include <LittleFS.h>

class Config
{
  
  public:

    Config();
    
    void begin();

    void save(); 

    int firstRun = 0;

    int port = 80;

    int bellDuration = 6;

    int przestawienieCzasu = 0;
   
    char* wifi_ssid;
    char* wifi_pass;

    char* www_user;
    char* www_pass;

    bool mainSave();

    int monBell[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };
    int tueBell[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };
    int wedBell[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };
    int thuBell[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };
    int friBell[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };
    int satBell[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };
    int sunBell[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };

    int template0[24][2] = 
    {
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1}
    };
    int template1[24][2]= 
    {
      {-1,-1} , {-1,-1} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {-1,-1} , {-1,-1} ,
      {-1,-1} , {-1,-1}
    };
    int template2[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };
    int template3[24][2] = 
    {
      {7,10} , {7,55} ,
      {8,0} , {8,45} ,
      {8,55} , {9,40} ,
      {9,50} , {10,35} ,
      {10,45} , {11,30} ,
      {11,45} , {12,30} ,
      {12,40} , {13,25} ,
      {13,35} , {14,20} ,
      {14,25} , {15,10} ,
      {15,15} , {16,0} ,
      {16,5} , {16,50} ,
      {16,55} , {17,40}
    };

  private:
    void load();

    void loadTemplates();
    void loadBells();

    void saveTemplates();
    void saveBells();

    void loadDoW();

    void saveDoW();


    void loadMon();
    void loadTue();
    void loadWed();
    void loadThu();
    void loadFri();
    void loadSat();
    void loadSun();
    
    void loadTemplate0();
    void loadTemplate1();
    void loadTemplate2();
    void loadTemplate3();

    void saveMon();
    void saveTue();
    void saveWed();
    void saveThu();
    void saveFri();
    void saveSat();
    void saveSun();
    
    void saveTemplate0();
    void saveTemplate1();
    void saveTemplate2();
    void saveTemplate3();

    void loadConfig();

    void saveConfig(); 

    const char * fileConfig = "Config.json";

    const char * fileMon = "Bell_Mon.json";
    const char * fileTue = "Bell_Tue.json";
    const char * fileWed = "Bell_Wed.json";
    const char * fileThu = "Bell_Thu.json";
    const char * fileFri = "Bell_Fri.json";
    const char * fileSat = "Bell_Sat.json";
    const char * fileSun = "Bell_Sun.json";

    const char * fileTemplate0 = "Template0.json";
    const char * fileTemplate1 = "Template1.json";
    const char * fileTemplate2 = "Template2.json";
    const char * fileTemplate3 = "Template3.json";
   
    
};

#endif