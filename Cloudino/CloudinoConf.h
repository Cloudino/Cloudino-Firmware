struct config_t
{
  char ini[4]="ini";
  char mdns_dns[16];
  uint16 mdns_ttl;  
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
} configuration;

String toStringID(unsigned long x)
{
  String id=String(x,16);
  id.toUpperCase();
  return id;
}
