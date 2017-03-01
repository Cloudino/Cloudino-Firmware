/*
  OrionConnector.h - Orion Context Broker Connector for Cloudino Platform.
  Created by Javier Solis, javier.solis@infotec.mx, softjei@gmail.com, July 8, 2015
  Released into the public domain.
*/

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

  OrionConnector(MessageProc &proc,config_t &config): CloudConnector(proc,config)
  {
    tupd=0;
  }

  String getName()
  {
    return "OCB";
  }

  void onContent(String url, String content_type, String content)
  {
      boolean exist=true;
      {
          //Serial.print("onMessage:"+content);      
          HTTPClient http;
          http.begin("http://"+String(_config->ocb_dns)+":"+String(_config->ocb_port)+url);
          http.addHeader("Content-Type",content_type);   
          //http.addHeader("Accept","application/json");  
          http.addHeader("Content-Length",String(content.length()));
          if(token.length()>0)http.addHeader("X-Auth-Token",token);    
          int httpCode = http.PUT(content);
          if(httpCode > 0) 
          {
              if(httpCode != HTTP_CODE_OK) {
                  String cont=http.getString();
                  //Entity not exist
                  if(cont.indexOf("NotFound")>-1)
                  {
                      exist=false;
                  }              
              }
          }
          http.end();    
      }

      if(!exist)
      {
          String create=fileRead("/ocb_create.json");
          if(create.length()>0)
          {
              HTTPClient http;
              //create="{\"id\": \""+String(_config->ocb_devid)+"\"}";
              http.begin("http://"+String(_config->ocb_dns)+":"+String(_config->ocb_port)+"/v2/entities");
              http.addHeader("Content-Type","application/json");   
              http.addHeader("Content-Length",String(content.length()));
              if(token.length()>0)http.addHeader("X-Auth-Token",token);    
              int httpCode = http.POST(create);
              if(httpCode > 0) 
              {
                  if(httpCode == HTTP_CODE_OK) {
                      String cont=http.getString();
                  }
              }
              http.end();           
          }
      }

/*    
      WiFiClient client;
      if (!client.connect(_config->ocb_dns, _config->ocb_port)) {
        //Serial.println("connection failed");//todo:callback
        return;
      }

      client.print("PUT "+url+" HTTP/1.1\n");
      client.print("Host: "+String(_config->ocb_dns)+":"+String(_config->ocb_dns)+"\n");
      client.print("Content-Length: "+String(content.length())+"\n");
      client.print("Accept: application/json\n");
      if(token.length()>0)client.print("X-Auth-Token: "+token+"\n");
      client.print("Content-Type: "+content_type+"\n");
      client.print("\n");
      client.print(content);
      delay(10);
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        //Serial.print(line);
      }
      client.stop();
      //Serial.print("End");
*/      
  }  

  void onMessage(String topic,String message)
  {
      //Update Token
      String user=String(_config->ocb_user);
      
      //if(user.length()==0)
      if(user.length()>0 && (tupd==0 || ((millis()-tupd)>59*60*1000)))
      {
        String cont="{\"username\": \""+user+"\", \"password\":\""+String(_config->ocb_pwd)+"\"}";
        HTTPClient http;
        //http.begin("https://orion.lab.fiware.org/token");
        //http.begin("http://cloudino.io/ocbToken.jsp");
        http.begin(String(_config->ocb_auth));
        
        
        int httpCode = http.POST(cont);
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK) {
                token=http.getString();
            }
        }
        http.end();         
        tupd=millis();                
      }
    
      if(topic=="$CONTENT")
      {
        onContent("/v2/entities/"+String(_config->ocb_devid)+"/attrs","application/json",message);
      }else
      {
        String cnt="\""+message+"\"";
        onContent("/v2/entities/"+String(_config->ocb_devid)+"/attrs/"+topic+"/value","text/plain",cnt);
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
  String token="";
  unsigned long tupd=0;
}; 
#endif
