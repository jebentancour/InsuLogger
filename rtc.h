#include <stdint.h>

void rtc_init(void);


void rtc_tick_set_flag(volatile uint8_t* main_tick_flag);


void rtc_reset(void);


uint32_t rtc_get(void);


void rtc_set(uint32_t rtc_counter_new);