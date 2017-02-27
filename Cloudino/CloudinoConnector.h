/*
  Cloudinoonnector.h - Connector for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef CLOUDINOCONNECTOR_H
#define CLOUDINOCONNECTOR_H

#include "CloudinoUploader.h"
//#include <WiFiClientSecure.h>

#define ARDUINO_FIRMWARE_PATH "/update.bin"

class CloudinoConnector : public CloudConnector
{
private:
  boolean uploading=false;
  long speed;

  //WiFiClientSecure client;
  WiFiClient client;

  boolean waitClient()
  {
    unsigned long time=millis();
    while(client.available()==0 && millis()-time<1000){delay(1);}
    return client.available()>0;
  }
  
  bool connect()
  {
    if(!isConnected())
    {
      if((millis()-time)>tdelay)
      {
        time=millis();
        if (!client.connect(_config->cdino_dns, _config->cdino_port)) {
          //Serial.println("Cloudino Connection failed");//todo:callback
          tdelay=15000;
          return false;
        }
        String topic="$ID";
        String data=String(_config->cdino_authtoken);
        //data.toUpperCase();
        client.print(SEP);
        client.print(MSEP);
        client.print(topic.length());  
        client.print(SEP);  
        client.print(topic);    
        //Serial.print(SEP);
        client.print(SSEP);
        client.print(data.length());  
        client.print(SEP);
        client.print(data);    
        //Serial.println("Cloudino Connect");//todo:callback   
        return true;  
      }
      return false;
    }
    return true;
  }

/******* ARDUINO UPDATE ******************************/
  boolean wait_for(uint8_t c)
  {
      boolean fnd=false;
      boolean timeout=false;
      unsigned long time=millis();
      do
      {
          timeout=(millis()-time)>5000;
          while(Serial.available()==0 && !timeout)
          {
              delay(5);
              timeout=(millis()-time)>5000;
          }
          if(!timeout && Serial.available()>0)
          {
              uint8_t r=Serial.read();
              //onLog("r:"+String(r));
              if(r==c)fnd=true;
          }
      }while(!fnd && !timeout);
      return fnd;  
  }
  
  boolean acknowledge()
  {
      return wait_for(0x14) && wait_for(0x10);
  }

  boolean arduinoFlash(int speed)
  {
      boolean ret=true; 
  
      if(SPIFFS.exists(ARDUINO_FIRMWARE_PATH))
      {    
          File f = SPIFFS.open(ARDUINO_FIRMWARE_PATH, "r");
          onLog("[CCP]Start Arduino Flashing...");

          Serial.flush();
          delay(50);
          Serial.end();
          delay(100);
          Serial.begin(speed);
          delay(50);
          Serial.flush();
          delay(100);
          digitalWrite(configuration.arduino_gpio, LOW);   // turn the LED on (LOW is the voltage level)
          delay(100);              // wait for a second
          digitalWrite(configuration.arduino_gpio, HIGH);    // turn the LED off by making the voltage HIGH
          delay(400);
          
          while(f.available()) 
          {
            uint8_t c=f.read();
            if(c=='[' && f.available())
            {
                uint8_t d=f.read();     
                if(d=='A' && f.available())
                {
                    uint8_t e=f.read();     
                    if(e==']')
                    {
                        if(!acknowledge())
                        {
                            onLog("[CCP]acknowledge: error");
                            ret=false;
                            break;
                        }else
                        {
                            //onLog("acknowledge: true");
                        }
                    }else
                    {
                        Serial.write(c);
                        Serial.write(d);
                        Serial.write(e);
                    }
                }else
                {
                    Serial.write(c);
                    Serial.write(d);
                }
            }else
            {
                Serial.write(c);
                //onLog("w:"+String(c));
            }
          }
          f.close();

          Serial.end();
          Serial.begin(SERIAL_SPEED); 
          
      }else ret=false;       
      
      return ret;
  }
  
  boolean arduinoUpdate(String url, int speed)
  {
      boolean ret=true; 
      HTTPClient http;
      File fsUploadFile;
  
      onLog("[CCP]Init arduino update...\n");
  
      // configure server and url
      http.begin(url);
  
      //onLog("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      if(httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          onLog("[CCP]Server response code: "+String(httpCode)+"\n");
  
          // file found at server
          if(httpCode == HTTP_CODE_OK) {
  
              // get lenght of document (is -1 when Server sends no Content-Length header)
              int len = http.getSize();
  
              // create buffer for read
              uint8_t buff[128] = { 0 };
  
              // get tcp stream
              WiFiClient * stream = http.getStreamPtr();
  
              fsUploadFile = SPIFFS.open(ARDUINO_FIRMWARE_PATH, "w");
              // read all data from server
              while(http.connected() && (len > 0 || len == -1)) {
                  // get available data size
                  size_t size = stream->available();
  
                  if(size) {
                      // read up to 128 byte
                      int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
  
                      // write it to Serial
                      if(fsUploadFile)fsUploadFile.write(buff, c);
                      else ret=false; 
  
                      if(len > 0) {
                          len -= c;
                      }
                  }
                  delay(1);
              }
              if(fsUploadFile)fsUploadFile.close();
              else ret=false;
  
              onLog("[CCP]Server response end.\n");
          }
      } else {
          onLog("[CCP]Server response failed, error: "+http.errorToString(httpCode)+"\n");
          ret=false;
      }
      http.end();
  
      if(ret)ret=arduinoFlash(speed);
  
      return ret;
  }

  
public:
  CloudinoConnector(MessageProc &proc,config_t &config): CloudConnector(proc,config)
  {
    client.setNoDelay(true);
    connect();
  }
  
  ~CloudinoConnector() // destructor
  {
    //client.stop();
  }

  String getName()
  {
    return "Cloudino";
  }

  void onMessage(String topic,String data)
  {
    //Serial.println("onMessage");
    if(connect())
    {
        client.print(proc.encodePost(topic,data));
    }
  }

  void onLog(String log)
  {
    if(connect())
    {
      client.print(proc.encodeLog(log));
    }
  }

  bool isConnected()
  {
    return client.connected();
  }

  void WebMessages()
  {
    if(client.available())
    {
      uint8_t d=client.read();
      if(d==SEP)
      {
        if(waitClient())
        {
          d=client.read();
          if(d==MSEP)
          {
            uint8_t x=client.parseInt();
            //Serial.println(x);
            if(waitClient())
            {
              if(x>0 && client.read()==SEP)
              {
                char topic[x+1];
                client.readBytes(topic, x);
                topic[x]=0;                 //cerrar el string
                //Serial.println(topic);
                if(waitClient() && client.read()==SSEP)
                {
                  uint16_t y=client.parseInt();
                  //Serial.println(y);
                  if(waitClient() && client.read()==SEP)
                  {
                    char msg[y+1];
                    client.readBytes(msg, y);
                    msg[y]=0;                 //cerrar el string   
                    //Serial.println(msg);
                    post(topic,msg);
                  }
                }
              }
            }
          }
        }   
      }
    }        
  }

  /**
   * Post Message to Arduino
   */
  void post(String topic, String data)
  {
    //Serial.println("Topic:"+topic);
    //Serial.println("data:"+data);
#ifdef JS_SUPPORT    
    if(topic=="$CDINOJSCMD")
    {
       String ret=js->evaluate(data);
       onMessage("$CDINOJSRSP",ret);
       return;
    }else if(topic=="$CDINOJSINIT")
    {
       String script=data;
       int s=script.length();
       configuration.js_size=s+1;
       EEPROM_write(0,configuration,configuration.js_size);  
       EEPROM_write(sizeof(configuration), script.c_str(), s+1);       
       js->evaluate("reset();");
       js->execute(script);
       return;
    }else 
#endif    
#ifdef CDINO_ARDUINO 
    if(topic=="$CDINOUPDT")
    {
      uploading=true;
      speed=atol(data.c_str());
      Serial.end();
      digitalWrite(configuration.arduino_gpio, LOW);   // turn the LED on (LOW is the voltage level)
      delay(50);              // wait for a second
      digitalWrite(configuration.arduino_gpio, HIGH);    // turn the LED off by making the voltage HIGH
      delay(50);
      Serial.begin(speed);
      uploading=true;
      return;
    }else if(topic=="$CDINOUPDT2")
    {
      speed=115200;
      int i=data.indexOf('|');
      if(i>-1)
      {
        speed=atol(data.substring(0,i).c_str());
        data=data.substring(i+1);
      }

      if(arduinoUpdate("http://"+String(_config->cdino_dns)+data,speed))
      {
          onLog("[CCP]Arduino Update OK");
      }else
      {
          onLog("[CCP]Arduino Update Error...");
      }      
      return;
    }
#endif    
    _proc->post(topic,data);
  }
  
  void loop()
  {
    if(!uploading && connect())
    {
      WebMessages();
    }
#ifdef CDINO_ARDUINO
    if(uploading && client)
    {
      if(client.connected())
      {
        while(Serial.available()) client.print((char)Serial.read());
        while(client.available()) Serial.write(client.read());
      }else
      {
        uploading=false;
        Serial.end();
        Serial.begin(SERIAL_SPEED);
      }
    }else if(uploading)
    {
      uploading=false;
      Serial.end();
      Serial.begin(SERIAL_SPEED);
    }
#endif    
  }

  boolean isUploading()
  {
    return uploading;
  }

  void close()
  {
    client.stop();
  }

protected:
  unsigned int tdelay=1000;
  unsigned long time=millis();
};
#endif
