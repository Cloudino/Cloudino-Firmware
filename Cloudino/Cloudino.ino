/*
  Cloudino.ino - Library for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#define CDINO_VERSION "0.9.8"

#define CDINO_UPLOADER
#define CDINO_CONNECTOR
#define OCB_CONNECTOR
#define MQTT_CONNECTOR
//#define COAP_CONNECTOR
//#define BLYNK_CONNECTOR
#define JS_SUPPORT
#define HTTP_UPDATE
#define CAPTIVE_PORTAL

#ifdef HTTP_UPDATE
#define CDINO_UPDURL F("http://cloudino.io/firmware/cloudino/esp-12/Cloudino.bin")
#define CDINO_VERSIONPATH F("/firmware/cloudino/esp-12/version.txt")
#endif

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
//#include <ArduinoJson.h>
#include "CloudinoConf.h"
config_t configuration;
#include "EEPROMAnything.h"

#ifdef CDINO_UPLOADER
#include "CloudinoUploader.h"
#endif

#ifdef JS_SUPPORT
#include "TinyJS.h"
#include "TinyJS_Functions.h"
#include "JSTimer.h"
CTinyJS *js = new CTinyJS();
Timer *timer= new Timer();
#endif

#include "SMessageProc.h"
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
#include <BlynkSimpleEsp8266.h>
#include "BLYNKConnector.h"
#endif

//ADC_MODE(ADC_VCC);



#ifdef CDINO_UPLOADER
CloudinoUploader uploader(9494);
#endif
MessageProc proc;
CloudConnector *connect;

#ifdef JS_SUPPORT
void callBack(const String funct)
{
  js->execute(funct);
  //js->blockExecute(funct);
}

void onPostMessage(String topic,String message)
{
  //Serial.println(topic+"->"+message);
  CScriptVarLink t = js->evaluateComplex("Cloudino._tmp");
  CScriptVar *v=t.var;
  if(v)
  {
    v->setString(message);  
  }
  String f="Cloudino._on."+topic+"(Cloudino._tmp);";
  js->execute(f);  
}
#endif

#include "CloudinoWeb.h"

void onMessage(String topic,String message)
{
  //Serial.print("onMessage:"+topic+":"+message);
  if(connect)connect->onMessage(topic,message);
}

void onLog(String log)
{
  if(connect)connect->onLog(log);
}

void setup() {
  //WiFi.mode(WIFI_AP_STA);
  //WiFi.enableAP(true);
  initEEPROMConfig();
#ifdef CDINO_UPLOADER
  uploader.begin();  
#endif  
  proc.onMessage(onMessage);
  proc.onLog(onLog);
  initWebServer();

#ifdef JS_SUPPORT
  timer->setGlobalCallBack(callBack);
  registerFunctions(js,timer,&proc);
  proc.onServerMessage(onPostMessage);
#endif
}

void loop() { 
  if(connect && connect->isUploading())
  {
    connect->loop();
    return;
  }
#ifdef CDINO_UPLOADER   
  uploader.handleClient();  
  if(!uploader.isUploading())
#endif  
  {
    loopWebServer();
    proc.handleMessages();
    
    if(wifiConnected && connect)connect->loop();
    if(resetConnector)
    {
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
