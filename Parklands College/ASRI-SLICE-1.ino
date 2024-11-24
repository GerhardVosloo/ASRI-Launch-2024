#define DATAFILE "/data.csv"
// pins for I2C
#define I2C_SDA 17 //SDA pin
#define I2C_SCL 18 //SCL pin

// expansion port pins for SD Card
#define MMC_CLK 36
#define MMC_CMD 37
#define MMC_DAT0 38
#define MMC_DAT1 34
#define MMC_DAT2 33
#define MMC_DAT3 35

// general libraries
#include <ArduinoJson.h>
#include <Wire.h>

// libraries for SD card
#include "FS.h"
#include "SD_MMC.h"

// libraries for sensors
#include <IGA.h> //air quality
IGA iga;

#include <IIA.h> //accelerometer
IIA iia;

#include <IWB.h> //barometer
IWB iwb;

const char* columnHeadings = "Time (ms), CO2 (ppm), TVOC (ppb), Accel_X (g), Accel_Y (g), Accel_Z (g), Pressure (hPa), Temperature (C)";

void setup() {
  Serial.begin(115200);
  
  // init xChips
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin();

  if (iga.begin()) {
        Serial.println("IGA initialized successfully.");
    } else {
        Serial.println("Failed to initialize IGA");
        exit(0);
    }

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

  //init SD Card
  if (!SD_MMC.setPins(MMC_CLK, MMC_CMD, MMC_DAT0, MMC_DAT1, MMC_DAT2, MMC_DAT3)) {
    Serial.println("Pin change failed!");
    exit(0);
  }

  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed.");
    exit(0);
  }

  // check if data file exists
  if (!SD_MMC.exists(DATAFILE)) {
    // if not, create it and add column headings
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
  char buffer[200];

  // milli seconds in the buffer
  sprintf(buffer, "%ld", millis());

  // add air quality data to buffer
  uint16_t co2ppm, tvocppb;
  if (iga.getData(co2ppm, tvocppb)) {
    sprintf(buffer, "%s,%d,%d", buffer, co2ppm, tvocppb);
  } else {
    Serial.println("Failed to get IGA data");
  }

  // add accelerometer data to buffer
  float x, y, z;
  if (iia.getData(x, y, z)) {
    sprintf(buffer, "%s,%.4f,%.4f,%.4f", buffer, x, y, z);
  } else {
    Serial.println("Failed to get IIA data");
  }
  
  // add barometer data to buffer
  double hPa, celsius;
  if (iwb.getData(hPa, celsius)) {
    sprintf(buffer, "%s,%.4f,%.4f", buffer, hPa, celsius);
  } else {
    Serial.println("Failed to get IWB data");
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
