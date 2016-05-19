/*
  CloudUploder.cpp - Uploader for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/
#include <Arduino.h>
#include "WiFiServer.h"
#include "WiFiClient.h"
#include "CloudinoUploader.h"
#include "CloudinoConf.h"

extern config_t configuration;

void CloudinoUploader::begin()
{
    Serial.begin(SERIAL_SPEED);  
    pinMode(configuration.arduino_gpio, OUTPUT);  
    digitalWrite(configuration.arduino_gpio, HIGH);  
    _uplSevr.begin();
    //_uplSevr.setNoDelay(true);    
}

void CloudinoUploader::handleClient()
{
    if (_uplSevr.hasClient())
    {
      if(_uplClient)
      {
        if(_uplClient.connected())_uplClient.stop();
      }
      _uplClient = _uplSevr.available();
      //whait for init
      unsigned long time=millis();
      long speed=0;
      byte counter=0;
      boolean stop=false;
      while(counter<5)
      {
        if(_uplClient.available())
        {
          byte b=_uplClient.read();
          _uplClient.write(b);
          if(counter==0 && b!=0)
          {
            stop=true;
          }else
          {
            speed=(speed<<8)+b;
          }
          counter++;
        }
        if(stop || ((millis()-time)>5000))
        {
          _uplClient.write("Error");
          _uplClient.stop();
          return;
        }
      }
      Serial.end();
      digitalWrite(configuration.arduino_gpio, LOW);   // turn the LED on (LOW is the voltage level)
      delay(50);              // wait for a second
      digitalWrite(configuration.arduino_gpio, HIGH);    // turn the LED off by making the voltage HIGH
      delay(50);
      Serial.begin(speed);
      uploading=true;
    }

    if(uploading && _uplClient)
    {
      if(_uplClient.connected())
      {
        while(Serial.available()) _uplClient.write(Serial.read());
        while(_uplClient.available()) Serial.write(_uplClient.read());
      }else 
      {
        uploading=false;
        Serial.end();
        Serial.begin(SERIAL_SPEED);
      }
    }
}

