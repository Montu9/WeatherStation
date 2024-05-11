#include "SensorsData.h"

const char* ratingToString(Rating rating) {
  switch (rating) {
    case Rating::VeryLow:
      return "Very Low";
    case Rating::Low:
      return "Low";
    case Rating::Ok:
      return "Ok";
    case Rating::High:
      return "High";
    case Rating::VeryHigh:
    default:
      return "Very High";
  }
}