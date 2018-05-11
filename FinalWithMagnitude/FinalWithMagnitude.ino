#include "arduinoFFT.h"
#define SAMPLES 256
#define SAMPLING_FREQUENCY 25600
#include <Servo.h>

int trigPin = 7;    //Trig - purple Jumper
int echoPin = 8;    //Echo - blue Jumper

const int ledPin = 13;

const int leftMotor =  10;
const int rightMotor =  9;

//General
int h9 = 1;
int h8 = 1;
int h7 = 1;
int h6 = 1;
int h5 = 1;
int h4 = 1;
int h3 = 1;
int h2 = 1;
int h1 = 1;
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

double vRealValue[7];
double readagain[7];
int distance;
int Magnitude[7];
// int freq = 0;
// int bin = 0;

// Frequency function.
double binValues(int j)
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
    Magnitude[j] = vReal[k];
    Serial.print("peak1 = ");
    Serial.print(peak);
    Serial.print("\n");
    return peak;
}


void checkPosition()
{
//  if(distance>=30)
//  {
    int mini;
    int x = ultrasonic();
    stopRobot();
    delay(1000);
    turnLeft();
    delay(500);
    stopRobot();
    delay(50);
    int y = ultrasonic();
    turnRight();
    delay(1000);
    stopRobot();
    delay(50);
    int z = ultrasonic();
    turnLeft();
    delay(500);
    stopRobot();
    delay(50);
    if(x<y && x<z)
    {
      mini = x;
    }
    else if(y<x && y<z)
    {
      mini =y;
    }
    else
    {
      mini = z;
    }
    if(mini == x)
    {
      
      forward();
      delay(50);
      stopRobot();
      delay(50);
    }
    else if(mini == y)
    {
      turnLeft();
      delay(500);
      stopRobot();
      delay(50);
      forward();
      delay(50);
      stopRobot();
      delay(50);
    }
    else
    {
      turnRight();
      delay(500);
      stopRobot();
      delay(50);
      forward();
      delay(50);
      stopRobot();
      delay(50);
    }
//  }
}

void readFrequency()
{
  for(int j=0; j<7; j++)
  {
    vRealValue[j] = binValues(j);
    delay(200);
  }
}
void readFrequencyAgain()
{
  for(int j=0; j<7; j++)
  {
    readagain[j] = binValues(j);
    delay(200);
  }
}

void  rotate()
{
  turnLeft(); // will move in approx 10 degrees
  delay(250);
  stopRobot();
  delay(10);
}


// Motion routines for forward, reverse, turns, and stop
void forward() 
{
  speedControlL(0,25); // first variable for direction 0- forward, 1- reverse ; Second variable for speed control.
  speedControlR(0,30);
}

void reverse() 
{
  speedControlL(1, 35);
  speedControlR(1, 35);
}
void turnRight() 
{
  speedControlL(1,0);
  speedControlR(0,30);
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

void aroundBecon()
{
 turnLeft();
 delay(1500);
 stopRobot();
 delay(10);
 forward();
 delay(3000);
 turnRight();
 delay(2000);
 stopRobot();
 delay(10);
 forward();
 delay(5000);
 stopRobot();
 delay(10);
 turnRight();
 delay(1500);
 stopRobot();
 delay(10);
 forward();
 delay(3000);
 stopRobot();
 delay(10);
}
  

void setup() 
{
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
  
  // Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  //Led Pin
  pinMode(ledPin, OUTPUT);
 
}

void loop() 
{
  // put your main code here, to run repeatedly:  
  // float distance = peakcheck(vRealValue);
  distance = ultrasonic();
  // Check for Becons Position using ultrasonic
  if(distance <=30)
  {
    // read 7 smaples at one position with a delay of 200ms
    readFrequency();
    // Check the samples in descending order of the frequency that is 9.5k first and 5k last
    for (int i =0; i<7;i++)
    {
//      vRealValue[i] = binValues();
      // Check what frequency it is and go in the Direction
      if (vRealValue[i] <= 9550 && vRealValue[i] >= 9450)
      {
        forward();
        delay(50);
        Serial.println(vRealValue[i]);
        Serial.print("\n");
        distance = ultrasonic(); //read distance of becon from ultrasonic
        // Check if the ultrasonic is too close
        if (distance <=8 && Magnitude[i] >= 700)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          while(1); 
        }
        //Till the becon is ahaed stay in that position
        while(distance<=30 && distance>=6 && h==1)
        {
          //Read for new samples
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
            // Check if the samples are the correct samples 
            if (readagain[i] <= 9550 && readagain[i] >= 9450)
            {
              h=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
            {
              stopRobot();
              digitalWrite(ledPin, HIGH);
              while(1); 
            }
          }
        }
        // once you detect 9.5k dont listen to any other frequency.
        flag9 = 0;
        flag8 = 0;
        flag7 = 0;
        flag6 = 0;
        flag5 = 0;
        flag4 = 0;
        flag3 = 0;
        flag2 = 0;
        flag1 = 0;
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
        distance = ultrasonic();
        if (distance <=8 && Magnitude[i] >= 700)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag9 = 0; 
          h9 = 0;
          aroundBecon();
        }
        while(distance<=30 && distance>=6 && h9==1)
        {
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
            if (readagain[i] <= 9050 && readagain[i] >= 8950)
            {
              h9=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h9 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
            {
              stopRobot();
              digitalWrite(ledPin, HIGH);
              delay(1000);
              digitalWrite(ledPin, LOW);
              delay(1000);
              flag9 = 0; 
              h9 = 0;
              aroundBecon();
            }
          }
        }
        flag8 = 0;
        flag7 = 0;
        flag6 = 0;
        flag5 = 0;
        flag4 = 0;
        flag3 = 0;
        flag2 = 0;
        flag1 = 0;
      }
      else if(vRealValue[i] <= 8550 && vRealValue[i] >= 8450 && flag8 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        distance = ultrasonic();
        if (distance <=8 && Magnitude[i] >= 700)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag8 = 0; 
          h8 = 0;
          aroundBecon();
        }
        while(distance<=30 && distance>=6 && h8==1)
        {
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
//            vRealValue[i] = binValues();
            if (readagain[i] <= 8550 && readagain[i] >= 8450)
            {
              h8=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h8 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
            {
              stopRobot();
              digitalWrite(ledPin, HIGH);
              delay(1000);
              digitalWrite(ledPin, LOW);
              delay(1000);
              flag8 = 0; 
              h8 = 0;
              aroundBecon();
            }
          }
        }
        flag7 = 0;
        flag6 = 0;
        flag5 = 0;
        flag4 = 0;
        flag3 = 0;
        flag2 = 0;
        flag1 = 0;
      }
      else if(vRealValue[i] <= 8050 && vRealValue[i] >= 7950 && flag7 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8 && Magnitude[i] >= 700)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag7 = 0; 
          h7 = 0;
          aroundBecon();
        }
        while(distance<=30 && distance>=6 && h7==1)
        {
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
            if (readagain[i] <= 8050 && readagain[i] >= 7950)
            {
              h7=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h7 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
            {
              stopRobot();
              digitalWrite(ledPin, HIGH);
              delay(1000);
              digitalWrite(ledPin, LOW);
              delay(1000);
              flag7 = 0;
              h7 = 0; 
              aroundBecon();
            }
          }
        }
        flag6 = 0;
        flag5 = 0;
        flag4 = 0;
        flag3 = 0;
        flag2 = 0;
        flag1 = 0;
      }
      else if(vRealValue[i] <= 7550 && vRealValue[i] >= 7450 && flag6 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8 && Magnitude[i] >= 700)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag6 = 0;
          h6 = 0; 
          aroundBecon();
        }
        while(distance<=30 && distance>=6 && h6==1)
        {
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
            if (readagain[i] <= 7550 && readagain[i] >= 7450)
            {
              h6=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h6 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
            {
              stopRobot();
              digitalWrite(ledPin, HIGH);
              delay(1000);
              digitalWrite(ledPin, LOW);
              delay(1000);
              flag6 = 0;
              h6 = 0; 
              aroundBecon();
            }
          }
        }
        flag5 = 0;
        flag4 = 0;
        flag3 = 0;
        flag2 = 0;
        flag1 = 0;
      }
      else if(vRealValue[i] <= 7050 && vRealValue[i] >= 6950 && flag5 == 1)
      {
        forward();
        delay(100);

        Serial.println(vRealValue[i]);
        Serial.print("\n");
        int distance = ultrasonic();
        if (distance <=8 && Magnitude[i] >= 700)
        {
          stopRobot();
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          delay(1000);
          flag5 = 0;
          h5 = 0; 
          aroundBecon();
        }
        while(distance<=30 && distance>=6 && h5==1)
        {
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
//            vRealValue[i] = binValues();
            if (readagain[i] <= 7050 && readagain[i] >= 6950)
            {
              h5=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h5 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
            {
              stopRobot();
              digitalWrite(ledPin, HIGH);
              delay(1000);
              digitalWrite(ledPin, LOW);
              delay(1000);
              flag5 = 0; 
              h5 = 0;
              aroundBecon();
            }
          } 
        }
        flag4 = 0;
        flag3 = 0;
        flag2 = 0;
        flag1 = 0;
      }
     else if(vRealValue[i] <= 6550 && vRealValue[i] >= 6450 && flag4 == 1)
     {
       forward();
       delay(100);

       Serial.println(vRealValue[i]);
       Serial.print("\n");
       int distance = ultrasonic();
       if (distance <=8 && Magnitude[i] >= 700)
       {
         stopRobot();
         digitalWrite(ledPin, HIGH);
         delay(1000);
         digitalWrite(ledPin, LOW);
         delay(1000);
         flag4 = 0; 
         h4 = 0;
         aroundBecon();
       }
       while(distance<=30 && distance>=6 && h4==1)
        {
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
//            vRealValue[i] = binValues();
            if (readagain[i] <= 6550 && readagain[i] >= 6450)
            {
              h4=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h4 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
           {
             stopRobot();
             digitalWrite(ledPin, HIGH);
             delay(1000);
             digitalWrite(ledPin, LOW);
             delay(1000);
             flag4 = 0;
             h4 = 0;
             aroundBecon(); 
           }
          }  
        }
       flag3 = 0;
       flag2 = 0;
       flag1 = 0;
     }
     else if(vRealValue[i] <= 6050 && vRealValue[i] >= 5950 && flag3 == 1)
     {
       forward();
       delay(100);

       Serial.println(vRealValue[i]);
       Serial.print("\n");
       int distance = ultrasonic();
       if (distance <=8 && Magnitude[i] >= 700)
       {
         stopRobot();
         digitalWrite(ledPin, HIGH);
         delay(1000);
         digitalWrite(ledPin, LOW);
         delay(1000);
         flag3 = 0; 
         h3 = 0;
         aroundBecon();
       }
       while(distance<=30 && distance>=6 && h3==1)
        {
         readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
//            vRealValue[i] = binValues();
            if (readagain[i] <= 6050 && readagain[i] >= 5950)
            {
              h3=1;
              forward();
              delay(50);
              distance = ultrasonic();
              checkPosition();
            }
            else
            {
              h3 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
           {
             stopRobot();
             digitalWrite(ledPin, HIGH);
             delay(1000);
             digitalWrite(ledPin, LOW);
             delay(1000);
             flag3 = 0; 
             h3 = 0;
             aroundBecon();
           }
          }
        }
       flag2 = 0;
       flag1 = 0;
     }
     else if(vRealValue[i] <= 5550 && vRealValue[i] >= 5450 && flag2 == 1)
     {
       forward();
       delay(100);

       Serial.println(vRealValue[i]);
       Serial.print("\n");
       int distance = ultrasonic();
       if (distance <=8 && Magnitude[i] >= 700)
       {
         stopRobot();
         digitalWrite(ledPin, HIGH);
         delay(1000);
         digitalWrite(ledPin, LOW);
         delay(1000);
         flag2 = 0; 
         h2 = 0;
         aroundBecon();
       }
       while(distance<=30 && distance>=6 && h2==1)
        {
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
//            vRealValue[i] = binValues();
            if (readagain[i] <= 5550 && readagain[i] >= 5450)
            {
              h2=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h2 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
           {
             stopRobot();
             digitalWrite(ledPin, HIGH);
             delay(1000);
             digitalWrite(ledPin, LOW);
             delay(1000);
             flag2 = 0; 
             h2 = 0;
             aroundBecon();
           }
          }
        }
       flag1 = 0;
     }
     else if(vRealValue[i] <= 5050 && vRealValue[i] >= 4950 && flag1 == 1)
     {
       forward();
       delay(100);

       Serial.println(vRealValue[i]);
       Serial.print("\n");
       int distance = ultrasonic();
       if (distance <=8 && Magnitude[i] >= 700)
       {
         stopRobot();
         digitalWrite(ledPin, HIGH);
         delay(1000);
         digitalWrite(ledPin, LOW);
         delay(1000);
         flag1 = 0; 
         h1 = 0;
         aroundBecon();
       }
       while(distance<=30 && distance>=6 && h==1)
        {
          readFrequencyAgain();
          for(int i =0; i<7; i++)
          {
//            vRealValue[i] = binValues();
            if (readagain[i] <= 5050 && readagain[i] >= 4950)
            {
              h1=1;
              forward();
              delay(50);
              checkPosition();
            }
            else
            {
              h1 = 0;
            }
            distance = ultrasonic();
            if (distance <=8 && Magnitude[i] >= 700)
           {
             stopRobot();
             digitalWrite(ledPin, HIGH);
             delay(1000);
             digitalWrite(ledPin, LOW);
             delay(1000);
             flag1 = 0;
             h1 = 0;
             aroundBecon(); 
           }
          }  
        }
      }
      delay(200);
    }
    rotate();
 }
 else 
 {
   rotate();
 }
  
}

