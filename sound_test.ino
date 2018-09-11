#include <arduinoFFT.h>

uint8_t sound_dig_pin = 2;
uint8_t led_pin = 13;
uint8_t sound_an_pin = A5;

arduinoFFT FFT = arduinoFFT();
const uint16_t samples = 128;
const double samplingFrequency = 8000;
double vReal[samples];
double vImag[samples];
uint16_t offset;
uint8_t i;

unsigned int sampling_period_us;
unsigned long microseconds;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(sound_dig_pin, INPUT);
  pinMode(led_pin, OUTPUT);

  sampling_period_us = round(1000000*(1.0/samplingFrequency));
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(led_pin, digitalRead(sound_dig_pin));
  if(digitalRead(sound_dig_pin) == HIGH) {
    digitalWrite(led_pin, HIGH);

    offset = analogRead(A4);
    for(i = 0; i < samples; i++) {
      microseconds = micros();
      vReal[i] = analogRead(sound_an_pin);
      vImag[i] = 0;
      while(micros() < (microseconds + sampling_period_us));
    }
    
    FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, samples);
    
    for(int i = 0; i<(samples/2); i++) {
      Serial.print((i * 1.0 * samplingFrequency) / samples, 1);
      Serial.print(" ");
      Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
    }
  }
  else {
    digitalWrite(led_pin, LOW);
  }
}
