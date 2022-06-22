#include <Arduino.h>
#include "temp.h"
//what kind of tasks do I want to haave?

void TempTask(void *pvParameters);
TEMP Temp;

void setup()
{
  Serial.begin(115200);
  //Other setup garbarge will go here, like temp task etc. 
  Temp.Setup();
  Serial.print("Main Loop: Executing on core ");
  Serial.println(xPortGetCoreID());
  xTaskCreatePinnedToCore(TempTask, "Temp Task", 10000, NULL, 1, NULL, 1);
}

void loop()
{
  //Temp Loop goes here??
  delay(1);
  yield();
}

//Creates the dedicated thread for the temp task
void TempTask(void *pvParameters)
{
    Serial.print("Temp Task: Executing on core ");
    Serial.println(xPortGetCoreID());
    while (true)
    {
      Temp.Loop();
    }
}
