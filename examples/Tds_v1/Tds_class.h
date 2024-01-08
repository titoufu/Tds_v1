#ifndef TDS_CLASS_H
#define TDS_CLASS_H
#include "Arduino.h"

#define TdsFactor 1.0     // tds = ec / 2
#define TdsSensorPin A0
#define TdsCalibraPin D2  // se HIGH calibra, se LOW não.
#define TdsComunicaPin D5
#define SCOUNT 30         // sum of sample point
#define tdsRawValue 1414.0         // sponto de calibração.


class Tds_class
{
    
public:
    Tds_class();
    ~Tds_class();
    void begin();  // initialization
    void update(); // read and calculate
    void setPin(int pinCalibra,int pinSensor);
    void setTemperature(float temp);    // set the temperature and execute temperature compensation
    void setAref(float value);          // reference voltage on ADC, default 5.0V on Arduino UNO
    void setAdcRange(float range);      // 1024 for 10bit ADC;4096 for 12bit ADC
    void setKvalueAddress(int address); // set the EEPROM address to store the k value,default address:0x08
    float getKvalue();
    float getTdsValue();
    float getEcValue();
    float filterRead(int pin);
    void bubbleSort(int list[], int n);

private:
    int pinSensor;
    int pinCalibra;
    int pinComunica;
    float aref; // default 5.0V on Arduino UNO
    float adcRange;
    float temperature;
    int kValueAddress; // the address of the K value stored in the EEPROM
    float kValue;      // k value of the probe,you can calibrate in buffer solution ,such as 706.5ppm(1413us/cm)@25^C
    float analogValue;
    float voltage;
    float ecValue;   // before temperature compensation
    float ecValue25; // after temperature compensation
    float tdsValue;
    void readKValues();
    void ecCalibration();
    void medianFilter(int *input, int *output, int n);
    int getMedianNum(int x[]);
    int findMedian(int arr[], int n);
    void eepromWrite(float valueToSave,uint addr);
    float eepromRead(uint addr);
    float calcularMedia(int vetor[]);
};

#endif