#include "arduinoFFT.h"
#include <math.h>
#include <Servo.h>

// declare servo objects
Servo servoLeft;          // Define left servo
Servo servoRight;         // Define right servo

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

const uint16_t samples = 1024; // Blocksixe for fft input 
const double samplingFrequency = 50000; // 
double vReal[samples]; 
double vImag[samples];

// Filter coefficcionts - from MATLAB designed with: [b, a] = butter(2, 800*2/48762, 'high');
// Cutoff frequency of 800 Hz, at a sampling freq. of 50000 Hz.
double b0 = 0.9313;
double b1 = -1.8627;
double b2 = 0.9313;
double a0 = 1.0000;
double a1 = -1.8580;
double a2 = 0.8674;

// initialization for past inputs/outputs for difference equation
double x1 = 0;
double x2 = 0;
double Y1 = 0;
double y2 = 0;
double temp = 0; 


void setup() {
  Serial.begin(38400);
  servoLeft.attach(10);  // Set left servo to digital pin 10
  servoRight.attach(9);  // Set right servo to digital pin 9 
}
void loop() {
//  scan for 1khz
  turnLeft();
  unsigned long Time_start = micros();
  for ( uint16_t i = 0; i < samples; i++) 
  {
    vReal[i] = analogRead(A0);    // output of the microphone is at ADC0_RA
    vImag[i] = 0;
    delayMicroseconds(20);
  }
  unsigned long Time_end = micros();
  unsigned long ElapsedTime = Time_end - Time_start;   // I used this value and 1024 to determine the sampling frequency 
  // Filter the signal
  for ( uint16_t i = 0; i < samples; i++) 
  {
    temp = vReal[i];  // store the input singal value before it gets manipulated, for the filtering 
    vReal[i] = b0*vReal[i] + b1*x1 + b2*x2 - a1*Y1 - a2*y2 ; 
    
    y2 = Y1; 
    Y1 = vReal[i];
    x2 = x1;
    x1 = temp; 
    
  } 
  
  // perform fft
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  //  Weigh data 
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); //  Compute FFT 
  FFT.ComplexToMagnitude(vReal, vImag, samples); //  Compute magnitudes 
  // PrintVector(vReal, samples>>1, SCL_FREQUENCY);
   double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
    
  delay(100);
  Serial.println("\n\n\n\n\n\n\n\n\n\nExpected freq");
  Serial.println(x, 6);
  
}


// Motion routines for forward, reverse, turns, and stop
void forward() {
  servoLeft.write(0);
  servoRight.write(180);
}
void reverse() {
  servoLeft.write(180);
  servoRight.write(0);
}
void turnRight() {
  servoLeft.write(180);
  servoRight.write(180);
}
void turnLeft() {
  servoLeft.write(0);
  servoRight.write(0);
}
void stopRobot() {
  servoLeft.write(90);
  servoRight.write(90);
}
