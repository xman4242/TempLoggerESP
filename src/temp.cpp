#include "temp.h"

// Hopefully have the same instances of the classes avalible?????
// Need to figure out how to get this instance in the main file
TEMP::TEMP() : onewire(ONE_WIRE_PIN),
               dtemp(&onewire),
               spi(VSPI)
{
}

void TEMP::Setup()
{
  EEPROM.begin(1);
  numBoot = EEPROM.read(EEPROM_BOOT_COUNTER_LOCATION);
  numBoot++;
  EEPROM.write(EEPROM_BOOT_COUNTER_LOCATION, numBoot);
  EEPROM.commit();
  spi.begin(CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  pinMode(CS_PIN, OUTPUT);
  dtemp.begin();

  Serial.println("Initalizing the SD card...");
  if (!SD.begin(CS_PIN, spi, 80000000))
  {
    Serial.println("SD Init Failed, try again");
  }
  else
  {
    Serial.println("SD Init Success!");
  }
  // Only one file can be open at a time, and make sure we don't do a bad thing
  Serial.println("Generating file header....");
  makeFileName(fileString, numBoot);
  tempFile = SD.open(fileString, FILE_WRITE);
  tempFile.println("Millis Since Start,Probe 0, Probe 1");
  tempFile.println("(ms),(Deg F), (Deg F)");
  Serial.println("Starting the Recording...");
}

void TEMP::Loop()
{
  // Get the temp sensor data
  if (_NextTempMillis < millis())
  {
    _NextTempMillis += READ_WAIT_MS;
    for (int i = 0; i < numberOfSensors; i++)
    {
      temperatureVal[i] = dtemp.getTempFByIndex(i);
    }

    // put it in the csv file
    tempFile.printf("%ld,", _NextTempMillis - READ_WAIT_MS);
    for (int i = 0; i < numberOfSensors; i++)
    {
      tempFile.printf("%lf,", temperatureVal[i]);
    }
    tempFile.printf("\n");
  }
}

void TEMP::makeFileName(char *buffer, int value)
{
  sprintf(buffer, "temperatures%d.csv", value);
}
