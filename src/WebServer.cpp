#include <Arduino.h> 
#include "WebServer.h"
#include <Hash.h>


WebServer::WebServer()
{
}

void WebServer::begin(Config& config)
{
    this->config = config; 

    WiFi.mode(WIFI_STA);
    WiFi.begin(config.wifi_ssid, config.wifi_pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

   server = new AsyncWebServer(config.port);
 
   Serial.println(config.port);

  serverLoadCSS();
  serverLoadPNG();
  serverLoadHTML();
  serverLoadJS();
  serverLoadHandlers();

  server->begin(); 

}


void WebServer::serverLoadCSS()
{
  server->on("/about.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/about.css", "text/css",false);} );
  server->on("/bell.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/bell.css", "text/css",false);} );
  server->on("/form.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/form.css", "text/css",false);} );
  server->on("/functions.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/functions.css", "text/css",false);} );
  server->on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/main.css","text/css",false);} );
  server->on("/navbar.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/navbar.css", "text/css",false);} );
  server->on("/note.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/note.css", "text/css",false);} );
  server->on("/settings.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/settings.css", "text/css",false);} );
  server->on("/welcome.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/welcome.css", "text/css",false);} );
  server->on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/index.css", "text/css",false);} );
  server->on("/404.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/404.css", "text/css",false);} );
}


void WebServer::serverLoadPNG()
{
  server->on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/favicon.png", String() ,false);} );
  server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/favicon.png", String() ,false);} );
}


void WebServer::serverLoadJS()
{
}

void WebServer::serverLoadHTML()
{
  server->onNotFound([](AsyncWebServerRequest *request) { request->send(LittleFS, "/404.html", String() ,false);});
  server->on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/index.html", String() ,false);});
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/index.html", String() ,false);});
  server->on("/about.html", HTTP_GET, [this](AsyncWebServerRequest *request){ if(is_authenticated(request)) { request->send(LittleFS , "/about.html" , String() , false); } else { request->send(LittleFS , "/index.html" , String() , false); } });
  server->on("/bellConfigurator.html", HTTP_GET, [this](AsyncWebServerRequest *request)
  { 
    if(is_authenticated(request)) 
    { 
      request->send(LittleFS , "/bellConfigurator.html" , String() , false , [this](const String &var) -> String { return this->processorBellConfiguration(var); });
    } 
    else 
    { 
      request->send(LittleFS , "/index.html" , String() , false); 
    }
  });
  server->on("/bellTemplates.html", HTTP_GET, [this](AsyncWebServerRequest *request)
  { 
    if(is_authenticated(request)) 
    { 
      request->send(LittleFS , "/bellTemplates.html" , String() , false , [this](const String &var) -> String { return this->processorBellTemplates(var); }); 
    } 
    else 
    { 
      request->send(LittleFS , "/index.html" , String() , false); 
    } 
  });
  server->on("/functions.html", HTTP_GET, [this](AsyncWebServerRequest *request){ if(is_authenticated(request)) { request->send(LittleFS , "/functions.html" , String() , false); } else { request->send(LittleFS , "/index.html" , String() , false); } });
  server->on("/settings.html", HTTP_GET, [this](AsyncWebServerRequest *request)
  {
    if(is_authenticated(request)) 
    { 
      request->send(LittleFS , "/settings.html" , String() , false,[this](const String &var) -> String { return this->processorSettings(var); }); 
    } 
    else 
    { 
      request->send(LittleFS , "/index.html" , String() , false); 
    } 
  });
  server->on("/welcome.html", HTTP_GET, [this](AsyncWebServerRequest *request){ if(is_authenticated(request)) { request->send(LittleFS , "/welcome.html" , String() , false); } else { request->send(LittleFS , "/index.html" , String() , false); } });
}

void WebServer::serverLoadHandlers()
{
  server->on("/login", HTTP_POST, [this](AsyncWebServerRequest *request){ WebServer::handleLogin(request); });
  server->on("/logout", HTTP_GET, [this](AsyncWebServerRequest *request){ WebServer::handleLogout(request); });

}

/**
 * 
 * 
 * Handlers
 * 
 * 
*/
void WebServer::handleLogin(AsyncWebServerRequest *request)
{
  if (request->hasHeader("Cookie")) 
  {
      Serial.print("Found cookie: ");
      
      String cookie = request->header("Cookie");
      Serial.println(cookie);
  }

  if (request->hasArg("login") && request->hasArg("password")) 
  {
      
      Serial.print("Found parameter: ");
      if (request->arg("login") == String(config.www_user) && request->arg("password") == String(config.www_pass)) 
      {
      AsyncWebServerResponse *response = request-> beginResponse(LittleFS, "/welcome.html");  
 
      response->addHeader("Location", "/");
      response->addHeader("Cache-Control", "no-cache"); 
      
      String token = sha1(String(config.www_user) + ":" + String(config.www_pass) + ":" + request->client()->remoteIP().toString());
      
      response->addHeader("Set-Cookie", "ESPSESSIONID=" + token);

      request->send(response);
      return;
      } 

      AsyncWebServerResponse *response = request-> beginResponse(LittleFS, "/index.html");  
      Serial.println("Log in Failed");
      response->addHeader("Cache-Control", "no-cache");
      request->send(response);

      return;
  }
}

void WebServer::handleLogout(AsyncWebServerRequest *request) 
{ 
  AsyncWebServerResponse *response = request-> beginResponse(LittleFS, "/index.html", String());

  response->addHeader("Cache-Control", "no-cache");
  response->addHeader("Set-Cookie", "ESPSESSIONID=0");
  
  request->send(response);
  return;
}

bool WebServer::is_authenticated(AsyncWebServerRequest *request) 
{ 
  if (request -> hasHeader("Cookie")) 
  { 
    String cookie = request->header("Cookie");
    Serial.println(cookie);
    String token = sha1(String(config.www_user) + ":" +
    String(config.www_pass) + ":" +
    request->client()->remoteIP().toString()); 

    if (cookie.indexOf("ESPSESSIONID=" + token) != -1)
    { 
      return true;
    }
  } 
  return false;
}

/**
 * 
 * 
 * Procesors
 * 
 * 
 */
 
String WebServer::processorBellConfiguration(const String& str)
{
  for (int i = 0; i < 24; i++)
  {
    if(str.startsWith("mon_h"+String(i)))
    {
      return String(config.monBell[i][0]);
    }
    else if(str.startsWith("mon_m"+String(i)))
    {
      return String(config.monBell[i][1]);
    }
   
    if(str.startsWith("tue_h"+String(i)))
    {
      return String(config.tueBell[i][0]);
    }
    else if(str.startsWith("tue_m"+String(i)))
    {
      return String(config.tueBell[i][1]);
    }
  
    if(str.startsWith("wed_h"+String(i)))
    {
      return String(config.wedBell[i][0]);
    }
    else if(str.startsWith("wed_m"+String(i)))
    {
      return String(config.wedBell[i][1]);
    }
 
    if(str.startsWith("thu_h"+String(i)))
    {
      return String(config.thuBell[i][0]);
    }
    else if(str.startsWith("thu_m"+String(i)))
    {
      return String(config.thuBell[i][1]);
    }
  
    if(str.startsWith("fri_h"+String(i)))
    {
      return String(config.friBell[i][0]);
    }
    else if(str.startsWith("fri_m"+String(i)))
    {
      return String(config.friBell[i][1]);
    }
  
    if(str.startsWith("sat_h"+String(i)))
    {
      return String(config.satBell[i][0]);
    }
    else if(str.startsWith("sat_m"+String(i)))
    {
      return String(config.satBell[i][1]);
    }
   
    if(str.startsWith("sun_h"+String(i)))
    {
      return String(config.sunBell[i][0]);
    }
    else if(str.startsWith("sun_m"+String(i)))
    {
      return String(config.sunBell[i][1]);
    }
  }

  return String("");
}

String WebServer::processorBellTemplates(const String& str)
{
  for (int i = 0; i < 24; i++)
  {
    if(str.startsWith("temp_0_h"+String(i)))
    {
      return String(config.template0[i][0]);
    }
    else if(str.startsWith("temp_0_m"+String(i)))
    {
      return String(config.template0[i][1]);
    }
  }
  for (int i = 0; i < 24; i++)
  {
    if(str.startsWith("temp_1_h"+String(i)))
    {
      return String(config.template1[i][0]);
    }
    else if(str.startsWith("temp_1_m"+String(i)))
    {
      return String(config.template1[i][1]);
    }
  }
  for (int i = 0; i < 24; i++)
  {
    if(str.startsWith("temp_2_h"+String(i)))
    {
      return String(config.template2[i][0]);
    }
    else if(str.startsWith("temp_2_m"+String(i)))
    {
      return String(config.template2[i][1]);
    }
  }
  for (int i = 0; i < 24; i++)
  {
    if(str.startsWith("temp_3_h"+String(i)))
    {
      return String(config.template3[i][0]);
    }
    else if(str.startsWith("temp_3_m"+String(i)))
    {
      return String(config.template3[i][1]);
    }
  }
  
  return String("");
}

String WebServer::processorSettings(const String& str)
{
  if (str.startsWith("port"))
  {
    return String(config.port);
  }

  if (str.startsWith("ssid"))
  {
    return config.wifi_ssid;
  }

  if (str.startsWith("username"))
  {
    return config.www_user;
  }
  
  if (str.startsWith("bellDuration"))
  {
    return String(config.bellDuration);
  }

  return String("");
}
