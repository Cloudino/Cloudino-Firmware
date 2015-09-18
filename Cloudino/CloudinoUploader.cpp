#include <Arduino.h>
#include "WiFiServer.h"
#include "WiFiClient.h"
#include "CloudinoUploader.h"

void CloudinoUploader::begin()
{
    Serial.begin(SERIAL_SPEED);  
    pinMode(RST_PIN, OUTPUT);  
    digitalWrite(RST_PIN, HIGH);  
    _uplSevr.begin();
    _uplSevr.setNoDelay(true);    
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
      digitalWrite(RST_PIN, LOW);   // turn the LED on (LOW is the voltage level)
      delay(50);              // wait for a second
      digitalWrite(RST_PIN, HIGH);    // turn the LED off by making the voltage HIGH
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

