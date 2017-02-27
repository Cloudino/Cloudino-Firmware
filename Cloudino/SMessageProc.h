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
    void handleMessages();                      //Handle Messages from Serial
    void handleLocalMessages(Stream &stream);   //Handle Messages from UDP
    void handleStreamMessages(Stream &stream, MessageFunction msgHandler, LogFunction logHandler);  //Generic Handle Messages
    void onMessage(MessageFunction fn);
    void onServerMessage(MessageFunction fn);
    void onLocalMessage(MessageFunction fn);
    void onLog(LogFunction fn);
    String encodePost(String topic, String data);
    String encodeLog(String log);
    void post(String topic, String message);                //Envia mensaje al arduino y JS
    void processMessage(String topic, String msg);          //procesa mensaje recibido del arduino para el servidor
    void processServerMessage(String topic, String msg);    //Procesa mensaje recibido del servidor para JS
    void processLocalMessage(String topic, String msg);     //Procesa mensaje recibido del la red local para JS
    void on(String topic, String fn);
    void clear();
    LogFunction logHandler;    
  protected:   
  private:
    MessageFunction messageHandler=NULL; 
    MessageFunction serverMessageHandler=NULL; 
    MessageFunction localMessageHandler=NULL;    
    boolean waitStream(Stream &stream);
};

#endif
