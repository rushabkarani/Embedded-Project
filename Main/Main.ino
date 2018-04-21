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

// Filter coefficcionts - from MATLAB designed with: [b, a] = butter(2, 800*2/50000, 'high');
// Cutoff frequency of 800 Hz, at a sampling freq. of 50000 Hz.
double b0 =  0.9313;
double b1 = -1.8627;
double b2 =  0.9313;
double a0 = 1.0000;
double a1 = -1.8580;
double a2 = 0.8674;
// insitialization for past inputs/outputs for difference equation
double x1 = 0;
double x2 = 0;
double Y1 = 0;
double y2 = 0;
double temp = 0; 


void setup() {
  // put your setup code here, to run once:
  // ADC code
  //We want portD 1 for A0, but GPIO is default as analog so lucky us
  //We want to use ADC0, ADC0_SE5b, AD5b
  //Single ended channels AD23- AD4, we want AD5
  Serial.begin(38400);
  servoLeft.attach(10);  // Set left servo to digital pin 10
  servoRight.attach(9);  // Set right servo to digital pin 9
  
  //ADC0 Configuration register 1
  ADC0_CFG1 |= _BV(5) | (1U << 6); // clock division of 8. 9MHZ, 
  ADC0_CFG1 |= (1U << 2) | (1U << 3); //16 bit conversion mode
  ADC0_CFG1 |= _BV(4); // Long sample time
  
  //ADC0 configuration register 2
  ADC0_CFG2 |= _BV(4); //This selects ADXXb, in our case AD05b or just AD5b
  //ADC0_CFG2 |= ADC_CFG2_MUXSEL; //This does the same thing as above
  //ADC0 Programmable gain amplifier
  ADC0_PGA |= ADC_PGA_PGAEN; //Use this line if you want to use programmable gain amplified that Professor discussed
  ADC0_PGA |= _BV(18);  // gain of 8 
   
  //ADC0 Status and control register 3
  ADC0_SC3 |= ADC_SC3_ADCO | ADC_SC3_AVGE; //These are in kinetis.h, enables continuous mode and averaging function. You don't NEED averaging function.
  //ADC0 Status control register 1
  ADC0_SC1A = 5; // or binary 0101, it is equal not OR equal since I disabled the other configurations on purpose.
  
}
void loop() {
  // put your main code here, to run repeatedly:
//  scan for 1khz
    turnLeft();
  
  // The following loop stores the samples from ADC into array vREAL
  
  unsigned long Time_start = micros();
  
  for ( uint16_t i = 0; i < samples; i++) {
    vReal[i] = ADC0_RA;    // output of the microphone is at ADC0_RA
  
    vImag[i] = 0;
    
    delayMicroseconds(20);
     } // end for
 // Filter the signal
   for ( uint16_t i = 0; i < samples; i++) {
    temp = vReal[i];  // store the input singal value before it gets manipulated, for the filtering 
   
    vReal[i] = b0*vReal[i] + b1*x1 + b2*x2 - a1*Y1 - a2*y2 ; 
    
    y2 = Y1; 
    Y1 = vReal[i];
    x2 = x1;
    x1 = temp; 
    
  } // end for
  // perform fft
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  //  Weigh data 
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); //  Compute FFT 
  FFT.ComplexToMagnitude(vReal, vImag, samples); //  Compute magnitudes 
  // PrintVector(vReal, samples>>1, SCL_FREQUENCY);
   double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
    
   delay(100);
   Serial.println("\n\nExpected freq");
   Serial.println(x, 4);
   
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
