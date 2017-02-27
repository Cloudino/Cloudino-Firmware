/*
  CloudConnector.h - Connector for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef CLOUDCONNECTOR_H
#define CLOUDCONNECTOR_H

class CloudConnector
{
public:
  CloudConnector(MessageProc &proc,config_t &config)
  {
    _proc=&proc;
    _config=&config;
  }

  virtual String getName();

  virtual void onMessage(String topic,String message);

  virtual void onLog(String log);

  virtual void loop();

  virtual bool isConnected();

  virtual void close();

  virtual boolean isUploading();

protected:
  MessageProc *_proc;
  config_t *_config;
private:
}; 
#endif
