#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Config.h>
#include <SoftwareSerial.h>
#include <BellTimer.h>

class HTTPServer
{
  public:
    HTTPServer(SoftwareSerial& mySerial,Config& config,BellTimer& timer);
    
    void begin(); 

  private:

    void handleHTML();
    void handleCSS();
    void handleJS();
    void handlePNG();
    void handleFunctions();

    bool is_authenticated(AsyncWebServerRequest *request);
    void handleLogout(AsyncWebServerRequest *request);
    void handleLogin(AsyncWebServerRequest *request);

    void handleLoadTemplate(AsyncWebServerRequest *request);
    void nextBell(AsyncWebServerRequest *request);
    void getTime(AsyncWebServerRequest *request);
    void handleCallBell(AsyncWebServerRequest *request);
    void handleCallAlert(AsyncWebServerRequest *request);
    void handleReboot(AsyncWebServerRequest *request);

    String processorBellConfiguration(const String& str);
    String processorBellTemplates(const String& str);
    String processorSettings(const String& str);
    String processorWelcome(const String& str);

    void handleSaveTemplate0(AsyncWebServerRequest *request);
    void handleSaveTemplate1(AsyncWebServerRequest *request);
    void handleSaveTemplate2(AsyncWebServerRequest *request);
    void handleSaveTemplate3(AsyncWebServerRequest *request);

    void handleSaveMon(AsyncWebServerRequest *request);
    void handleSaveTue(AsyncWebServerRequest *request);
    void handleSaveWed(AsyncWebServerRequest *request);
    void handleSaveThu(AsyncWebServerRequest *request);
    void handleSaveFri(AsyncWebServerRequest *request);
    void handleSaveSat(AsyncWebServerRequest *request);
    void handleSaveSun(AsyncWebServerRequest *request);

    void handleUpdateUsername(AsyncWebServerRequest *request);
    void handleUpdatePassword(AsyncWebServerRequest *request);
    void handleUpdateBellDuration(AsyncWebServerRequest *request);
    void handleUpdateWifi(AsyncWebServerRequest *request);
    void handleUpdatePort(AsyncWebServerRequest *request);

    void setupServer();
    
    SoftwareSerial *mySerial;

    Config *config;

    BellTimer *timer;
    
    AsyncWebServer *server;
};

#endif