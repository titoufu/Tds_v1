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
  this->pinSensor = TdsSensorPin;
  this->pinCalibra = TdsCalibraPin;
  this->temperature = 25.0;
  this->aref = 5.0;
  this->adcRange = 1024.0;
  this->kValueAddress = 8;
  this->kValue = 1.0;
}

void Tds_class::setPin(int pinCalibra, int pinSensor)
{
  this->pinCalibra = pinSensor;
  this->pinSensor = pinCalibra;
}

void Tds_class::setTemperature(float temp)
{
  this->temperature = temp;
}

void Tds_class::setAref(float value)
{
  this->aref = value;
}

void Tds_class::setAdcRange(float range)
{
  this->adcRange = range;
}

void Tds_class::setKvalueAddress(int address)
{
  this->kValueAddress = address;
}

void Tds_class::begin()
{
  pinMode(this->pinCalibra, INPUT);
  pinMode(this->pinSensor, INPUT);
  pinMode(this->pinComunica, OUTPUT);
  readKValues();
}

float Tds_class::getKvalue()
{
  return this->kValue;
}

void Tds_class::update()
{
  if (digitalRead(TdsCalibraPin) > 0)
    ecCalibration();
  this->voltage = filterRead(TdsSensorPin);
  this->ecValue = (133.42 * this->voltage * this->voltage * this->voltage - 255.86 * this->voltage * this->voltage + 857.39 * this->voltage) * this->kValue;
  this->ecValue25 = this->ecValue / (1.0 + 0.02 * (this->temperature - 25.0)); // temperature compensation
  this->tdsValue = ecValue25 * TdsFactor;
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
  EEPROM_read(this->kValueAddress, this->kValue);
  if (EEPROM.read(this->kValueAddress) == 0xFF && EEPROM.read(this->kValueAddress + 1) == 0xFF && EEPROM.read(this->kValueAddress + 2) == 0xFF && EEPROM.read(this->kValueAddress + 3) == 0xFF)
  {
    this->kValue = 1.0; // default value: K = 1.0
    EEPROM_write(this->kValueAddress, this->kValue);
  }
}

void Tds_class::ecCalibration()
{
  int i = 0;
  static boolean ecCalibrationFinish = 0;
  static boolean enterCalibrationFlag = 0;
  float KValueTemp, rawECsolution;
  rawECsolution = tdsRawValue / (float)(TdsFactor);
  rawECsolution = rawECsolution * (1.0 + 0.02 * (temperature - 25.0));
  voltage = filterRead(TdsSensorPin);     
  KValueTemp = rawECsolution / (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage); // calibrate in the  buffer solution, such as tdsRawValueppm(1413us/cm)@25^c
 
  while (i < 30)
  {
    digitalWrite(TdsComunicaPin, LOW);
    delay(50);
    digitalWrite(TdsComunicaPin, HIGH);
    delay(50);
    i++;
  }
  if ((KValueTemp) > 0.25 && (KValueTemp < 4.0))
  {
    this->kValue = KValueTemp;
    EEPROM_write(kValueAddress, this->kValue);
    Serial.print(F(">>>  Sucesso na calibração. Valor de K Salvo na EEPROM, k="));
    Serial.println(this->kValue);
    digitalWrite(TdsComunicaPin, LOW);
  }
  else
  {
    Serial.println();
    Serial.println(F(">>> Calibração mal sucedida, tente de novo <<<"));
    digitalWrite(TdsComunicaPin, HIGH);
    Serial.println();
  }
}

float Tds_class::filterRead(int TdsSensorPin)
{
  int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
  int analogBufferTemp[SCOUNT];
  int analogBufferIndex = 0, index = 0;
  static unsigned long analogSampleTimepoint = millis();

  while (analogBufferIndex < SCOUNT)
  {
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin); // read the analog value and store into the buffer
    analogBufferIndex++;
    delay(100);
  }
  float averageVoltage = getMedianNum(analogBuffer) * (float)VREF / 1024.0;
  return averageVoltage;
}
void Tds_class::bubbleSort(int list[], int n)
{
  int i, j, temp;
  for (i = 0; i < n - 1; i++)
  {
    for (j = 0; j < n - i - 1; j++)
    {
      if (list[j] > list[j + 1])
      {
        temp = list[j];
        list[j] = list[j + 1];
        list[j + 1] = temp;
      }
    }
  }
}

int Tds_class::findMedian(int arr[], int n)
{
  bubbleSort(arr, n);
  if (n % 2 != 0)
    return arr[n / 2];
  return (arr[(n - 1) / 2] + arr[n / 2]) / 2;
}

void Tds_class::medianFilter(int *input, int *output, int n)
{
  int extended[n + 2];
  extended[0] = input[0];
  extended[n + 1] = input[n - 1];
  for (int i = 0; i < n; i++)
  {
    extended[i + 1] = input[i];
  }
  for (int i = 0; i < n; i++)
  {
    int window[3];
    window[0] = extended[i];
    window[1] = extended[i + 1];
    window[2] = extended[i + 2];
    output[i] = findMedian(window, 3);
  }
}
int Tds_class::getMedianNum(int x[])
{
  int n = sizeof(x) / sizeof(x[0]);
  int y[n];
  medianFilter(x, y, n);
  int sinalFiltrado = findMedian(y, n);
  return sinalFiltrado;
}
