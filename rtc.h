#include <stdint.h>

void rtc_init(void);


void rtc_reset(void);


uint32_t rtc_get(void);


void rtc_set(uint32_t rtc_counter_new);