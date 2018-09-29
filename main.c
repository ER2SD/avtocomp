#include "iostm8l152c6.h"
#include "stdio.h"
#include "lcd.h"
#include "counter.h"
#include "menu.h"
#include "set_pin.h"
#include "valcoder.h"
#include "alarm.h"
#include "flash.h"
#include "gidro_trans.h"
#include "speed.h"
#include "circle.h"
#include "fm.h"
#include "clock.h"

extern char* stroka_one;
extern char* stroka_two;
extern unsigned long int p_one;
extern unsigned long int p_two;
extern int selection;
extern int timer_aux;
extern int pin_5;
extern int pin_6;

int main( void )
{
asm("rim");             //�������� ����������
load_flash      ();     //�������� ���������� �� ������ ��� ��������� �������
diametr         ();     //�������� � ��������� TIM3 ��������� ������� ������
lcd_init2       ();     //��������� ������ ��� LCD
lcd_init        ();     //������������� LCD
set_tim_1       ();     //������������� TIM1
set_tim_2       ();     //������������� TIM2
set_tim_3       ();     //������������� TIM3
set_tim_4       ();     //������������� TIM4
set_button      ();     //������������� ����� ��� ������ ��������� �� ����
clock_ini       ();     //������������� I2C ����������
PE_ODR_bit.ODR6=1;
PE_ODR_bit.ODR7=1;

  while(1)
  {
    lcd_out();
    main_menu();        //����� ��������� ����
    up_down();          //���������� ������ ���������
    //led_alarm();
    gidrotransformator_on ();
    gidrotransformator_off ();
    if(selection==0)speed_city ();
    if(selection==1)speed_trass ();
    if(PE_IDR_bit.IDR7==1)
    {
    PA_ODR_bit.ODR7=0;
    }
    if(timer_aux ==20)
    {
      PC_ODR_bit.ODR5=pin_5;
      PC_ODR_bit.ODR6=pin_6;
      PB_ODR_bit.ODR1=1;
      PB_ODR_bit.ODR5=1;
      PB_ODR_bit.ODR6=1;
      PB_ODR_bit.ODR7=1;
    }
    if(PB_IDR_bit.IDR4==0)
    {
      stroka_one="SAVE DATA       ";
      stroka_two="                ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      save_flash ();
    }
    speed_button ();
  }

}