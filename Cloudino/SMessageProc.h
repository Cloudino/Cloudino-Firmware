/*
  SMessageProc.h - MessageProcessor for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef MessageProc_h
#define MessageProc_h

#include <Arduino.h>
#include "CloudinoConf.h"

const char SEP='|';
const char MSEP='M';
const char SSEP='S';
const char LSEP='L';

class MessageProc
{
  public:
    struct JSHandler;
    typedef void (*MessageFunction)(String,String);  
    typedef void (*LogFunction)(String);  
    void handleMessages();
    void onMessage(MessageFunction fn);
    void onServerMessage(MessageFunction fn);
    void onLog(LogFunction fn);
    void post(String topic, String message);   
    void processServerMessage(String topic, String msg);
    void on(String topic, String fn);
    void clear();
    MessageFunction messageHandler=NULL; 
    MessageFunction serverMessageHandler=NULL; 
    LogFunction logHandler;    
  protected:
  private:
    boolean waitSerial();
};

#endif
