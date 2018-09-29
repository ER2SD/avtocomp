#include "iostm8l152c6.h"
#include "counter.h"
#include "menu.h"
#include "gidro_trans.h"
#include "speed.h"
#include "set_pin.h"

extern int rattling_PA_6;
extern int d;
extern enum STATE_MENU state_menu;
extern int timer;
extern char buf1[16];
extern char buf2[16];
extern char* stroka_one;
extern char* stroka_two;

int flag=0;

enum KEY_TRANS read_button (void)
{
  static enum KEY_TRANS key2_state_pred=BT2_NO_PRESSED;
  static enum KEY_TRANS key2_state_now=BT2_NO_PRESSED; 
  if(rattling_PA_6 > KEY_TIME_READ_TRANS)
  {
    if(PA_IDR_bit.IDR6)
      {
        key2_state_now=BT2_NO_PRESSED;           //не нажата
      }
    else
      {
        key2_state_now=BT2_PRESSED;             //нажата
      }
    if(key2_state_now==key2_state_pred)         //проверка 1
      {
      if(key2_state_now==BT2_NO_PRESSED)         //если состояние одинаковы
        {
          rattling_PA_6 =0;
          key2_state_pred=key2_state_now;       //пред и нынеш - не нажаты
          return BT2_NO_PRESSED;                 //нет нажатия 
        }
      else
        {
        if(rattling_PA_6 > KEY_TIME_HOLDED_TRANS)            //пред и нынеш - нажаты
          {
            rattling_PA_6 =0;
            key2_state_pred=key2_state_now;
            return BT2_HOLDED;                  //удержание
          }
        }
      }
    else
      {
      if(key2_state_now==BT2_NO_PRESSED)         //когда не одинаковы
        {
          rattling_PA_6 =0;                           //key1_state_pred==BT1_PRESSED
          key2_state_pred=key2_state_now;       //key1_state_now = BT_NO_PRESSED
          return BT2_BACK;                      //отжатие
        }
      else
        {
          rattling_PA_6=0;                           //key1_state_pred==BT_NO_PRESSED
          key2_state_pred=key2_state_now;       //key1_state_now=BT1_PRESSED
          return BT2_PRESSED;
        }
      }
  }
return BT2_UNDETERM;
}

void gidrotransformator_on (void)
{
  if(read_button ()==KEY_CODE_NAJATA2 & d>=30)
    {
    timer=0;
    state_menu=GIDROTRANSFORMATOR;
    cpu_aux();
    PB_ODR_bit.ODR1=1;
    PB_ODR_bit.ODR5=1;
    PB_ODR_bit.ODR6=0;
    PB_ODR_bit.ODR7=0;
    }
}

void gidrotransformator_off (void)
{
  //if(d>30)flag=1;
  if(d<30 & PB_ODR_bit.ODR0==1)
  {
    cpu_aux();
    PB_ODR_bit.ODR0=0;
    PB_ODR_bit.ODR1=1;
    PB_ODR_bit.ODR5=0;
    PB_ODR_bit.ODR6=1;
    PB_ODR_bit.ODR7=1;
    flag=0;
  }
  if(PB_IDR_bit.IDR3==0 & PB_ODR_bit.ODR0==1)
    {
     cpu_aux();
     PB_ODR_bit.ODR1=1;
     PB_ODR_bit.ODR5=0;
     PB_ODR_bit.ODR6=1;
     PB_ODR_bit.ODR7=1;
    }

}