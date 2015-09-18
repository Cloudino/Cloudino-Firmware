#include "SMessageProc.h"

//|L|17|hola %d, JAVIER10
//|M5|helloS6|mundo!
//|M3|ledS2|on

void MessageProc::handleMessages()
{
  if(Serial.available())
  {
    uint8_t d=Serial.read();
    //Serial.println("D:"+String(d));
    if(d==SEP)
    {
      if(waitSerial())
      {
        d=Serial.read();
        if(d==MSEP)
        {
          uint8_t x=Serial.parseInt();
          if(waitSerial())
          {
            if(x>0 && Serial.read()==SEP)
            {
              char topic[x+1];
              Serial.readBytes(topic, x);
              topic[x]=0;                 //cerrar el string
              //Serial.println(topic);
              if(waitSerial() && Serial.read()==SSEP)
              {
                uint8_t y=Serial.parseInt();
                if(waitSerial() && Serial.read()==SEP)
                {
                  char msg[y+1];
                  Serial.readBytes(msg, y);
                  msg[y]=0;                 //cerrar el string   
                  //Serial.println(msg);
                  messageHandler(topic,msg);
                }
              }
            }
          }
        }else if(d==LSEP)
        {
          uint8_t x=Serial.parseInt();
          if(waitSerial())
          {
            if(x>0 && Serial.read()==SEP)
            {
              char txt[x+1];
              Serial.readBytes(txt, x);
              txt[x]=0;                 //cerrar el string
              logHandler(txt);
            }
          }
        }
      }   
    }
  }        
}

void MessageProc::post(String topic, String data)
{
  Serial.print(SEP);
  Serial.print(MSEP);
  Serial.print(topic.length());  
  Serial.print(SEP);  
  Serial.print(topic);    
  //Serial.print(SEP);
  Serial.print(SSEP);
  Serial.print(data.length());  
  Serial.print(SEP);
  Serial.print(data);      
}

void MessageProc::onMessage(MessageFunction fn)
{
  messageHandler=fn;
}

void MessageProc::onLog(LogFunction fn)
{
  logHandler=fn;
}

boolean MessageProc::waitSerial()
{
  unsigned long time=millis();
  while(!Serial.available() && millis()-time<1000){delay(1);}
  return Serial.available();
}
