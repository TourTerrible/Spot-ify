import numpy as np
import math
import serial
import cv2
import cv2.aruco as aruco


def dotproduct(v1, v2):
  return sum((a*b) for a, b in zip(v1, v2))

def length(v):
  return math.sqrt(dotproduct(v, v))

def angle(v1, v2):
  return math.acos2(dotproduct(v1, v2) / (length(v1) * length(v2)))
font = cv2.FONT_HERSHEY_COMPLEX

aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
params = aruco.DetectorParameters_create()
final=[0,0,0,0,0,0,0,0]
v1=[1,0]
#"http://10.3.77.5:8080/videofeed?x.mpeg"
cap= cv2.VideoCapture(0)
f = open('data.txt', 'w')
while True:
    
    ret, frame = cap.read()
    gray= cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    corners, ids, rejected = aruco.detectMarkers(gray, aruco_dict, parameters= params)
    detected = aruco.drawDetectedMarkers(frame, corners)

    if np.all(ids != None):
        # rvec, tvec = aruco.estimatePoseSingleMarkers(corners, 5, camera_matrix, dist_coeffs) # posture estimation from a single marker
        # frame = aruco.drawDetectedMarkers(frame, corners, ids, (0,255,0))
        # frame = aruco.drawAxis(frame, camera_matrix, dist_coeffs, rvec, tvec, 100) 
        #print('Detected :',corners[0][0])
        for i in range(len(ids)):
	        #print(str(ids[i][0]))
	        cv2.putText(detected, str(ids[i][0]),
	                            tuple((corners[i][0][0]+corners[i][0][2])/2),
	                            font, 0.5, (0, 250,0), 1, 4)
	        center_coordinate=(corners[i][0][0]+corners[i][0][2])/2
	        front_coordinate=(corners[i][0][0]+corners[i][0][1])/2
	        cv2.circle(frame, tuple(corners[i][0][1]), 5, (25,0,200), 2)
	        v2=[front_coordinate[0]-center_coordinate[0],front_coordinate[1]-center_coordinate[1]]
	        angle=-1*(np.degrees(np.math.atan2(np.linalg.det([v1,v2]),np.dot(v1,v2))))
	        if(angle<0):
	        	angle=angle+360
	        #print(angle)
	        #linked =[center coordinates x,y,arucoID]
	        linked= [center_coordinate[0],center_coordinate[1],int(angle),ids[i][0]]
	        
	        
	        if(linked[3]==1):
	            final[0:3]=linked[0:3]
	        
	        if(linked[3]==2):
	            final[3:6]=linked[0:3]
	        
	        if(linked[3]==3):
	            final[6:9]=linked[0:3]
	        
	        if(linked[3]==4):
	            final[9:12]=linked[0:3]

	        listToStr = ','.join([str(elem) for elem in final[0:3]])
	        listToStr += '-' 
	        listToStr += ','.join([str(elem) for elem in final[3:6]])
	        listToStr += '-' 
	        listToStr += ','.join([str(elem) for elem in final[6:9]])
	        listToStr += '-' 
	        listToStr += ','.join([str(elem) for elem in final[9:12]])
	        listToStr+="=468.5,389.0-345,155-529,885-890,441"
	        
	        
	        f.seek(0)
	        f.write(listToStr)
	        f.truncate()
	        print(listToStr)
	        

	        
	        
	        


    cv2.imshow("detection",frame)
    key=cv2.waitKey(1) & 0xFF
    if key==27:
        cap.release()
        cv2.destroyAllWindows()
        break
    
    
    
