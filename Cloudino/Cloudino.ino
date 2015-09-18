#define CDINO_UPLOADER
#define CDINO_CONNECTOR
#define OCB_CONNECTOR
#define MQTT_CONNECTOR
#define COAP_CONNECTOR
#define BLYNK_CONNECTOR

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
//#include <ESP8266httpUpdate.h>
//#include <ArduinoJson.h>
#include "WebServer.h"
#include "CloudinoConf.h"
#ifdef CDINO_UPLOADER
#include "CloudinoUploader.h"
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

ADC_MODE(ADC_VCC);

WebServer webserver(80);
#ifdef CDINO_UPLOADER
CloudinoUploader uploader(9494);
#endif
MessageProc proc;
CloudConnector *connect;

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
  initEEPROMConfig();
#ifdef CDINO_UPLOADER
  uploader.begin();  
#endif  
  proc.onMessage(onMessage);
  proc.onLog(onLog);
  initWebServer();
}

void loop() {
#ifdef CDINO_UPLOADER  
  if(connect && connect->isUploading())
  {
    connect->loop();
    return;
  }
  uploader.handleClient();  
  if(!uploader.isUploading())
#endif  
  {
    chechConnection();
    
    webserver.handleClient();
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
  }
}
