#include <Arduino.h>
#include "temp.h"

void TempTask(void *pvParameters);
TEMP Temp;

void setup()
{
  Serial.begin(115200);
  Temp.Setup();
  Serial.print("Main Loop: Executing on core ");
  Serial.println(xPortGetCoreID());
  //Creates a task pinned to the second core to monitor the temps
  xTaskCreatePinnedToCore(TempTask, "Temp Task", 10000, NULL, 1, NULL, 1);
}

void loop()
{
  //Do nothing but handle background ESP things on the main core
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
