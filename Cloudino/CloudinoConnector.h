#ifndef CLOUDINOCONNECTOR_H
#define CLOUDINOCONNECTOR_H

class CloudinoConnector : public CloudConnector
{
private:
  boolean uploading=false;
  long speed;

  WiFiClient client;

  boolean waitClient()
  {
    unsigned long time=millis();
    while(!client.available() && millis()-time<1000){delay(1);}
    return client.available();
  }
  
  bool connect()
  {
    if(!isConnected())
    {
      if(millis()-time>5000)
      {
        time=millis();
        if (!client.connect(_config->cdino_dns, _config->cdino_port)) {
          //Serial.println("connection failed");//todo:callback
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
        //Serial.println("Connect");//todo:callback   
        return true;  
      }
      return false;
    }
    return true;
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
    }
  }

  void onLog(String log)
  {
    if(connect())
    {
      client.print(SEP);
      client.print(LSEP);
      client.print(log.length());  
      client.print(SEP);  
      client.print(log);    
    }
  }

  bool isConnected()
  {
    return client.connected();
  }

  
  void postSerial(String topic,String data)
  {
      //Serial.println(topic);
      //Serial.println(data);
      if(topic=="$CDINOUPDT")
      {
        uploading=true;
        speed=atol(data.c_str());
        Serial.end();
        digitalWrite(RST_PIN, LOW);   // turn the LED on (LOW is the voltage level)
        delay(50);              // wait for a second
        digitalWrite(RST_PIN, HIGH);    // turn the LED off by making the voltage HIGH
        delay(50);
        Serial.begin(speed);
        uploading=true;
        return;
      }
    
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
                  uint8_t y=client.parseInt();
                  if(waitClient() && client.read()==SEP)
                  {
                    char msg[y+1];
                    client.readBytes(msg, y);
                    msg[y]=0;                 //cerrar el string   
                    //Serial.println(msg);
                    postSerial(topic,msg);
                  }
                }
              }
            }
          }
        }   
      }
    }        
  }
  
  void loop()
  {
    if(!uploading && connect())
    {
      WebMessages();
    }       

    if(uploading && client)
    {
      if(client.connected())
      {
        while(Serial.available()) client.write(Serial.read());
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
  unsigned long time=millis();
}; 
#endif
