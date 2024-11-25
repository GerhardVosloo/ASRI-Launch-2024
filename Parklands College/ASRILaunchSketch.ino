#define DATAFILE "/data.csv"
// pins for I2C
#define I2C_SDA 17//SDA
#define I2C_SCL 18//SCL


// expansion port pins for SD Card
#define MMC_CLK 36
#define MMC_CMD 37
#define MMC_DAT0 38
#define MMC_DAT1 34
#define MMC_DAT2 33
#define MMC_DAT3 35

//general 
#include <Arduino.h>
#include <Wire.h>

#include "FS.h"
#include "SD_MMC.h"

#include <IWB.h> //barometer
IWB iwb;

#include <IIA.h> //accelerometer
IIA iia;

#include <IWA.h> //weather
IWA iwa;

const char* columnHeadings = "Time (ms), Accel_X (g), Accel_Y (g), Accel_Z (g), Pressure (hPa), Temperature (C),  Temperature (C), Humidity(RH/n)" ;


void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);

  // init xChips
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin();


  if (iia.begin()) {
        Serial.println("IIA initialized successfully.");
    } else {
        Serial.println("Failed to initialize IIA");
        exit(0);
    }


  if (iwb.begin()) {
        Serial.println("IWB initialized successfully.");
    } else {
        Serial.println("Failed to initialize IWB");
        exit(0);
    }

    if (iwa.begin()) {
        Serial.println("IWA initialized successfully.");
    } else {
        Serial.println("Failed to initialize IWA");
        exit(0);
    }

     //init SD Card
  if (!SD_MMC.setPins(MMC_CLK, MMC_CMD, MMC_DAT0, MMC_DAT1, MMC_DAT2, MMC_DAT3)) {
    Serial.println("Pin change failed!");
    exit(0);
  }

  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed.");
    exit(0);
  }

  //check for data file
   if (!SD_MMC.exists(DATAFILE)) {
    // else create it and add column headings
    File file = SD_MMC.open(DATAFILE, FILE_APPEND);
    if (file) {
      file.println(columnHeadings);
      file.close();
    } else {
      Serial.println("Failed to create the file.");
      exit(0);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 char buffer[200];

  // milli seconds in the buffer
  sprintf(buffer, "%ld", millis());



  // adding accelerometer data to buffer
  float x, y, z;
  if (iia.getData(x, y, z)) {
    sprintf(buffer, "%s,%.4f,%.4f,%.4f", buffer, x, y, z);
  } else {
    Serial.println("Failed to get IIA data");
  }
  
  // adding barometer data to buffer
  double hPa, celsius;
  if (iwb.getData(hPa, celsius)) {
    sprintf(buffer, "%s,%.4f,%.4f", buffer, hPa, celsius);
  } else {
    Serial.println("Failed to get IWB data");
  }

 // adding weather sensor data to buffer
  float RH, temperature;
  delay (10) ;
  if (iwa.getData(RH, temperature)) {
    sprintf(buffer, "%s,%.4f,%.4f", buffer, RH, temperature);
  } else {
    Serial.println("Failed to get IWA data");
  }

    // Write data to SD Card:
  // Open file...
  File file = SD_MMC.open(DATAFILE, FILE_APPEND);  // Open file with filename defined at the top. Open for appending, meaning it creates a file if it doesn't exist and add data to the end of the file
  if (!file) {
    Serial.println("Failed to open file for appending");
    exit(0);
  }
  // Write to file...
  if (!file.println(buffer)) {
    Serial.println("Append failed");
    exit(0);
  }
  // Close file...
  file.close();

  // Print the data to the Serial Port
 Serial.println(buffer);
}



