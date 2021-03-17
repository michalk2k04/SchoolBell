#include <Arduino.h>
#include <HTTPServer.h>
#include <Hash.h>

HTTPServer::HTTPServer(SoftwareSerial& mySerial,Config& config,BellTimer& timer)
{
  this->mySerial = &mySerial;
  this->config = &config;
  this->timer = &timer;
}


void HTTPServer::begin()
{
  WiFi.mode(WIFI_STA);
  String ssid = config->wifi_ssid;
  String pass = config->wifi_pass;
 
  WiFi.begin(ssid,pass);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    mySerial->printf("WiFi Failed!\n");
    return;
  }

  mySerial->print("Esp connected to WiFi \n SSID -");
  mySerial->println(config->wifi_ssid);
  mySerial->print("IP Address: ");
  mySerial->print(WiFi.localIP());
  mySerial->println(":"+String(config->port));
  server = new AsyncWebServer(config->port);
  
  setupServer();

  server->begin();

}
  
void HTTPServer::setupServer()
{
  handleHTML();
  handleCSS();
  handleJS();
  handlePNG();
  handleFunctions();
}



void HTTPServer::handleHTML()
{
server->onNotFound([](AsyncWebServerRequest *request) { request->send(LittleFS, "/404.html", String() ,false);});
  server->on("/index.html", HTTP_GET, [this](AsyncWebServerRequest *request){
     if(is_authenticated(request)) { request->send(LittleFS , "/welcome.html" , String() , false,[this](const String &var) -> String { return this->processorWelcome(var); }); } 
     else { request->send(LittleFS , "/index.html" , String() , false); } });

  server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
     if(is_authenticated(request)) { request->send(LittleFS , "/welcome.html" , String() , false,[this](const String &var) -> String { return this->processorWelcome(var); }); } 
     else { request->send(LittleFS , "/index.html" , String() , false); } });

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
  server->on("/welcome.html", HTTP_GET, [this](AsyncWebServerRequest *request){
     if(is_authenticated(request)) { request->send(LittleFS , "/welcome.html" , String() , false,[this](const String &var) -> String { return this->processorWelcome(var); }); } 
     else { request->send(LittleFS , "/index.html" , String() , false); } });

}

void HTTPServer::handleCSS()
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

void HTTPServer::handleJS()
{
  server->on("/bell.js", HTTP_GET, [this](AsyncWebServerRequest *request){ if(is_authenticated(request)) { request->send(LittleFS , "/bell.js" , String() , false); } else { request->send(404); } });
  server->on("/welcome.js", HTTP_GET, [this](AsyncWebServerRequest *request){ if(is_authenticated(request)) { request->send(LittleFS , "/welcome.js" , String() , false); } else { request->send(404); } });
}

void HTTPServer::handlePNG()
{
  server->on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/favicon.png", String() ,false);} );
  server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(LittleFS, "/favicon.png", String() ,false);} );
}

void HTTPServer::handleFunctions()
{
  server->on("/login", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleLogin(request); });
  server->on("/logout", HTTP_GET, [this](AsyncWebServerRequest *request){ HTTPServer::handleLogout(request); });
  server->on("/nextBell", HTTP_GET, [this](AsyncWebServerRequest *request){ HTTPServer::nextBell(request); });
  server->on("/getTime", HTTP_GET, [this](AsyncWebServerRequest *request){ HTTPServer::getTime(request); });
  server->on("/callBell", HTTP_GET, [this](AsyncWebServerRequest *request){ HTTPServer::handleCallBell(request); });
  server->on("/callAlert", HTTP_GET, [this](AsyncWebServerRequest *request){ HTTPServer::handleCallAlert(request); });
  server->on("/rebootESP", HTTP_GET, [this](AsyncWebServerRequest *request){ HTTPServer::nextBell(request); });

  server->on("/saveTemplate0", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveTemplate0(request); });
  server->on("/saveTemplate1", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveTemplate1(request); });
  server->on("/saveTemplate2", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveTemplate2(request); });
  server->on("/saveTemplate3", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveTemplate3(request); });

  server->on("/saveMonday", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveMon(request); });
  server->on("/saveTuesday", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveTue(request); });
  server->on("/saveWednesday", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveWed(request); });
  server->on("/saveThursday", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveThu(request); });
  server->on("/saveFriday", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveFri(request); });
  server->on("/saveSaturday", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveSat(request); });
  server->on("/saveSunday", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveSun(request); });
  
  server->on("/saveTemplate3", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleSaveTemplate3(request); });

  server->on("/loadTemplate", HTTP_GET, [this](AsyncWebServerRequest *request){ HTTPServer::handleLoadTemplate(request); });

  server->on("/updateUsername", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleUpdateUsername(request); });
  server->on("/updatePassword", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleUpdatePassword(request); });
  server->on("/updateBellDuration", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleUpdateBellDuration(request); });
  server->on("/updateWifi", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleUpdateWifi(request); });
  server->on("/updatePort", HTTP_POST, [this](AsyncWebServerRequest *request){ HTTPServer::handleUpdatePort(request); });
}

/**
 * 
 * 
 * Handlers
 * 
 * 
 */

void HTTPServer::nextBell(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain",timer->toNextBell());
}

void HTTPServer::getTime(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain",timer->getTime());
}
void HTTPServer::handleCallBell(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
    request->send(LittleFS, "/functions.html",String() , false );
    timer->bell = true;
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleCallAlert(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
    request->send(LittleFS, "/functions.html",String() , false );
    timer->bellAlert = true;
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleReboot(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 
    request->send(LittleFS, "/functions.html",String() , false);
    mySerial->println("ESP restart");
    ESP.restart(); 

  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}


void HTTPServer::handleLoadTemplate(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    
    String templateID = request -> arg("template");
    String dayOfWeek = request -> arg("dayOfWeek");

    if(dayOfWeek.equalsIgnoreCase("mon"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(config->monBell,config->template0,sizeof(config->monBell));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(config->monBell,config->template1,sizeof(config->monBell));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(config->monBell,config->template2,sizeof(config->monBell));
      }
      else if(templateID.equalsIgnoreCase("template_3"))
      {
        memcpy(config->monBell,config->template3,sizeof(config->monBell));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("tue"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(config->tueBell,config->template0,sizeof(config->tueBell));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(config->tueBell,config->template1,sizeof(config->tueBell));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(config->tueBell,config->template2,sizeof(config->tueBell));
      }
      else if(templateID.equalsIgnoreCase("template_3"))
      {
        memcpy(config->tueBell,config->template3,sizeof(config->tueBell));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("wed"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(config->wedBell,config->template0,sizeof(config->wedBell));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(config->wedBell,config->template1,sizeof(config->wedBell));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(config->wedBell,config->template2,sizeof(config->wedBell));
      }
      else if(templateID.equalsIgnoreCase("template_3"))
      {
        memcpy(config->wedBell,config->template3,sizeof(config->wedBell));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("thu"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(config->thuBell,config->template0,sizeof(config->thuBell));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(config->thuBell,config->template1,sizeof(config->thuBell));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(config->thuBell,config->template2,sizeof(config->thuBell));
      }
      else if(templateID.equalsIgnoreCase("template_3"))
      {
        memcpy(config->thuBell,config->template3,sizeof(config->thuBell));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("fri"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(config->friBell,config->template0,sizeof(config->friBell));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(config->friBell,config->template1,sizeof(config->friBell));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(config->friBell,config->template2,sizeof(config->friBell));
      }
      else if(templateID.equalsIgnoreCase("template_3"))
      {
        memcpy(config->friBell,config->template3,sizeof(config->friBell));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("sat"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(config->satBell,config->template0,sizeof(config->satBell));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(config->satBell,config->template1,sizeof(config->satBell));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(config->satBell,config->template2,sizeof(config->satBell));
      }
      else if(templateID.equalsIgnoreCase("template_3"))
      {
        memcpy(config->satBell,config->template3,sizeof(config->satBell));
      }
    }
    else if (dayOfWeek.equalsIgnoreCase("sun"))
    {
      if(templateID.equalsIgnoreCase("template_0"))
      {
        memcpy(config->sunBell,config->template0,sizeof(config->sunBell));
      }
      else if(templateID.equalsIgnoreCase("template_1"))
      {
        memcpy(config->sunBell,config->template1,sizeof(config->sunBell));
      }
      else if(templateID.equalsIgnoreCase("template_2"))
      {
        memcpy(config->sunBell,config->template2,sizeof(config->sunBell));
      }
      else if(templateID.equalsIgnoreCase("template_3"))
      {
        memcpy(config->sunBell,config->template3,sizeof(config->sunBell));
      }
    }
    
    
    
    
    

    request->send(LittleFS , "/bellTemplates.html" , String() , false , [this](const String &var) -> String { return this->processorBellTemplates(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

/**
 * 
 * 
 * Handlers Login
 * 
 * 
*/
void HTTPServer::handleLogin(AsyncWebServerRequest *request)
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
    if (request->arg("login") == String(config->www_user) && request->arg("password") == String(config->www_pass)) 
    {
      AsyncWebServerResponse *response = request-> beginResponse(LittleFS, "/welcome.html",String() , false,[this](const String &var) -> String { return this->processorWelcome(var); });
      
      response->addHeader("Location", "/");
      response->addHeader("Cache-Control", "no-cache"); 
      
      String token = sha1(String(config->www_user) + ":" + String(config->www_pass) + ":" + request->client()->remoteIP().toString());
      
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

void HTTPServer::handleLogout(AsyncWebServerRequest *request) 
{ 
  AsyncWebServerResponse *response = request-> beginResponse(LittleFS, "/index.html", String());

  response->addHeader("Cache-Control", "no-cache");
  response->addHeader("Set-Cookie", "ESPSESSIONID=0");
  
  request->send(response);
  return;
}

bool HTTPServer::is_authenticated(AsyncWebServerRequest *request) 
{ 
  if (request -> hasHeader("Cookie")) 
  { 
    String cookie = request->header("Cookie");
    Serial.println(cookie);
    String token = sha1(String(config->www_user) + ":" +
    String(config->www_pass) + ":" +
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
 
String HTTPServer::processorBellConfiguration(const String& str)
{
    for (int i = 0; i < 24; i++)
  {
    if (str.equalsIgnoreCase("mon_h"+String(i)))
    {
      return String(config->monBell[i][0]);
    }

    if (str.equalsIgnoreCase("mon_m"+String(i)))
    {
      return String(config->monBell[i][1]);
    } 
    if (str.equalsIgnoreCase("tue_h"+String(i)))
    {
      return String(config->tueBell[i][0]);
    }

    if (str.equalsIgnoreCase("tue_m"+String(i)))
    {
      return String(config->tueBell[i][1]);
    } 
    if (str.equalsIgnoreCase("wed_h"+String(i)))
    {
      return String(config->wedBell[i][0]);
    }

    if (str.equalsIgnoreCase("wed_m"+String(i)))
    {
      return String(config->wedBell[i][1]);
    } 
    if (str.equalsIgnoreCase("thu_h"+String(i)))
    {
      return String(config->thuBell[i][0]);
    }

    if (str.equalsIgnoreCase("thu_m"+String(i)))
    {
      return String(config->thuBell[i][1]);
    } 
    if (str.equalsIgnoreCase("fri_h"+String(i)))
    {
      return String(config->friBell[i][0]);
    }

    if (str.equalsIgnoreCase("fri_m"+String(i)))
    {
      return String(config->friBell[i][1]);
    } 
    if (str.equalsIgnoreCase("sat_h"+String(i)))
    {
      return String(config->satBell[i][0]);
    }

    if (str.equalsIgnoreCase("sat_m"+String(i)))
    {
      return String(config->satBell[i][1]);
    } 

    if (str.equalsIgnoreCase("sun_h"+String(i)))
    {
      return String(config->sunBell[i][0]);
    }

    if (str.equalsIgnoreCase("sun_m"+String(i)))
    {
      return String(config->sunBell[i][1]);
    }
  }


  return String("");
}

String HTTPServer::processorBellTemplates(const String& str)
{
  for (int i = 0; i < 24; i++)
  {
    if (str.equalsIgnoreCase("temp_0_h"+String(i)))
    {
      return String(config->template0[i][0]);
    }

    if (str.equalsIgnoreCase("temp_0_m"+String(i)))
    {
      return String(config->template0[i][1]);
    }

    if (str.equalsIgnoreCase("temp_1_h"+String(i)))
    {
      return String(config->template1[i][0]);
    }

    if (str.equalsIgnoreCase("temp_1_m"+String(i)))
    {
      return String(config->template1[i][1]);
    }

    if (str.equalsIgnoreCase("temp_2_h"+String(i)))
    {
      return String(config->template2[i][0]);
    }

    if (str.equalsIgnoreCase("temp_2_m"+String(i)))
    {
      return String(config->template2[i][1]);
    }

    if (str.equalsIgnoreCase("temp_3_h"+String(i)))
    {
      return String(config->template3[i][0]);
    }

    if (str.equalsIgnoreCase("temp_3_m"+String(i)))
    {
      return String(config->template3[i][1]);
    }
  }
  
  return String("");
}

String HTTPServer::processorSettings(const String& str)
{
  if (str.startsWith("port"))
  {
    return String(config->port);
  }

  if (str.startsWith("ssid"))
  {
    return config->wifi_ssid;
  }

  if (str.startsWith("username"))
  {
    return config->www_user;
  }
  
  if (str.startsWith("bellDuration"))
  {
    return String(config->bellDuration);
  }

  return String("");
}

String HTTPServer::processorWelcome(const String& str)
{
  if(str.equalsIgnoreCase("nextBell"))
  {
    return timer->toNextBell();
  }

  if (str.equalsIgnoreCase("time"))
  {
    return timer->getTime();
  }
  

  return "";
  
}

/**
 * 
 * 
 *  Save handlers
 * 
 */
void HTTPServer::handleSaveTemplate0(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("temp_0_h"+String(i)).toInt();
      int b = request -> arg("temp_0_m"+String(i)).toInt();

      config->template0[i][0] = a;
      config->template0[i][1] = b;
    }

    request->send(LittleFS , "/bellTemplates.html" , String() , false , [this](const String &var) -> String { return this->processorBellTemplates(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveTemplate1(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("temp_0_h"+String(i)).toInt();
      int b = request -> arg("temp_0_m"+String(i)).toInt();

      config->template1[i][0] = a;
      config->template1[i][1] = b;
    }

    request->send(LittleFS , "/bellTemplates.html" , String() , false , [this](const String &var) -> String { return this->processorBellTemplates(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveTemplate2(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("temp_2_h"+String(i)).toInt();
      int b = request -> arg("temp_2_m"+String(i)).toInt();

      config->template2[i][0] = a;
      config->template2[i][1] = b;
    }

    request->send(LittleFS , "/bellTemplates.html" , String() , false , [this](const String &var) -> String { return this->processorBellTemplates(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveTemplate3(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("temp_3_h"+String(i)).toInt();
      int b = request -> arg("temp_3_m"+String(i)).toInt();

      config->template3[i][0] = a;
      config->template3[i][1] = b;
    }

    request->send(LittleFS , "/bellTemplates.html" , String() , false , [this](const String &var) -> String { return this->processorBellTemplates(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

/**
 * 
 * 
 * Handle save bells
 * 
 * 
 */

void HTTPServer::handleSaveMon(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("mon_h"+String(i)).toInt();
      int b = request -> arg("mon_m"+String(i)).toInt();

      config->monBell[i][0] = a;
      config->monBell[i][1] = b;
    }

    request->send(LittleFS , "/bellConfigurator.html" , String() , false , [this](const String &var) -> String { return this->processorBellConfiguration(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveTue(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("tue_h"+String(i)).toInt();
      int b = request -> arg("tue_m"+String(i)).toInt();

      config->tueBell[i][0] = a;
      config->tueBell[i][1] = b;
    }

    request->send(LittleFS , "/bellConfigurator.html" , String() , false , [this](const String &var) -> String { return this->processorBellConfiguration(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveWed(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("wed_h"+String(i)).toInt();
      int b = request -> arg("wed_m"+String(i)).toInt();

      config->wedBell[i][0] = a;
      config->wedBell[i][1] = b;
    }

    request->send(LittleFS , "/bellConfigurator.html" , String() , false , [this](const String &var) -> String { return this->processorBellConfiguration(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveThu(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("thu_h"+String(i)).toInt();
      int b = request -> arg("thu_m"+String(i)).toInt();

      config->thuBell[i][0] = a;
      config->thuBell[i][1] = b;
    }

    request->send(LittleFS , "/bellConfigurator.html" , String() , false , [this](const String &var) -> String { return this->processorBellConfiguration(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveSun(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("sun_h"+String(i)).toInt();
      int b = request -> arg("sun_m"+String(i)).toInt();

      config->sunBell[i][0] = a;
      config->sunBell[i][1] = b;
    }

    request->send(LittleFS , "/bellConfigurator.html" , String() , false , [this](const String &var) -> String { return this->processorBellConfiguration(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveFri(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("fri_h"+String(i)).toInt();
      int b = request -> arg("fri_m"+String(i)).toInt();

      config->friBell[i][0] = a;
      config->friBell[i][1] = b;
    }

    request->send(LittleFS , "/bellConfigurator.html" , String() , false , [this](const String &var) -> String { return this->processorBellConfiguration(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleSaveSat(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  {  
    for (int i = 0; i < 24; i++)
    {
      int a = request -> arg("sat_h"+String(i)).toInt();
      int b = request -> arg("sat_m"+String(i)).toInt();

      config->satBell[i][0] = a;
      config->satBell[i][1] = b;
    }

    request->send(LittleFS , "/bellConfigurator.html" , String() , false , [this](const String &var) -> String { return this->processorBellConfiguration(var); });

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

/**
 * 
 * 
 * Handle save settings
 * 
 */

void HTTPServer::handleUpdateUsername(AsyncWebServerRequest *request)
{
 if (is_authenticated(request))
  { 

    String un = request->arg("username");
    int unSize = un.length(); 
    un.toCharArray(config->www_user,unSize+1);

    request->send(LittleFS , "/settings.html" , String() , false,[this](const String &var) -> String { return this->processorSettings(var); }); 

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleUpdatePassword(AsyncWebServerRequest *request)
{
if (is_authenticated(request))
  { 

    String oldPass = request->arg("oldPass");
    String newPass = request->arg("newPass");
    int passLeanght = newPass.length();

    if (oldPass.equals(config->www_pass))
    {    
      newPass.toCharArray(config->www_pass,passLeanght+1);
    }
    
    request->send(LittleFS , "/settings.html" , String() , false,[this](const String &var) -> String { return this->processorSettings(var); }); 

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
  
}

void HTTPServer::handleUpdateBellDuration(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 

    String bellDur = request->arg("bellDuration"); 

    config->bellDuration = bellDur.toInt();
    
    request->send(LittleFS , "/settings.html" , String() , false,[this](const String &var) -> String { return this->processorSettings(var); }); 

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleUpdateWifi(AsyncWebServerRequest *request)
{
if (is_authenticated(request))
  { 

    String ssid = request->arg("ssid");
    int ssidSize = ssid.length();
    ssid.toCharArray(config->wifi_ssid,ssidSize+1);
    
    String pass = request->arg("pass");
    int passSize = pass.length();
    pass.toCharArray(config->wifi_pass,passSize+1);

    request->send(LittleFS , "/settings.html" , String() , false,[this](const String &var) -> String { return this->processorSettings(var); }); 

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}

void HTTPServer::handleUpdatePort(AsyncWebServerRequest *request)
{
  if (is_authenticated(request))
  { 

    String port = request->arg("port"); 

    config->port = port.toInt();
    
    request->send(LittleFS , "/settings.html" , String() , false,[this](const String &var) -> String { return this->processorSettings(var); }); 

    config->save();
  }
  else
  {
    request->send(LittleFS, "/index.html",String() , false);
  }
}
