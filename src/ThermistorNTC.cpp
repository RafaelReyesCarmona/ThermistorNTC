/*
ThermistorNTC.cpp - Library to used to derive a precise temperature of a thermistor,
fastest Calc (26~18% faster)
v0.2

Copyright © 2021 Francisco Rafael Reyes Carmona.
All rights reserved.

rafael.reyes.carmona@gmail.com


  This file is part of ThermistorNTC.

  ThermistorNTC is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ThermistorNTC is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ThermistorNTC.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "ThermistorNTC.h"
#include <math.h>

// Constructor para 4 parametros (A,B,C,D).
Thermistor::Thermistor(int PIN,
                       long RESISTOR,
                       long NTC_25C,
                       double A,
                       double B,
                       double C,
                       double D,
                       float VREF){
  _PIN = PIN;
  _RESISTOR = RESISTOR;
  _NTC_25C = NTC_25C;
  _A = A;
  _B = B;
  _C = C;
  _D = D;
  _VREF = VREF;

  pinMode(_PIN, INPUT);
}

// Constructor para 3 parametros (A,B,D.. C = 0).
Thermistor::Thermistor(int PIN,
                       long RESISTOR,
                       long NTC_25C,
                       double A,
                       double B,
                       double D,
                       float VREF){
  _PIN = PIN;
  _RESISTOR = RESISTOR;
  _NTC_25C = NTC_25C;
  _A = A;
  _B = B;
  _D = D;
  _VREF = VREF;

  pinMode(_PIN, INPUT);
}

// Constructor para parametro BETA del termistor.
Thermistor::Thermistor(int PIN,
                       long RESISTOR,
                       long NTC_25C,
                       float BETA,
                       float VREF){
  _PIN = PIN;
  _RESISTOR = RESISTOR;
  _NTC_25C = NTC_25C;
  _BETA = BETA;
  _VREF = VREF;

  pinMode(_PIN, INPUT);
}

// Constructor cuando se desconoce los parámetros del termistor.
Thermistor::Thermistor(int PIN,
                       long RESISTOR,
                       long NTC_1,
                       float TEMP_1,
                       long NTC_2,
                       float TEMP_2,
                       long NTC_3,
                       float TEMP_3,
                       float VREF){
  _PIN = PIN;
  _RESISTOR = RESISTOR;
  _VREF = VREF;

  calcCoefficients(TEMP_1, NTC_1, TEMP_2, NTC_2, TEMP_3, NTC_3);

  pinMode(_PIN, INPUT);
}

void Thermistor::SteinhartHart(Thermistor_connection ConType){
  float E = log(calcNTC(ConType)/(float)_NTC_25C);
  _temp_k = _A + (_B*E) + (_C*(E*E)) + (_D*(E*E*E));
  _temp_k = 1.0 / _temp_k;
  _temp_c = _temp_k - 273.15;
}


void Thermistor::SteinhartHart_beta(Thermistor_connection ConType){
  _temp_k = log(calcNTC(ConType)/(float)_NTC_25C);
  _temp_k /= _BETA;
  _temp_k += 1.0 / 298.15;
  _temp_k = 1.0 / _temp_k;
  _temp_c = _temp_k - 273.15;
}


void Thermistor::SteinhartHart_fast(Thermistor_connection ConType){
  _temp_k = log(calcNTC(ConType)/(float)_NTC_25C);
  _temp_k *= 298.15;
  _temp_k += _BETA;
  _temp_k = (_BETA * 298.15) / _temp_k;
  _temp_c = _temp_k - 273.15;
}


double Thermistor::getTempKelvin(Thermistor_connection ConType) {
  _BETA > 0.0 ? SteinhartHart_beta(ConType) : SteinhartHart(ConType);
  return _temp_k;
}


double Thermistor::getTempCelsius(Thermistor_connection ConType) {
  _BETA > 0.0 ? SteinhartHart_beta(ConType) : SteinhartHart(ConType);
  return _temp_c;
}


double Thermistor::getTempFahrenheit(Thermistor_connection ConType){
  return getTempCelsius(ConType) * 9/5 + 32;
}


double Thermistor::fastTempKelvin(Thermistor_connection ConType) {
  _BETA > 0.0 ? SteinhartHart_fast(ConType) : SteinhartHart(ConType);
  return _temp_k;
}


double Thermistor::fastTempCelsius(Thermistor_connection ConType) {
  _BETA > 0.0 ? SteinhartHart_fast(ConType) : SteinhartHart(ConType);
  return _temp_c;
}


double Thermistor::fastTempFahrenheit(Thermistor_connection ConType){
  return fastTempCelsius(ConType) * 9/5 + 32;
}


float Thermistor::getADC(int numsamples){
  float EMA_LOW;
  int microdelay;

  microdelay = (1 <<((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)));
  microdelay = microdelay * 1000000 / F_CPU;

  EMA_LOW = analogRead(_PIN);

  for (byte i = numsamples; i--; ){
    delayMicroseconds(microdelay);
    EMA_LOW = (_alphaEMA_LOW * (float)analogRead(_PIN)) + ((1.0 - _alphaEMA_LOW) * EMA_LOW);
  }

  return EMA_LOW;
}


double Thermistor::calcNTC(Thermistor_connection ConType){
  double NTC;
  float ADC_VALUE = getADC();
  if (ConType == VCC){
    NTC = (float)_ADC_MAX * (float)_RESISTOR;
    NTC -= ADC_VALUE * (float)_RESISTOR;
    NTC /= ADC_VALUE;
    return NTC;
  }
    NTC = ADC_VALUE * _VREF / (float)_ADC_MAX;
    NTC /= (_VREF - NTC);
    NTC *= (float)_RESISTOR;
    return NTC;
}


void Thermistor::calcCoefficients(float T1, long RT1, float T2, long RT2, float T3, long RT3){
  float L1 = log((float)RT1);
  float L2 = log((float)RT2);
  float L3 = log((float)RT3);
  float Y1 = 1.0f / (T1 + 273.15f);
  float Y2 = 1.0f / (T2 + 273.15f);
  float Y3 = 1.0f / (T3 + 273.15f);
  double yY2 = (Y2 - Y1)/(L2 - L1);
  double yY3 = (Y3 - Y1)/(L3 - L1);

  _D = (yY3 - yY2);
  _D /= (L3 -L2) * (L1 + L2 + L3);
  _B = (L1 * L1) + (L1 * L2) + (L2 *L2);
  _B *= _D;
  _B = yY2 - _B;
  _A = _D * L1 * L1;
  _A += _B;
  _A *= L1;
  _A = Y1 - _A;

  float _T1 = T1 + 273.15f;
  float _T2 = T2 + 273.15f;
  float _T3 = T3 + 273.15f;
  float BETA1 = _T1 * _T2 * (L1 - L2) / (_T2-_T1);
  float BETA2 = _T2 * _T3 * (L2 - L3) / (_T3-_T2);
  _BETA = (BETA1 + BETA2) / 2.0f;

  long NTC_25C1 = RT1 / exp(- _BETA * (_T1 - 298.15f)/ _T1 / 298.15f);
  long NTC_25C2 = RT2 / exp(- _BETA * (_T2 - 298.15f)/ _T2 / 298.15f);
  long NTC_25C3 = RT3 / exp(- _BETA * (_T3 - 298.15f)/ _T3 / 298.15f);
  _NTC_25C = (NTC_25C1 + NTC_25C2 + NTC_25C3) / 3L ;
/*
  _D =((1/_T1-1/_T2)-(log((float)RT1)- log((float)RT2))*(1/_T1-1/_T3)/(log((float)RT1)-log((float)RT3)))/((pow(log((float)RT1),3)-pow(log((float)RT2),3)) - (log((float)RT1)-log((float)RT2))*(pow(log((float)RT1),3)-pow(log((float)RT3),3))/(log((float)RT1)-log((float)RT3)));
  _B =((1/_T1-1/_T2)-_D*pow(log((float)RT1),3)-pow(log((float)RT2),3))/(log((float)RT1)-log((float)RT2));
  _A = 1/_T1-_D*log((float)RT1)*log((float)RT1)*log((float)RT1)-_B*log((float)RT1);
*/
}


void Thermistor::setADC(int ADC_MAX){
  _ADC_MAX = ADC_MAX;
}


void Thermistor::setEMA(float EMA){
  _alphaEMA_LOW = EMA;
}
