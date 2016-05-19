/*
  CloudUploder.h - Uploader for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

#ifndef CLOUDINO_UPLOADER_H
#define CLOUDINO_UPLOADER_H

#ifndef SERIAL_SPEED
#define SERIAL_SPEED 57600
#endif

#ifndef UPL_PORT
#define UPL_PORT 9494
#endif

class CloudinoUploader
{
public:
  CloudinoUploader(int port = UPL_PORT): _uplSevr(port){}
  void begin();
  void handleClient();
  boolean isUploading(){return uploading;};

//****************************************/
protected:
  WiFiServer _uplSevr;
  WiFiClient _uplClient;  
  boolean uploading=false;
}; 
#endif
