#ifndef LDR_H
#define LDR_H

class LDR_sensor {  
  public:
    LDR_sensor(int pin_num);
    int readLDRLight();

  private:
    int ldr_pin;
};

#endif