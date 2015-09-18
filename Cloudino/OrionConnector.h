#ifndef ORIONCONNECTOR_H
#define ORIONCONNECTOR_H

/*
POST /ngsi10/contextEntities/urn:infotec:test:t0001 HTTP/1.1
Host: orion.lab.fi-ware.org:1026
Content-Length: 46
Accept: application/json
X-Auth-Token: 9g1wBgYPG4deba93J9zsLDOndDAdeT
Content-Type: application/json

{"attributes":[{"name":"pin03","value":"38"}]}
--------------------------------------------------------------------------------
GET /ngsi10/contextEntities/urn:infotec:test:t0001 HTTP/1.1
Host: orion.lab.fi-ware.org:1026
Accept: application/json
X-Auth-Token: 9g1wBgYPG4deba93J9zsLDOndDAdeT
*/

class OrionConnector : public CloudConnector
{
public:

  OrionConnector(MessageProc &proc,config_t &config): CloudConnector(proc,config){}

  String getName()
  {
    return "OCB";
  }

  void onContent(String content)
  {
      //Serial.print("onMessage2");
      WiFiClient client;
      if (!client.connect(_config->ocb_dns, _config->ocb_port)) {
        //Serial.println("connection failed");//todo:callback
        return;
      }

      client.print("POST /ngsi10/contextEntities/"+String(_config->ocb_devid)+" HTTP/1.1\n");
      client.print("Host: "+String(_config->ocb_dns)+":"+String(_config->ocb_dns)+"\n");
      client.print("Content-Length: "+String(content.length())+"\n");
      client.print("Accept: application/json\n");
      client.print("X-Auth-Token: "+String(_config->ocb_token)+"\n");
      client.print("Content-Type: application/json\n");
      client.print("\n");
      client.print(content);
      delay(10);
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        //Serial.print(line);
        //TODO:
      }
      client.stop();
      //Serial.print("End");
  }  

  void onMessage(String topic,String message)
  {
      if(topic=="$CONTENT")
      {
        onContent(message);
      }else
      {
        String cnt="{\"attributes\":[{\"name\":\""+topic+"\",\"value\":\""+message+"\"}]}\n";
        onContent(cnt);
      }
  }

  void onLog(String log)
  {
    
  }

  bool isConnected()
  {
    return true;
  }

  void close()
  {
  }  

  boolean isUploading()
  {
    return false;
  }  

  void loop()
  {
    
  }

protected:
private:
}; 
#endif
