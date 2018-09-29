#include "iostm8l152c6.h"
#include "menu.h"
#include "lcd.h"
#include "valcoder.h"
#include "alarm.h"
#include "gidro_trans.h"
#include "speed.h"
#include "circle.h"

int d=0;                                //���������� ��������                                        
unsigned long int odometr=0;                  //���������� ��������
int NewState,OldState,upState,downState;
volatile int rattling_PA_4=0;           //����������� ������ "��������" menu.c
volatile int rattling_PA_5=0;           //����������� ������ "��������" speed.c
volatile int rattling_PA_6=0;           //����������� ������ "������������������" gidro_trans.c
volatile int rattling_PA_SAVE_FM=0;       //����������� ������ "Save" fm.c

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

//���������� �� ������������ TIM1
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
  /*���� ������ �������� ���������� ��������� ����������, �������� � ��� ���� ����
  ����� ��� �������� �������� � 2 ����. ���� ��������� ��������� �������� � 4 ����,
  ������������� ��� �������� �������� � 4 ���� d=d*2;*/
  cnt_start=cnt_end;
  TIM1_SR1_bit.UIF = 0;
};

//���������� �� ������������ TIM2
#pragma vector = TIM2_OVR_UIF_vector //���������� �� ������������ TIM4
  __interrupt void TIM2_UIF(void)
  {
  timer_aux++;
  TIM2_SR1_bit.UIF = 0;  //��� ������ �� ���������� ����������� �������� ������ ���, ����� ����� ��������� � ����������.  
  };
    
    

/*
������ ������� �������� � ������� �������� 1250(���������� ���������)*0.8 
(����� �� 1 �������� ������) �� ��������� � ������ ��������. � ���� ���� 1250 ��� 
1000 ������ ����������� ���� �� ��������
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
//��������� ��� ������� �������� ������ � ��� ������ ��������� *****************
//------------------------------------------------------------------------------
#pragma vector = TIM4_UIF_vector //���������� �� ������������ TIM4
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
TIM4_SR1_bit.UIF = 0;  //��� ������ �� ���������� ����������� �������� ������ ���, ����� ����� ��������� � ����������.
};

void set_tim_1 (void)
{
  CLK_PCKENR2 = 0x02;  //����� ������������ �� TIM1
  TIM1_PSCRH  = 0x00;  //������������ = 144 ����������� �������� =0. =72 ����������� �������� +-2�� (�������)
  TIM1_PSCRL  = 0x90;  //=36 ����������� �������� +-4��
  TIM1_ARRH   = 0x9C;  //������� �� 0 �� 40000
  TIM1_ARRL   = 0x40;
  TIM1_CR1    = 0x01;  //���������� ������
  TIM1_IER    = 0x01;  //��������� ���������� �� ������������
}

void set_tim_2 (void)
{
  CLK_PCKENR1 = 0x07;  //����� ������������ �� TIM2
  TIM2_PSCR   = 0x33;  //������������ = 7A
  TIM2_CR1    = 0x01;  //���������� ������
  TIM2_IER    = 0x01;  //��������� ���������� �� ������������
}

void set_tim_3 (void)
{
  CLK_PCKENR1 = 0x07;   //����� ������������ �� TIM3
  TIM3_ARRH   = arh;     //0x04;  //������� �� 1250(04E2). ��� �������, ��� 1 ���� =0.8 (�������)
  TIM3_ARRL   = arl;     //0xE2;  //����� �������� 1250*0.8=1000 ������ (1��)
  TIM3_PSCR   = 0x00;   //������������ = 0
  TIM3_CR1    = 0x01;   //���������� ������
  TIM3_IER    = 0x01;   //��������� ���������� �� ������������
  TIM3_ETR_bit.ECE = 1;
}

void set_tim_4 (void)
{
  CLK_PCKENR1 = 0x07;  //����� ������������ �� TIM4
  TIM4_PSCR   = 0x33;  //������������ = 7A
  TIM4_CR1    = 0x01;  //���������� ������
  TIM4_IER    = 0x01;  //��������� ���������� �� ������������
}

