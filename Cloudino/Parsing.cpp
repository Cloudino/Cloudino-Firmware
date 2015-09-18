/* 
  Parsing.cpp - HTTP request parsing.

  Copyright (c) 2015 Ivan Grokhotkov. All rights reserved.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  Modified 8 May 2015 by Hristo Gochkov (proper post and file upload handling)
*/

#include <Arduino.h>
#include "WiFiServer.h"
#include "WiFiClient.h"
#include "WebServer.h"

// #define DEBUG
#define DEBUG_OUTPUT Serial1

bool WebServer::_parseRequest(WiFiClient& client) {
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');
  client.readStringUntil('\n');

  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
#ifdef DEBUG
    DEBUG_OUTPUT.print("Invalid request: ");
    DEBUG_OUTPUT.println(req);
#endif
    return false;
  }
  
  String methodStr = req.substring(0, addr_start);
  String url = req.substring(addr_start + 1, addr_end);
  String searchStr = "";
  int hasSearch = url.indexOf('?');
  if (hasSearch != -1){
    searchStr = url.substring(hasSearch + 1);
    url = url.substring(0, hasSearch);
  }
  _currentUri = url;
  
  HTTPMethod method = HTTP_GET;
  if (methodStr == "POST") {
    method = HTTP_POST;
  } else if (methodStr == "DELETE") {
    method = HTTP_DELETE;
  } else if (methodStr == "PUT") {
    method = HTTP_PUT;
  } else if (methodStr == "PATCH") {
    method = HTTP_PATCH;
  }
  _currentMethod = method;
  
#ifdef DEBUG
  DEBUG_OUTPUT.print("method: ");
  DEBUG_OUTPUT.print(methodStr);
  DEBUG_OUTPUT.print(" url: ");
  DEBUG_OUTPUT.print(url);
  DEBUG_OUTPUT.print(" search: ");
  DEBUG_OUTPUT.println(searchStr);
#endif

  String formData;
  // below is needed only when POST type request
  if (method == HTTP_POST || method == HTTP_PUT || method == HTTP_PATCH || method == HTTP_DELETE){
    String boundaryStr;
    String headerName;
    String headerValue;
    uint32_t contentLength = 0;
    //parse headers
    while(1){
      req = client.readStringUntil('\r');
      client.readStringUntil('\n');
      if (req == "") break;//no moar headers
      int headerDiv = req.indexOf(':');
      if (headerDiv == -1){
        break;
      }
      headerName = req.substring(0, headerDiv);
      headerValue = req.substring(headerDiv + 2);
      if (headerName == "Content-Type"){
        //
      } else if (headerName == "Content-Length"){
        contentLength = headerValue.toInt();
      }
    }
  

    if (searchStr != "") searchStr += '&';
    searchStr += client.readStringUntil('\r');
    client.readStringUntil('\n');
    _parseArguments(searchStr);
  } else {
    _parseArguments(searchStr);
  }
  client.flush();

#ifdef DEBUG
  DEBUG_OUTPUT.print("Request: ");
  DEBUG_OUTPUT.println(url);
  DEBUG_OUTPUT.print(" Arguments: ");
  DEBUG_OUTPUT.println(searchStr);
#endif

  return true;
}


void WebServer::_parseArguments(String data) {
#ifdef DEBUG
  DEBUG_OUTPUT.print("args: ");
  DEBUG_OUTPUT.println(data);
#endif
  if (_currentArgs)
    delete[] _currentArgs;
  _currentArgs = 0;
  if (data.length() == 0) {
    _currentArgCount = 0;
    return;
  }
  _currentArgCount = 1;

  for (int i = 0; i < data.length(); ) {
    i = data.indexOf('&', i);
    if (i == -1)
      break;
    ++i;
    ++_currentArgCount;
  }
#ifdef DEBUG
  DEBUG_OUTPUT.print("args count: ");
  DEBUG_OUTPUT.println(_currentArgCount);
#endif

  _currentArgs = new RequestArgument[_currentArgCount];
  int pos = 0;
  int iarg;
  for (iarg = 0; iarg < _currentArgCount;) {
    int equal_sign_index = data.indexOf('=', pos);
    int next_arg_index = data.indexOf('&', pos);
#ifdef DEBUG
    DEBUG_OUTPUT.print("pos ");
    DEBUG_OUTPUT.print(pos);
    DEBUG_OUTPUT.print("=@ ");
    DEBUG_OUTPUT.print(equal_sign_index);
    DEBUG_OUTPUT.print(" &@ ");
    DEBUG_OUTPUT.println(next_arg_index);
#endif
    if ((equal_sign_index == -1) || ((equal_sign_index > next_arg_index) && (next_arg_index != -1))) {
#ifdef DEBUG
      DEBUG_OUTPUT.print("arg missing value: ");
      DEBUG_OUTPUT.println(iarg);
#endif
      if (next_arg_index == -1)
        break;
      pos = next_arg_index + 1;
      continue;
    }
    RequestArgument& arg = _currentArgs[iarg];
    arg.key = data.substring(pos, equal_sign_index);
    arg.value = data.substring(equal_sign_index + 1, next_arg_index);
#ifdef DEBUG
    DEBUG_OUTPUT.print("arg ");
    DEBUG_OUTPUT.print(iarg);
    DEBUG_OUTPUT.print(" key: ");
    DEBUG_OUTPUT.print(arg.key);
    DEBUG_OUTPUT.print(" value: ");
    DEBUG_OUTPUT.println(arg.value);
#endif
    ++iarg;
    if (next_arg_index == -1)
      break;
    pos = next_arg_index + 1;
  }
  _currentArgCount = iarg;
#ifdef DEBUG
  DEBUG_OUTPUT.print("args count: ");
  DEBUG_OUTPUT.println(_currentArgCount);
#endif

}





