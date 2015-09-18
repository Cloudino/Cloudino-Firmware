/*
  Cloudino.h - Library for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, July 8, 2015
  Released into the public domain.
*/
#ifndef MessageProc_h
#define MessageProc_h

#include <Arduino.h>

const char SEP='|';
const char MSEP='M';
const char SSEP='S';
const char LSEP='L';

class MessageProc
{
  public:
    typedef void (*MessageFunction)(String,String);  
    typedef void (*LogFunction)(String);  
    void handleMessages();
    void onMessage(MessageFunction fn);
    void onLog(LogFunction fn);
    void post(String topic, String message);    
  protected:
  private:
    boolean waitSerial();
    MessageFunction messageHandler;
    LogFunction logHandler;
};

#endif
