#include "ci2c_com.h"

Ci2c_com::Ci2c_com()
{
  // Constructor
}

Ci2c_com::~Ci2c_com()
{
  // Destructor
}

void Ci2c_com::reset() {
  // only relevant for A2B-Master
};

void Ci2c_com::i2c_write(uint8_t i2cAddress, uint8_t i2cRegister, uint8_t data) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(i2cRegister);
  Wire.write(data);
  Wire.endTransmission(true); // with Stopbit
};

int Ci2c_com::i2c_read(uint8_t i2cAddress, uint8_t i2cRegister, uint8_t* data) {
  // transmit the desired register to I2C-counterpart
  Wire.beginTransmission(i2cAddress);
  Wire.write(i2cRegister);
  Wire.endTransmission(true); // with Stopbit

  // read data
  Wire.requestFrom(i2cAddress, 1, true); // with Stopbit
  *data = Wire.read();
  return 0; // no error
};

void Ci2c_com::i2c_write_array(uint8_t i2cAddress, uint8_t i2cRegister, uint8_t* data, uint8_t length) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(i2cRegister);
  for (uint8_t i=0; i<length; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission(true); // with Stopbit
};

int Ci2c_com::i2c_read_array(uint8_t i2cAddress, uint8_t i2cRegister, uint8_t* data, uint8_t length) {
  // transmit the desired register to I2C-counterpart
  Wire.beginTransmission(i2cAddress);
  Wire.write(i2cRegister);
  Wire.endTransmission(true); // with Stopbit

  // read data
  Wire.requestFrom(i2cAddress, length, true); // with Stopbit
  uint8_t pointer = 0;
  while (Wire.available()) {
    data[pointer++] = Wire.read();
  }
  return 0;
};
