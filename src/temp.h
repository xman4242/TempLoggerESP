#ifndef TEMP_h
#define TEMP_h

#include <string>
#include <iostream>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <TFT_eSPI.h>

#define ONE_WIRE_PIN 2
#define CS_PIN 33
#define CLK_PIN 25
#define MOSI_PIN 26
#define MISO_PIN 27
#define SD_CS 15
//CONFIG OPTIONS
#define EEPROM_BOOT_COUNTER_LOCATION 10
//Milliseconds to wait in between temperature readings
#define READ_WAIT_MS 1000

class TEMP
{ 
  public:
    TEMP();
    OneWire onewire;
    DallasTemperature dtemp;
    SPIClass spi;
    File tempFile;
    
    void Setup();
    void Loop();
    void initFile();
    void makeFileName(char* buffer, int value);
    const static int numberOfSensors = 2;
    String fileName = "temps";
    String fileType = ".csv";
    char fileString[30];
    char buffer[300];
    

  private:
      long _NextDisplayMillis = 0;
      long _NextTempMillis = 0;
      double temperatureVal[numberOfSensors];
      int numBoot = 0;
};

#endif