# Real-Time Motion Planning #

*Real time motion planning with the help of aruco markers, computer vision and ESP8266 module.*

<img src="https://github.com/ahad18abd/real_time_motion_planning/blob/master/Images%20and%20Video/Bots.jpeg" width="50%" height="50%">

We designed this for __Inter Hostel Tech Competetion-Kriti 2019.__

* The setup for this consists of:
    *  A camera placed on top.
    *  A local Wi-Fi server.
    *  4 Aruco Markers with known Id.
    
### BOT Making ###
Each Bot is a three wheel system designed as small as possible to reduce the probability of collision.
* Each Bot Consists of :
    * A ESP8266 (NodeMcu) : Reading Coordinates File and Controlling Motor Drivers.
    * Two Geared DC Motor and Wheel : Motion of the Bot
    * A Castor Wheel      : Helps in Stability and Rotation
    * A  Motor Driver (LM298) : control Motors
    * 9V Duracell Batteries : Power Source    
    
## Working ##
* The camera placed on top records the real time coordinates of Bots i.e Aruco markers.

<img src="https://github.com/ahad18abd/real_time_motion_planning/blob/master/Images%20and%20Video/Screenshot%20from%202019-12-29%2000-24-44.png" width="50%" height="50%" >

* Real-Time Position Coordinates and Goal Coordinates keep updating in file.
* ESP8266(NodeMcu) access the file on local server and read coordinates by sending a GET request.
* The Motion of Bot follows the following Finite State Machine(FSM) to reach desired goal position without Collision.
![picture alt](https://github.com/ahad18abd/real_time_motion_planning/blob/master/Images%20and%20Video/Screenshot%20(70).png "FSM")


### Files and their Use ### 
__NodeMcu Related__
<details>
<summary>align_n_go.ino</summary>
<p>Bots align themselves with the line to goal point and keep moving towards their goal without taking care collision with other bot.</p>
</details>
<details>
<summary>Version_1.ino</summary>
<p>In addition of aling_n_go, bots take care of collision except head to head and head to tail collision. </p>
</details>
<details>
<summary>Version_2.ino</summary>
<p>In addition of aling_n_go, bots take care of all type of collisions. </p>
</details>

__Setup Related__
<details>
<summary>aruco_detect_camera.py</summary>
<p>Detect Aruco markers placed on each bot and update the file data.txt </p>
</details>
<details>
<summary>data.txt</summary>
<p>Coordinates file,Real Time Updating by aruco_detect_camera.py </p>
</details>
<details>
<summary>server.py</summary>
<p>Python programme to host local server on desired adress and port. </p>
</details>

__Test__
<details>
<summary>Calibrate.ino</summary>
<p>The two Dc motors used differ sligtly due to their mechanical properties, so the speed on  providing same PWM is not same. We can calibrate or tune the two values of PWM for straight movement </p>
</details>
