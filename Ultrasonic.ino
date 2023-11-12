#include <Servo.h>
Servo servo1;
int servo_control;
int servo_pin = 10;
bool automatic=false;
char command;
bool servo_state;

int ena = 6; //pwm of motors
int enb = 3;
int in1 = 11; //forward for left motor
int in2 = 9; //
int in3 = 5;
int in4 = 4;
int left_motor_speed = 100;  
int right_motor_speed = 100;
int manual_speed_right = 245;
int manual_speed_left = 250;
int right_inner = A4;
int left_inner = A2;
int right_sensor = A5; //out 5 - right sensor
int left_sensor = A1; //out 1
int middle_sensor = A3;

const int triggerPin = 7; // Ultrasonic sensor trigger pin
const int echoPin = 8;    // Ultrasonic sensor echo pin
long duration;
int distance;

void setup() {
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  servo1.attach(10);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(right_sensor, INPUT);
  pinMode(left_sensor,INPUT);
//  pinMode(sensor3,INPUT);
//  pinMode(sensor4,INPUT);
  pinMode(middle_sensor,INPUT);
  
  close_hatch();
  command ='s';
  servo_state=false;

  //configuration for BT Module
  Serial.begin(9600);
  
}

void loop() {
  // Measure distance using the ultrasonic sensor
  if (Serial.available()>0){
    command = Serial.read();
    Serial.println(command);

  }

  if (command=='L'){
    right_turn();
    Serial.println("Turning Right");
    
  }

  if (command=='F'){
    forward();
    Serial.println("Turning Left");
  }

  if (command=='R'){
    left_turn();
    Serial.println("Turning Left");
  }

  if (command=='B'){
    reverse();
    Serial.println("Reversing");
  }

  if (command=='I'){
    forward_right();
    Serial.println("Forward Right");
  }

  if (command=='G'){
    forward_left();
    Serial.println("Forward Right");
  }

   if (command=='H'){
    back_right();
    Serial.println("Forward Right");
  }

  if (command=='J'){
    back_left();
    Serial.println("Forward Right");
  }

  

if (command == 'W' or command=='w') {
    if (automatic) {
        // Turn off automatic mode
        automatic = false;
        Serial.println("Automatic Mode Off");
        halt();
    } else {
        // Turn on automatic mode
        automatic = true;
        Serial.println("Automatic Mode On");
        halt();
        
    }
}


if (command == 'V' or command=='v') {
    if (servo_state) {
        servo_state=false;
        close_hatch();
        Serial.println("Hatch closed");
        
    } else {
        // Turn on automatic mode
        servo_state = true;
        Serial.println("Hatch open");
        open_hatch();   
    }
}

  
  
  if (automatic==true){
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    
    int LEFT_SENSOR1  = digitalRead(left_sensor);
    int RIGHT_SENSOR1 = digitalRead(right_sensor);
    
    int LEFT_SENSOR2  = digitalRead(left_inner);
    int RIGHT_SENSOR2 = digitalRead(right_inner);
    int MIDDLE_SENSOR = digitalRead(middle_sensor);

  
    if (distance > 8) {
      // If no obstacle is detected within 8cm, follow the line
      if(RIGHT_SENSOR1==1 && LEFT_SENSOR1==1) {
          linear(); //FORWARD
          Serial.println("Sensor straight");
    }

    else if((RIGHT_SENSOR1==0  && LEFT_SENSOR1==1)||(RIGHT_SENSOR2==0  && LEFT_SENSOR2==1)) {
     line_right(); //Move Right
     Serial.println("Sensor right");
  }

    else if((RIGHT_SENSOR1==1  && LEFT_SENSOR1==0)||(RIGHT_SENSOR2==1  && LEFT_SENSOR2==0)) {
      line_left(); //Move Left
      Serial.println("Sensor left");
  }

//  else if(RIGHT_SENSOR2==0  && LEFT_SENSOR2==1) {
//     line_right(); //Move Right
//     Serial.println("Sensor right");
//  }
//
//    else if(RIGHT_SENSOR2==1  && LEFT_SENSOR2==0) {
//      line_left(); //Move Left
//      Serial.println("Sensor left");
//  }
//
//  else if(RIGHT_SENSOR2==0  && LEFT_SENSOR2==0) {
//          linear(); //FORWARD
//          Serial.println("Sensor straight");
//    }

  

    else if(RIGHT_SENSOR1==0  && LEFT_SENSOR1==0) {
     halt();  //STOP
//     open_hatch();
     delay(300);
  }
    
  }
  else{
    halt();
  }
 }//brace for automatic condition
  else{
    halt();
  }
}







void linear(){
  analogWrite(ena, right_motor_speed);
  analogWrite(enb, left_motor_speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in4, LOW);
}

void halt(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); //left motor
  digitalWrite(in4, LOW);
}

void rotation_360(){
  analogWrite(ena, right_motor_speed);
  analogWrite(enb, left_motor_speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); //left motor
  digitalWrite(in4, HIGH);
}

void open_hatch(){
  for (int i = 0; i <= 60; i++){
    servo1.write(i);
    delay(15);
  }
}

void close_hatch(){
  for (int i = 60; i >= 0; i--){
    servo1.write(i);
    delay(15);
  }
}

void left_motor_run(){
  analogWrite(ena, right_motor_speed);
  analogWrite(enb, left_motor_speed);
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in4, LOW);
}

void right_motor_run(){
  analogWrite(ena, right_motor_speed);
  analogWrite(enb, left_motor_speed);
  digitalWrite(in1, HIGH); //right motor
  digitalWrite(in2, LOW);
}

void left_escape(){
  analogWrite(ena, 70);
  analogWrite(enb, 70);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); //right motor
  digitalWrite(in4, LOW);
  delay(400);
  halt();
  delay(100);
  linear();
  delay(100);
  halt();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); //right motor
  digitalWrite(in4, HIGH);
  delay(400);
  linear();
  delay(400);
  halt();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); //right motor
  digitalWrite(in4, HIGH);
  delay(400);
  halt();
  linear();
  delay(400);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); //right motor
  digitalWrite(in4, LOW);
  delay(200);
  
}

void left_turn(){
  analogWrite(ena, right_motor_speed);
  analogWrite(enb, left_motor_speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in4, LOW);
}

void right_turn(){
  analogWrite(ena, right_motor_speed);
  analogWrite(enb, left_motor_speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); //left motor
  digitalWrite(in4, HIGH);
}

void reverse(){
  analogWrite(ena, manual_speed_right);
  analogWrite(enb, manual_speed_left);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); //left motor
  digitalWrite(in4, HIGH);
}


void line_right(){
  
  analogWrite(ena, 40);
  analogWrite(enb, 150);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in4, LOW);

}

void line_left(){
  
  analogWrite(ena, 150);
  analogWrite(enb, 40);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in4, LOW);

}

void forward(){
 
  analogWrite(ena, manual_speed_right);
  analogWrite(enb, manual_speed_left);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in4, LOW);

}

void forward_right(){
  analogWrite(ena, manual_speed_right-160);
  analogWrite(enb, manual_speed_left);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in4, LOW);
}

void forward_left(){
  analogWrite(ena, manual_speed_right);
  analogWrite(enb, manual_speed_left-160);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in4, LOW);
}

void back_right(){
  analogWrite(ena, manual_speed_right);
  analogWrite(enb, manual_speed_left-160);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); //left motor
  digitalWrite(in4, HIGH);
}

void back_left(){
  analogWrite(ena, manual_speed_right-160);
  analogWrite(enb, manual_speed_left);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); //left motor
  digitalWrite(in4, HIGH);
}
