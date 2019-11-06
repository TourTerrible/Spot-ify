#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Realme 1"; //replace this with your WiFi network name
const char* password = "12345678"; //replace this with your WiFi network passwor

String in1,in2,in3,in4,spid1,spid2;

void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());

  pinMode(D8,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);

}

void loop() {
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
 
    http.begin("http://abdwebtest.000webhostapp.com/data2.txt");
   
  
    int httpCode = http.GET();
    delay(1000);
    if(httpCode>0){
    
    String payload = http.getString();
    //Serial.println(payload);

    in1=payload.substring(0, payload.indexOf(','));
      payload.remove(payload.indexOf(in1),in1.length()+1);
      in2=payload.substring(0, payload.indexOf(','));
      payload.remove(payload.indexOf(in2),in2.length()+1);
      in3=payload.substring(0, payload.indexOf(','));
      payload.remove(payload.indexOf(in3),in3.length()+1);  
      in4=payload.substring(0, payload.indexOf(','));
      payload.remove(payload.indexOf(in4),in4.length()+1);
      spid1=payload.substring(0, payload.indexOf(','));
      payload.remove(payload.indexOf(spid1),spid1.length()+1); 
      spid2=payload;

//      Serial.print("in1:");
//      Serial.println(in1);
//      Serial.print("in2:");
//
//      Serial.println(in2);
//      Serial.print("in3:");
//      Serial.println(in3);
//      Serial.print("in4:");
//      Serial.println(in4);
//      Serial.print("spid:");
//      Serial.println(spid);

      digitalWrite(D3,in1.toInt());
      digitalWrite(D4,in2.toInt());
      analogWrite(D2,spid1.toInt());
      digitalWrite(D5,in3.toInt());
      digitalWrite(D6,in4.toInt());
      analogWrite(D7,spid2.toInt());
      if(in1.toInt()==0 and in2.toInt()==0 and in3.toInt()==0 and in4.toInt()==0){
       digitalWrite(D8,HIGH); 
      }
      else{
        digitalWrite(D8,LOW); 
      }
    
    }
    http.end();

}
    

  

  
  // put your main code here, to run repeatedly:

}
