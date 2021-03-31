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
  timeClient = new NTPClient( *this->ntpUDP, "ntp.itl.waw.pl", utcOffsetInSeconds,updateInMs);
  
  timeClient->begin();
  timeClient->forceUpdate();
}

void BellTimer::handleBell()
{
  if(lastUpdate != timeClient->getHours()+config->przestawienieCzasu)
  {  
    lastUpdate = timeClient->getHours()+config->przestawienieCzasu;
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
  
   if(timeClient->getHours()+config->przestawienieCzasu != hour)
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
  digitalWrite(D1,HIGH);
  Serial.println("Dzwonek zaczoł dzwonić");
  delay(config->bellDuration*1000);
  Serial.println("Dzwonek przestał dzwonić"); 
  digitalWrite(D0,LOW);
  digitalWrite(D1,LOW);
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
  if(timeClient->getHours()+config->przestawienieCzasu<10)
  {
    str += ("0");
  }
  str += timeClient->getHours()+config->przestawienieCzasu;
  str += ":";
  if(timeClient->getMinutes()<10)
  {
    str += ("0");
  }
  str += timeClient->getMinutes();
  str += ":";
  if(timeClient->getSeconds()<10)
  {
    str += ("0");
  }
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
        if (timeClient->getHours()+config->przestawienieCzasu == config->sunBell[i][0])
        {
          if (timeClient->getMinutes() < config->sunBell[i][1])
          {
            bellNumber = i;
            
            if(config->sunBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->sunBell[i][0]);
            str += ":";
            if(config->sunBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->sunBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+config->przestawienieCzasu+1) == config->sunBell[i][0] && i != 23)
        {

            bellNumber = i;
            
            if(config->sunBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->sunBell[i][0]);
            str += ":";
            if(config->sunBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->sunBell[i][1]);
            return str;

        }
        
      }
      
      break;
    case 1: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours()+config->przestawienieCzasu == config->monBell[i][0])
        {
          if (timeClient->getMinutes() < config->monBell[i][1])
          {
            bellNumber = i;
            
            if(config->monBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->monBell[i][0]);
            str += ":";
            if(config->monBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->monBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+config->przestawienieCzasu+1) == config->monBell[i][0] && i != 23)
        {
            bellNumber = i;
            
            if(config->monBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->monBell[i][0]);
            str += ":";
            if(config->monBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->monBell[i][1]);
            return str;

        }
        
      }
      
      break;
    case 2: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours()+config->przestawienieCzasu == config->tueBell[i][0])
        {
          if (timeClient->getMinutes() < config->tueBell[i][1])
          {
            bellNumber = i;
            
            if(config->tueBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->tueBell[i][0]);
            str += ":";
            if(config->tueBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->tueBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+config->przestawienieCzasu+1) == config->tueBell[i][0] && i != 23)
        {
            bellNumber = i;
            
            if(config->tueBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->tueBell[i][0]);
            str += ":";
            if(config->tueBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->tueBell[i][1]);
            return str;
        }
        
      }
      
      break;
    case 3: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours()+config->przestawienieCzasu == config->wedBell[i][0])
        {
          if (timeClient->getMinutes() < config->wedBell[i][1])
          {
            bellNumber = i;
            
            if(config->wedBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->wedBell[i][0]);
            str += ":";
            if(config->wedBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->wedBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+config->przestawienieCzasu+1) == config->wedBell[i][0] && i != 23)
        {
            bellNumber = i;
            
            if(config->wedBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->wedBell[i][0]);
            str += ":";
            if(config->wedBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->wedBell[i][1]);
            return str;

        }
        
      }
      
      break;
    case 4: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours()+config->przestawienieCzasu == config->thuBell[i][0])
        {
          if (timeClient->getMinutes() < config->thuBell[i][1])
          {
            bellNumber = i;
            
            if(config->thuBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->thuBell[i][0]);
            str += ":";
            if(config->thuBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->thuBell[i][1]);
            return str;

          }
        }

        if ((timeClient->getHours()+config->przestawienieCzasu+1) == config->thuBell[i][0] && i != 23)
        {
            bellNumber = i;
            
            if(config->thuBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->thuBell[i][0]);
            str += ":";
            if(config->thuBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->thuBell[i][1]);
            return str;

        }
        
      }
      
      break;
    case 5: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours()+config->przestawienieCzasu == config->friBell[i][0])
        {
          if (timeClient->getMinutes() < config->friBell[i][1])
          {
            bellNumber = i;
            
            if(config->friBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->friBell[i][0]);
            str += ":";
            if(config->friBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->friBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+config->przestawienieCzasu+1) == config->friBell[i][0] && i != 23)
        {
            bellNumber = i;
            
            if(config->friBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->friBell[i][0]);
            str += ":";
            if(config->friBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->friBell[i][1]);
            return str;

        }
        
      }
      
      break;
    case 6: 

      for (int i = 0; i < 24; i++)
      {
        if (timeClient->getHours()+config->przestawienieCzasu == config->satBell[i][0])
        {
          if (timeClient->getMinutes() < config->satBell[i][1])
          {
            bellNumber = i;
            
            if(config->satBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->satBell[i][0]);
            str += ":";
            if(config->satBell[i][1]<10)
            {
              str += ("0");
            }
            str += String(config->satBell[i][1]);
            return str;
          }
        }

        if ((timeClient->getHours()+config->przestawienieCzasu+1) == config->satBell[i][0] && i != 23)
        {
            bellNumber = i;
            
            if(config->satBell[i][0]<10)
            {
              str += ("0");
            }
            str += String(config->satBell[i][0]);
            str += ":";
            if(config->satBell[i][1]<10)
            {
              str += ("0");
            }
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
    return "Not today !";
  }
 
  return str; 
}

