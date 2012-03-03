#include <DHT22.h>

DHT22 captHT(9);

void setup()
{
 Serial.begin(115200); 
}

void loop()
{
  Serial.print(captHT.getHumidity());
  Serial.print("  ");
  Serial.println(captHT.getTemperatureC());
  delay(200);
}
