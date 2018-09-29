#include "iostm8l152c6.h"
#include "menu.h"
#include "lcd.h"
#include "valcoder.h"
#include "alarm.h"
#include "gidro_trans.h"
#include "speed.h"
#include "circle.h"

int d=0;                                //Переменная скорости                                        
unsigned long int odometr=0;                  //Переменная одометра
int NewState,OldState,upState,downState;
volatile int rattling_PA_4=0;           //Антидребезг кнопки "Валкодер" menu.c
volatile int rattling_PA_5=0;           //Антидребезг кнопки "Скорость" speed.c
volatile int rattling_PA_6=0;           //Антидребезг кнопки "Гидротрансформатор" gidro_trans.c
volatile int rattling_PA_SAVE_FM=0;       //Антидребезг кнопки "Save" fm.c

volatile int timer=0;
volatile int timer_aux=0;
extern unsigned char arh;
extern unsigned char arl;
extern unsigned int ddll;

extern unsigned int vew_engin_oil;
extern unsigned int vew_akpp_oil;
extern unsigned int vew_oil_reducer;
extern unsigned int vew_fuel_filter;
extern unsigned int vew_air_filter;
extern unsigned int vew_oil_filter;
extern unsigned int vew_cleaning_tnvd;
extern unsigned int vew_changing_the_injector;
extern unsigned int vew_candles_of_chenge;

//Прерывание по переполнению TIM1
#pragma vector = TIM1_OVR_UIF_vector 
    __interrupt void TIM1_UIF(void)
{
  static unsigned int cnt_start=0;
  static unsigned int cnt_end=0;
  cnt_end=(TIM3_CNTRH<<8)|TIM3_CNTRL;
  if(cnt_end<cnt_start)
  {
    d=ddll-cnt_start+cnt_end; //(TIM3_ARRH<<8)|TIM3_ARRL=(1595/100)*16
  }
  else
  {
    d=(cnt_end-cnt_start);
  }
  /*Если меняем скорость обновления показаний спидометра, например в два раза чаще
  тогда тут умножаем скорость в 2 раза. Если оновление показаний изменяем в 4 раза,
  соответсвенно тут скорость умножаем в 4 раза d=d*2;*/
  cnt_start=cnt_end;
  TIM1_SR1_bit.UIF = 0;
};

//Прерывание по переполнению TIM2
#pragma vector = TIM2_OVR_UIF_vector //Прерывание по переполнению TIM4
  __interrupt void TIM2_UIF(void)
  {
  timer_aux++;
  TIM2_SR1_bit.UIF = 0;  //При выходе из прерывания обязательно обнулить данный бит, иначе будем болтаться в прерывании.  
  };
    
    

/*
Таймер считает импульсы с датчика оборотов 1250(количество импульсов)*0.8 
(метра за 1 тактовый имульс) Мы стремимся к целому значению. В этом коде 1250 это 
1000 метров пройденного пути на одометре
*/
#pragma vector = TIM3_OVR_UIF_vector 
    __interrupt void TIM3_UIF(void)
{
  odometr++;
  if(vew_engin_oil              != 0)   vew_engin_oil--;
  if(vew_akpp_oil               != 0)   vew_akpp_oil--;
  if(vew_oil_reducer            != 0)   vew_oil_reducer--;
  if(vew_fuel_filter            != 0)   vew_fuel_filter--;
  if(vew_air_filter             != 0)   vew_air_filter--;
  if(vew_oil_filter             != 0)   vew_oil_filter--;
  if(vew_cleaning_tnvd          != 0)   vew_cleaning_tnvd--;
  if(vew_changing_the_injector  != 0)   vew_changing_the_injector--;
  if(vew_candles_of_chenge      != 0)   vew_candles_of_chenge--;
  TIM3_SR1_bit.UIF = 0;
};

//------------------------------------------------------------------------------
//Использую для функций дребезга кнопок и для работы валкодера *****************
//------------------------------------------------------------------------------
#pragma vector = TIM4_UIF_vector //Прерывание по переполнению TIM4
  __interrupt void TIM4_UIF(void)
  {
  rattling_PA_5++;
  rattling_PA_6++;
  rattling_PA_4++;
  rattling_PA_SAVE_FM++;
  timer++;
  
  NewState=PA_IDR & 0x0C;
    if(NewState!=OldState)
    {
      switch(OldState)
      {
      case 2:
      {
      if(NewState == 0) upState++;
      if(NewState == 12) downState++; 
      break;
      }
      case 0:
      {
      if(NewState == 4) upState++;
      if(NewState == 8) downState++; 
      break;
      }
      case 1:
      {
      if(NewState == 12) upState++;
      if(NewState == 0) downState++; 
      break;
      }
      case 3:
      {
      if(NewState == 8) upState++;
      if(NewState == 4) downState++; 
      break;
      }
      }            
  OldState=NewState;
  }
TIM4_SR1_bit.UIF = 0;  //При выходе из прерывания обязательно обнулить данный бит, иначе будем болтаться в прерывании.
};

void set_tim_1 (void)
{
  CLK_PCKENR2 = 0x02;  //Подаём тактирование на TIM1
  TIM1_PSCRH  = 0x00;  //Предделитель = 144 погрешность скорости =0. =72 погрешность скорости +-2км (перенос)
  TIM1_PSCRL  = 0x90;  //=36 погрешность скорости +-4км
  TIM1_ARRH   = 0x9C;  //Считаем от 0 до 40000
  TIM1_ARRL   = 0x40;
  TIM1_CR1    = 0x01;  //Активируем таймер
  TIM1_IER    = 0x01;  //Разрешаем прерывание по переполнению
}

void set_tim_2 (void)
{
  CLK_PCKENR1 = 0x07;  //Подаём тактирование на TIM2
  TIM2_PSCR   = 0x33;  //Предделитель = 7A
  TIM2_CR1    = 0x01;  //Активируем таймер
  TIM2_IER    = 0x01;  //Разрешаем прерывание по переполнению
}

void set_tim_3 (void)
{
  CLK_PCKENR1 = 0x07;   //Подаём тактирование на TIM3
  TIM3_ARRH   = arh;     //0x04;  //Считаем до 1250(04E2). При условии, что 1 такт =0.8 (перенос)
  TIM3_ARRL   = arl;     //0xE2;  //метра получаем 1250*0.8=1000 метров (1км)
  TIM3_PSCR   = 0x00;   //Предделитель = 0
  TIM3_CR1    = 0x01;   //Активируем таймер
  TIM3_IER    = 0x01;   //Разрешаем прерывание по переполнению
  TIM3_ETR_bit.ECE = 1;
}

void set_tim_4 (void)
{
  CLK_PCKENR1 = 0x07;  //Подаём тактирование на TIM4
  TIM4_PSCR   = 0x33;  //Предделитель = 7A
  TIM4_CR1    = 0x01;  //Активируем таймер
  TIM4_IER    = 0x01;  //Разрешаем прерывание по переполнению
}

