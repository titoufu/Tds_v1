# 1 "C:\\Users\\jribe\\OneDrive\\Documentos\\Arduino\\Tds_v1\\examples\\Tds_v1\\Tds_v1.ino"
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
# 34 "C:\\Users\\jribe\\OneDrive\\Documentos\\Arduino\\Tds_v1\\examples\\Tds_v1\\Tds_v1.ino"
// incluindo as bibliotecas

# 37 "C:\\Users\\jribe\\OneDrive\\Documentos\\Arduino\\Tds_v1\\examples\\Tds_v1\\Tds_v1.ino" 2
# 38 "C:\\Users\\jribe\\OneDrive\\Documentos\\Arduino\\Tds_v1\\examples\\Tds_v1\\Tds_v1.ino" 2


Tds_class Tds;

float temperature = 25, tdsValue = 0;

void setup()
{
  Serial.begin(115200);
  Tds.setPin(A0);
  Tds.setAref(5.0); // reference voltage on ADC, default 5.0V on Arduino UNO
  Tds.setAdcRange(1024); // 1024 for 10bit ADC;4096 for 12bit ADC
  Tds.begin(); // initialization
}

void loop()
{
  // temperature = readTemperature();  //add your temperature sensor and read it
  Tds.setTemperature(temperature); // set the temperature and execute temperature compensation
  Tds.update(); // sample and calculate
  tdsValue = Tds.getTdsValue(); // then get the value
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  delay(1000);
}
