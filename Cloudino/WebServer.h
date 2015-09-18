
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <functional>

enum HTTPMethod { HTTP_ANY, HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_PATCH, HTTP_DELETE };

#define HTTP_DOWNLOAD_UNIT_SIZE 1460
#define HTTP_MAX_DATA_WAIT 1000 //ms to wait for the client to send the request
#define HTTP_MAX_CLOSE_WAIT 2000 //ms to wait for the client to close the connection

class WebServer
{
public:
  WebServer(int port = 80);
  ~WebServer();

  void begin();
  void handleClient();

  typedef std::function<void(void)> THandlerFunction;
  void on(const char* uri, THandlerFunction handler);
  void on(const char* uri, HTTPMethod method, THandlerFunction fn);
  void onNotFound(THandlerFunction fn);  //called when handler is not assigned

  String uri() { return _currentUri; }
  HTTPMethod method() { return _currentMethod; }
  WiFiClient client() { return _currentClient; }
  
  String arg(const char* name);   // get request argument value by name
  String arg(int i);              // get request argument value by number
  String argName(int i);          // get request argument name by number
  int args();                     // get arguments count
  bool hasArg(const char* name);  // check if argument exists
  
  void sendHeader(String name, String value, bool first = false);
  
  //****************** Adding methods ***************************
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

  void printFromFlash(const char* content) 
  {
    int l=strlen(content);
    //Serial.print(l);
    int s=0;
    while(s<l)
    {
      int n=l-s;if(n>100)n=100;else break;
      //Serial.print(" (");
      //Serial.print(n);
      //Serial.print(") ");
      int r=_currentClient.write(content+s,n);if(r==0)break;
      s+=r;
      //Serial.print(s);
      delay(1);
    }
    //Serial.println();
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
  
//****************************************  
protected:
  void _handleRequest();
  bool _parseRequest(WiFiClient& client);
  void _parseArguments(String data);
  static const char* _responseCodeToString(int code);
  struct RequestHandler;
  struct RequestArgument {
    String key;
    String value;
  };

  WiFiServer  _server;
  WiFiClient  _currentClient;
  HTTPMethod  _currentMethod;
  String      _currentUri;

  size_t           _currentArgCount;
  RequestArgument* _currentArgs;

  String           _responseHeaders;

  RequestHandler*  _firstHandler;
  RequestHandler*  _lastHandler;
  THandlerFunction _notFoundHandler;

};

#endif

