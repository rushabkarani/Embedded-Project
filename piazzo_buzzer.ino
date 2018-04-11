
void setup() {
  // put your setup code here, to run once:
//pinMode(10, OUTPUT);
Serial.begin(9600);

//ADC0 Configuration register 1
//ADC0_CFG1 |= _BV(5) | (1U << 6); // clock division of 8. 9MHZ, 
//ADC0_CFG1 |= (1U << 2) | (1U << 3); //16 bit conversion mode
//ADC0_CFG1 = 0b00000000000000000000000000001100;

//ADC0 configuration register 2
//ADC0_CFG2 |= _BV(4); //This selects ADXXb, in our case AD05b or just AD5b
//ADC0_CFG2 |= ADC_CFG2_MUXSEL; //This does the same thing as above
//ADC0_CFG2 = 0b00000000000000000000000000010000;

//ADC0 Programmable gain amplifier
//ADC0_PGA |= ADC_PGA_PGAEN; //Use this line if you want to use programmable gain amplified that Professor discussed

//ADC0 Status and control register 3

//ADC0_SC3 |= ADC_SC3_ADCO | ADC_SC3_AVGE; //These are in kinetis.h, enables continuous mode and averaging function. You don't NEED averaging function. 
//ADC0_SC3 |= ADC_SC3_ADCO ;

//ADC0 Status control register 1
//ADC0_SC1A = 5; // or binary 0101, it is equal not OR equal since I disabled the other configurations on purpose.

}

void loop() {
  // put your main code here, to run repeatedly:
//tone(10,2000);
//delay(1000);
//noTone (10);
//delay (1000);
//uint16_t data = ADC0_RA; // Data result register A

int sensorVal = analogRead(A0);

Serial.println(sensorVal);
delay(10);
}

