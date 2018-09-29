#include "iostm8l152c6.h"
#include "counter.h"
#include "menu.h"
#include "alarm.h"


extern unsigned int vew_engin_oil;
extern unsigned int vew_akpp_oil;
extern unsigned int vew_oil_reducer;
extern unsigned int vew_fuel_filter;
extern unsigned int vew_air_filter;
extern unsigned int vew_oil_filter;
extern unsigned int vew_cleaning_tnvd;
extern unsigned int vew_changing_the_injector;
extern unsigned int vew_candles_of_chenge;

void led_alarm (void)
{
  if (vew_engin_oil              ==0
     ||vew_akpp_oil              ==0
     ||vew_oil_reducer           ==0
     ||vew_fuel_filter           ==0
     ||vew_air_filter            ==0
     ||vew_oil_filter            ==0
     ||vew_cleaning_tnvd         ==0
     ||vew_changing_the_injector ==0
     ||vew_candles_of_chenge     ==0)
    {
      PB_ODR_bit.ODR0^=1;
    }
}