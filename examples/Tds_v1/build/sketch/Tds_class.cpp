#line 1 "C:\\Users\\jribe\\OneDrive\\Documentos\\Arduino\\Tds_v1\\examples\\Tds_v1\\Tds_class.cpp"
#include <EEPROM.h>
#include "Tds_class.h"
#define EEPROM_write(address, p)        \
  {                                     \
    int i = 0;                          \
    byte *pp = (byte *)&(p);            \
    for (; i < sizeof(p); i++)          \
      EEPROM.write(address + i, pp[i]); \
  }
#define EEPROM_read(address, p)         \
  {                                     \
    int i = 0;                          \
    byte *pp = (byte *)&(p);            \
    for (; i < sizeof(p); i++)          \
      pp[i] = EEPROM.read(address + i); \
  }


Tds_class::~Tds_class()
{
}
Tds_class::Tds_class()
{
  pin = A0;
  temperature = 25.0;
  aref = 5.0;
  adcRange = 1024.0;
  kValueAddress = 8;
  kValue = 1.0;
}

void Tds_class::setPin(int pin)
{
  pin = pin;
}

void Tds_class::setTemperature(float temp)
{
  temperature = temp;
}

void Tds_class::setAref(float value)
{
  aref = value;
}

void Tds_class::setAdcRange(float range)
{
  adcRange = range;
}

void Tds_class::setKvalueAddress(int address)
{
  kValueAddress = address;
}

void Tds_class::begin()
{
  pinMode(pin, INPUT);
  readKValues();
}

float Tds_class::getKvalue()
{
  return kValue;
}

void Tds_class::update()
{


  analogValue = filterRead(pin);


  voltage = analogValue / adcRange * aref;
  ecValue = (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage) * kValue;
  ecValue25 = ecValue / (1.0 + 0.02 * (temperature - 25.0)); // temperature compensation
  tdsValue = ecValue25 * TdsFactor;
  if (digitalRead(D5) > 0)
  {
    ecCalibration(); // if D5 is HIGH
  }
}

float Tds_class::getTdsValue()
{
  return tdsValue;
}

float Tds_class::getEcValue()
{
  return ecValue25;
}

void Tds_class::readKValues()
{
  EEPROM_read(kValueAddress, kValue);
  if (EEPROM.read(kValueAddress) == 0xFF && EEPROM.read(kValueAddress + 1) == 0xFF && EEPROM.read(kValueAddress + 2) == 0xFF && EEPROM.read(kValueAddress + 3) == 0xFF)
  {
    kValue = 1.0; // default value: K = 1.0
    EEPROM_write(kValueAddress, kValue);
  }
}

void Tds_class::ecCalibration()
{
  char *cmdReceivedBufferPtr;
  static boolean ecCalibrationFinish = 0;
  static boolean enterCalibrationFlag = 0;
  float KValueTemp, rawECsolution;
  enterCalibrationFlag = 1;
  ecCalibrationFinish = 0;

  rawECsolution = 707;
  rawECsolution = rawECsolution * (1.0 + 0.02 * (temperature - 25.0));

  KValueTemp = rawECsolution / (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage); // calibrate in the  buffer solution, such as 707ppm(1413us/cm)@25^c
  if ((rawECsolution > 0) && (rawECsolution < 2000) && (KValueTemp > 0.25) && (KValueTemp < 4.0))
  {
    Serial.println();
    Serial.print(F(">>>Confrim Successful,K:"));
    Serial.print(KValueTemp);
    kValue = KValueTemp;
    ecCalibrationFinish = 1;
  }
  else
  {
    Serial.println();
    Serial.println(F(">>>Confirm Failed,Try Again<<<"));
    Serial.println();
    ecCalibrationFinish = 0;
  }

}

void Tds_class::filterRead(int pin){

}

