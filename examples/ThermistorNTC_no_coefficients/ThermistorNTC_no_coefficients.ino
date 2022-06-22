/*
ThermistorNTC_no_coefficients.ino - Library to used to derive a precise temperature of a thermistor,
fastest Calc (26~18% faster)
v0.3.2

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

 Thermistor thermistor1(/* PIN */       A1,
                       /* RESISTOR */  22170L,
                       /* NTC 25ºC */  91869L,
                       /* BETA */      4403.45,
                       /* Vref */      5.072);

 Thermistor thermistor2(/* PIN */      A2,
                       /* RESISTOR */  22170L,
                       /* NTC_T1 */    355000L,
                       /* T1 (ºC) */   0.0,  // 273,15 ºK
                       /* NTC_T2 */    157500L,
                       /* T2 (ºC) */   14.0, // 287,15 ºK
                       /* NTC_T3 */    79300L,
                       /* T3 (ºC) */   28.0, // 301,15 ºK
                       /* NTC_T4 */    58300L,
                       /* T4 (ºC) */   35.0, // 308,15 ºK
                       /* Vref */      5.072);

 void setup(void)
 {
   Serial.begin(57600);
 }


 void loop(void)
 {
   double sensor1 = thermistor1.fastTempCelsius();
   Serial.print("Sensor1 calc. Temp(ºC): ");
   Serial.println(sensor1);

   double sensor2 = thermistor2.getTempCelsius_SteinHart();
   Serial.print("Sensor2 calc. SteinHart Temp(ºC): ");
   Serial.println(sensor2);

   sensor2 = thermistor2.getTempCelsius();
   Serial.print("Sensor2 calc. Beta equ. Temp(ºC): ");
   Serial.println(sensor2);

   sensor2 = thermistor2.fastTempCelsius();
   Serial.print("Sensor2 calc. Fast equ. Temp(ºC): ");
   Serial.println(sensor2);
   Serial.println();
   delay(1000);  // Comment for serial plotter.
 }
