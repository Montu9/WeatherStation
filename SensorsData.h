#ifndef SENSORSDATA_H
#define SENSORSDATA_H

enum class Rating { VeryLow, Low, Ok, High, VeryHigh };

struct SoilMoisture {
  float value;
  Rating rating;
};

struct SensorsData {
  float temperature;
  float humidity;
  float pressure;
  float brightness;
  SoilMoisture soilMoisture;
};

const char* ratingToString(Rating rating);

#endif  // SENSORSDATA_H
