#ifndef SOIL_H
#define SOIL_H

class SOIL_sensor {
  public:
    SOIL_sensor(int pin_num);
    float readSOILMoist();

  private:
    int soil_pin;
};

#endif