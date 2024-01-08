#line 1 "C:\\Users\\jribe\\OneDrive\\Documentos\\Arduino\\Tds_v1\\examples\\Tds_v1\\Tds_class.h"
#ifndef TDS_CLASS_H
#define TDS_CLASS_H
#include "Arduino.h"

#define TdsFactor 0.5  // tds = ec / 2

class Tds_class
{
public:
    Tds_class();
    ~Tds_class();
    void begin();  // initialization
    void update(); // read and calculate
    void setPin(int pin);
    void setTemperature(float temp);    // set the temperature and execute temperature compensation
    void setAref(float value);          // reference voltage on ADC, default 5.0V on Arduino UNO
    void setAdcRange(float range);      // 1024 for 10bit ADC;4096 for 12bit ADC
    void setKvalueAddress(int address); // set the EEPROM address to store the k value,default address:0x08
    float getKvalue();
    float getTdsValue();
    float getEcValue();
    float filterRead(int pin);
    

private:

    int pin;
    float aref; // default 5.0V on Arduino UNO
    float adcRange;
    float temperature;
    int kValueAddress;                                // the address of the K value stored in the EEPROM
    float kValue; // k value of the probe,you can calibrate in buffer solution ,such as 706.5ppm(1413us/cm)@25^C
    float analogValue;
    float voltage;
    float ecValue;   // before temperature compensation
    float ecValue25; // after temperature compensation
    float tdsValue;
    void readKValues();
    void ecCalibration();
};

#endif