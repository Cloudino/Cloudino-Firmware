/*
  CloudinoConf.h - Configuration for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef CLOUDINO_CONF_H
#define CLOUDINO_CONF_H

struct config_t
{
  char ini[4]="ini";
  char auth_user[21];
  char auth_passwd[21];  
  char mdns_dns[16];
  uint16 mdns_ttl;
  boolean arduino_active;
  uint8 arduino_gpio;
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
