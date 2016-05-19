/*
  WebServer.cpp - WebServer Library for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#include <Arduino.h>
#include "WiFiServer.h"
#include "WiFiClient.h"
#include "WebServer.h"
#include <libb64/cencode.h>

// #define DEBUG
#define DEBUG_OUTPUT Serial

struct WebServer::RequestHandler {
  RequestHandler(WebServer::THandlerFunction fn, const char* uri, HTTPMethod method)
  : fn(fn)
  , uri(uri)
  , method(method)
  , next(NULL)
  {
  }

  WebServer::THandlerFunction fn;
  String uri;
  HTTPMethod method;
  RequestHandler* next;

};

WebServer::WebServer(int port)
: _server(port)
, _firstHandler(0)
, _lastHandler(0)
, _currentArgCount(0)
, _currentArgs(0)
{
}

WebServer::~WebServer()
{
  if (!_firstHandler)
    return;
  RequestHandler* handler = _firstHandler;
  while (handler) {
    RequestHandler* next = handler->next;
    delete handler;
    handler = next;
  }
}

void WebServer::begin() {
  _server.begin();
}

bool WebServer::authenticate(const char * username, const char * password)
{
  if(_autorization.length()>0)
  {
    String authReq = _autorization;
    if(authReq.startsWith("Basic")){
      authReq = authReq.substring(6);
      authReq.trim();
      char toencodeLen = strlen(username)+strlen(password)+1;
      char *toencode = new char[toencodeLen + 1];
      if(toencode == NULL){
        return false;
      }
      char *encoded = new char[base64_encode_expected_len(toencodeLen)+1];
      if(encoded == NULL){
        delete[] toencode;
        return false;
      }
      sprintf(toencode, "%s:%s", username, password);
      if(base64_encode_chars(toencode, toencodeLen, encoded) > 0 && authReq.equals(encoded)){
        delete[] toencode;
        delete[] encoded;
        return true;
      }
      delete[] toencode;
      delete[] encoded;
    }
  }
  return false;
}

void WebServer::requestAuthentication(){
  print("HTTP/1.1 401 Access Denied\r\n\
WWW-Authenticate: Basic realm=\"Cloudino\"\r\n\
Content-Length: 0\r\n\r\n");
}


void WebServer::on(const char* uri, WebServer::THandlerFunction handler)
{
  on(uri, HTTP_ANY, handler);
}

void WebServer::on(const char* uri, HTTPMethod method, WebServer::THandlerFunction fn)
{
  RequestHandler* handler = new RequestHandler(fn, uri, method);
  if (!_lastHandler) {
    _firstHandler = handler;
    _lastHandler = handler;
  }
  else {
    _lastHandler->next = handler;
    _lastHandler = handler;
  }
}

void WebServer::handleClient()
{
  WiFiClient client = _server.available();
  if (!client) {
    return;
  }

#ifdef DEBUG
  DEBUG_OUTPUT.println("New client");
#endif

  // Wait for data from client to become available
  uint16_t maxWait = HTTP_MAX_DATA_WAIT;
  while(client.connected() && !client.available() && maxWait--){
    delay(1);
  }

  if (!_parseRequest(client)) {
    return;
  }

  _currentClient = client;
  _handleRequest();
}

void WebServer::sendHeader(String name, String value, bool first) {
  String headerLine = name;
  headerLine += ": ";
  headerLine += value;
  headerLine += "\r\n";

  if (first) {
    _responseHeaders = headerLine + _responseHeaders;
  }
  else {
    _responseHeaders += headerLine;
  }
}


String WebServer::arg(const char* name) {
  for (int i = 0; i < _currentArgCount; ++i) {
    if (_currentArgs[i].key == name)
      return _currentArgs[i].value;
  }
  return String();
}

String WebServer::arg(int i) {
  if (i < _currentArgCount)
    return _currentArgs[i].value;
  return String();
}

String WebServer::argName(int i) {
  if (i < _currentArgCount)
    return _currentArgs[i].key;
  return String();
}

int WebServer::args() {
  return _currentArgCount;
}

bool WebServer::hasArg(const char* name) {
  for (int i = 0; i < _currentArgCount; ++i) {
    if (_currentArgs[i].key == name)
      return true;
  }
  return false;
}

void WebServer::onNotFound(THandlerFunction fn) {
  _notFoundHandler = fn;
}

void WebServer::_handleRequest() {
  RequestHandler* handler;
  for (handler = _firstHandler; handler; handler = handler->next)
  {
    if (handler->method != HTTP_ANY && handler->method != _currentMethod)
      continue;

    if (handler->uri != _currentUri)
      continue;

    handler->fn();
    break;
  }

  if (!handler){
#ifdef DEBUG
    DEBUG_OUTPUT.println("request handler not found");
#endif

    if(_notFoundHandler) {
      _notFoundHandler();
    }
    else {
      printHead(404,"text/plain");
      print(String("Not found: ") + _currentUri);
      closeClient();
    }
  }

  _currentClient   = WiFiClient();
  _currentUri      = String();
}

const char* WebServer::_responseCodeToString(int code) {
  switch (code) {
    case 101: return "Switching Protocols";
    case 200: return "OK";
    case 403: return "Forbidden";
    case 404: return "Not found";
    case 500: return "Fail";
    default:  return "";
  }
}

