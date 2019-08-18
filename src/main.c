/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <stddef.h>
#include "stm8s.h"
#include "clock.h"
#include "tim4_system_tick.h"
#include "pb5_heartbeat.h"
#include "tiny_timer.h"
#include "watchdog.h"
#include "uart1.h"
#include "spi.h"
#include "tcs34725.h"
#include "i2c.h"

static tiny_timer_group_t timer_group;
static tiny_timer_t timer;
static i_tiny_uart_t* uart;
static i_tiny_spi_t* spi;
static i_tiny_i2c_t* i2c;
static tcs34725_t tcs34725;

static void kick_watchdog(tiny_timer_group_t* _timer_group, void* context) {
  (void)context;
  (void)_timer_group;
  watchdog_kick();
  tiny_timer_start(&timer_group, &timer, 1, kick_watchdog, NULL);
}

void main(void) {
  disableInterrupts();
  {
    watchdog_init();
    clock_init();
    tiny_timer_group_init(&timer_group, tim4_system_tick_init());
    uart = uart1_init();
    // pb5_heartbeat_init(&timer_group);
    spi = spi_init();
    i2c = i2c_init();
  }
  enableInterrupts();

  tcs34725_init(&tcs34725, &timer_group, i2c);

  tiny_timer_start(&timer_group, &timer, 1, kick_watchdog, NULL);

  while(true) {
    tiny_timer_group_run(&timer_group);
    wfi();
  }
}
