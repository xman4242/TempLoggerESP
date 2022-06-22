#ifndef TEMP_h
#define TEMP_h

#include <string>
#include <iostream>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>

#define ONE_WIRE_PIN 2
#define CS_PIN 33
#define CLK_PIN 25
#define MOSI_PIN 26
#define MISO_PIN 27
#define SD_CS 15

class TEMP
{ 
  public:
    TEMP();
    OneWire onewire;
    DallasTemperature dtemp;
    File tempFile;
    
    void Setup();
    void Loop();
    void makeFileName(char* buffer, int value);
    const static int numberOfSensors = 2;
    String fileName = "temps";
    String fileType = ".csv";
    char fileString[30];

  private:
      long _NextDisplayMillis = 0;
      long _NextTempMillis = 0;
      double tempval[numberOfSensors];
      int numBoot = 0;
};

#endif