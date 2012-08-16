#Intro
Bibliothèque fonctionnelle avec la Arduino Mini, branché en I2C (parce que le SPI est pris par la SD (et que j'ai pas envie d'utiliser 
de bus)). Composant 3.3V, et test effectué sans adapation de niveau.

#Branchement

- Connect Arduino 3.3V to ADXL345 VCC, SDO and CS.
- Connect Arduino GND to ADXL345 GND.
- Connect ADXL345 SDA to Arduino 3.3V using one of your resistors.
- Connect ADXL345 SCL to Arduino 3.3V using one of your resistors.
- Connect ADXL345 SDA to Arduino Analog 4 (I2C Data) on your (Pin 20 on an Arduino Mega).
- Connect ADXL345 SCL to Arduino Analog 5 (I2C Clock) on your (Pin 21 on an Arduino Mega).

