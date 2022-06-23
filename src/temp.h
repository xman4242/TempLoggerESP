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

//Bus Pins
#define ONE_WIRE_PIN 2
#define CS_PIN 33
#define CLK_PIN 25
#define MOSI_PIN 26
#define MISO_PIN 27
//#define SD_CS 15
//CONFIG OPTIONS
#define EEPROM_BOOT_COUNTER_LOCATION 10
//Milliseconds to wait in between temperature readings
#define READ_WAIT_MS 1000
#define MS_PER_TEMP 250

class TEMP
{ 
  public:
    TEMP();
    OneWire onewire;
    DallasTemperature dtemp;
    SPIClass spi;
    File tempFile;
    TFT_eSPI screen;
    
    void Setup();
    void Loop();
    void initFile();
    void makeFileName(char* buffer, int value);
    void endLogging(int flashMS, int numFlash);
    void displayLoop();
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
      bool buttonState = true;
      bool prevButtonState = false;
      bool offButtonState = true;
      bool prevOffButtonState = false;
      bool isRunning = false;
    //This is the thermometer image, is the whole display at 135x240
    const unsigned char img [32400] PROGMEM = 
    {
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x06, 0x00, 0x00, 0x3f, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x1f, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x0f, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0x80, 
	    0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xff, 
	    0xc0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 
	    0xff, 0xe0, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xf0, 0x01, 0xff, 
	    0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xf0, 0x03, 
	    0xff, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xe0, 
	    0x07, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0xe0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0xff, 0xff, 
	    0xff, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 
	    0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xfe, 
	    0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x03, 
	    0xfe, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xf0, 0x00, 0x00, 0x00, 
	    0x03, 0xfe, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xf0, 0x00, 0x00, 
	    0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xf8, 0x00, 
	    0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xf8, 
	    0x00, 0x00, 0x00, 0x0f, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xf9, 0xff, 0xff, 0xc0, 0x1f, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xe0, 0x7f, 0xff, 0xe0, 
	    0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x7f, 0xff, 
	    0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 
	    0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 
	    0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 
	    0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 
	    0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 
	    0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 
	    0x1f, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 
	    0xe0, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 
	    0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 
	    0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 
	    0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 
	    0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 
	    0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 
	    0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 
	    0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 
	    0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 
	    0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 
	    0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 
	    0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 
	    0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x0f, 
	    0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 
	    0x07, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 
	    0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 
	    0x00, 0x00, 0x03, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 
	    0x00, 0x00, 0x00, 0x03, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 
	    0xf0, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 
	    0x1f, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 
	    0xe0, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 
	    0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 
	    0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 
	    0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 
	    0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 
	    0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 
	    0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 
	    0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 
	    0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 
	    0x3f, 0xff, 0xe0, 0x1f, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 
	    0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 
	    0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 
	    0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xfe, 0x00, 0x00, 0x3f, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 
	    0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 
	    0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 
	    0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 
	    0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 
	    0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 
	    0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 
	    0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0x00, 0x00, 
	    0x00, 0x3f, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf8, 0x00, 
	    0x00, 0x00, 0x0f, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xf0, 
	    0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 
	    0xf0, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 
	    0x1f, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 
	    0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 
	    0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 
	    0x3f, 0xff, 0xe0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 
	    0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 
	    0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 
	    0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x07, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x03, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xe0, 0x01, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xf0, 
	    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xc0, 0x3f, 0xff, 
	    0xf0, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x3f, 
	    0xff, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0xc0, 
	    0x3f, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0x00, 0x0f, 0xff, 0xff, 
	    0xc0, 0x3f, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0x00, 0x1f, 0xff, 
	    0xff, 0xc0, 0x3f, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfe, 0x00, 0x3f, 
	    0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfc, 0x00, 
	    0x7f, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfc, 
	    0x00, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xf8, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xfe, 0x00, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 
	    0x7f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x1f, 0xff, 0xff, 
	    0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xc0, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 
	    0xff, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xc0, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x07, 
	    0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x80, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	    0x03, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x80, 0x3f, 0xff, 0xf8, 0x00, 0x00, 
	    0x00, 0x01, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0x00, 
	    0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x00, 0x7f, 0xff, 0xe0, 
	    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x00, 0x7f, 0xff, 
	    0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x00, 0x7f, 
	    0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x00, 
	    0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
	    0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x0f, 0xff, 0xff, 
	    0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x0f, 0xff, 
	    0xff, 0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x0f, 
	    0xff, 0xff, 0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 
	    0x0f, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	    0xf0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 
	    0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
	    0x1f, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 
	    0x00, 0x1f, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 
	    0x00, 0x00, 0x1f, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0x00, 0xff, 0xff, 0x80, 0x00, 
	    0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x00, 0xff, 0xff, 0x80, 
	    0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x00, 0xff, 0xff, 
	    0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x00, 0xff, 
	    0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x00, 
	    0x7f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0x80, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x1f, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0x80, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x3f, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0x80, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x80, 
	    0x3f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xc0, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 
	    0x80, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	    0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x3f, 
	    0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xe0, 0x00, 0x00, 
	    0xff, 0xff, 0xfe, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xf8, 0x00, 
	    0x03, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 
	    0x00, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xf0, 0x03, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xf8, 0x01, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfc, 0x00, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfc, 
	    0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xfe, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xc0, 
	    0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xf8, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	    0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 
	    0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 
	    0xff, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x80, 0x00, 0x3f, 
	    0xff, 0xff, 0x80, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xc0, 0x00, 
	    0x01, 0xff, 0xf8, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xe0, 
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe
    };

};

#endif