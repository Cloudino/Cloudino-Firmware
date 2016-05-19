/*
  MQTTConnector.h - MQTTConnector for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef MQTTCONNECTOR_H
#define MQTTCONNECTOR_H

MessageProc *__proc;

class MQTTConnector : public CloudConnector
{
private:
  WiFiClient ethClient;
  PubSubClient client;
  bool connect()
  {
    bool ret=isConnected(); 
    //Serial.println("Conecting...");
    if(!ret)
    {
      //Serial.println("c1");
      if (client.connect("Cloudino",_config->mqtt_user,_config->mqtt_passwd)) {
        //Serial.println("c2");
        if(strlen(_config->mqtt_sub_path)>0)
          client.subscribe(_config->mqtt_sub_path);
        //Serial.println("Conected");
        ret=true;
      }   
      //Serial.println("c3");       
    }
    //Serial.println("ret:"+String(ret));
    return ret;
  }
  
public:
  MQTTConnector(MessageProc &proc,config_t &config): CloudConnector(proc,config), ethClient(), client(ethClient)
  {
      __proc=&proc;
      //Serial.println("ini");
      client.setServer(config.mqtt_dns, config.mqtt_port);
      client.setCallback([](char* topic, uint8_t* payload, unsigned int length) {
        String tp=String(topic);
        char data[length+1];
        for(int i=0;i<length;i++)data[i]=payload[i];
        tp=tp.substring(tp.lastIndexOf("/")+1);
        __proc->post(tp,data);
      });
    //Serial.println("ini 2");
    connect();
  }
  
  ~MQTTConnector() // destructor
  {
    //client.disconnect();
  }

  String getName()
  {
    return "MQTT";
  }

  void onMessage(String topic,String message)
  {
    //Serial.println("onMessage");
    if(connect())
    {
      //Serial.println("publish:"+topic);
      client.publish((char *)(String(_config->mqtt_pub_path)+topic).c_str(),(char *)message.c_str());
    }
  }

  void onLog(String log)
  {
    
  }

  bool isConnected()
  {
    return client.connected();
  }

  void close()
  {
    if (client.connected())client.disconnect();
  }  

  boolean isUploading()
  {
    return false;
  }  

  void loop()
  {
    if (client.connected())
    {
      client.loop();
    }
  }

protected:
}; 
#endif
