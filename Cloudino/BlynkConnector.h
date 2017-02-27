/*
  BlynkConnector.h - Connector for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef BLYNKCONNECTOR_H
#define BLYNKCONNECTOR_H

#include <BlynkSimpleEsp8266.h>

// This is called for all virtual pins, that don't have BLYNK_WRITE handler
BLYNK_WRITE_DEFAULT() {
  String tp="V"+String(request.pin);
  String msg=param.begin().asString();
  //Serial.println("1.-"+tp+":"+msg);
/*  
  Serial.print("input V");
  Serial.print(request.pin);
  Serial.println(":");
  // Print all parameter values
  for (auto i = param.begin(); i < param.end(); ++i) {
    Serial.print("* ");
    Serial.println(i.asString());
  }
*/  
  proc.post(tp,msg);    
  //Serial.println("2.-"+tp+":"+msg);
}

/*
// This is called for all virtual pins, that don't have BLYNK_READ handler
BLYNK_READ_DEFAULT() {
  // Generate random response
  int val = random(0, 100);
  Serial.print("output V");
  Serial.print(request.pin);
  Serial.print(": ");
  Serial.println(val);
  Blynk.virtualWrite(request.pin, val);
}  
*/

class BLYNKConnector : public CloudConnector
{
private:

  bool connect()
  {
    if(!isConnected())
    {
      if((millis()-time)>tdelay)
      {
        time=millis();
        if (!Blynk.connect()) {
          //Serial.println("Bkink connection failed");//todo:callback
          tdelay=15000;
          return false;
        }else
        {
          //Serial.println("Blynk Connect");
          //Blynk.syncAll();
        }
        return true;  
      }
      return false;
    }
    return true;
  }
  
public:
  BLYNKConnector(MessageProc &proc,config_t &config): CloudConnector(proc,config)
  {
    //Serial.begin(SERIAL_SPEED); 
    //Serial.println("\ninit");
    Blynk.config(_config->blynk_auth,_config->blynk_dns,_config->blynk_port);   
  }
  
  ~BLYNKConnector() // destructor
  {
    Blynk.disconnect();
  }

  String getName()
  {
    return "BLYNK";
  }

  void onMessage(String topic,String message)
  {
    //Serial.println("onMessage:"+topic+":"+message);
    if(connect())
    {
        if(topic.startsWith("V"))
        {
            Blynk.virtualWrite(atoi(topic.substring(1).c_str()),message);
        }else if(topic=="notify")
        {
            Blynk.notify(message);
        }else if(topic=="tweet")
        {
            Blynk.tweet(message);
        }else if(topic=="sms")
        {
            Blynk.sms(message);
        }
    }
  }

  void onLog(String log)
  {
    
  }

  bool isConnected()
  {
    return Blynk.connected();
  }

  void close()
  {
    if (Blynk.connected())Blynk.disconnect();
  }  

  boolean isUploading()
  {
    return false;
  }  

  void loop()
  {
    Blynk.run();

    if(connect())
    {
      if((millis()-time)>tresync){
        Blynk.syncAll();
        time=millis();
        tresync=1000*60;
      }
    }
  }

protected:
  unsigned int tdelay=1000;
  unsigned int tresync=2000;
  unsigned long time=millis();
}; 
#endif
