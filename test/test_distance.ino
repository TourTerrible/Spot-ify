#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <math.h>

//WIFI HOTSPOT CONNECTION

const char* SSID_ = "Redmi";
const char* PASSWORD = "244466666";


#define LEFT -1
#define RIGHT -2
#define ANTICLOCKWISE -1
#define CLOCKWISE 1


int BOT_NUM=3;
int SPEED_LEFT=300;
int SPEED_RIGHT=340;

String x1,y_1,A1;
String Fx1,Fy1;

int Xself,Yself,Aself,F_X_self,F_Y_self;
   

int relative_angle,distance,horizontal_angle_with_final;
int is_align,i;
double CM_PER_PIXEL=0.35;
int RADIUS_IN_CM=12;
int   TimePerDegree= 100;
String FILE_ADRESS="http://192.168.43.132:8080/data2.txt";


int enA = D2; //Left
int in1 = D3;
int in2 = D4;
int in3 = D5;
int in4 = D6;
int enB = D7; //Right

void Forward(int motor_side){
  if(motor_side==LEFT){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, SPEED_LEFT);
  }
  if(motor_side==RIGHT){
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, SPEED_RIGHT);
  }
}

void Backward(int motor_side){
  if(motor_side==LEFT){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, SPEED_LEFT);
  }
  if(motor_side==RIGHT){
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    
    analogWrite(enB, SPEED_RIGHT);
  }
}

void Stop(){
 
    digitalWrite(in2, LOW);
    digitalWrite(in1, LOW);
    digitalWrite(in4, LOW);
    digitalWrite(in3, LOW);
 
}

void Rotate(int dir){
  if(dir==ANTICLOCKWISE){
  Backward(LEFT);
  Forward(RIGHT);
  }
  if(dir==CLOCKWISE){
  Backward(RIGHT);
  Forward(LEFT);
  }
  
}

void ReadCoordinates(String string_line){
x1=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(x1),x1.length()+1);
      y_1=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(y_1),y_1.length()+1);
      A1=string_line.substring(0, string_line.indexOf('='));
      string_line.remove(string_line.indexOf(A1),A1.length()+1);

      Fx1=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(Fx1),Fx1.length()+1);
      Fy1=string_line;

      Serial.print("x1:");
      Serial.println(x1);
      Serial.print("y1:");
      Serial.println(y_1);
      Serial.print("A1:");
      Serial.println(A1);
       Serial.print("Fx1:");
      Serial.println(Fx1);
      Serial.print("Fy1:");
      Serial.println(Fy1);
      
}

int DistanceBetweenCoordinates(int x1, int y1,int x2, int y2){
  return(sqrt((sq(x1-x2))+(sq(y1-y2))));
  
}


bool AlignToFinal(int orientation,int angle_with_final){
  int dir=FindDirOfRotation(orientation, angle_with_final);
  if(dir==ANTICLOCKWISE){
  Backward(LEFT);
  Forward(RIGHT);
    if(abs(orientation-angle_with_final)<10){
        Stop();
      return(1);
    }
    else
       return(0);
  
  }
  if(dir=CLOCKWISE){
  Backward(RIGHT);
  Forward(LEFT);
  if(abs(orientation-angle_with_final)<10){
      Stop();
      return(1);
    }
    else
       return(0);
  
  }
}

int FindDirOfRotation(int Aself,int calculated){
  if(Aself-calculated >0 and  Aself-calculated<180)
      return(CLOCKWISE);
  if(Aself-calculated >0 and  Aself-calculated>180)
      return(ANTICLOCKWISE);
  if(Aself-calculated <0 and  abs(Aself-calculated)<180)
      return(ANTICLOCKWISE);
  if(Aself-calculated <0 and  abs(Aself-calculated)>180)
      return(CLOCKWISE);            
  
}

double CalculateAngleOfFinal(int v1,int v2,int h1,int h2){
  //v1:Ycurrent,v2:Ygoal,h1:X current;h2:X goal
  //will calculate angle of straight line wrt. horizontal between initial and final point
  //in anticlock with range 0-360

double rad=atan2((v2-v1),(h2-h1));
Serial.println(rad);
int theta=(rad*180)/3.14;

if(theta>=0){
return(theta);
}
if(theta<0){
return(theta+360);
}
}
void setup() {
 Serial.begin(115200);
  WiFi.begin(SSID_, PASSWORD);
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("..");
  }
   Xself=x1.toInt();
    Yself=y_1.toInt();
    Aself=A1.toInt();
    F_X_self= Fx1.toInt();
    F_Y_self= Fy1.toInt();
  Serial.println("Connected");
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  digitalWrite(enA, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(enB, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
}

void loop() {
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(FILE_ADRESS);
    int httpCode = http.GET();

    if (httpCode > 0) {
      
       String coordinates = http.getString();
       Serial.println(coordinates);
       ReadCoordinates(coordinates);
       
       
        distance=DistanceBetweenCoordinates(x1.toInt(),y_1.toInt(),Fx1.toInt(),Fy1.toInt());
        Serial.println(distance);
        //Serial.println(Xself);
       
        if(distance<50){
        Stop();
       }
       else{
        Forward(LEFT);
        Forward(RIGHT);
       }

        
    }
    else{
      Serial.print("couldn't get data from file");   
    }
    
  }  

}
