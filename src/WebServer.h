#ifndef WebServer_H
#define WebServer_H

#include <Arduino.h> 
#include <Config.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WebServer : public AsyncWebHandler
{ 

    public:
        WebServer(); 
        virtual ~WebServer() {}


        void begin(Config& config);
    private: 

        void serverLoadCSS();
        void serverLoadPNG();
        void serverLoadHTML();
        void serverLoadJS();
        void serverLoadHandlers();

        void handleLogin(AsyncWebServerRequest *request);
        void handleLogout(AsyncWebServerRequest *request);
        bool is_authenticated(AsyncWebServerRequest *request);
 
        String processorBellConfiguration(const String& str);
        String processorBellTemplates(const String& str);
        String processorSettings(const String& str);
        
        AsyncWebServer *server;
        Config config;
    
};

#endif