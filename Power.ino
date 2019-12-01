unsigned long totalWatts;
unsigned long startTime;
unsigned long samples;

int16_t vSense;
int16_t adcMax;
float vDivider;
float volts;

float getWattHours() {
  totalWatts += getWatts();
  samples++;
  unsigned long currentTime = millis() - startTime;
  float elapsedTime = (float) currentTime / 1000.0;
  float averageWatts = totalWatts / samples;
  float wattSeconds = averageWatts * elapsedTime;
  float wattHours = wattSeconds / 3600;

  return wattHours;
}

int getWatts() {
  // Return watts
  return (int) (getVolts() * getAmps());
}

float getVolts() {
  // Get voltage
  float vcc = float(getVCC()) / 1000;
  int16_t vSense = adc.readADC_SingleEnded(2);
  if(vSense < 0) vSense = 0;
  int16_t adcMax = adc.readADC_SingleEnded(3);
  float vDivider = vSense * (vcc / adcMax);
  float volts = vDivider / (2200.0 / (50000.0 + 2200.0));
  return rollingAverage(vBuffer, BUFFERSIZE, volts);
}

float getAmps() {
  // Get current
  float vcc = float(getVCC()) / 1000;
  int16_t cSense = adc.readADC_SingleEnded(0);
  int16_t cRef = adc.readADC_SingleEnded(1);
  int16_t adcMax = adc.readADC_SingleEnded(3);
  float cIn = cSense * (vcc / adcMax);
  float cRefIn = cRef * (vcc / adcMax);
  float amps = abs((cIn - cRefIn) * 200.0 / 1.25);
  //return amps;
  return rollingAverage(aBuffer, BUFFERSIZE, amps);
}

long getVCC() {
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));

  uint8_t low  = ADCL; 
  uint8_t high = ADCH;

  long result = (high<<8) | low;

  result = 1125300L / result; 
  return result;
}
