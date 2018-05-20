#include "rtc.h"

#include <stdint.h>

#include "app_util_platform.h"
#include "app_timer.h"
#include "ble_uart.h"

#define MS 250

APP_TIMER_DEF(rtc_id);

uint32_t rtc_ticks = APP_TIMER_TICKS(MS, APP_TIMER_PRESCALER);

static uint32_t rtc_ms_counter = 0;
static volatile uint8_t* m_tick_flag;

/* Funcion a ejecutar cuando expira el timer */
static void rtc_timeout_handler(void * rtc_timeout_handler_pointer)
{
    rtc_ms_counter += MS;
    *m_tick_flag = 1;
}

void rtc_init()
{	
    app_timer_create(&rtc_id, APP_TIMER_MODE_REPEATED, rtc_timeout_handler); /* El timer se reinicia cada vez que expira el tiempo */
    app_timer_start(rtc_id, rtc_ticks, NULL);
}

void rtc_tick_set_flag(volatile uint8_t* main_tick_flag)
{
    m_tick_flag = main_tick_flag;
}

void rtc_reset()
{
    app_timer_stop(rtc_id);
    rtc_ms_counter = 0;
    app_timer_start(rtc_id, rtc_ticks, NULL);
}

uint32_t rtc_get()
{
    uint32_t rtc_aux;
    CRITICAL_REGION_ENTER();
    rtc_aux = rtc_ms_counter;
    CRITICAL_REGION_EXIT();
    return rtc_aux;
}

void rtc_set(uint32_t rtc_counter_new)
{
    app_timer_stop(rtc_id);
    rtc_ms_counter = rtc_counter_new;
    app_timer_start(rtc_id, rtc_ticks, NULL);
}