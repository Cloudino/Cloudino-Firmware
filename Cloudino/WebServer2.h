#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "ESP8266WebServer.h"

class WebServer : public ESP8266WebServer 
{
    public:
        WebServer(int port):ESP8266WebServer(port){};
 //****************** Adding methods ***************************
  String _plain=""; 
 
  void printHead(int code, const char* content_type = NULL)
  {
    String response = "HTTP/1.1 ";
    response += String(code);
    response += " ";
    response += _responseCodeToString(code);
    response += "\r\n";
  
    if (!content_type)
      content_type = "text/html";
    
    sendHeader("Content-Type", content_type, true);
    sendHeader("Connection", "close");
    sendHeader("Access-Control-Allow-Origin", "*");
    
    response += _responseHeaders;
    response += "\r\n";
    _responseHeaders = String();
    print(response);
  }
  
  void printHead(int code, char* content_type){
    printHead(code, (const char*)content_type);
  }
  
  void printHead(int code, String content_type) {
    printHead(code, (const char*)content_type.c_str());
  }  


  void print(const __FlashStringHelper *content)
  {
    Serial.print("Flash");
    _currentClient.print(content);
  }

  void print_P(PGM_P content) {
    char contentUnit[HTTP_DOWNLOAD_UNIT_SIZE + 1];
    contentUnit[HTTP_DOWNLOAD_UNIT_SIZE] = '\0';
    while (content != NULL) {
        size_t contentUnitLen;
        PGM_P contentNext;

        // due to the memccpy signature, lots of casts are needed
        contentNext = (PGM_P)memccpy_P((void*)contentUnit, (PGM_VOID_P)content, 0, HTTP_DOWNLOAD_UNIT_SIZE);

        if (contentNext == NULL) {
            // no terminator, more data available
            content += HTTP_DOWNLOAD_UNIT_SIZE;
            contentUnitLen = HTTP_DOWNLOAD_UNIT_SIZE;
        }
        else {
            // reached terminator. Do not send the terminator
            contentUnitLen = contentNext - contentUnit - 1;
            content = NULL;
        }

        // write is so overloaded, had to use the cast to get it pick the right one
        _currentClient.write((const char*)contentUnit, contentUnitLen);
    }
  }
  
  void print(const char* content) 
  {
    int l=strlen(content);
    //Serial.print(l);
    int s=0;
    while(s<l)
    {
      //Serial.print(" (");
      int n=l-s;if(n>2000)n=2000;
      //Serial.print(n);
      //Serial.print(") ");
      int r=_currentClient.write(content+s,n);if(r==0)break;
      s+=r;
      //Serial.print(s);
      delay(1);
    }
    //Serial.println();
  }

  void print(char* content) {
    print((const char*)content);
  }

  void print(String content) {
    print((const char*)content.c_str());
  }
  
  void closeClient()
  {
    _currentClient.flush();
    _currentClient.stop();
    //_currentClient.stop();
  }
    private:
};
#endif
