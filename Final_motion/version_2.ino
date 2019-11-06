#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <math.h>

#define LEFT -1
#define RIGHT 1
#define ANTICLOCKWISE -1
#define CLOCKWISE 1
#define NA -4

//WIFI HOTSPOT CONNECTION

const char* SSID_ = "ROBO_IITG_2.4G";
const char* PASSWORD = "iamthefuture";





int BOT_NUM=4;

String FILE_ADRESS="http://192.168.0.46:8080/data.txt";


int SPEED_LEFT;
int SPEED_RIGHT;
int FLAG=0;
int FLAG_NA=1;

String x1,y_1,A1,x2,y2,A2,x3,y3,A3,x4,y4,A4;

String Fx1,Fy1,Fx2,Fy2,Fx3,Fy3,Fx4,Fy4;

int Xself,Yself,Aself,F_X_self,F_Y_self;
   

int distance,calculated_angle;

double CM_PER_PIXEL=0.35;
int RADIUS_IN_CM=14;
int TOLERANCE_IN_CM=1;
int i=0;

bool intersection_array[]={0,0,0,0};
int A[]={0,0,0,0};



int LED = D8;
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
      A1=string_line.substring(0, string_line.indexOf('-'));
      string_line.remove(string_line.indexOf(A1),A1.length()+1);
      
      x2=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(x2),x2.length()+1);
      y2=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(y2),y2.length()+1);
      A2=string_line.substring(0, string_line.indexOf('-'));
      string_line.remove(string_line.indexOf(A2),A2.length()+1);

      x3=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(x3),x3.length()+1);
      y3=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(y3),y3.length()+1);
      A3=string_line.substring(0, string_line.indexOf('-'));
      string_line.remove(string_line.indexOf(A3),A3.length()+1);
      
      x4=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(x4),x4.length()+1);
      y4=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(y4),y4.length()+1);
      A4=string_line.substring(0, string_line.indexOf('='));
      string_line.remove(string_line.indexOf(A4),A4.length()+1);

      Fx1=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(Fx1),Fx1.length()+1);
      Fy1=string_line.substring(0, string_line.indexOf('-'));
      string_line.remove(string_line.indexOf(Fy1),Fy1.length()+1);

      Fx2=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(Fx2),Fx2.length()+1);
      Fy2=string_line.substring(0, string_line.indexOf('-'));
      string_line.remove(string_line.indexOf(Fy2),Fy2.length()+1);
      
      Fx3=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(Fx3),Fx3.length()+1);
      Fy3=string_line.substring(0, string_line.indexOf('-'));
      string_line.remove(string_line.indexOf(Fy3),Fy3.length()+1);
      
      Fx4=string_line.substring(0, string_line.indexOf(','));
      string_line.remove(string_line.indexOf(Fx4),Fx4.length()+1);
      Fy4=string_line;
      

      
      //Serial.println(string_line); 
      
     
//      Serial.print("x1:");
//      Serial.println(x1);
//      Serial.print("y1:");
//      Serial.println(y_1);
//      Serial.print("A1:");
//      Serial.println(A1);
//  
//      
//      Serial.print("Fx1:");
//      Serial.println(Fx1);
//      Serial.print("Fy1:");
//      Serial.println(Fy1);
//      
//      Serial.print("Fx2:");
//      Serial.println(Fx2);
//      Serial.print("Fy2:");
//      Serial.println(Fy2);
//      
//      Serial.print("Fx3:");
//      Serial.println(Fx3);
//      Serial.print("Fy3:");
//      Serial.println(Fy3);
//
//      Serial.print("Fx4:");
//      Serial.println(Fx4);
//      Serial.print("Fy4:");
//      Serial.println(Fy4);
// 


}
int DistanceBetweenCoordinates(int x1, int y1,int x2, int y2){
  return(sqrt((sq(x1-x2))+(sq(y1-y2))));
  
}




double CalculateAngleOfFinal(int v1,int v2,int h1,int h2){
  //v1:Ycurrent,v2:Ygoal,h1:X current;h2:X goal
  //will calculate angle of straight line wrt. horizontal between initial and final point
  //in anticlock with range 0-360

double rad=atan2((v2-v1),(h2-h1));

int theta=(rad*180)/3.14;

if(theta>=0){
return(360-theta);
}
if(theta<0){
return(theta*(-1));
}
}

void AssignBotCoord(int BOT_NUMBER){
  
  if(BOT_NUMBER==1){
    Xself=x1.toInt();
    Yself=y_1.toInt();
    Aself=A1.toInt();
    F_X_self= Fx1.toInt();
    F_Y_self= Fy1.toInt();
   
    
  }
  if(BOT_NUMBER==2){
    Xself=x2.toInt();
    Yself=y2.toInt();
    Aself=A2.toInt();
    F_X_self= Fx2.toInt();
    F_Y_self= Fy2.toInt();
  
  }
  if(BOT_NUMBER==3){
    Xself=x3.toInt();
    Yself=y3.toInt();
    Aself=A3.toInt();
    F_X_self= Fx3.toInt();
    F_Y_self= Fy3.toInt();
  
  }
  if(BOT_NUMBER==4){
    Xself=x4.toInt();
    Yself=y4.toInt();
    Aself=A4.toInt();
    F_X_self= Fx4.toInt();
    F_Y_self= Fy4.toInt();
    
  }
}

int BoundaryCheck(int BOT_NUMBER){
  
  if(BOT_NUMBER==1){
    if(x1.equals("NA")){


          switch (FLAG_NA) {
              case 1:
                Backward(LEFT);
                Backward(RIGHT);
                return(1);
                break;
              case 2:
                  Forward(LEFT);
                  Forward(RIGHT);
                  return(1);
                  break;
            
               }       
        }
     else {
         return(0);   
     }
    }
    
  if(BOT_NUMBER==2){
   if(x2.equals("NA")){
      switch (FLAG_NA) {
              case 1:
                Backward(LEFT);
                Backward(RIGHT);
                return(1);
                break;
              case 2:
                  Forward(LEFT);
                  Forward(RIGHT);
                  return(1);
              break;
            
               } 
    }
    else {
         return(0);   
     }
  
  }
  if(BOT_NUMBER==3){
    if(x3.equals("NA")){
    switch (FLAG_NA) {
              case 1:
                Backward(LEFT);
                Backward(RIGHT);
                return(1);
                break;
              case 2:
                  Forward(LEFT);
                  Forward(RIGHT);
                  return(1);
              break;
            
               } 
    }
    else {
         return(0);   
     }          
  
  }
  if(BOT_NUMBER==4){
    if(x4.equals("NA")){
    switch (FLAG_NA) {
              case 1:
                Backward(LEFT);
                Backward(RIGHT);
                return(1);
                break;
              case 2:
                  Forward(LEFT);
                  Forward(RIGHT);
                  return(1);
              break;
            
               } 
    }
    else {
         return(0);   
     }           
    
  }
}

void AssignBotSpeed(int BOT_NUMBER){
  
  if(BOT_NUMBER==1){
   
    SPEED_LEFT=460;
    SPEED_RIGHT=500;
  }
  if(BOT_NUMBER==2){
    SPEED_LEFT=370;
    SPEED_RIGHT=365;
    

  }
  if(BOT_NUMBER==3){

      SPEED_LEFT=555;
    SPEED_RIGHT=340;
    

  }
  if(BOT_NUMBER==4){
  SPEED_LEFT=430;
  SPEED_RIGHT=400;

  }
}

int  IsIntersect(int x_1,int y_1,int x_2,int y_2){

  double distance=DistanceBetweenCoordinates(x_1,y_1,x_2,y_2);
  if(distance<2*(RADIUS_IN_CM/CM_PER_PIXEL))
  return(1);
  else
  return(0);
  
}
void  FindAllIntersection(int BOT_Number){

  if(BOT_Number==1){
       intersection_array[1]=IsIntersect(Xself,Yself,x2.toInt(),y2.toInt());
       intersection_array[2]=IsIntersect(Xself,Yself,x3.toInt(),y3.toInt());
       intersection_array[3]=IsIntersect(Xself,Yself,x4.toInt(),y4.toInt());
       }
 if(BOT_Number==2){
       intersection_array[0]=IsIntersect(Xself,Yself,x1.toInt(),y_1.toInt());
       intersection_array[2]=IsIntersect(Xself,Yself,x3.toInt(),y3.toInt());
       intersection_array[3]=IsIntersect(Xself,Yself,x4.toInt(),y4.toInt());
       }
 if(BOT_Number==3){
       intersection_array[0]=IsIntersect(Xself,Yself,x1.toInt(),y_1.toInt());
       intersection_array[1]=IsIntersect(Xself,Yself,x2.toInt(),y2.toInt());
       intersection_array[3]=IsIntersect(Xself,Yself,x4.toInt(),y4.toInt());
       }
 if(BOT_Number==4){
       intersection_array[0]=IsIntersect(Xself,Yself,x1.toInt(),y_1.toInt());
       intersection_array[1]=IsIntersect(Xself,Yself,x2.toInt(),y2.toInt());
       intersection_array[2]=IsIntersect(Xself,Yself,x3.toInt(),y3.toInt());
       
       }

       
  
}

void setup() {
 Serial.begin(115200);
  WiFi.begin(SSID_, PASSWORD);
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("..");
  }
  AssignBotSpeed(BOT_NUM);
  
  Serial.println("Connected");
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
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
       //Serial.println(coordinates);
       ReadCoordinates(coordinates);
       AssignBotCoord(BOT_NUM);
       A[0]=A1.toInt();
       A[1]=A2.toInt();
       A[2]=A3.toInt();
       A[3]=A4.toInt();
       
//------------------------------------------------------------------------
       
      

//-------------------------------------------------------------------------------
       if(!BoundaryCheck(BOT_NUM)){
       
       
        distance=DistanceBetweenCoordinates(Xself,Yself,F_X_self,F_Y_self);
        //Serial.println(distance);
        calculated_angle=CalculateAngleOfFinal(Yself,F_Y_self,Xself,F_X_self);
        //Serial.println(calculated_angle);
       // Serial.println(abs(calculated_angle-A1.toInt()));

       FindAllIntersection(BOT_NUM);
        
//------------------------------------------------------------------------
        if(distance>(TOLERANCE_IN_CM/CM_PER_PIXEL)){
          digitalWrite(LED,LOW);
          FLAG=0;
          
          for(i=0;i<4;i++){
            
            if(intersection_array[i]==1)         //i=intersecting bot num-1

            {

              if(BOT_NUM<i+1)              
              {                              ///if BOT_NUM has Low priority
                 if(abs(Aself-A[i])>170 and abs(Aself-A[i])<190){
                  Rotate(CLOCKWISE);
                  FLAG=1;
                 }
                 else if(abs(Aself-A[i])<10 and abs(Aself-A[i])<350){
                  Rotate(CLOCKWISE);
                  FLAG=1;
                 }
                 
                 else
                  {                               
                    Backward(LEFT);
                    Backward(RIGHT);
                    FLAG_NA=2;
                    FLAG=1;
                  }
              }
              else{         
               //if high priority then stop
               Stop();
               FLAG=1;
             
              }
//====================================================================================================                         
            }

       
          }
            if(FLAG==0)

//----------------------------------------------------------------            
            {
              if(abs(calculated_angle-Aself)>10)
              {
                  
                  if(calculated_angle-Aself>0){
                  Rotate(ANTICLOCKWISE);
                  }
                  if(calculated_angle-Aself<0){
                  Rotate(CLOCKWISE);
                  }
              }
              
              else{
                
                Forward(LEFT);
                Forward(RIGHT);
                FLAG_NA=1;
                
              }
          
          }
//---------------------------------------------------------------------------          
          
        }
        else{
          Stop();
          digitalWrite(LED,HIGH);
          
        }

       }
//------------------------------------------------------------------------        

       

        
    }
    else{
      //Serial.print("Couldn't get data from file");   
    }
    
  }  

}
