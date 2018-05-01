#include "arduinoFFT.h"
#define SAMPLES 256
#define SAMPLING_FREQUENCY 25600
#include <Servo.h>


Servo servoLeft;          // Define left servo
Servo servoRight;         // Define right servo

arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
  int freq = 0;
  int bin = 0;
  
void setup() 
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
  servoLeft.attach(10,1300,1480);  // Set left servo to digital pin 10
  servoRight.attach(9,1520,1700);
}

void loop() {
  // put your main code here, to run repeatedly:

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
 
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY); 
    int k = round(peak/SAMPLING_FREQUENCY*SAMPLES);
    int freq[10] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
    double bins[10] = {vReal[10], vReal[20], vReal[30], vReal[40], vReal[50], vReal[60], vReal[70], vReal[80], vReal[90], vReal[100]};
    int PreviousAmplitude = vReal[k];
    int location = 0;
    
    if (peak>=10000){
      //move to listen to larger amplitude vReal[100]
      turnRight();
      delay(2000); //figure out how long to move for a 90 degree turn
      stopRobot();
      double* vRealValue = binValues();
      double Amplitude = vRealValue[100];
      if (Amplitude > PreviousAmplitude)
      {
        PreviousAmplitude = Amplitude;
        location = 1;
       }
      turnLeft();
      delay(4000); //figure out delay amount to turn back 180 degrees
      stopRobot();
      vRealValue = binValues();
      Amplitude = vRealValue[100];
      if (Amplitude > PreviousAmplitude){
        PreviousAmplitude = Amplitude;
        location = 2;
        }
      if (location == 2){
        forward();
        delay(2000);
        stopRobot();
      }
      else if (location == 0){
        turnRight();
        delay(2000);
        forward();
        delay(2000);
        stopRobot();
      }
      else{
        turnRight();
        delay(4000); //delay value to move back 180 degrees
        forward();
        delay(2000);
        stopRobot();
     }
     
    }
    if (9000<=peak<=9200){
      // move and listen to vReal[90]
       turnRight();
      delay(2000); //figure out how long to move for a 90 degree turn
      stopRobot();
      double* vRealValue = binValues();
      double Amplitude = vRealValue[90];
      if (Amplitude > PreviousAmplitude){
        PreviousAmplitude = Amplitude;
        location = 1;
        }
      turnLeft();
      delay(4000); //figure out delay amount to turn back 180 degrees
      stopRobot();
      vRealValue = binValues();
      Amplitude = vRealValue[90];
      if (Amplitude > PreviousAmplitude){
        PreviousAmplitude = Amplitude;
        location = 2;
        }
      if (location == 2){
        forward();
        delay(2000);
        stopRobot();
      }
      else if (location == 0){
        turnRight();
        delay(2000);
        forward();
        delay(2000);
        stopRobot();
      }
      else{
        turnRight();
        delay(4000); //delay value to move back 180 degrees
        forward();
        delay(2000);
        stopRobot();
     }
     

      
      }
 

    // check amplitude and move car 
    // move for dealy(2000)
    // stopRobot();
    
 Serial.print("peak1 = ");

    Serial.print(peak);
    Serial.print(" k:");
    Serial.print(k);
    Serial.print(" Amplitude = ");
    Serial.println(vReal[k]);
    delay(1000); 

}

double* binValues(){
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
    return vReal;
  }

  // Motion routines for forward, reverse, turns, and stop
void reverse() {
  servoLeft.write(0);
  servoRight.write(180);
}

void forward() {
  servoLeft.write(180);
  servoRight.write(0);
}

void turnLeft() {
  servoLeft.write(180);
  servoRight.write(180);
}
void turnRight() {
  servoLeft.write(0);
  servoRight.write(0);
}

void stopRobot() {
  servoLeft.write(90);
  servoRight.write(90);
}

  

