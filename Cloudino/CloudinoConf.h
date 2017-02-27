/*
  CloudinoConf.h - Configuration for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef CLOUDINO_CONF_H
#define CLOUDINO_CONF_H

#define CDINO_VERSION F("0.9.14")

#define CDINO_ARDUINO
#define CDINO_UPLOADER
#define CDINO_CONNECTOR
#define OCB_CONNECTOR
#define MQTT_CONNECTOR
#define COAP_CONNECTOR
#define BLYNK_CONNECTOR
#define JS_SUPPORT
#define HTTP_UPDATE
#define CAPTIVE_PORTAL
//#define HTTPS_SUPPORT
//#define RCSWITCH_SUPPORT

#ifdef HTTP_UPDATE
#define CDINO_UPDURL F("http://cloudino.cc/firmware/cloudino/esp-12/Cloudino.bin")
#define CDINO_VERSIONPATH F("/firmware/cloudino/esp-12/version.txt")
#endif

#define UDP_PORT 10234

struct config_t
{
  char ini[4]="ini";
  char auth_user[21];
  char auth_passwd[21];  
  char mdns_dns[16];
  uint16 mdns_ttl;
#ifdef CDINO_ARDUINO  
  boolean arduino_active;
  uint8 arduino_gpio;
#endif  
#ifdef CDINO_CONNECTOR 
  boolean cdino_active;
  char cdino_dns[51];
  uint16 cdino_port;
  char cdino_token[51];
  char cdino_authtoken[51];
#endif
#ifdef OCB_CONNECTOR
  boolean ocb_active;
  char ocb_dns[51];
  uint16 ocb_port;
  char ocb_token[51];
  char ocb_devid[51];
#endif
#ifdef MQTT_CONNECTOR        
  boolean mqtt_active;
  char mqtt_dns[51];
  uint16 mqtt_port;
  char mqtt_user[21];
  char mqtt_passwd[21];    
  char mqtt_pub_path[101];
  char mqtt_sub_path[101];    
#endif
#ifdef BLYNK_CONNECTOR        
  boolean blynk_active;
  char blynk_dns[51];
  uint16 blynk_port;
  char blynk_auth[51];  
#endif
//#ifdef JS_SUPPORT
  boolean js_active;
  uint16 js_size;
//#endif
};

#endif
