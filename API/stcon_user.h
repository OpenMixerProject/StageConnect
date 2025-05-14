#pragma once

#include "types.h"
#include "io.h"
#include "utils.h"
#include "config.h"

#define A2B_WAIT_FUNCTION(ms)     os_thread_sleep(ms);
// #define A2B_WAIT_FUNCTION(ms)     wait(ms);
#define A2B_DEBUG_A2B(...)        DBG(DBG_A2B, __VA_ARGS__);
#define A2B_DEBUG_SC(...)         DBG(DBG_SC, __VA_ARGS__);

// #define A2B_MASTER_TXBCLK_RISING      // clk edge to data out is 8ns (might be too close to fpga latching edge)

