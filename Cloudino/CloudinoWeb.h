/*
  CloudWeb.h - WebInterface for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef CLOUDINOWEB_H
#define CLOUDINOWEB_H

#include "ESP8266WebServer.h"
#ifdef CAPTIVE_PORTAL
#include <DNSServer.h>
#endif

#ifdef HTTP_UPDATE
#include <WiFiUdp.h>
#endif

#include <WiFiUdp.h>
WiFiUDP udp;

extern "C" {
//#include "c_types.h"
//#include "ets_sys.h"
//#include "os_type.h"
//#include "osapi.h"
//#include "mem.h"
#include "user_interface.h"
//#include "upgrade.h"
//#include "espconn.h"
//#include "smartconfig.h"
//#include "lwip/opt.h"
//#include "lwip/err.h"
//#include "lwip/dns.h"
//#include "ssl/ssl_crypto.h"
}

bool wifiConnected=false;
bool jsInited=false;

ESP8266WebServer webserver(80);

MDNSResponder mdns;
#ifdef CAPTIVE_PORTAL
DNSServer dnsServer;
#define DNS_PORT 53
IPAddress apIP(192, 168, 4, 1);
#endif

unsigned long _time=millis();
unsigned long _time2=_time;

byte _timeCounter=0;

bool resetConnector=false;

const char home[] PROGMEM ="<html><head>\n\
<meta charset=\"utf-8\">\n\
<title>Cloudino</title>\n\
<meta name=\"viewport\" content=\"width=device-width\">\n\
<style>html,button,input,select,textarea{color:#222}\n\
body{font-size:1em;line-height:1.4;margin:0}\n\
::-moz-selection{background:#b3d4fc;text-shadow:none}\n\
::selection{background:#b3d4fc;text-shadow:none}\n\
hr{display:block;height:1px;border:0;border-top:1px solid #ccc;margin:1em 0;padding:0}\n\
img{vertical-align:middle}\n\
fieldset{border:0;margin:0;padding:0}\n\
textarea{resize:vertical}\n\
body{font:16px/26px Helvetica,Helvetica Neue,Arial}\n\
.wrapper{width:90%;margin:0 5%}\n\
.header-container{border-bottom:20px solid #BE3A46}\n\
.footer-container,.main aside{border-top:20px solid #BE3A46}\n\
.header-container,.footer-container,.main aside{background:#E63A4A}\n\
.title{color:#fff}\n\
nav ul{margin:0;padding:0}\n\
nav a{display:block;margin-bottom:10px;padding:15px 0;text-align:center;text-decoration:none;font-weight:700;color:#fff;background:#BE3A46}\n\
nav a:hover,nav a:visited{color:#fff}\n\
nav a:hover{text-decoration:underline}\n\
.main{padding:30px 0}\n\
.main article h1{font-size:2em;color:#E63A4A;}\n\
.main aside{color:#fff;padding:0 5% 10px}\n\
.footer-container *{color:#FFF;letter-spacing:1px}\n\
.footer-container{width:100%;position_:fixed;bottom:0;}\n\
.footer-container footer{text-align:center}\n\
footer a{padding:5px 20px 0;display:inline-block;font-weight:bold}\n\
footer span{ display:block;font-size:11px}\n\
#term_log * {margin: 0px -5px;}\n\
@media only screen and (min-width: 480px) {\n\
nav a{float:left;width:27%;margin:0 1.7%;padding:25px 2%;margin-bottom:0}\n\
nav li:first-child a{margin-left:0}\n\
nav li:last-child a{margin-right:0}\n\
nav ul li{display:inline}\n\
.alert{border-color: #1C1F1F;background-color: #E63A4A;color: whitesmoke;border: 2px;padding: 8;border-style: dashed;font-size: medium;}\n\
@media only screen and (min-width: 768px) {\n\
.header-container,.main aside{-webkit-box-shadow:0 5px 10px #aaa;-moz-box-shadow:0 5px 10px #aaa;box-shadow:0 5px 10px #aaa}\n\
.title{float:left}\n\
nav{float:right;width:38%}\n\
.main article{float:left;width:60%}\n\
.main aside{float:right;width:28%}\n\
}\n\
@media only screen and (min-width: 1140px) {\n\
.wrapper{width:1026px;margin:0 auto}\n\
}\n\
.clearfix:before,.clearfix:after{content:\" \";display:table}\n\
.clearfix:after{clear:both}\n\
.clearfix{*zoom:1}\n\
table{width:100%;border-collapse:collapse}\n\
tbody tr:nth-child(odd){background:rgba(230,58,74,.1);}\n\
input[type=\"button\"],input[type=\"submit\"]{width:100px;margin:5px;}\n\
input[type=\"text\"]{width:250px}\n\
.sidebar-nav{margin:0;padding:0;list-style:none}\n\
.sidebar-nav li{text-indent:20px;line-height:40px}\n\
.sidebar-nav li a{display:block;text-decoration:none;color:#FFF;}\n\
.sidebar-nav li a:hover{background:rgba(255,255,255,0.2)}</style><script type=\"text/javascript\">\n\
var cdino = {getHtml:function(url,node,method,data,prefix){var xmlhttp;if (window.XMLHttpRequest){xmlhttp=new XMLHttpRequest();}else{xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");}\n\
xmlhttp.onreadystatechange=function(){if (xmlhttp.readyState==4 && xmlhttp.status==200){if(node===null)return;if(typeof node === 'string'){document.getElementById(node).innerHTML = (prefix?prefix:\"\")+xmlhttp.responseText;}else{node.innerHTML = (prefix?prefix:\"\")+xmlhttp.responseText;}}};\n\
xmlhttp.open(!method?\"GET\":method,url,true);xmlhttp.send(data);},\n\
invoke:function(func,timeout,inter){setTimeout(func,timeout);if(inter)window.setInterval(func,inter);},\n\
seg:function(seg,next,time){if(next){cdino.invoke(function(){cdino.seg(next);},time?time:1000);}cdino.getHtml('seg/'+seg,'content');},\n\
getForm:function(form){if(!form)form=document.forms[0];if(form.checkValidity()){var ret=\"\";var arr=form.getElementsByTagName('input');for (x=0; x<arr.length; ++x){var inp=arr[x];if(inp.name && (inp.type!='radio' || inp.checked==true))ret+=\"&\"+inp.name+\"=\"+inp.value;}if(ret.length>0)return \"?\"+ret.substr(1);}},\n\
addDiv:function(node){var idv=document.createElement('div');node.appendChild(idv);return idv;},\n\
};</script>\n\
</head><body>\n\
<div class=\"header-container\">\n\
<header class=\"wrapper clearfix\">\n\
<a href=\"/\"><div class=\"title\"><svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\" x=\"0px\" y=\"0px\" width=\"50px\" height=\"70px\" viewBox=\"-413.2 0 200 200\" xml:space=\"preserve\"><style>.style0{fill:  #FFFFFF;}.style1{fill:  #FFFFFF;}.style2{fill:  #FFFFFF;}.style3{fill:  #E63A4A;}.style4{fill:  #FFFFFF;}.style5{fill:  #FFFFFF;}</style><g><path d=\"M-298.6 3.7c-4.3 0-8 2.4-9.8 6h-2.8c-5.1 0-10 1.6-13.8 4.5c-4.4 3.4-6.8 8.1-6.8 13.3v14.7h9.8V27.5 c0-4.9 5.7-8.1 10.8-8.1h2.6c1.8 3.8 5.6 6.5 10.1 6.5c6.1 0 11.1-5 11.1-11.1C-287.5 8.6-292.5 3.7-298.6 3.7z M-298.6 18 c-1.8 0-3.2-1.4-3.2-3.2c0-1.8 1.4-3.2 3.2-3.2c1.8 0 3.2 1.4 3.2 3.2C-295.4 16.6-296.9 18-298.6 18z\" class=\"style5\"></path><g><path d=\"M-368.7 152.1c1.5-7.9 3.6-17.1 6.2-26.7l-23-5.7c-2.8 10.4-5.1 20.5-6.8 29.5 c-11.1 3.7-19.2 14.2-19.2 26.5h55.9C-355.6 165.8-360.9 157-368.7 152.1z\" class=\"style1\"></path><path d=\"M-261.5 149.3c-1.9-10.1-4.5-21.7-7.9-33.5l-23.1 5.8c3.1 10.9 5.6 21.4 7.3 30.4 c-7.9 4.9-13.2 13.7-13.2 23.7h55.9C-242.5 163.5-250.4 153-261.5 149.3z\" class=\"style1\"></path></g><path d=\"M-276.3 93.9c-5.8-16.3-12.1-29.6-18.7-39.5c-9.7-14.6-20.5-22-31.9-22s-22.2 7.4-31.9 22 c-6.6 9.9-12.8 23.1-18.7 39.5c-21 4.8-34.6 12.5-34.6 21.2c0 14.6 38.1 26.3 85.2 26.3c47 0 85.2-11.8 85.2-26.3 C-241.7 106.4-255.3 98.7-276.3 93.9z\" class=\"style2\"></path><circle cx=\"-327.2\" cy=\"79.7\" r=\"20.8\" class=\"style3\"></circle><path d=\"M-327.2 90.8c-6.1 0-11.1-5-11.1-11.1c0-6.1 5-11.1 11.1-11.1s11.1 5 11.1 11.1 C-316.1 85.8-321.1 90.8-327.2 90.8z\" class=\"style4\"></path></g></svg></div><h1 class=\"title\">Cloudino</h1></a>\n\
</header>\n\
</div>\n\
<div class=\"main-container\">\n\
<div class=\"main wrapper clearfix\">\n\
<aside id=\"menu\"></aside>\n\
<article id=\"content\"><h1>Welcome to Cloudino Platform</h1>\n\
<h3>An easy way to connect your Arduino to the Cloud</h3>\n\
\n\
<section>\n\
<p>Cloudino is an Internet of Things Platform (Cloud Connector) that allows you to fully connect to the cloud, differents MCU platforms (Atmel AVR, Microchip PIC, etc.) in a very easy and transparent way.</p>\n\
</section>\n\
<footer>\n\
<p>Cloidino is developed and maintained by INFOTEC (Public Research Center of CONACYT) as a Open Source and Open Hardware Platform</p>\n\
</footer></article>\n\
</div>\n\
</div>\n\
<div class=\"footer-container\">\n\
<footer class=\"wrapper\">\n\
<a href=\"http://cloudino.cc/\">cloudino.cc</a> <a href=\"http://infotec.mx/\">infotec.mx</a> <span>Cloudino © 2015 INFOTEC</span>\n\
</footer>\n\
</div>\n\
<script type=\"text/javascript\">cdino.getHtml('seg/menu','menu');</script>\n\
</body></html>\n\
";

void resetEEPROM()
{
    EEPROM_clean(0,configuration);
    EEPROM_read(0,configuration); 
    strcpy(configuration.ini,"ini");
#ifdef CDINO_CONNECTOR     
    strcpy(configuration.cdino_dns,"cloudino.cc");
    configuration.cdino_port=9494;
#endif    
#ifdef CDINO_ARDUINO     
    configuration.arduino_active=false;
    configuration.arduino_gpio=5;
#endif    
#ifdef BLYNK_CONNECTOR     
    strcpy(configuration.blynk_dns,"blynk-cloud.com");
    configuration.blynk_port=8442;
#endif    

    EEPROM_write(0,configuration,configuration.js_size);  
}

String toStringID(unsigned long x)
{
  String id=String(x,16);
  id.toUpperCase();
  return id;
}

void resetWIFI()
{
    WiFi.mode(WIFI_AP_STA); 
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    String ssid=toStringID(ESP.getChipId());
    ssid=String()+F("Cloudino_")+ssid;
    WiFi.softAP(ssid.c_str());
    WiFi.disconnect();     
    configuration.auth_user[0]=0;
    configuration.auth_passwd[0]=0;
    EEPROM_write(0,configuration,configuration.js_size);
}

void setConnectorActive(String con,bool value)
{ 
#ifdef CDINO_CONNECTOR
  configuration.cdino_active=false;
#endif        
#ifdef OCB_CONNECTOR  
  configuration.ocb_active=false;
#endif        
#ifdef MQTT_CONNECTOR
  configuration.mqtt_active=false;
#endif        
#ifdef BLYNK_CONNECTOR  
  configuration.blynk_active=false;
#endif        

  if(false)
  {
#ifdef CDINO_CONNECTOR   
  }else if(con=="cdino"){
    configuration.cdino_active=value;
#endif        
#ifdef OCB_CONNECTOR   
  }else if(con=="ocb"){
    configuration.ocb_active=value;
#endif        
#ifdef MQTT_CONNECTOR   
  }else if(con=="mqtt"){
    configuration.mqtt_active=value;
#endif        
#ifdef BLYNK_CONNECTOR   
  }else if(con=="blynk"){
    configuration.blynk_active=value;
#endif        
  }
  resetConnector=true;
}

String getAddress(IPAddress ip)
{
    return String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]);
}

void handleRoot() {
    webserver.send_P ( 200, "text/html; charset=utf-8", home );
}

void handleMenu() {
  String ret;
  ret+=F("<h2>Configuration</h2>\n");
  ret+=F("<ul class=\"sidebar-nav\">\n");
  ret+=F("<li><a href=\"javascript:cdino.seg('status');\">Cloudino Status</a></li>\n");
  ret+=F("<li><a href=\"javascript:cdino.seg('ap');\">Access Point Configuration</a></li>\n");
  ret+=F("<li><a href=\"javascript:cdino.seg('wifi');\">Wifi Configuration</a></li>\n");
  ret+=F("<li><a href=\"javascript:cdino.seg('mdns');\">mDNS Service Discovery</a></li>\n");
#ifdef JS_SUPPORT
  ret+=F("<li><a href=\"#\" onclick=\"jscript.style.display=((jscript.style.display=='none')?'':'none');\">JavaScript Configuration</a></li>\n");
  ret+=F("<ul id=\"jscript\" style=\"display:none\">\n");
  ret+=F("<li><a href=\"javascript:cdino.seg('jscript');\">Initial Script</a></li>\n");
  ret+=F("<li><a href=\"javascript:cdino.seg('jsterm');\">JavaScript Terminal</a></li>\n");
  ret+=F("</ul>\n");
#endif
  ret+=F("<li><a href=\"#\" onclick=\"server.style.display=((server.style.display=='none')?'':'none');\">Server Configuration</a></li>\n");
  ret+=F("<ul id=\"server\" style=\"display:none\">\n");
#ifdef CDINO_CONNECTOR
  ret+=F("<li><a href=\"javascript:cdino.seg('srv_cloudino');\">Cloudino Server</a></li>\n");
#endif
#ifdef OCB_CONNECTOR
  ret+=F("<li><a href=\"javascript:cdino.seg('srv_ocb');\">Orion Context Broker</a></li>\n");
#endif
#ifdef MQTT_CONNECTOR
  ret+=F("<li><a href=\"javascript:cdino.seg('srv_mqtt');\">MQTT Server</a></li>\n");
#endif
#ifdef COAP_CONNECTOR
  ret+=F("<li><a href=\"javascript:cdino.seg('srv_coap');\">COAP Server</a></li>\n");
#endif
#ifdef BLYNK_CONNECTOR
  ret+=F("<li><a href=\"javascript:cdino.seg('srv_blynk');\">Blynk Server</a></li>\n");
#endif
  ret+=F("</ul>\n");
  ret+=F("<li><a href=\"javascript:cdino.seg('sys');\">System Tools</a></li>\n");
  ret+=F("</ul>\n");
  webserver.send( 200, "text/html; charset=utf-8", ret);
}

String alert(String msg){
  if(msg.length()>0)
    return String()+F("<div class=\"alert\">")+msg+F("</div>");
  else return "";
}

String addtr(String prop, String value)
{
  return String()+F("<tr><td>")+prop+F(":</td><td>")+value+F("</td></tr>\n");
}

String addtrrad(String prop, String name, bool value)
{
  String t;
  String f="checked";
  if(value)
  {
    f="";
    t="checked";      
  }
  return addtr(prop, String()+F("<input type=\"radio\" name=\"")+name+F("\" value=\"0\" ")+f+F("> False <input type=\"radio\" name=\"")+name+F("\" value=\"1\" ")+t+F("> True"));
}

String addtrinp(String prop, String type, String name, String value, int min=0, int max=50)
{
  return addtr(prop, String()+F("<input type=\"")+type+F("\" name=\"")+name+F("\" minlength=\"")+min+F("\" maxlength=\"")+max+F("\" value=\"")+value+F("\">"));
}

String  addbutton(String value, String onclick)
{
  return String()+F("<input onclick=\"")+onclick+F("\" type=\"button\" value=\"")+value+F("\">\n");    
}

void handleSegStatus()
{
    char temp[10];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;
    snprintf ( temp, 10,"%02d:%02d:%02d",hr, min % 60, sec % 60);

    const char* modes[] = {"NULL", "STA", "AP", "STA+AP"};

    String ret;
    
    ret+="<header>\n";
    ret+=F("<h1>Cloudino Status</h1>\n");
    ret+=F("<p>The status of the Cloudino Router</p>\n");
    ret+="</header>\n";
    ret+="<section>\n";
    ret+="<table class=\"table\">\n";
    
    ret+=addtr(F("Cloudino Version"),CDINO_VERSION);
    ret+=addtr(F("Uptime"),temp);
    ret+=addtr(F("Mode"),modes[wifi_get_opmode()]);
    ret+=addtr(F("Free Heap"), String(ESP.getFreeHeap()));
    ret+=addtr(F("ChipId"), toStringID(ESP.getChipId()));
    ret+=addtr(F("Flash ChipId"), toStringID(ESP.getFlashChipId()));
    ret+=addtr(F("Flash ChipSize"), String(ESP.getFlashChipSize()));
    ret+=addtr(F("Flash Real Size"), String(ESP.getFlashChipRealSize()));
    ret+=addtr(F("Flash Chip Speed"), String(ESP.getFlashChipSpeed()));
    FlashMode_t ideMode = ESP.getFlashChipMode();
    ret+=addtr(F("Flash ide mode"),(ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
    //addtr("Vcc", String(ESP.getVcc()));
    ret+="</table>\n";
    ret+=addbutton(F("Reload"),F("cdino.seg('status');"));
    ret+="</section>\n";

    webserver.send( 200, "text/html; charset=utf-8", ret);
} 

void handleSegAP()
{
    String msg;
    String ret;
    ret+="<header>\n";
    ret+=F("<h1>Access Point Configuration</h1>\n");
    ret+=F("<p>The Access Point configuration</p>\n");
    ret+="</header>\n";
    ret+="<section>\n";
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";

    if(webserver.hasArg("m"))
    {
      String m=webserver.arg("m");
      ETS_UART_INTR_DISABLE();
      wifi_set_opmode((uint8)m.toInt());
      ETS_UART_INTR_ENABLE();
      msg="Changes Applied";
    }

    if(webserver.hasArg("ssid"))
    {
      String ssid=webserver.arg("ssid");
      String pwd=webserver.arg("pwd");
      String channel=webserver.arg("channel");
      String hidden=webserver.arg("hidden");
      //WiFi.softAP(ssid.c_str(),pwd.c_str());
      WiFi.softAP(ssid.c_str(),pwd.c_str(),(int)channel.toInt(),(int)hidden.toInt()); //Next Version
      //WiFi.softAP(ssid.c_str(),pwd.c_str(),(int)channel.toInt()); 
      msg="Changes Applied";
    }    
    
    struct softap_config conf;
    wifi_softap_get_config(&conf);
    const char* authmode[] = {"AUTH_OPEN","AUTH_WEP","AUTH_WPA_PSK","AUTH_WPA2_PSK","AUTH_WPA_WPA2_PSK"};
    const char* modes[] = {"OFF", "OFF", "ON", "ON"};

    ret+=addtr(F("Status"),modes[wifi_get_opmode()]);
    ret+=addtrinp(F("SSID"),"text","ssid",reinterpret_cast<char*>(conf.ssid));
    ret+=addtrinp(F("Password"),"text","pwd",reinterpret_cast<char*>(conf.password));
    ret+=addtrinp(F("Channel"),"number","channel",String(conf.channel));
    ret+=addtrrad(F("SSID Hidden"),"hidden",conf.ssid_hidden);
    ret+=addtr(F("Authmode"),authmode[conf.authmode]);
    ret+=addtr(F("IP"),getAddress(WiFi.softAPIP()));
    ret+=addtr(F("MAC"),WiFi.softAPmacAddress());
    ret+=addtr(F("Mask"),"255.255.255.0");
    ret+=addtr(F("Gateway"),getAddress(WiFi.softAPIP()));
    ret+="</table>\n";
    ret+=addbutton(F("Save"),F("cdino.seg('ap'+cdino.getForm());"));
    ret+=addbutton(F("Start AP"),F("cdino.seg('ap?m=3');"));
    ret+=addbutton(F("Stop AP"),F("cdino.seg('ap?m=1');"));
    ret+="</form>\n";
    ret+=alert(msg);
    ret+="</section>\n";
    webserver.send( 200, "text/html; charset=utf-8", ret);
}

void handleConnect()
{
    if(webserver.hasArg("ssid"))
    {
      String ssid=webserver.arg("ssid"); 
      if(webserver.hasArg("pwd"))
      {
        String pwd=webserver.arg("pwd");  
        WiFi.begin(ssid.c_str(),pwd.c_str());
      }else
      {
        WiFi.begin(ssid.c_str());
      }
    }
    webserver.send( 200, "text/html; charset=utf-8", "OK");
}

void handleSegWifi()
{
    String msg;
    String ret;  
    if(webserver.hasArg("d"))
    {
      WiFi.disconnect();  
      msg="Changes Applied";
    }
    
    if(webserver.hasArg("id"))
    {
      String id=webserver.arg("id");  
      if(webserver.hasArg("pwd"))
      {
        String pwd=webserver.arg("pwd");  
        WiFi.begin(WiFi.SSID(id.toInt()).c_str(),pwd.c_str());
      }else
      {
        WiFi.begin(WiFi.SSID(id.toInt()).c_str());
      }
      msg="Changes Applied";
    }
    
    const char* status[] = {"IDLE_STATUS", "NO_SSID_AVAIL", "SCAN_COMPLETED", "CONNECTED","CONNECT_FAILED","CONNECTION_LOST","DISCONNECTED"};
    const char* phymodes[] = {"", "B", "G", "N"};
    const char* boo[] = {"OFF", "ON"};
    const char* auth[] = {"-","-","WPA","-","WPA2","WEP","-","OPEN","WPA/WPA2"};

    if(webserver.hasArg("s"))
    {
      ret+="<header>\n";
      ret+=F("<h1>WiFi Scan</h1>\n");
      ret+=F("<p>List of availables WiFi Networks</p>\n");
      ret+="</header>\n";
      ret+="<section>\n";
      ret+="<table class=\"table\">\n";
      ret+=F("<tr><th>SSID</th><th>RSSI</th><th>Encryption</th><th></th></tr>\n");
      int n = WiFi.scanNetworks();
      for (int i = 0; i < n; ++i)
      {
        ret+="<tr>";
        ret+="<td>"+String(WiFi.SSID(i))+"</td>";
        ret+="<td>"+String(WiFi.RSSI(i))+"</td>";
        ret+="<td>"+String(auth[WiFi.encryptionType(i)])+"</td>";
        ret+=String()+F("<td><input onclick=\"cdino.seg('wifi?id=")+String(i)+"'";
        if(WiFi.encryptionType(i)>0)ret+=F("+'&pwd='+prompt('Password','')");
        ret+=F(",'wifi',5000);\" type=\"button\" value=\"Connect\"></td>");
        ret+=F("</tr>\n");
      }
      ret+="</table>\n";
      ret+=addbutton(F("Return"),F("cdino.seg('wifi');"));
      ret+="</section>\n";
      webserver.send( 200, "text/html; charset=utf-8", ret);
      return;
    } 
      
    ret+="<header>\n";
    ret+=F("<h1>WiFi Status</h1>\n");
    ret+=F("<p>The WiFi configuration of the Cloudino Router</p>\n");
    ret+="</header>\n";
    ret+="<section>\n";
    ret+="<table class=\"table\">\n";
    ret+=addtr(F("Status"),status[WiFi.status()]);
    ret+=addtr(F("SID"),WiFi.SSID());
    ret+=addtr(F("Channel"),String(wifi_get_channel()));
    ret+=addtr(F("PHY mode"),phymodes[(int)wifi_get_phy_mode()]);
    ret+=addtr(F("Auto connect"),boo[wifi_station_get_auto_connect()]);
    ret+=addtr(F("IP"),getAddress(WiFi.localIP()));
    ret+=addtr(F("MAC"),WiFi.macAddress());
    ret+=addtr(F("Mask"),getAddress(WiFi.subnetMask()));
    ret+=addtr(F("Gateway"),getAddress(WiFi.gatewayIP()));
    ret+="</table>\n";
    ret+=addbutton(F("Reload"),F("cdino.seg('wifi');"));
    ret+=addbutton(F("Scan"),F("cdino.seg('wifi?s=1');"));
    ret+=addbutton(F("Disconect"),F("cdino.seg('wifi?d=1');"));
    ret+=alert(msg);
    ret+="</section>";
   
    webserver.send( 200, "text/html; charset=utf-8", ret);
} 

void handleSegMDNS()
{
    String msg;
    String ret;
    ret+="<header>\n";
    ret+=F("<h1>Multicast DNS Service Discovery</h1>\n");
    ret+=F("<p>With this option, you can configure Multicast DNS Service Discovery inside your Wifi network that respond to [mDNS].local</p>\n");
    ret+="</header>\n";
    if(configuration.mdns_ttl==0)configuration.mdns_ttl=3600;

    if(webserver.hasArg("s"))
    {
      wifiConnected=false;
    }

    if(webserver.hasArg(F("mdns_dns")))
    {
      strcpy(configuration.mdns_dns,webserver.arg(F("mdns_dns")).c_str());
      configuration.mdns_ttl=(int)webserver.arg(F("mdns_ttl")).toInt();
      EEPROM_write(0,configuration,configuration.js_size);
      wifiConnected=false;
      msg="Changes Applied";
    }
    
    ret+="<section>\n";
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";    
    ret+=addtrinp(F("mDNS"),"text",F("mdns_dns"),configuration.mdns_dns,0,15);
    ret+=addtrinp(F("TTL"),"number",F("mdns_ttl"),String(configuration.mdns_ttl));
    ret+="</table>\n";
    ret+=addbutton(F("Save"),F("cdino.seg('mdns'+cdino.getForm());"));
    ret+=addbutton(F("Start"),F("cdino.seg('mdns'?s=1);"));
    ret+="</form>\n";
    ret+=alert(msg);
    ret+="</section>\n";

    ret+=F("<section>\n\
<p>Install host software:<br>\n\
- For Linux, install Avahi (http://avahi.org/).<br>\n\
- For Windows, install Bonjour (http://www.apple.com/support/bonjour/).<br>\n\
- For Mac OSX and iOS support is built in through Bonjour already.\n\
</p></section>");

    webserver.send( 200, "text/html; charset=utf-8", ret);
} 

#ifdef JS_SUPPORT
void handleSegJScript()
{
    String ret;  
    ret+="<header>\n";
    ret+=F("<h1>Initial Script</h1>\n");
    ret+=F("<p>With this option, you can define a JavaScript code to be excecuted on startup</p>\n");
    ret+="</header>\n";
    ret+="<section>\n";
    ret+=F("<textarea id=\"js_script\" style=\"font-family: monospace; width: 100%; height:250px;\" autofocus>");
    int s=configuration.js_size;
    if(s>10000)s=0;
    //ret+=String(s));
    if(s>0)
    {
        char *txt=(char*)malloc(s);    
        EEPROM_read(sizeof(configuration), txt, s);
        ret+=txt;  
        free(txt);  
    }
    ret+="</textarea>\n";
    ret+=F("<span><input type=\"button\" onclick=\"cdino.getHtml('jsinit',null,'POST','['+ encodeURIComponent(js_script.value));\" value=\"Save\"></span>\n");
    ret+="</section>\n";
    webserver.send( 200, "text/html; charset=utf-8", ret);
} 

void handleSegJSTerm()
{
    String ret;  
    ret+="<header>\n";
    ret+=F("<h1>JavaScript Terminal</h1>\n");
    ret+=F("<p>With this option, you can have access to the internal JavaScript Interpreter</p>\n");
    ret+="</header>\n";
    ret+="<section>\n";
    ret+=F("<span><input type=\"text\" id=\"term_script\" style=\"width:75%;\" onkeydown=\"if(event.keyCode==13)term_send.click()\"> <input type=\"button\" id=\"term_send\"onclick=\"var idv=cdino.addDiv(term_log);idv.innerHTML='> '+term_script.value;idv=cdino.addDiv(term_log);cdino.getHtml('js',idv,'POST','['+ encodeURIComponent(term_script.value),'< ');term_script.value='';term_log.scrollTop=term_log.scrollHeight;\" value=\"Send\"></span>\n");
    ret+=F("<div id=\"term_log\" style=\"width:100%; height:250px; overflow: scroll; padding: 5px 0px 0px 7px; color:#E63A4A; border-style: double; background-color: #FFFFFF; font-size: 12px; font-family: monospace;\"></div>\n");
    ret+="</section>\n";
    webserver.send( 200, "text/html; charset=utf-8", ret); 
} 
#endif

#ifdef CDINO_CONNECTOR
void handleSegCloudino()
{
    String msg;
    String ret;
    ret+="<header>\n";
    ret+=F("<h1>Cloudino Cloud Configuration</h1>\n");
    ret+="</header>\n";
    
    if(webserver.hasArg("cdino_active"))
    {
      strcpy(configuration.cdino_dns,webserver.arg("cdino_dns").c_str());
      //strcpy(configuration.cdino_token,webserver.arg("cdino_token").c_str());
      strcpy(configuration.cdino_authtoken,webserver.arg("cdino_authtoken").c_str());
      configuration.cdino_port=(int)webserver.arg("cdino_port").toInt();
      //ret+="<p>Changes will apply next restarting.</p>\n");
      setConnectorActive("cdino",(bool)webserver.arg("cdino_active").toInt());
      EEPROM_write(0,configuration,configuration.js_size);
      msg="Changes Applied";
    }
    ret+="<section>\n";
    ret+=F("<p>This is the Cloudino Cloud Configuration page, to get these information you need to register in <a href=\"http://cloudino.cc\">cloudino.cc</a></p>\n");
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";
    //addtr("Status","ON");
    bool st=false;if(connect && connect->getName()=="CDINO" && connect->isConnected())st=true;
    if(st)ret+=addtr(F("Status"),F("<span style=\"color:#5FB404\">Online</span>"));else ret+=addtr("Status","Offline");    
    ret+=addtrrad(F("Active"),"cdino_active",configuration.cdino_active);
    ret+=addtrinp(F("DNS"),"text","cdino_dns",configuration.cdino_dns,0,50);
    ret+=addtrinp(F("Port"),"number","cdino_port",String(configuration.cdino_port),0,50);
    ret+=addtrinp(F("Auth Token"),"text","cdino_authtoken",configuration.cdino_authtoken,0,50);
    ret+="</table>\n";
    ret+=addbutton(F("Save"),F("cdino.seg('srv_cloudino'+cdino.getForm());"));
    ret+="</form>\n";
    ret+=alert(msg);
    ret+="</section>\n";
    
    webserver.send( 200, "text/html; charset=utf-8", ret); 
} 
#endif

#ifdef OCB_CONNECTOR
void handleSegOCB()
{
    String msg;
    String ret;
    ret+="<header>\n";
    ret+=F("<h1>Orion Context Broker Configuration</h1>\n");
    ret+="</header>\n";
    
    if(webserver.hasArg("ocb_active"))
    {
      strcpy(configuration.ocb_dns,webserver.arg("ocb_dns").c_str());
      strcpy(configuration.ocb_token,webserver.arg("ocb_token").c_str());
      strcpy(configuration.ocb_devid,webserver.arg("ocb_devid").c_str());
      configuration.ocb_port=(int)webserver.arg("ocb_port").toInt();
      //ret+="<p>Changes will apply next restarting.</p>\n");
      setConnectorActive("ocb",(bool)webserver.arg("ocb_active").toInt());
      EEPROM_write(0,configuration,configuration.js_size);
      msg="Changes Applied";
    }
    ret+="<section>\n";
    ret+=F("<p>This is the Orion Context Broker Configuration page, to get these information you need to register in <a href=\"http://www.fiware.org/\">www.fiware.org</a></p>\n");
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";
    ret+=addtrrad(F("Active"),"ocb_active",configuration.ocb_active);
    ret+=addtrinp(F("DNS"),"text","ocb_dns",configuration.ocb_dns,0,50);
    ret+=addtrinp(F("Port"),"number","ocb_port",String(configuration.ocb_port),0,15);
    ret+=addtrinp(F("User Token"),"text","ocb_token",configuration.ocb_token,0,50);
    ret+=addtrinp(F("Entity ID"),"text","ocb_devid",configuration.ocb_devid,0,50);
    ret+="</table>\n";
    ret+=addbutton(F("Save"),F("cdino.seg('srv_ocb'+cdino.getForm());"));
    ret+="</form>\n";
    ret+=alert(msg);
    ret+="</section>\n";
    
    webserver.send( 200, "text/html; charset=utf-8", ret); 
} 
#endif

#ifdef MQTT_CONNECTOR
void handleSegMQTT()
{
    String msg;
    String ret;
    ret+="<header>\n";
    ret+=F("<h1>MQTT Broker Configuration</h1>\n");
    ret+="</header>\n";
    
    if(webserver.hasArg("mqtt_active"))
    {
      strcpy(configuration.mqtt_dns,webserver.arg("mqtt_dns").c_str());
      configuration.mqtt_port=(int)webserver.arg("mqtt_port").toInt();
      strcpy(configuration.mqtt_user,webserver.arg("mqtt_user").c_str());
      strcpy(configuration.mqtt_passwd,webserver.arg("mqtt_passwd").c_str());
      strcpy(configuration.mqtt_pub_path,webserver.arg("mqtt_pub_path").c_str());
      strcpy(configuration.mqtt_sub_path,webserver.arg("mqtt_sub_path").c_str());
      setConnectorActive("mqtt",(bool)webserver.arg("mqtt_active").toInt());
      EEPROM_write(0,configuration,configuration.js_size);
      msg="Changes Applied";
    }
    ret+="<section>\n";
    ret+=F("<p>This is the MQTT Broker Configuration page, you can use any MQTT Server.</p>\n");
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";
    
    bool st=false;if(connect && connect->getName()=="MQTT" && connect->isConnected())st=true;
    if(st)ret+=addtr(F("Status"),F("<span style=\"color:#5FB404\">Online</span>"));else ret+=addtr(F("Status"),F("Offline"));
    ret+=addtrrad(F("Active"),"mqtt_active",configuration.mqtt_active);
    ret+=addtrinp(F("DNS"),"text","mqtt_dns",configuration.mqtt_dns,0,50);
    ret+=addtrinp(F("Port"),"number","mqtt_port",String(configuration.mqtt_port),0,15);
    ret+=addtrinp(F("User"),"text","mqtt_user",configuration.mqtt_user,0,20);
    ret+=addtrinp(F("Password"),"text","mqtt_passwd",configuration.mqtt_passwd,0,20);
    ret+=addtrinp(F("Publish base path"),"text","mqtt_pub_path",configuration.mqtt_pub_path,0,100);
    ret+=addtrinp(F("Subscribe path"),"text","mqtt_sub_path",configuration.mqtt_sub_path,0,100);
    
    ret+="</table>\n";
    ret+=addbutton(F("Save"),F("cdino.seg('srv_mqtt'+cdino.getForm());"));
    ret+="</form>\n";
    ret+=alert(msg);
    ret+="</section>\n";
    
    webserver.send( 200, "text/html; charset=utf-8", ret); 
} 
#endif

#ifdef BLYNK_CONNECTOR
void handleSegBlynk()
{
    String msg;
    String ret;
    ret+="<header>\n";
    ret+=F("<h1>Blynk Configuration</h1>\n");
    ret+="</header>\n";
    
    if(webserver.hasArg("blynk_active"))
    {
      strcpy(configuration.blynk_dns,webserver.arg("blynk_dns").c_str());
      configuration.blynk_port=(int)webserver.arg("blynk_port").toInt();
      strcpy(configuration.blynk_auth,webserver.arg("blynk_auth").c_str());
      setConnectorActive("blynk",(bool)webserver.arg("blynk_active").toInt());
      EEPROM_write(0,configuration,configuration.js_size);
      msg="Changes Applied";
    }
    
    ret+="<section>\n";
    ret+=F("<p>This is the Blynk Configuration page, you can use any Blynk Server.</p>\n");
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";
    bool st=false;if(connect && connect->getName()=="BLYNK" && connect->isConnected())st=true;
    if(st)ret+=addtr(F("Status"),F("<span style=\"color:#5FB404\">Online</span>"));else ret+=addtr("Status","Offline");
    ret+=addtrrad(F("Active"),"blynk_active",configuration.blynk_active);
    ret+=addtrinp(F("DNS"),"text","blynk_dns",configuration.blynk_dns,0,50);
    ret+=addtrinp(F("Port"),"number","blynk_port",String(configuration.blynk_port),0,15);
    ret+=addtrinp(F("Auth"),"text","blynk_auth",configuration.blynk_auth,0,50);
    ret+="</table>\n";
    ret+=addbutton(F("Save"),F("cdino.seg('srv_blynk'+cdino.getForm());"));
    ret+="</form>\n";
    ret+=alert(msg);
    ret+="</section>\n";
    webserver.send( 200, "text/html; charset=utf-8", ret); 
} 
#endif        


void handlePost() 
{
   String ret;
   if(webserver.hasArg("topic"))
   {
      String topic=webserver.arg("topic");
      String msg="";
      if(webserver.hasArg("msg"))msg=webserver.arg("msg");
      proc.post(topic,msg);
      ret="{\"topic\":\""+topic+"\",\"msg\":\""+msg+"\"}";
   }
   webserver.send( 200, "text/html; charset=utf-8", ret);
}

#ifdef JS_SUPPORT   
void handleJSInit() 
{
   String script=webserver.arg("plain");    //webserver._plain;
   int s=script.length();
   if(s>0 && script.charAt(0)=='[')script=script.substring(1);
   configuration.js_size=s+1;
   EEPROM_write(0,configuration,configuration.js_size);  
   EEPROM_write(sizeof(configuration), script.c_str(), s+1);
   js->evaluate("reset();");
   js->execute(script);
   webserver.send( 200, "text/html; charset=utf-8", "OK");
}

void handleJS() 
{
   String script=webserver.arg("plain");  //webserver._plain;
   int s=script.length();
   if(s>0 && script.charAt(0)=='[')script=script.substring(1);
   String ret=js->evaluate(script);
   webserver.send( 200, "text/html; charset=utf-8", ret);
}

#endif      

#ifdef HTTP_UPDATE
String chk_upd()
{
  int port=80;
  String host="cloudino.cc";
  String path=CDINO_VERSIONPATH;
  
  WiFiClient client;
  if (!client.connect(host.c_str(), port)) {
    return "Error: connection failed";
  }  

  // This will send the request to the server
  client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  int responseCode=0;
  //Read the Headers
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      //Serial.println("headers received");
      break;
    }else
    {
      if(line.startsWith("HTTP/"))
      {
         int n=line.indexOf(' ');
         if(n>-1)
         {
            responseCode=line.substring(n+1,line.indexOf(' ',n+1)).toInt();
         }
      }
      //Serial.println("head:"+line);
    }
  }

  if(responseCode!=200)
  {
      return String()+F("Error: Bad response code:")+String(responseCode);
  }
  
  if(client.connected())
  {
    //if(client.available()>0){
      String line = client.readStringUntil('\r');
      if(line==CDINO_VERSION)return F("There isn't new version of Cloudino Firmware");
      else return String()+F("New version (")+line+F(") of Cloudino Firmware is available");
    //}
  }
  return "Error: No update information found...";
}

String update(String url)
{
  int port=80;
  String host;
  String path;
  
  //Parse the URL
  if(url.startsWith("http://"))
  {
    url=url.substring(7);
  }else if(url.startsWith("https://"))
  {
    url=url.substring(8);
    port=443;
  }
  int i=url.indexOf('/');
  if(i>-1)
  {
    host=url.substring(0,i);
    path=url.substring(i);
  }else
  {
    host=url;
    path="/";
  }
  i=host.indexOf(':');
  if(i>-1)
  {
    port=host.substring(i+1).toInt();
    host=host.substring(0,i);
  }

  //Serial.println("Host:"+host);
  //Serial.println("Port:"+String(port));
  //Serial.println("Path:"+path);

  WiFiClient client;
  if (!client.connect(host.c_str(), port)) {
    return "Error: connection failed";
  }  

  // This will send the request to the server
  client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  int responseCode=0;
  //Read the Headers
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      //Serial.println("headers received");
      break;
    }else
    {
      if(line.startsWith("HTTP/"))
      {
         int n=line.indexOf(' ');
         if(n>-1)
         {
            responseCode=line.substring(n+1,line.indexOf(' ',n+1)).toInt();
         }
      }
      //Serial.println("head:"+line);
    }
  }

  if(responseCode!=200)
  {
      return String()+F("Error: Bad response code:")+String(responseCode);
  }
  
  WiFiUDP::stopAll();
  uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if(!Update.begin(maxSketchSpace)){
    return F("Error: Initializing update");//Update.printError(Serial);
  }
  
  while(client.connected())
  {
    uint8 buf[1024];
    while(client.available()>0){
      uint ret=client.read(buf,1024);
      //Serial.write(buf,ret);
      if(Update.write(buf, ret) != ret)
      {
        //Update.printError(Serial);
        Update.end();
        //client.disconnect();
        return F("Error: Writing data");
      }        
    }
    delay(50);
  }

  if(!Update.end(true))
  { 
    //Update.printError(Serial);
    return F("Error: Ending update");
  } 
  return "OK";
}
#endif

void handleSegSys()
{
    String msg;
    String ret;
    ret+="<header>\n";
    ret+=F("<h1>System Tools</h1>\n");
    ret+="</header>\n";

    if(webserver.hasArg("restart"))
    {
      ret+="Restarting...";  
      webserver.send( 200, "text/html; charset=utf-8", ret);
      delay(50);
      //ESP.wdtDisable();
      ESP.restart();  
      //delay(100);      
      return;
    }

#ifdef HTTP_UPDATE    

    if(webserver.hasArg("chk_upd"))
    {
      String ret2=chk_upd();    
      ret+=ret2;  
      if(ret2.startsWith("New"))
      {
        ret+="<br>";  
        String url=CDINO_UPDURL;
        url=String()+F("cdino.seg('sys?upd_url='+encodeURI('")+url+F("'));");
        ret+=addbutton("Update",url.c_str());        
      }          
      webserver.send( 200, "text/html; charset=utf-8", ret);
      return;
    }

    if(webserver.hasArg("upd_url2"))
    {
      ret+=F("<p>Updating...</p>");  
      webserver.send( 200, "text/html; charset=utf-8", ret);
      return;
    }        

    if(webserver.hasArg("upd_url"))
    {
      String ret2=update(webserver.arg("upd_url"));
      if(ret2=="OK")
      {
        ret+=F("Update Success<br>\nYou need to ");
        ret+=addbutton(F("Reboot Cloudino"),F("cdino.seg('sys?restart=','sys',3000);"));
        webserver.send( 200, "text/html; charset=utf-8", ret);
        return;
      }else
      {
        ret+=ret2;
        webserver.send( 200, "text/html; charset=utf-8", ret);
        return;
      }
    }      
#endif    

    if(webserver.hasArg("freset"))
    {
      resetEEPROM();
      msg="Changes Applied";
    }     
    
    if(webserver.hasArg("auth_user"))
    {
      strcpy(configuration.auth_user,webserver.arg("auth_user").c_str());
      strcpy(configuration.auth_passwd,webserver.arg("auth_passwd").c_str());
      EEPROM_write(0,configuration,configuration.js_size);
      msg="Changes Applied";
    }

#ifdef CDINO_ARDUINO 
    if(webserver.hasArg("arduino_active"))
    {
      configuration.arduino_active=webserver.arg("arduino_active").toInt();
      configuration.arduino_gpio=webserver.arg("arduino_gpio").toInt();
      EEPROM_write(0,configuration,configuration.js_size);
      msg="Changes Applied";
    }    
    
    ret+="<section>\n";
    ret+=F("<p>Arduino Connector Support.</p>\n");
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";
    ret+=addtrrad(F("Active"),"arduino_active",configuration.arduino_active);
    ret+=addtrinp(F("Reset GPIO"),"number","arduino_gpio",String(configuration.arduino_gpio),0,2);
    ret+="</table>\n";
    ret+=addbutton(F("Save"),F("cdino.seg('sys'+cdino.getForm());"));
    ret+="</form>\n";
    ret+="</section>\n";
#endif    

    ret+="<section>\n";
    ret+=F("<p>Basic Authentication</p>\n");
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";
    ret+=addtrinp("User","text","auth_user",configuration.auth_user,0,20);
    ret+=addtrinp("Password","text","auth_passwd",configuration.auth_passwd,0,20);
    ret+="</table>\n";
    ret+=addbutton(F("Save"),F("cdino.seg('sys'+cdino.getForm(document.forms[1]));"));
    ret+="</form>\n";
    ret+="</section>\n";

#ifdef HTTP_UPDATE
    ret+="<section>\n";
    ret+=F("<p>Http Firmware Update</p>\n");
    ret+="<form>\n";
    ret+="<table class=\"table\">\n";
    ret+=addtr(F("Version"),CDINO_VERSION);
    ret+=addtrinp(F("Update URL"),"text","upd_url",CDINO_UPDURL,0,200);
    ret+="</table>\n";
    ret+=addbutton(F("Update"),F("cdino.seg('sys?upd_url2=','sys'+cdino.getForm(document.forms[2]),100);"));
    ret+=addbutton(F("Check for Updates"),F("cdino.seg('sys?chk_upd=');"));
    ret+="</form>\n";
    ret+="</section>\n";
#endif

    ret+="<section>\n";
    ret+=F("<p>Reset Cloudino Connector</p>\n");
    ret+="<form>\n";
    ret+=addbutton(F("Reboot Cloudino"),F("cdino.seg('sys?restart=','sys',3000);"));
    ret+=addbutton(F("Delete Configurations"),F("cdino.seg('sys?freset=');"));    
    ret+="</form>\n";
    ret+="</section>\n";    
    ret+=alert(msg);
    
    webserver.send( 200, "text/html; charset=utf-8", ret);
} 

void handleNotFound() {
    webserver.send( 404, "text/html; charset=utf-8", String()+F("<html><head><title>Cloudino</title></head><body>URI: ")+webserver.uri()+F(" Not Found</body></html>"));
}

void initEEPROMConfig()
{
  pinMode(0, INPUT_PULLUP);              //RESET WIFI  
  
  EEPROM_read(0,configuration);  
  if(strcmp(configuration.ini,"ini")!=0)
  {
    resetEEPROM();
    resetWIFI();
  }
}

void loopWebServer()
{
  //wait for connection
#ifdef CAPTIVE_PORTAL    
  dnsServer.processNextRequest();
#endif  
  webserver.handleClient();  

  unsigned long m=millis();

  if(m-_time>=5000)
  {
    _time=m;
    
    //Reset Wifi after 5 seconds pressing GPIO0
    if(!digitalRead(0))
    {
        resetWIFI();
    }    

    if(WiFi.status() != WL_CONNECTED)
    {
      wifiConnected=false;
      //Serial.println("Wifi Disconnected...");
    }
  }
  
  if(m-_time2>=1000)
  {
    _time2=m;

    //don't init JS Init Script if pressing GPIO0
    if(jsInited==false && !digitalRead(0))
    {
        jsInited=true;
    }

#ifdef JS_SUPPORT
    if(!jsInited && (wifiConnected || _timeCounter==10))
    {
        //Serial.println("JavaScript Code Initialized...");
        jsInited=true;
        int s=configuration.js_size;
        if(s>10000)s=0;
        if(s>0)
        {
            char *txt=(char*)malloc(s);    
            EEPROM_read(sizeof(configuration), txt, s);
            js->execute(txt);
            free(txt);  
        }
    }
#endif    

    if(_timeCounter<10)
    {
      _timeCounter++;
    }
    
/*    
    if(_timeCounter==12)
    {
      WiFi.mode(WIFI_AP_STA);      //Activa AP si no se conecta despues de 60 segundos, activa el accesspoint y borra el sid
      WiFi.disconnect(); 
    }
*/    

    if(!wifiConnected)
    {
      if(WiFi.status() == WL_CONNECTED)
      {
        wifiConnected=true;  
        resetConnector=true;
        if(strlen(configuration.mdns_dns)>0)
        {
          mdns.begin(configuration.mdns_dns);
          mdns.enableArduino(9494,false);
          //mdns.addService("http", "tcp", 80);
          //mdns.addService("_arduino._tcp.local.", "tcp", 9494);
          //mdns.addService("arduino", "tcp", 9494);
          //mdns.addService("_arduino", "_tcp", 9494);
          //mdns.addService("avrisp", "tcp", 9494);
        }
      }
    }

  }  

  if(jsInited)
  {
    if(udp.parsePacket())
    {
      proc.handleLocalMessages(udp);
    }
  }
}

std::function<void(void)> secureHandler(std::function<void(void)> fn)
{
    if(configuration.auth_user[0]!=0)
    {
      fn=[fn](){
        if(!webserver.authenticate(configuration.auth_user,configuration.auth_passwd))
        {
          return webserver.requestAuthentication();
        }
        fn();
      };
    }
    return fn;
}

void initWebServer()
{
#ifdef CAPTIVE_PORTAL  
  dnsServer.start(DNS_PORT, "*", apIP);
#endif  

  webserver.on ( "/", handleRoot);
  webserver.on ( "/seg/menu", handleMenu);
  webserver.on ( "/seg/status", secureHandler(handleSegStatus));
  webserver.on ( "/seg/ap", secureHandler(handleSegAP));
  webserver.on ( "/seg/wifi", secureHandler(handleSegWifi));
  webserver.on ( "/seg/mdns", secureHandler(handleSegMDNS)); 
#ifdef JS_SUPPORT
  webserver.on ( "/seg/jscript", secureHandler(handleSegJScript));
  webserver.on ( "/seg/jsterm", secureHandler(handleSegJSTerm)); 
#endif
#ifdef CDINO_CONNECTOR
  webserver.on ( "/seg/srv_cloudino", secureHandler(handleSegCloudino));
#endif
#ifdef OCB_CONNECTOR
  webserver.on ( "/seg/srv_ocb", secureHandler(handleSegOCB));
#endif
#ifdef MQTT_CONNECTOR
  webserver.on ( "/seg/srv_mqtt", secureHandler(handleSegMQTT));
#endif
#ifdef BLYNK_CONNECTOR
  webserver.on ( "/seg/srv_blynk", secureHandler(handleSegBlynk));  
#endif
  webserver.on ( "/seg/sys", secureHandler(handleSegSys));  
  webserver.on ( "/post", secureHandler(handlePost));
  webserver.on ( "/connect", secureHandler(handleConnect));  
#ifdef JS_SUPPORT  
  webserver.on ("/jsinit", secureHandler(handleJSInit));
  webserver.on ("/js", secureHandler(handleJS));
#endif           
#ifdef CAPTIVE_PORTAL 
  webserver.on ("/fwlink/", handleRoot);
  webserver.on ("/hotspot-detect.html", handleRoot);
#endif           
  webserver.onNotFound (handleNotFound);  
  //webserver.onNotFound (handleAny);
  //webserver.onNotFound (handleRoot);
  webserver.begin();  

  udp.begin(UDP_PORT);
}

#endif

