#ifndef CI2C_COM_H
#define CI2C_COM_H

#include <Wire.h>

class Ci2c_com {
  public:
    // constructor
    Ci2c_com();

    // destructor
    virtual ~Ci2c_com();

    // communication-functions
    void reset();

    void i2c_write(uint8_t i2cAddress, uint8_t i2cRegister, uint8_t data);

    int i2c_read(uint8_t i2cAddress, uint8_t i2cRegister, uint8_t* data);

    void i2c_write_array(uint8_t i2cAddress, uint8_t i2cRegister, uint8_t* data, uint8_t length);

    int i2c_read_array(uint8_t i2cAddress, uint8_t i2cRegister, uint8_t* data, uint8_t length);
  private:
};

#endif // CI2C_COM_H