#include <Arduino.h>
#include "temp.h"

void TempTask(void *pvParameters);
void gpsTask (void *pvParameters);

TEMP Temp;

void setup()
{
  Serial.begin(115200);
  Temp.Setup();
  Serial.print("Main Loop: Executing on core ");
  Serial.println(xPortGetCoreID());
  //Creates a task pinned to the second core to monitor the temps and one pinned to the first core to monitor the display
  xTaskCreatePinnedToCore(TempTask, "Temp Task", 10000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(gpsTask,"GPS Task", 10000, NULL, 1, NULL, 1);
}

void loop()
{
  //Do nothing but handle background ESP things on the main core
}

//Creates the dedicated thread for the temp task
void TempTask(void *pvParameters)
{
    Serial.print("Temp Task: Executing on core ");
    Serial.println(xPortGetCoreID());
    while (true)
    {
      Temp.Loop();
      delay(1);
    }
}
void gpsTask(void *pvParameters)
{
    Serial.print("GPS Task: Executing on core ");
    Serial.println(xPortGetCoreID());
    while (true)
    {
      Temp.gpsLoop();
      delay(1);
    }
}
