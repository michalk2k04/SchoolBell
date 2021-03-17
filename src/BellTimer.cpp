#include <Arduino.h>
#include <BellTimer.h>

BellTimer::BellTimer(SoftwareSerial& mySerial,Config& config)
{
  this->mySerial = &mySerial;
  this->config = &config;
} 

void BellTimer::begin()
{
  ntpUDP = new WiFiUDP();
  timeClient = new NTPClient( *this->ntpUDP, "pool.ntp.org", utcOffsetInSeconds,updateInMs);

  timeClient->begin();
  timeClient->forceUpdate();
}

void BellTimer::handleBell()
{
  if(lastUpdate != timeClient->getHours())
  {
    lastUpdate = timeClient->getHours();
    timeClient->forceUpdate(); 
  }

  for (int i = 0; i < 22; i++)
  {
    if (isTime(1,config->monBell[i][0],config->monBell[i][1]))
    {
      bell = true;
    }
    
    if (isTime(2,config->tueBell[i][0],config->tueBell[i][1]))
    {
      bell = true;
    }
    
    if (isTime(3,config->wedBell[i][0],config->wedBell[i][1]))
    {
      bell = true;
    }
    
    if (isTime(4,config->thuBell[i][0],config->thuBell[i][1]))
    {
      bell = true;
    } 
    
    if (isTime(5,config->friBell[i][0],config->friBell[i][1]))
    {
      bell = true;
    } 
    if (isTime(6,config->friBell[i][0],config->friBell[i][1]))
    {
      bell = true;
    }
    
    if (isTime(0,config->sunBell[i][0],config->sunBell[i][1]))
    {
      bell = true;
    }
    
  } 
}

bool BellTimer::isTime(int day,int hour,int minute)
{
  if (timeClient->getDay() != day)
  {
    return false;
  }
  
   if(timeClient->getHours() != hour)
   {
    return false;
   }
   
   if(timeClient->getMinutes() != minute)
   {
    return false;
   }

   if(timeClient->getSeconds() >= 5)
   {
      return false;
   }

   return true;
}

void BellTimer::dzwon()
{ 
  digitalWrite(D0,HIGH);
  Serial.println("Dzwonek zaczoł dzwonić");
  delay(config->bellDuration*1000);
  Serial.println("Dzwonek przestał dzwonić"); 
  digitalWrite(D0,LOW);
}

void BellTimer::alert()
{
  dzwon();
  delay(3000);
  dzwon();
  delay(3000);
  dzwon();
  delay(15000);

  
  dzwon();
  delay(3000);
  dzwon();
  delay(3000);
  dzwon(); 

   
}


String BellTimer::getTime()
{
  String str = "";
  str += timeClient->getHours();
  str += ":";
  str += timeClient->getMinutes();
  str += ":";
  str += timeClient->getSeconds();
  return str;
}

String BellTimer::toNextBell()
{ 
  int bellNumber = -1;
  String str = "";

  switch (timeClient->getDay())
  {
    case 0:  

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours() == config->sunBell[i][0])
        {
          if (timeClient->getMinutes() < config->sunBell[i][1])
          {
            bellNumber = i;
            str += String(config->sunBell[i][0]);
            str += ":";
            str += String(config->sunBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+1) == config->sunBell[i][0] && i != 23)
        {

          bellNumber = i;
          str += String(config->sunBell[i][0]);
          str += ":";
          str += String(config->sunBell[i][1]);
          return str;

        }
        
      }
      
      break;
    case 1: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours() == config->monBell[i][0])
        {
          if (timeClient->getMinutes() < config->monBell[i][1])
          {
            bellNumber = i;
            str += String(config->monBell[i][0]);
            str += ":";
            str += String(config->monBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+1) == config->monBell[i][0] && i != 23)
        {

          bellNumber = i;
          str += String(config->monBell[i][0]);
          str += ":";
          str += String(config->monBell[i][1]);
          return str;

        }
        
      }
      
      break;
    case 2: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours() == config->tueBell[i][0])
        {
          if (timeClient->getMinutes() < config->tueBell[i][1])
          {
            bellNumber = i;
            str += String(config->tueBell[i][0]);
            str += ":";
            str += String(config->tueBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+1) == config->tueBell[i][0] && i != 23)
        {

          bellNumber = i;
          str += String(config->tueBell[i][0]);
          str += ":";
          str += String(config->tueBell[i][1]);
          return str;

        }
        
      }
      
      break;
    case 3: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours() == config->wedBell[i][0])
        {
          if (timeClient->getMinutes() < config->wedBell[i][1])
          {
            bellNumber = i;
            str += String(config->wedBell[i][0]);
            str += ":";
            str += String(config->wedBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+1) == config->wedBell[i][0] && i != 23)
        {

          bellNumber = i;
          str += String(config->wedBell[i][0]);
          str += ":";
          str += String(config->wedBell[i][1]);
          return str;

        }
        
      }
      
      break;
    case 4: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours() == config->thuBell[i][0])
        {
          if (timeClient->getMinutes() < config->thuBell[i][1])
          {
            bellNumber = i;
            str += String(config->thuBell[i][0]);
            str += ":";
            str += String(config->thuBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+1) == config->thuBell[i][0] && i != 23)
        {

          bellNumber = i;
          str += String(config->thuBell[i][0]);
          str += ":";
          str += String(config->thuBell[i][1]);
          return str;

        }
        
      }
      
      break;
    case 5: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours() == config->friBell[i][0])
        {
          if (timeClient->getMinutes() < config->friBell[i][1])
          {
            bellNumber = i;
            str += String(config->friBell[i][0]);
            str += ":";
            str += String(config->friBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+1) == config->friBell[i][0] && i != 23)
        {

          bellNumber = i;
          str += String(config->friBell[i][0]);
          str += ":";
          str += String(config->friBell[i][1]);
          return str;

        }
        
      }
      
      break;
    case 6: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours() == config->satBell[i][0])
        {
          if (timeClient->getMinutes() < config->satBell[i][1])
          {
            bellNumber = i;
            str += String(config->satBell[i][0]);
            str += ":";
            str += String(config->wedBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+1) == config->satBell[i][0] && i != 23)
        {

          bellNumber = i;
          str += String(config->satBell[i][0]);
          str += ":";
          str += String(config->satBell[i][1]);
          return str;

        }
        
      }
      
      break;

    default:
      break;
  }

  if (bellNumber == -1)
  {
    return "Tomorrow !";
  }
 
  return str; 
}

