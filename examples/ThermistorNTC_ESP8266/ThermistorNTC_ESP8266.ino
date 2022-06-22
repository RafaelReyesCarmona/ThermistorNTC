/*
ThermistorNTC_ESP8266.ino - Example file for Library to used to derive a precise temperature of a thermistor,
fastest Calc (26~18% faster)
v0.3.1

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

/**
 * Example sketch for the ThermistorNTC library.
 */

#include <ThermistorNTC.h>

Thermistor thermistor0(/* PIN */       A0,
                      /* RESISTOR */  21900L,
                      /* NTC 25ºC */  9950L,
                      /* A */         3354016e-9,
                      /* B */         2569850e-10,
                      /* C */         2620131e-12,
                      /* D */         6383091e-14,
                      /* Vref */      3.30);

//****************************************************
// Example with beta parameter.
//
//Thermistor thermistor0(/* PIN */       A0,
//                      /* RESISTOR */  21900L,
//                      /* NTC 25ºC */  9950L,
//                      /* BETA */      4190.0,
//                      /* Vref */      3.30);
//****************************************************

void setup(void)
{
  Serial.begin(57600);
}


void loop(void)
{
  double sensor0 = thermistor0.getTempCelsius();
  Serial.print("Sensor0 calc. Temp(ºC): ");
  Serial.println(sensor0);

  double sensor0_fast = thermistor0.fastTempCelsius();
  Serial.print("Sensor0_fast calc. Temp(ºC): ");
  Serial.println(sensor0_fast);

  delay(1000);
}
