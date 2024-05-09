#ifndef LDR_H
#define LDR_H

class LDR_sensor {
  public:
    LDR_sensor(int pin_num);
    float readLDRLight();

  private:
    int ldr_pin;
};

#endif