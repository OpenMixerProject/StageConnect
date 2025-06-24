#pragma once

#include <stdio.h>
#include <Arduino.h>
#include "ci2c_com.h"

void serial_printf(HardwareSerial& serial, const char* fmt, ...);

#define A2B_WAIT_FUNCTION(ms)     delay(ms);
// #define A2B_WAIT_FUNCTION(ms)     wait(ms);
#define A2B_DEBUG_A2B(...)        serial_printf(Serial,__VA_ARGS__);
#define A2B_DEBUG_SC(...)         serial_printf(Serial,__VA_ARGS__);

// #define A2B_MASTER_TXBCLK_RISING      // clk edge to data out is 8ns (might be too close to fpga latching edge)
