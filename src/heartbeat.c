/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include <stddef.h>
#include <stdbool.h>
#include "heartbeat.h"

enum {
  half_period_in_msec = 500,
  pin_5 = (1 << 5)
};

static tiny_timer_t timer;

static void blink(tiny_timer_group_t* group, void* context) {
  (void)context;
  GPIOB->ODR ^= pin_5;
  tiny_timer_start(group, &timer, half_period_in_msec, blink, NULL);
}

void heartbeat_init(tiny_timer_group_t* timer_group) {
  // Configure push pull output
  GPIOB->CR1 |= pin_5;
  GPIOB->DDR |= pin_5;

  tiny_timer_start(timer_group, &timer, half_period_in_msec, blink, NULL);
}
