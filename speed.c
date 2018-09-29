#include "iostm8l152c6.h"
#include "counter.h"
#include "menu.h"
#include "speed.h"
#include "set_pin.h"

extern unsigned int vew_set_speed_city;
extern unsigned int vew_set_speed_trass;
extern int rattling_PA_5;
extern int timer;
extern int d;
int status=0;
int selection=0;
int cnt=0;
void delay_audio(void)               //Функция задержки
{
  for (cnt=0; cnt<30000; cnt++);
}

enum KEY_SPEED read_button_speed (void)
{
  static enum KEY_SPEED key3_state_pred=BT3_NO_PRESSED;
  static enum KEY_SPEED key3_state_now=BT3_NO_PRESSED; 
  if(rattling_PA_5 > KEY_TIME_READ_SPEED)
  {
    if(PA_IDR_bit.IDR5)
      {
        key3_state_now=BT3_NO_PRESSED;           //не нажата
      }
    else
      {
        key3_state_now=BT3_PRESSED;             //нажата
      }
    if(key3_state_now==key3_state_pred)         //проверка 1
      {
      if(key3_state_now==BT3_NO_PRESSED)         //если состояние одинаковы
        {
          rattling_PA_5 =0;
          key3_state_pred=key3_state_now;       //пред и нынеш - не нажаты
          return BT3_NO_PRESSED;                 //нет нажатия 
        }
      else
        {
        if(rattling_PA_5 > KEY_TIME_HOLDED_SPEED)            //пред и нынеш - нажаты
          {
            rattling_PA_5 =0;
            key3_state_pred=key3_state_now;
            return BT3_HOLDED;                  //удержание
          }
        }
      }
    else
      {
      if(key3_state_now==BT3_NO_PRESSED)         //когда не одинаковы
        {
          rattling_PA_5 =0;                           //key1_state_pred==BT1_PRESSED
          key3_state_pred=key3_state_now;       //key1_state_now = BT_NO_PRESSED
          return BT3_BACK;                      //отжатие
        }
      else
        {
          rattling_PA_5=0;                           //key1_state_pred==BT_NO_PRESSED
          key3_state_pred=key3_state_now;       //key1_state_now=BT1_PRESSED
          return BT3_PRESSED;
        }
      }
  }
return BT3_UNDETERM;
}

void speed_button (void)
{
  if (read_button_speed ()==KEY_CODE_NAJATA_SPEED)
  {
    selection^=1;
  }
    if(selection==0)
    {
      PD_ODR_bit.ODR2=1;
      PD_ODR_bit.ODR0=0;
    }
    if(selection==1)
    {
      PD_ODR_bit.ODR2=0;      
      PD_ODR_bit.ODR0=1;
    }
}

void speed_city (void)
{ 
  if(d >= vew_set_speed_city)
  {
    PD_ODR_bit.ODR3^=1;
      if(status==0)
      {
        cpu_aux();
        PB_ODR_bit.ODR1=1;
        PB_ODR_bit.ODR5=1;
        PB_ODR_bit.ODR6=0;
        PB_ODR_bit.ODR7=1;
        status=1;
      }
  }
  else
  {
    status=0;
    PD_ODR_bit.ODR3=0;
  }
}


void speed_trass (void)
{
  if(d >= vew_set_speed_trass)
  {
    PD_ODR_bit.ODR3^=1;
      if(status==0)
      {
        cpu_aux();
        PB_ODR_bit.ODR1=1;
        PB_ODR_bit.ODR5=1;
        PB_ODR_bit.ODR6=0;
        PB_ODR_bit.ODR7=1;
        status=1;
      }
  }
  else
  {
    status=0;
    PD_ODR_bit.ODR3=0;
  }
}
