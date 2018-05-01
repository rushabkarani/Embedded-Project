#include <Servo.h>
 
int trigPin = 7;    //Trig - purple Jumper
int echoPin = 8;    //Echo - blue Jumper
long duration, cm, inches;


Servo servoLeft;          // Define left servo
Servo servoRight;         // Define right servo

 
void setup() 
{
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
//  servoLeft.attach(10);  // Set left servo to digital pin 10
//  servoRight.attach(9);
  servoLeft.attach(10,1300,1480);  // Set left servo to digital pin 10
  servoRight.attach(9,1520,1700);  // Set right servo to digital pin 9
}
 
void loop()
{
  float x;
  x = distance();
  Serial.print(x);
  Serial.print("in \n");
  if( x <= 1)
  {
    stopRobot();
  }
  else if(x<=10)
  {
    stopRobot();
    obstacleAvoid(); 
  }
  else
  {
    forward();
  }
}


float distance()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // convert the time into a distance
  cm = (duration/2) / 29.1;
  inches = (duration/2) / 74; 
  

  return (inches);
}
void forward() {
  servoLeft.write(0);
  servoRight.write(180);
} 
void reverse() {
  servoLeft.write(180);
  servoRight.write(0);
}
void turnRight() 
{
  servoLeft.write(180);
  servoRight.write(180);
}
void turnLeft() 
{
  servoLeft.write(0);
  servoRight.write(0);
}
void stopRobot() 
{
  servoRight.write(90);
  servoLeft.write(90);
}
void obstacleAvoid()
{
    stopRobot();
    delay(2000);
    turnRight();
    delay(2000);
    forward();
    delay(2000);
    stopRobot();
    delay(2000);
    turnLeft();
    delay(2000);
    forward();
    delay(2000);
    stopRobot();
    delay(2000);
    turnLeft();
    delay(2000);
    forward();
    delay(2000);
    stopRobot();
    delay(2000);
    turnRight();
    delay(2000);
    forward();
    delay(2000);
    stopRobot(); 
    delay(2000);

}

