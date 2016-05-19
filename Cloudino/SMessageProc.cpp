/*
  SMessageProc.cpp - MessageProcessor for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#include "SMessageProc.h"

//|L|17|hola %d, JAVIER10
//|M5|helloS6|mundo!
//|M3|ledS2|on

extern config_t configuration;

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
                uint16_t y=Serial.parseInt();
                if(waitSerial() && Serial.read()==SEP)
                {
                  char msg[y+1];
                  Serial.readBytes(msg, y);
                  msg[y]=0;                 //cerrar el string   
                  //Serial.println(msg);
                  if(messageHandler!=NULL)
                  {
                    messageHandler(topic,msg);
                  }
                }
              }
            }
          }
        }else if(d==LSEP)
        {
          uint16_t x=Serial.parseInt();
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

/**
 * Post Message to Arduino or JavaScript
 */
void MessageProc::post(String topic, String data)
{
  //TODO
  //Verificar si hay o no arduino
  if(configuration.arduino_active)
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

  //de lo contrario enviar a Javascript
  processServerMessage(topic,data);
}

void MessageProc::onMessage(MessageFunction fn)
{
  messageHandler=fn;
}

void MessageProc::onServerMessage(MessageFunction fn)
{
  serverMessageHandler=fn;
}

void MessageProc::onLog(LogFunction fn)
{
  logHandler=fn;
}

boolean MessageProc::waitSerial()
{
  unsigned long time=millis();
  while(Serial.available()==0 && millis()-time<1000){delay(1);}
  return Serial.available()>0;
}

void MessageProc::processServerMessage(String topic, String msg)
{
  serverMessageHandler(topic,msg);
}

void MessageProc::clear()
{
}
