/*
   Tds_v0.h

   Calcula a Condutividade Elétrica (EC) de uma solução
   usando módulo do arduino.

   Obs. Não faz calibração e  nem compensação da temperatura

   Autor: José Francisco Ribeiro
   Janeiro de 2024

   Ref:

**************************************************

  DFRobot Gravity: Analog TDS Sensor / Meter For Arduino
  <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>

  Created 2017-8-22
  By Jason <jason.ling@dfrobot.com@dfrobot.com>

  GNU Lesser General Public License.
  See <http://www.gnu.org/licenses/> for details.
  All above must be included in any redistribution

  /***********Notice and Trouble shooting***************
  1. This code is tested on Arduino Uno and Leonardo with Arduino IDE 1.0.5 r2 and 1.8.2.
  2. More details, please click this link: <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>

****************************************************

*/

#include <EEPROM.h>
#include "Tds_class.h"
float averageVoltage = 0, tdsValue = 0, temperature = 25;
Tds_class Tds;

void setup()
{
  
  Serial.begin(115200);
   //Tds.setPin(TdsCalibraPin,TdsSensorPin,TdsCalibraPin);
  pinMode(TdsCalibraPin,OUTPUT);
  pinMode(TdsComunicaPin,OUTPUT);
  pinMode(TdsSensorPin,INPUT);
 // Tds.setAref(4.94);      // reference voltage on ADC, default 5.0V on Arduino UNO
  //Tds.setAdcRange(1023); // 1024 for 10bit ADC;4096 for 12bit ADC
  Tds.begin();           // initialization
}

void loop()
{
  // temperature = readTemperature();  //add your temperature sensor and read it
  Tds.setTemperature(temperature); // set the temperature and execute temperature compensation
  Tds.update();                    // sample and calculate
  tdsValue = Tds.getTdsValue();    // then get the value
  Serial.print(int (tdsValue));
  Serial.println(" mS/cm");
  Serial.print("Kvalue= ");Serial.println(Tds.getKvalue());
  delay(1000); 
}