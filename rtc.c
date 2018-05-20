#include "rtc.h"

#include <stdint.h>

#include "nrf_log_ctrl.h"
#include "app_timer.h"
#include "ble_uart.h"

#define MS 250

static uint32_t rtc_ms_counter = 0;

APP_TIMER_DEF(rtc_id);            

app_timer_mode_t rtc_mode = APP_TIMER_MODE_REPEATED;	        //timer se reinicia cada vez que expira el tiempo

uint32_t rtc_ticks = APP_TIMER_TICKS(MS, APP_TIMER_PRESCALER);

void * rtc_handler_parameters = NULL;

void * p_context = NULL;

//funcion a ejecutar cuando expira el timer
static void rtc_timeout_handler(void * rtc_timeout_handler_pointer)
{
    rtc_ms_counter+=250;
}

void rtc_init()
{	
    app_timer_create(&rtc_id, rtc_mode, rtc_timeout_handler);
    app_timer_start(rtc_id, rtc_ticks, rtc_handler_parameters);
}

void rtc_reset()
{
    app_timer_stop(rtc_id);
    rtc_ms_counter = 0;
    app_timer_start(rtc_id, rtc_ticks, p_context);
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
    app_timer_start(rtc_id, rtc_ticks, p_context);
}