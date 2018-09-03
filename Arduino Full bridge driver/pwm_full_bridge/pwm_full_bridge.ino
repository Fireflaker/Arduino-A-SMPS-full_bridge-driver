
///https://playground.arduino.cc/Main/TimerPWMCheatsheet-
//Setting                          Prescale_factor
//TCCR0B = _BV(CS00);              1
//TCCR0B = _BV(CS01);              8
//TCCR0B = _BV(CS00) | _BV(CS01);  64
//TCCR0B = _BV(CS02);              256
//TCCR0B = _BV(CS00) | _BV(CS02);  1024

//To use Fast PWM on Timer 0
//Write this line in the setup function
//TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
//And to calculate the PWM frequency, use
//Fast_PWM_frequency = (16 000 000)/(Prescale_factor*256);
//
//To use Phase-correct PWM on Timer 0 (half the frequency of Fast PWM)
//Write this line in the setup function
//TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
//And to calculate the PWM frequency, use
//Phase_correct_PWM_frequency = (16 000 000)/(Prescale_factor*510);

#define Q1 9
#define Q2 10
#define Tin A2
#define Ain A1
#define Vin A0
#define lowled 5
#define highled 6
#define faultled 7
#define faultin 2
#define resetpin 4

//=============PWM def start===========
#define PRESCALER 1
#define PRESCALER_BITS 0x01
#define CLK 16000000UL

int duty = 10;
int change = 1;
//=============PWM def end===========

//=============ADC def start===========
// Define various ADC prescaler
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

#include "avdweb_AnalogReadFast.h"

int v = 888;
int vset = 300;

//=============ADC def end===========

//=============Misc start==============
#include <digitalWriteFast.h>
//=============Misc end==============

//=============For testing start============
int start = 0;
int i = 0;
//=============For testing end============

void setup() {
  //=============PWM set start===========
  TCCR1A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20); //I dont know. So I must keep these in order.
  TCCR1B = (TCCR1B & 0b11111000) | 0x01; //set prescaler
  TCCR1A = TCCR1A | 0x30;                //Invert
  TCNT1  = 0;//initialize counter value to 0
  //  analogWrite(Q1, 0);
  //  analogWrite(Q2, 0);
  //  use fancy version instead. OCR1A = 0 = 0% and 255 = 100%
  //
  OCR1A = 0;
  OCR1B = 0;

  pinMode (Q1, OUTPUT);
  pinMode (Q2, OUTPUT);
  //=============PWM set end===========

  //=============ADC set start===========
  pinMode (Tin, INPUT);
  pinMode (Ain, INPUT);
  pinMode (Vin, INPUT);
  if (analogRead(Tin));
  if (analogRead(Ain));
  if (analogRead(Vin));  //Kick the analog reading to work

  ADCSRA &= ~PS_128;  // remove bits set by Arduino library

  // you can choose a prescaler from above.
  // PS_64 or PS_128
  ADCSRA |= PS_16;    // set our own prescaler to 16
  //=============ADC set end===========
  //=============Misc start==============
  pinMode (faultin, INPUT_PULLUP);
  pinMode (resetpin, OUTPUT);
  digitalWriteFast(resetpin, HIGH);
  attachInterrupt(digitalPinToInterrupt(faultin), fault, LOW); //just in case, don't connect any LED. A316j can pulldown 8mA max.
  //=============Misc end==============

  //=============For testing start============
  Serial.begin(115200);
  //=============For testing end============


}



void loop() {

  v = analogReadFast(A0);

  change = vset - v;
  duty = duty + (change / 4); // remove devide 4 for more responsiveness. Leave /4 for ease of troubleshooting

  if (duty <= 1 ) {          // Too low
    duty = 1;
    digitalWriteFast(lowled, HIGH);
  }

  if (duty >= 120) {        // Too high
    duty = 120;
    digitalWriteFast(highled, HIGH);
  }

  OCR1A = duty;             //analogWrite(Q1, duty);
  OCR1B = (255 - duty);     //analogWrite(Q2, (255 - duty));


  digitalWriteFast(lowled, LOW);
  digitalWriteFast(highled, LOW);

  //=============For testing start============
  Serial.println(v);

  delay(20);
  //=============For testing end============

}

void fault() {
  OCR1A = 0;                                        // It will reset after 500sec, the fault goes high and we are good.
  OCR1B = 0;
  pinModeFast(Q1, INPUT);
  pinModeFast(Q2, INPUT);
  digitalWriteFast(faultled, HIGH);
  //  bool boollow = digitalReadFast(Q1);
  //  bool boolhigh = digitalReadFast(Q2);
  //  digitalWriteFast(lowled, boollow);
  //  digitalWriteFast(highled, boolhigh);
  delay(500000);
  digitalWriteFast(faultled, LOW);
  delay(200);
  digitalWriteFast(faultled, HIGH);
  delay(200);
  digitalWriteFast(faultled, LOW);
  delay(200);
  digitalWriteFast(faultled, HIGH);
  delay(200);
  digitalWriteFast(faultled, LOW);
  delay(200);
  digitalWriteFast(resetpin, LOW);
  delay(100);
  digitalWriteFast(resetpin, HIGH);
  delay(50);
}



