#include "arduinoFFT.h"
#define SAMPLES 256
#define SAMPLING_FREQUENCY 25600
#include <Servo.h>

int trigPin = 7;    //Trig - purple Jumper
int echoPin = 8;    //Echo - blue Jumper
//Servo servoLeft;          // Define left servo
//Servo servoRight;         // Define right servo
const int ledPin = 13;

const int leftMotor =  10;
const int rightMotor =  9;

//General
int h = 1;
int flag9 = 1;
int flag8 = 1;
int flag7 = 1;
int flag6 = 1;
int flag5 = 1;
int flag4 = 1;
int flag3 = 1;
int flag2 = 1;
int flag1 = 1;

arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
// int freq = 0;
// int bin = 0;



// Frequency function.
double binValues()
{
  for(int i=0; i<SAMPLES; i++)
  {
    microseconds = micros();
    vReal[i] = analogRead(A0);
    vImag[i] = 0;
    while(micros()<(microseconds + sampling_period_us)){}
  }

    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    vReal[0] = 0;
    vReal[1] = 0;
    vReal[2] = 0;

    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY); 
    int k = round(peak/SAMPLING_FREQUENCY*SAMPLES);
    // double bins[10] = {vReal[10], vReal[20], vReal[30], vReal[40], vReal[50], vReal[60], vReal[70], vReal[80], vReal[90], vReal[100]}
    Serial.print("peak1 = ");
    Serial.print(peak);
    // Serial.print(" k:");
    // Serial.print(k);
    // Serial.print(" Amplitude = ");
    // Serial.println(vReal[k]);
    Serial.print("\n");
//    delay(1000);
    return peak;
}


// Motion routines for forward, reverse, turns, and stop
void forward() 
{
  speedControlL(0,24); // first variable for direction 0- forward, 1- reverse ; Second variable for speed control.
  speedControlR(0,25);
}

void reverse() 
{
  speedControlL(1, 35);
  speedControlR(1, 35);
}
void turnRight() 
{
  speedControlL(1,0);
  speedControlR(0,25);
}
void turnLeft() 
{
  speedControlL(0,25);
  speedControlR(1,0);
}
void stopRobot() 
{

  analogWrite(leftMotor, 76);
  analogWrite(rightMotor, 78);
  // digitalWrite(ledPin, HIGH);
}

// Function Motor Direction and speed control
 void speedControlL(int a, int x)
 {
   int y;
   if(a == 1)
   {
     y = map(x, 0, 100, 76, 51);
   }
   else
   {
     y = map(x, 0, 100, 78, 102);
   }
//   Serial.print (y);
   analogWrite(leftMotor, y);
   // analogWrite(rightMotor, y);
 }
 void speedControlR(int a, int x)
 {
   int y;
   if(a == 1)
   {
     y = map(x, 0, 100, 76, 51);
   }
   else
   {
     y = map(x, 0, 100, 78, 102);
   }
   // analogWrite(leftMotor, y);
   analogWrite(rightMotor, y);
 }


// Check the peak freq and return the ultrasonic distance of bot from becon

// Ultrasonic code returns distance in inches
float ultrasonic()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  float duration = pulseIn(echoPin, HIGH);
 
  // convert the time into a distance
//  cm = (duration/2) / 29.1;
  float inches = (duration/2) / 74; 
  Serial.print(inches);
  Serial.print("in \n");
  return (inches);
}
  


  
void setup() 
{
  // put your setup code here, to run once:
  // Motor Initialisation
  analogWriteFrequency(9,50);
  analogWriteFrequency(10,50); // if you change one pin's PWM frequency, all other pins for the same timer change.
  analogWriteResolution(10);               
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);

  //Serial 
  Serial.begin(115200);
  
  // FFT 
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
  // int freq[10] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
  
  // Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  //Led Pin
  pinMode(ledPin, OUTPUT);
 
}

void loop() 
{
  // put your main code here, to run repeatedly:

  double vRealValue[7];
  double readagain[7];
//  for(int j=0; j<7; j++)
//  {
//    vRealValue[j] = binValues();
//    delay(200);
//  }
  // float distance = peakcheck(vRealValue);
//  int distance = ultrasonic();
//  if(distance <=35)
//  {
    for (int i =0; i<7;i++)
    {
      vRealValue[i] = binValues();
      if (vRealValue[i] <= 9550 && vRealValue[i] >= 9450)
      {
        forward();
        delay(50);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          while(1); 
        }
        h = 1;
        while(distance<=20 && h==1)
        {
          for(int i =0; i<7; i++)
          {
            vRealValue[i] = binValues();
            if (readagain[i] <= 9550 && readagain[i] >= 9450)
            {
              h=1;
              forward();
              delay(50);
              distance = ultrasonic();
              if (distance <=8)
              {
                stopRobot();
                digitalWrite(ledPin, HIGH);
                while(1); 
              }
              else if(distance >=30)
              {
                h=0;
              }
            }
            if (distance <=8)
            {
              stopRobot();
              digitalWrite(ledPin, HIGH);
              while(1); 
            }
            delay(200);
          }
          distance = ultrasonic();
          if (distance <=8)
          {
            stopRobot();
            digitalWrite(ledPin, HIGH);
            while(1); 
          }
        }
        flag9 = 0;
        flag8 =0;
        flag7 = 0;
        flag6 =0;
        flag5 =0;
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
        delay(1000);
        }
      else if(vRealValue[i] <= 9050 && vRealValue[i] >= 8950 && flag9 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag9 = 0; 
        }
        flag8 =0;
        flag7 = 0;
        flag6 =0;
        flag5 =0;
      }
      else if(vRealValue[i] <= 8550 && vRealValue[i] >= 8450 && flag8 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag8 = 0; 
        }
        flag7 = 0;
        flag6 =0;
        flag5 =0;
      }
      else if(vRealValue[i] <= 8050 && vRealValue[i] >= 7950 && flag7 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag7 = 0; 
        }
        flag6 =0;
        flag5 =0;
      }
      else if(vRealValue[i] <= 7550 && vRealValue[i] >= 7450 && flag6 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag6 = 0; 
        }
        flag5 =0;
      }
      else if(vRealValue[i] <= 7050 && vRealValue[i] >= 6950 && flag5 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag5 = 0; 
        }
      }
     else if(vRealValue[i] <= 6550 && vRealValue[i] >= 6450 && flag4 == 1)
     {
       forward();
       delay(100);

       Serial.println(vRealValue[i]);
       Serial.print("\n");
       int distance = ultrasonic();
       if (distance <=8)
       {
         stopRobot();
         digitalWrite(ledPin, HIGH);
         delay(1000);
         digitalWrite(ledPin, LOW);
         delay(1000);
         flag4 = 0; 
       }
     }
     else if(vRealValue[i] <= 6050 && vRealValue[i] >= 5950 && flag3 == 1)
     {
       forward();
       delay(100);

       Serial.println(vRealValue[i]);
       Serial.print("\n");
       int distance = ultrasonic();
       if (distance <=8)
       {
         stopRobot();
         digitalWrite(ledPin, HIGH);
         delay(1000);
         digitalWrite(ledPin, LOW);
         delay(1000);
         flag3 = 0; 
       }
     }
     else if(vRealValue[i] <= 5550 && vRealValue[i] >= 5450 && flag2 == 1)
     {
       forward();
       delay(100);

       Serial.println(vRealValue[i]);
       Serial.print("\n");
       int distance = ultrasonic();
       if (distance <=8)
       {
         stopRobot();
         digitalWrite(ledPin, HIGH);
         delay(1000);
         digitalWrite(ledPin, LOW);
         delay(1000);
         flag2 = 0; 
       }
     }
     else if(vRealValue[i] <= 5050 && vRealValue[i] >= 4950 && flag1 == 1)
     {
       forward();
       delay(100);

       Serial.println(vRealValue[i]);
       Serial.print("\n");
       int distance = ultrasonic();
       if (distance <=8)
       {
         stopRobot();
         digitalWrite(ledPin, HIGH);
         delay(1000);
         digitalWrite(ledPin, LOW);
         delay(1000);
         flag1 = 0; 
       }
     }
      delay(200);
    }
    turnLeft(); // will move in approx 10 degrees
    delay(500);
    stopRobot();
    delay(100);
//  }
//  else 
//  {
//    turnLeft(); // will move in approx 10 degrees
//    delay(500);
//    stopRobot();
//    delay(100);
//  }
  
  
}

