
#ifndef CLOUDINO_UPLOADER_H
#define CLOUDINO_UPLOADER_H

#ifndef SERIAL_SPEED
#define SERIAL_SPEED 57600
#endif

#ifndef UPL_PORT
#define UPL_PORT 9494
#endif

#ifndef RST_PIN
#define RST_PIN 16
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
