/*
  Cloudino.ino - Library for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>
//#include <ArduinoJson.h>
#include "CloudinoConf.h"
config_t configuration; 
#include "EEPROMAnything.h"

#ifdef CDINO_ARDUINO 
#ifdef CDINO_UPLOADER
#include "CloudinoUploader.h"
#endif
#endif

#include "SMessageProc.h"
MessageProc proc;

#ifdef JS_SUPPORT
#include "TinyJS.h"
#include "TinyJS_Functions.h"
#include "JSTimer.h"
CTinyJS *js = new CTinyJS();
Timer *timer= new Timer();
#endif

#include "CloudConnector.h"
#ifdef CDINO_CONNECTOR
#include "CloudinoConnector.h"
#endif
#ifdef OCB_CONNECTOR
#include "OrionConnector.h"
#endif
#ifdef MQTT_CONNECTOR
#include <PubSubClient.h>
#include "MQTTConnector.h"
#endif
#ifdef BLYNK_CONNECTOR
#include "BlynkConnector.h"
#endif

//ADC_MODE(ADC_VCC);

#ifdef CDINO_ARDUINO 
#ifdef CDINO_UPLOADER
CloudinoUploader uploader(9494);
#endif
#endif
CloudConnector *connect;

#ifdef JS_SUPPORT
void callBack(const String funct)
{
  js->execute(funct);
  //js->blockExecute(funct);
}

void onPostMessage(String topic,String message)
{
  //Serial.println("onPostMessage:"+topic+"->"+message);
  {
    String fs;
    fs+=F("Cloudino._on.");
    fs+=topic;
    CScriptVar *f=js->getScriptVariable(fs);
    //Serial.println(f==0);
    if(f)
    {
      CScriptVar *v=js->getScriptVariable(F("Cloudino._msg"));
      v->setString(message);
      js->execute(fs+F("(Cloudino._msg);"));     
    }  
  }

  {
    String fs;
    fs+=F("Cloudino.onAny");
    CScriptVar *f=js->getScriptVariable(fs);
    //Serial.println(f==0);
    if(f)
    {
      CScriptVar *m=js->getScriptVariable(F("Cloudino._msg"));
      m->setString(message);
      CScriptVar *t=js->getScriptVariable(F("Cloudino._tp"));
      t->setString(topic);    
      //Serial.println(fs+F("(Cloudino._tp,Cloudino._msg);"));
      js->execute(fs+F("(Cloudino._tp,Cloudino._msg);"));   
      //Serial.println("end");  
    } 
  }       
}

void onLocalPostMessage(String topic,String message)
{
  //Serial.println("onPostMessage:"+topic+"->"+message);
  {
    String fs;
    fs+=F("Cloudino._onLocal.");
    fs+=topic;
    CScriptVar *f=js->getScriptVariable(fs);
    //Serial.println(f==0);
    if(f)
    {
      CScriptVar *v=js->getScriptVariable(F("Cloudino._msg"));
      v->setString(message);
      js->execute(fs+F("(Cloudino._msg);"));     
    }  
  }

  {
    String fs;
    fs+=F("Cloudino.onLocalAny");
    CScriptVar *f=js->getScriptVariable(fs);
    //Serial.println(f==0);
    if(f)
    {
      CScriptVar *m=js->getScriptVariable(F("Cloudino._msg"));
      m->setString(message);
      CScriptVar *t=js->getScriptVariable(F("Cloudino._tp"));
      t->setString(topic);    
      //Serial.println(fs+F("(Cloudino._tp,Cloudino._msg);"));
      js->execute(fs+F("(Cloudino._tp,Cloudino._msg);"));   
      //Serial.println("end");  
    } 
  }           
}
#endif

#include "CloudinoWeb.h"

void onMessage(String topic,String message)
{
  //Serial.print("onMessage:"+topic+":"+message);
  if(connect && wifiConnected)connect->onMessage(topic,message);
}

void onLog(String log)
{
  if(connect && wifiConnected)connect->onLog(log);
}

void setup() {
  //Serial.begin(57600);
  //WiFi.mode(WIFI_AP_STA);
  //WiFi.enableAP(true);
  initEEPROMConfig();
  SPIFFS.begin();
  
#ifdef CDINO_ARDUINO 
#ifdef CDINO_UPLOADER
  uploader.begin();  
#endif  
#endif
  proc.onMessage(onMessage);
  proc.onLog(onLog);

#ifdef CDINO_WEBSERVER   
  initWebServer();
#endif  

#ifdef JS_SUPPORT
  timer->setGlobalCallBack(callBack);
  registerFunctions(js,timer,&proc);
  proc.onServerMessage(onPostMessage);
  proc.onLocalMessage(onLocalPostMessage);
#endif
}

void loop() { 
  if(connect && connect->isUploading())
  {
    connect->loop();
    return;
  }
#ifdef CDINO_ARDUINO 
#ifdef CDINO_UPLOADER   
  uploader.handleClient();  
  if(!uploader.isUploading())
#endif  
#endif
  {
#ifdef CDINO_WEBSERVER      
    loopWebServer();
#endif    
    proc.handleMessages();
    
    if(wifiConnected && connect)connect->loop();
    if(resetConnector)
    {
      //Serial.println("Reseting Connector");
      resetConnector=false;
      if(connect)
      {
        connect->close();
        delete connect;
        connect=NULL;
      }
      if(false)
      {        
#ifdef CDINO_CONNECTOR        
      }else if(configuration.cdino_active){
        connect=new CloudinoConnector(proc,configuration);
#endif        
#ifdef OCB_CONNECTOR        
      }else if(configuration.ocb_active){
        connect=new OrionConnector(proc,configuration);
#endif        
#ifdef MQTT_CONNECTOR        
      }else if(configuration.mqtt_active){
        connect=new MQTTConnector(proc,configuration);
#endif        
#ifdef BLYNK_CONNECTOR        
      }else if(configuration.blynk_active){
        connect=new BLYNKConnector(proc,configuration);
#endif        
      }    
    }

#ifdef JS_SUPPORT
    timer->loop();
#endif  
  }
}
