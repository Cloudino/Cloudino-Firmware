#ifndef BLYNKCONNECTOR_H
#define BLYNKCONNECTOR_H

class BLYNKConnector : public CloudConnector
{
private:

  bool connect()
  {
    bool ret=isConnected(); 
    //Serial.println("Conecting...");
    if(!ret)
    {

    }
    //Serial.println("ret:"+String(ret));
    return ret;
  }
  
public:
  BLYNKConnector(MessageProc &proc,config_t &config): CloudConnector(proc,config)
  {
    Blynk.config(_config->blynk_auth);
  }
  
  ~BLYNKConnector() // destructor
  {
    //client.disconnect();
  }

  String getName()
  {
    return "BLYNK";
  }

  void onMessage(String topic,String message)
  {
    //Serial.println("onMessage");
    if(connect())
    {
      //Serial.println("publish:"+topic);
    }
  }

  void onLog(String log)
  {
    
  }

  bool isConnected()
  {
    return true;
    //return client.connected();
  }

  void close()
  {
    //if (client.connected())client.disconnect();
  }  

  boolean isUploading()
  {
    return false;
  }  

  void loop()
  {
    Blynk.run();
  }

protected:
}; 
#endif
