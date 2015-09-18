#ifndef CLOUDINOWEB_H
#define CLOUDINOWEB_H

#include "EEPROMAnything.h"

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
}

bool wifiConnected=false;
MDNSResponder mdns;

unsigned long _time=millis();

//int _timeCounter=0;

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
.header-container{border-bottom:20px solid #e44d26}\n\
.footer-container,.main aside{border-top:20px solid #e44d26}\n\
.header-container,.footer-container,.main aside{background:#f16529}\n\
.title{color:#fff}\n\
nav ul{margin:0;padding:0}\n\
nav a{display:block;margin-bottom:10px;padding:15px 0;text-align:center;text-decoration:none;font-weight:700;color:#fff;background:#e44d26}\n\
nav a:hover,nav a:visited{color:#fff}\n\
nav a:hover{text-decoration:underline}\n\
.main{padding:30px 0}\n\
.main article h1{font-size:2em;color:#F16529;}\n\
.main aside{color:#fff;padding:0 5% 10px}\n\
.footer-container *{color:#FFF;letter-spacing:1px}\n\
.footer-container{width:100%;position_:fixed;bottom:0;}\n\
.footer-container footer{text-align:center}\n\
footer a{padding:5px 20px 0;display:inline-block;font-weight:bold}\n\
footer span{ display:block;font-size:11px}\n\
@media only screen and (min-width: 480px) {\n\
nav a{float:left;width:27%;margin:0 1.7%;padding:25px 2%;margin-bottom:0}\n\
nav li:first-child a{margin-left:0}\n\
nav li:last-child a{margin-right:0}\n\
nav ul li{display:inline}\n\
}\n\
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
tbody tr:nth-child(odd){background:rgba(241,101,41,0.1)}\n\
input[type=\"button\"],input[type=\"submit\"]{width:100px;margin:5px;}\n\
input[type=\"text\"]{width:250px}\n\
.sidebar-nav{margin:0;padding:0;list-style:none}\n\
.sidebar-nav li{text-indent:20px;line-height:40px}\n\
.sidebar-nav li a{display:block;text-decoration:none;color:#FFF;}\n\
.sidebar-nav li a:hover{background:rgba(255,255,255,0.2)}</style><script type=\"text/javascript\">\n\
var cdino = {getHtml:function(url,node,method,data){var xmlhttp;if (window.XMLHttpRequest){xmlhttp=new XMLHttpRequest();}else{xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");}\n\
xmlhttp.onreadystatechange=function(){if (xmlhttp.readyState==4 && xmlhttp.status==200){document.getElementById(node).innerHTML=xmlhttp.responseText;}};\n\
xmlhttp.open(!method?\"GET\":method,url,true);xmlhttp.send(data);},\n\
invoke:function(func,timeout,inter){setTimeout(func,timeout);if(inter)window.setInterval(func,inter);},\n\
seg:function(seg,next,time){if(next){cdino.invoke(function(){cdino.seg(next);},time?time:1000);}cdino.getHtml('seg/'+seg,'content');},\n\
getForm:function(form){if(!form)form=document.forms[0];if(form.checkValidity()){var ret=\"\";var arr=form.getElementsByTagName('input');for (x=0; x<arr.length; ++x){var inp=arr[x];if(inp.name && (inp.type!='radio' || inp.checked==true))ret+=\"&\"+inp.name+\"=\"+inp.value;}if(ret.length>0)return \"?\"+ret.substr(1);}},\n\
}</script>\n\
</head><body>\n\
<div class=\"header-container\">\n\
<header class=\"wrapper clearfix\">\n\
<a href=\"/\"><div class=\"title\"><svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\" x=\"0px\" y=\"0px\" width=\"50px\" height=\"70px\" viewBox=\"-413.2 0 200 200\" xml:space=\"preserve\"><style>.style0{fill:  #FFFFFF;}.style1{fill:  #FFFFFF;}.style2{fill:  #FFFFFF;}.style3{fill:  #F16529;}.style4{fill:  #FFFFFF;}.style5{fill:  #FFFFFF;}</style><g><path d=\"M-298.6 3.7c-4.3 0-8 2.4-9.8 6h-2.8c-5.1 0-10 1.6-13.8 4.5c-4.4 3.4-6.8 8.1-6.8 13.3v14.7h9.8V27.5 c0-4.9 5.7-8.1 10.8-8.1h2.6c1.8 3.8 5.6 6.5 10.1 6.5c6.1 0 11.1-5 11.1-11.1C-287.5 8.6-292.5 3.7-298.6 3.7z M-298.6 18 c-1.8 0-3.2-1.4-3.2-3.2c0-1.8 1.4-3.2 3.2-3.2c1.8 0 3.2 1.4 3.2 3.2C-295.4 16.6-296.9 18-298.6 18z\" class=\"style5\"></path><g><path d=\"M-368.7 152.1c1.5-7.9 3.6-17.1 6.2-26.7l-23-5.7c-2.8 10.4-5.1 20.5-6.8 29.5 c-11.1 3.7-19.2 14.2-19.2 26.5h55.9C-355.6 165.8-360.9 157-368.7 152.1z\" class=\"style1\"></path><path d=\"M-261.5 149.3c-1.9-10.1-4.5-21.7-7.9-33.5l-23.1 5.8c3.1 10.9 5.6 21.4 7.3 30.4 c-7.9 4.9-13.2 13.7-13.2 23.7h55.9C-242.5 163.5-250.4 153-261.5 149.3z\" class=\"style1\"></path></g><path d=\"M-276.3 93.9c-5.8-16.3-12.1-29.6-18.7-39.5c-9.7-14.6-20.5-22-31.9-22s-22.2 7.4-31.9 22 c-6.6 9.9-12.8 23.1-18.7 39.5c-21 4.8-34.6 12.5-34.6 21.2c0 14.6 38.1 26.3 85.2 26.3c47 0 85.2-11.8 85.2-26.3 C-241.7 106.4-255.3 98.7-276.3 93.9z\" class=\"style2\"></path><circle cx=\"-327.2\" cy=\"79.7\" r=\"20.8\" class=\"style3\"></circle><path d=\"M-327.2 90.8c-6.1 0-11.1-5-11.1-11.1c0-6.1 5-11.1 11.1-11.1s11.1 5 11.1 11.1 C-316.1 85.8-321.1 90.8-327.2 90.8z\" class=\"style4\"></path></g></svg></div><h1 class=\"title\">Cloudino</h1></a>\n\
</header>\n\
</div>\n\
<div class=\"main-container\">\n\
<div class=\"main wrapper clearfix\">\n\
<aside>\n\
<h2>Configuration</h2>\n\
<ul class=\"sidebar-nav\">\n\
<li><a href=\"javascript:cdino.seg('status');\">Cloudino Status</a></li>\n\
<li><a href=\"javascript:cdino.seg('ap');\">Access Point Configuration</a></li>\n\
<li><a href=\"javascript:cdino.seg('wifi');\">Wifi Configuration</a></li>\n\
<li><a href=\"javascript:cdino.seg('mdns');\">mDNS Configuration</a></li>\n\
<li>Server Configuration</li>\n\
<ul>\n\
<li><a href=\"javascript:cdino.seg('srv_cloudino');\">Cloudino Server</a></li>\n\
<li><a href=\"javascript:cdino.seg('srv_ocb');\">Orion Context Broker</a></li>\n\
<li><a href=\"javascript:cdino.seg('srv_mqtt');\">MQTT Server</a></li>\n\
<li><a href=\"javascript:cdino.seg('srv_coap');\">COAP Server</a></li>\n\
<li><a href=\"javascript:cdino.seg('srv_blynk');\">Blynk Server</a></li>\n\
</ul>\n\
</ul>\n\
</aside>\n\
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
<a href=\"http://cloudino.io/\">cloudino.io</a> <a href=\"http://infotec.mx/\">infotec.mx</a> <span>Cloudino © 2015 INFOTEC</span>\n\
</footer>\n\
</div>\n\
</body></html>\n\
                                                                                                    ";      //Se agregan 100 caracteres al final para eliminar bug de PROGMEM


const char mdnsinstall[]="<section>\n\
<p>Install host software:<br>\n\
- For Linux, install Avahi (http://avahi.org/).<br>\n\
- For Windows, install Bonjour (http://www.apple.com/support/bonjour/).<br>\n\
- For Mac OSX and iOS support is built in through Bonjour already.\n\
</p></section>";


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
    webserver.printHead(200,"text/html; charset=utf-8");
    webserver.printFromFlash(home);
    webserver.closeClient();
}

void addtr(const char *prop, String value)
{
  webserver.print("<tr><td>"+String(prop)+":</td><td>"+value+"</td></tr>\n");
}

void addtrrad(const char *prop, String name, bool value)
{
  String t="";
  String f="checked";
  if(value)
  {
    f="";
    t="checked";      
  }
  addtr(prop, "<input type=\"radio\" name=\""+name+"\" value=\"0\" "+f+"> False <input type=\"radio\" name=\""+name+"\" value=\"1\" "+t+"> True");
}

void addtrinp(const char *prop, String type, String name, String value, int min=0, int max=50)
{
  addtr(prop, "<input type=\""+type+"\" name=\""+name+"\" minlength=\""+min+"\" maxlength=\""+max+"\" value=\""+value+"\">");
}

void addbutton(const char *value, const char *onclick)
{
  webserver.print("<input onclick=\""+String(onclick)+"\" type=\"button\" value=\""+String(value)+"\">\n");    
}

void handleSegStatus()
{
    char temp[10];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;
    snprintf ( temp, 400,"%02d:%02d:%02d",hr, min % 60, sec % 60);

    const char* modes[] = {"NULL", "STA", "AP", "STA+AP"};

    webserver.printHead(200,"text/html; charset=utf-8");
    
    if(webserver.hasArg("restart"))
    {
      webserver.print("Restarting...");  
      webserver.closeClient();  
      delay(50);
      ESP.reset();          
      return;
    }

    if(webserver.hasArg("freset"))
    {
      EEPROM_clean(0);
      EEPROM_read(0,configuration); 
    }    

    webserver.print(
"<header>\n\
<h1>Cloudino Status</h1>\n\
<p>The status of the Claudino Router</p>\n\
</header>\n\
<section>\n\
<table class=\"table\">\n"
    );
    //webserver.print("<tbody>\n");
    addtr("Uptime",temp);
    addtr("Mode",modes[wifi_get_opmode()]);
    addtr("Free Heap", String(ESP.getFreeHeap()));
    addtr("ChipId", toStringID(ESP.getChipId()));
    addtr("Flash ChipId", toStringID(ESP.getFlashChipId()));
    addtr("Flash ChipSize", String(ESP.getFlashChipSize()));
    addtr("Flash Chip Speed", String(ESP.getFlashChipSpeed()));
    addtr("Vcc", String(ESP.getVcc()));
    
    //webserver.print("</tbody>\n");
    webserver.print("</table>\n");
    addbutton("Reload","cdino.seg('status');");
    addbutton("Restart Router","cdino.seg('status?restart=','status',1000);");
    addbutton("Factory Reset","cdino.seg('status?freset=');");
    webserver.print("</section>");
    webserver.closeClient();  
} 

void handleSegAP()
{
    webserver.printHead(200,"text/html; charset=utf-8");
    webserver.print(
"<header>\n\
<h1>Access Point Configuration</h1>\n\
<p>The Access Point configuration of the Claudino Router</p>\n\
</header>\n\
<section>\n\
<form>\n\
<table class=\"table\">\n"
    );
    //webserver.print("<tbody>\n");

    if(webserver.hasArg("m"))
    {
      String m=webserver.arg("m");
      ETS_UART_INTR_DISABLE();
      wifi_set_opmode((uint8)m.toInt());
      ETS_UART_INTR_ENABLE();
    }

    if(webserver.hasArg("ssid"))
    {
      String ssid=webserver.arg("ssid");
      String pwd=webserver.arg("pwd");
      String channel=webserver.arg("channel");
      String hidden=webserver.arg("hidden");
      WiFi.softAP(ssid.c_str(),pwd.c_str(),(int)channel.toInt(),(int)hidden.toInt()); //Next Version
      //WiFi.softAP(ssid.c_str(),pwd.c_str(),(int)channel.toInt()); 
    }    
    
    struct softap_config conf;
    wifi_softap_get_config(&conf);
    const char* authmode[] = {"AUTH_OPEN","AUTH_WEP","AUTH_WPA_PSK","AUTH_WPA2_PSK","AUTH_WPA_WPA2_PSK"};
    const char* modes[] = {"OFF", "OFF", "ON", "ON"};

    addtr("Status",modes[wifi_get_opmode()]);
    //addtr("Status",String(wifi_get_opmode()));
    addtrinp("SSID","text","ssid",reinterpret_cast<char*>(conf.ssid));
    addtrinp("Password","text","pwd",reinterpret_cast<char*>(conf.password));
    addtrinp("Channel","number","channel",String(conf.channel));
    addtrrad("SSID Hidden","hidden",conf.ssid_hidden);
    addtr("Authmode",authmode[conf.authmode]);
    addtr("IP",getAddress(WiFi.softAPIP()));
    addtr("Mask","255.255.255.0");
    addtr("Gateway",getAddress(WiFi.softAPIP()));
    //webserver.print("</tbody>\n");
    webserver.print("</table>\n");
    addbutton("Save","cdino.seg('ap'+cdino.getForm());");
    addbutton("Start AP","cdino.seg('ap?m=3');");
    addbutton("Stop AP","cdino.seg('ap?m=1');");
    webserver.print("</form>\n");
    webserver.print("</section>");
    webserver.closeClient();  
}

void handleConnect()
{
    webserver.printHead(200,"text/html; charset=utf-8");
    
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
}

void handleSegWifi()
{
    webserver.printHead(200,"text/html; charset=utf-8");
  
    if(webserver.hasArg("d"))
    {
      WiFi.disconnect();  
    }
    
    if(webserver.hasArg("id"))
    {
      String id=webserver.arg("id");  
      if(webserver.hasArg("pwd"))
      {
        String pwd=webserver.arg("pwd");  
        WiFi.begin(WiFi.SSID(id.toInt()),pwd.c_str());
      }else
      {
        WiFi.begin(WiFi.SSID(id.toInt()));
      }
    }
    
    const char* status[] = {"IDLE_STATUS", "NO_SSID_AVAIL", "SCAN_COMPLETED", "CONNECTED","CONNECT_FAILED","CONNECTION_LOST","DISCONNECTED"};
    const char* phymodes[] = {"", "B", "G", "N"};
    const char* boo[] = {"OFF", "ON"};
    const char* auth[] = {"-","-","WPA","-","WPA2","WEP","-","OPEN","WPA/WPA2"};
      
    webserver.print(
"<header>\n\
<h1>Wifi Status</h1>\n\
<p>The Wifi configuration of the Claudino Router</p>\n\
</header>\n\
<section>\n\
<table class=\"table\">\n"
    );
    //webserver.print("<tbody>\n");
    addtr("Status",status[WiFi.status()]);
    addtr("SID",WiFi.SSID());
    addtr("Channel",String(wifi_get_channel()));
    addtr("PHY mode",phymodes[(int)wifi_get_phy_mode()]);
    addtr("Auto connect",boo[wifi_station_get_auto_connect()]);
    addtr("IP",getAddress(WiFi.localIP()));
    addtr("Mask",getAddress(WiFi.subnetMask()));
    addtr("Gateway",getAddress(WiFi.gatewayIP()));
    //webserver.print("</tbody>\n");
    webserver.print("</table>\n");
    addbutton("Reload","cdino.seg('wifi');");
    addbutton("Scan","cdino.seg('wifi?s=1');");
    addbutton("Disconect","cdino.seg('wifi?d=1');");
    webserver.print("</section>");

    if(webserver.hasArg("s"))
    {
      webserver.print("<h3>Wifi Nets</h3>\n");
      webserver.print("<section>\n");
      webserver.print("<table class=\"table\">\n");
      //webserver.print("<tbody>\n");
      webserver.print("<tr><th>SSID</th><th>RSSI</th><th>Encryption</th><th></th></tr>\n");
      int n = WiFi.scanNetworks();
      for (int i = 0; i < n; ++i)
      {
        webserver.print("<tr>");
        webserver.print("<td>"+String(WiFi.SSID(i))+"</td>");
        webserver.print("<td>"+String(WiFi.RSSI(i))+"</td>");
        webserver.print("<td>"+String(auth[WiFi.encryptionType(i)])+"</td>");
        webserver.print("<td><input onclick=\"cdino.seg('wifi?id="+String(i)+"'");
        if(WiFi.encryptionType(i)>0)webserver.print("+'&pwd='+prompt('Password','')");
        webserver.print(",'wifi',5000);\" type=\"button\" value=\"Connect\"></td>");
        webserver.print("</tr>\n");
      }
      //webserver.print("</tbody>\n");
      webserver.print("</table>\n");
      webserver.print("</section>");
    }
    
    webserver.closeClient();  
} 

void handleSegMDNS()
{

    webserver.printHead(200,"text/html; charset=utf-8");
    
    webserver.print(
"<header>\n\
<h1>Local DNS Configuration</h1>\n\
<p>With this option, you can configure a dns inside your Wifi network that respond to [mDNS].local</p>\n"
    );
    if(configuration.mdns_ttl==0)configuration.mdns_ttl=3600;

    if(webserver.hasArg("s"))
    {
      wifiConnected=false;
    }

    if(webserver.hasArg("mdns_dns"))
    {
      strcpy(configuration.mdns_dns,webserver.arg("mdns_dns").c_str());
      configuration.mdns_ttl=(int)webserver.arg("mdns_ttl").toInt();
      EEPROM_write(0,configuration);
      wifiConnected=false;
    }
    
    webserver.print(
"</header>\n\
<section>\n\
<form>\n\
<table class=\"table\">\n"
    );
    
    //webserver.print("<tbody>\n");

    addtrinp("mDNS","text","mdns_dns",configuration.mdns_dns,0,15);
    addtrinp("TTL","number","mdns_ttl",String(configuration.mdns_ttl));
      //webserver.print("</tbody>\n");
    webserver.print("</table>\n");
    addbutton("Save","cdino.seg('mdns'+cdino.getForm());");
    addbutton("Start","cdino.seg('mdns'?s=1);");
    webserver.print("</form>\n");
    webserver.print("</section>");

    webserver.print(mdnsinstall);
    webserver.closeClient();  
} 

#ifdef CDINO_CONNECTOR
void handleSegCloudino()
{
    webserver.printHead(200,"text/html; charset=utf-8");
    webserver.print(
"<header>\n\
<h1>Cloudino Cloud Configuration</h1>\n"
    );
    
    if(webserver.hasArg("cdino_active"))
    {
      strcpy(configuration.cdino_dns,webserver.arg("cdino_dns").c_str());
      //strcpy(configuration.cdino_token,webserver.arg("cdino_token").c_str());
      strcpy(configuration.cdino_authtoken,webserver.arg("cdino_authtoken").c_str());
      configuration.cdino_port=(int)webserver.arg("cdino_port").toInt();
      //webserver.print("<p>Changes will apply next restarting.</p>\n");
      setConnectorActive("cdino",(bool)webserver.arg("cdino_active").toInt());
      EEPROM_write(0,configuration);
    }
    webserver.print(
"</header>\n\
<section>\n\
<p>This is the Cloudino Cloud Configuration page, to get these information you need to register in <a href=\"http://cloudino.io\">cloudino.io</a></p>\n\
<form>\n\
<table class=\"table\">\n"
    );
    //webserver.print("<tbody>\n");

    //addtr("Status","ON");
    bool st=false;if(connect && connect->getName()=="CDINO" && connect->isConnected())st=true;
    if(st)addtr("Status","<span style=\"color:#5FB404\">Online</span>");else addtr("Status","Offline");    
    addtrrad("Active","cdino_active",configuration.cdino_active);
    addtrinp("DNS","text","cdino_dns",configuration.cdino_dns,0,50);
    addtrinp("Port","number","cdino_port",String(configuration.cdino_port),0,50);
    //addtrinp("User Token","text","cdino_token",configuration.cdino_token,0,50);
    addtrinp("Auth Token","text","cdino_authtoken",configuration.cdino_authtoken,0,50);
    //webserver.print("</tbody>\n");
    webserver.print("</table>\n");
    addbutton("Save","cdino.seg('srv_cloudino'+cdino.getForm());");
    webserver.print("</form>\n");
    webserver.print("</section>");
    
    webserver.closeClient();  
} 
#endif

#ifdef OCB_CONNECTOR
void handleSegOCB()
{
    webserver.printHead(200,"text/html; charset=utf-8");
    webserver.print(
"<header>\n\
<h1>Orion Context Broker Configuration</h1>\n"
    );
    
    if(webserver.hasArg("ocb_active"))
    {
      strcpy(configuration.ocb_dns,webserver.arg("ocb_dns").c_str());
      strcpy(configuration.ocb_token,webserver.arg("ocb_token").c_str());
      strcpy(configuration.ocb_devid,webserver.arg("ocb_devid").c_str());
      configuration.ocb_port=(int)webserver.arg("ocb_port").toInt();
      //webserver.print("<p>Changes will apply next restarting.</p>\n");
      setConnectorActive("ocb",(bool)webserver.arg("ocb_active").toInt());
      EEPROM_write(0,configuration);
    }
    webserver.print(
"</header>\n\
<section>\n\
<p>This is the Orion Context Broker Configuration page, to get these information you need to register in <a href=\"http://www.fiware.org/\">www.fiware.org</a></p>\n\
<form>\n\
<table class=\"table\">\n"
    );
    //webserver.print("<tbody>\n");

    //addtr("Status","ON");
    addtrrad("Active","ocb_active",configuration.ocb_active);
    addtrinp("DNS","text","ocb_dns",configuration.ocb_dns,0,50);
    addtrinp("Port","number","ocb_port",String(configuration.ocb_port),0,15);
    addtrinp("User Token","text","ocb_token",configuration.ocb_token,0,50);
    addtrinp("Entity ID","text","ocb_devid",configuration.ocb_devid,0,50);
      //webserver.print("</tbody>\n");
    webserver.print("</table>\n");
    addbutton("Save","cdino.seg('srv_ocb'+cdino.getForm());");
    webserver.print("</form>\n");
    webserver.print("</section>");
    
    webserver.closeClient();  
} 
#endif

#ifdef MQTT_CONNECTOR
void handleSegMQTT()
{
    webserver.printHead(200,"text/html; charset=utf-8");
    webserver.print(
"<header>\n\
<h1>MQTT Broker Configuration</h1>\n"
    );
    
    if(webserver.hasArg("mqtt_active"))
    {
      strcpy(configuration.mqtt_dns,webserver.arg("mqtt_dns").c_str());
      configuration.mqtt_port=(int)webserver.arg("mqtt_port").toInt();
      strcpy(configuration.mqtt_user,webserver.arg("mqtt_user").c_str());
      strcpy(configuration.mqtt_passwd,webserver.arg("mqtt_passwd").c_str());
      strcpy(configuration.mqtt_pub_path,webserver.arg("mqtt_pub_path").c_str());
      strcpy(configuration.mqtt_sub_path,webserver.arg("mqtt_sub_path").c_str());
      //webserver.print("<p>Changes will apply next restarting.</p>\n");
      setConnectorActive("mqtt",(bool)webserver.arg("mqtt_active").toInt());
      EEPROM_write(0,configuration);
    }
    webserver.print(
"</header>\n\
<section>\n\
<p>This is the MQTT Broker Configuration page, you can use any MQTT Server.</p>\n\
<form>\n\
<table class=\"table\">\n"
    );
    //webserver.print("<tbody>\n");
    
    bool st=false;if(connect && connect->getName()=="MQTT" && connect->isConnected())st=true;
    if(st)addtr("Status","<span style=\"color:#5FB404\">Online</span>");else addtr("Status","Offline");
    addtrrad("Active","mqtt_active",configuration.mqtt_active);
    addtrinp("DNS","text","mqtt_dns",configuration.mqtt_dns,0,50);
    addtrinp("Port","number","mqtt_port",String(configuration.mqtt_port),0,15);
    addtrinp("User","text","mqtt_user",configuration.mqtt_user,0,20);
    addtrinp("Password","text","mqtt_passwd",configuration.mqtt_passwd,0,20);
    addtrinp("Publish base path","text","mqtt_pub_path",configuration.mqtt_pub_path,0,100);
    addtrinp("Subscribe path","text","mqtt_sub_path",configuration.mqtt_sub_path,0,100);
    
      //webserver.print("</tbody>\n");
    webserver.print("</table>\n");
    addbutton("Save","cdino.seg('srv_mqtt'+cdino.getForm());");
    webserver.print("</form>\n");
    webserver.print("</section>");
    
    webserver.closeClient();  
} 
#endif

#ifdef BLYNK_CONNECTOR
void handleSegBlynk()
{
    webserver.printHead(200,"text/html; charset=utf-8");
    webserver.print(
"<header>\n\
<h1>Blynk Configuration</h1>\n"
    );
    
    if(webserver.hasArg("blynk_active"))
    {
      strcpy(configuration.blynk_dns,webserver.arg("blynk_dns").c_str());
      configuration.blynk_port=(int)webserver.arg("blynk_port").toInt();
      strcpy(configuration.blynk_auth,webserver.arg("blynk_auth").c_str());
      setConnectorActive("blynk",(bool)webserver.arg("blynk_active").toInt());
      EEPROM_write(0,configuration);
    }
    webserver.print(
"</header>\n\
<section>\n\
<p>This is the Blynk Configuration page, you can use any Blynk Server.</p>\n\
<form>\n\
<table class=\"table\">\n"
    );
    //webserver.print("<tbody>\n");
    
    bool st=false;if(connect && connect->getName()=="BLYNK" && connect->isConnected())st=true;
    if(st)addtr("Status","<span style=\"color:#5FB404\">Online</span>");else addtr("Status","Offline");
    addtrrad("Active","blynk_active",configuration.blynk_active);
    addtrinp("DNS","text","blynk_dns",configuration.blynk_dns,0,50);
    addtrinp("Port","number","blynk_port",String(configuration.blynk_port),0,15);
    addtrinp("Auth","text","blynk_auth",configuration.blynk_auth,0,50);
      //webserver.print("</tbody>\n");
    webserver.print("</table>\n");
    addbutton("Save","cdino.seg('srv_blynk'+cdino.getForm());");
    webserver.print("</form>\n");
    webserver.print("</section>");
    webserver.closeClient();  
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
   webserver.printHead(200,"application/json; charset=utf-8");
   webserver.print(ret);
   webserver.closeClient();  
}

void handleUpdate() 
{
   //t_httpUpdate_return ret=ESPhttpUpdate.update("192.168.100.13", 80, "/update/Cloudino.bin");
   webserver.printHead(200,"application/json; charset=utf-8");
   webserver.print("OK");
   webserver.closeClient();    
}

/*
LOCAL void ICACHE_FLASH_ATTR
upDate_rsp(void *arg)
{
  struct upgrade_server_info *server = (struct upgrade_server_info *)arg;
  if(server->upgrade_flag == true)
  {
    //Serial.println("device_upgrade_success");
    //webserver.print("device_upgrade_success\r\n");
    //webserver.closeClient();  
    //at_backOk;
    system_upgrade_reboot();
  }
  else
  {
    //Serial.println("device_upgrade_failed");
    //webserver.print("device_upgrade_failed\r\n");
    //webserver.closeClient();  
    //at_backError;
  }
  delete server->url;
  server->url = NULL;
  delete server;
  server = NULL;
}

void handleUpgrade() 
{
  upgrade_server_info *upServer = new upgrade_server_info();
  upServer->ip[0]=192;
  upServer->ip[1]=168;
  upServer->ip[2]=100;
  upServer->ip[3]=13;
  upServer->port=80;

  os_memcpy(upServer->upgrade_version, "v1.01", 5);
  upServer->upgrade_version[5] = '\0';
  os_memcpy(upServer->pre_version, "v1.02", 5);
  upServer->pre_version[5] = '\0';
  
  upServer->check_cb=upDate_rsp;
  upServer->check_times = 60000;

  upServer->url = new uint8[512];
  char url[]="GET /update/Cloudino.bin HTTP/1.1\nHost: 192.168.100.13\nConnection: keep-alive\nCache-Control: no-cache\n\n";
  
  os_memcpy(upServer->url,url,strlen(url));
  upServer->url[strlen(url)] = '\0';

  //upServer->pespconn=NULL;
  struct espconn *pespconn = new (struct espconn)();
  pespconn->type = ESPCONN_TCP;
  pespconn->state = ESPCONN_NONE;
  //pespconn->proto.tcp = new esp_tcp();
  //pespconn->proto.tcp->local_port = espconn_port();
  //pespconn->proto.tcp->remote_port = 80;
  //os_memcpy(pespconn->proto.tcp->remote_ip, &upServer->ip, 4);  
  upServer->pespconn = pespconn;  
  
  if(system_upgrade_start(upServer))
  {
    webserver.printHead(200,"text/html; charset=utf-8");
    webserver.print("ok");  
    webserver.closeClient();  
  }
  else
  {
    webserver.printHead(200,"text/html; charset=utf-8");
    webserver.print("error");  
    webserver.closeClient();  
  }
}
*/


void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += webserver.uri();
    message += "\nMethod: ";
    message += ( webserver.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += webserver.args();
    message += "\n";

    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      message += " " + webserver.argName ( i ) + ": " + webserver.arg ( i ) + "\n";
    }
    webserver.printHead(404,"text/plain; charset=utf-8");
    webserver.print(message);
    webserver.closeClient();
}

void initEEPROMConfig()
{
  pinMode(0, INPUT_PULLUP);              //RESET WIFI  
  
  EEPROM_read(0,configuration);  
  if(strcmp(configuration.ini,"ini")!=0)
  {
    EEPROM_clean(0,configuration);
    EEPROM_read(0,configuration); 
    strcpy(configuration.ini,"ini");
    EEPROM_write(0,configuration);
    String ssid=toStringID(ESP.getChipId());
    ssid="Cloudino_"+ssid;
    WiFi.softAP(ssid.c_str());
  }
}

void chechConnection()
{
  //wait for connection
  
  
  if(millis()-_time>=5000)
  {
    _time=millis();

    if(!digitalRead(0))
    {
        WiFi.mode(WIFI_AP_STA); 
        String ssid=toStringID(ESP.getChipId());
        ssid="Cloudino_"+ssid;
        WiFi.softAP(ssid.c_str());
        WiFi.disconnect(); 
    }
/*    
    _timeCounter++;
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
          mdns.begin(configuration.mdns_dns, WiFi.localIP());
          mdns.addService("http", "tcp", 80);
          mdns.addService("_arduino._tcp.local.", "tcp", 9494);
          mdns.addService("arduino", "tcp", 9494);
          mdns.addService("_arduino", "_tcp", 9494);
        }
      }
    }
  }  
}


void initWebServer()
{
  webserver.on ( "/", handleRoot );
  webserver.on ( "/seg/status", handleSegStatus);
  webserver.on ( "/seg/ap", handleSegAP);
  webserver.on ( "/seg/wifi", handleSegWifi);
  webserver.on ( "/seg/mdns", handleSegMDNS);    
#ifdef CDINO_CONNECTOR  
  webserver.on ( "/seg/srv_cloudino", handleSegCloudino);
#endif        
#ifdef OCB_CONNECTOR    
  webserver.on ( "/seg/srv_ocb", handleSegOCB);
#endif        
#ifdef MQTT_CONNECTOR    
  webserver.on ( "/seg/srv_mqtt", handleSegMQTT);
#endif        
#ifdef BLYNK_CONNECTOR    
  webserver.on ( "/seg/srv_blynk", handleSegBlynk);  
#endif         
  webserver.on ( "/post", handlePost);
  webserver.on ( "/connect", handleConnect);  
  webserver.on ("/update", handleUpdate);
  webserver.onNotFound (handleNotFound);
  webserver.begin();  
}

#endif

