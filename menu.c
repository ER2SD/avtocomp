#include "iostm8l152c6.h"
#include "stdio.h"
#include "lcd.h"
#include "counter.h"
#include "valcoder.h"
#include "menu.h"
#include "flash.h"
#include "gidro_trans.h"
#include "speed.h"
#include "fm.h"
#include "clock.h"

unsigned int vew_engin_oil=0;
unsigned int vew_akpp_oil=0;
unsigned int vew_oil_reducer=0;
unsigned int vew_fuel_filter=0;
unsigned int vew_air_filter=0;
unsigned int vew_oil_filter=0;
unsigned int vew_cleaning_tnvd=0;
unsigned int vew_changing_the_injector=0;
unsigned int vew_candles_of_chenge=0;
unsigned int vew_set_speed_city=0;
unsigned int vew_set_speed_trass=0;
unsigned int vew_wheel_size_input;

unsigned int inst_engin_oil=0;
unsigned int inst_akpp_oil=0;
unsigned int inst_oil_reducer=0;
unsigned int inst_fuel_filter=0;
unsigned int inst_air_filter=0;
unsigned int inst_oil_filter=0;
unsigned int inst_cleaning_tnvd=0;
unsigned int inst_changing_the_injector=0;
unsigned int inst_candles_of_chenge=0;
unsigned int inst_set_speed_city=0;
unsigned int inst_set_speed_trass=0;
unsigned int inst_wheel_size_input;

unsigned int input_engin_oil=0;
unsigned int input_akpp_oil=0;
unsigned int input_oil_reducer=0;
unsigned int input_fuel_filter=0;
unsigned int input_air_filter=0;
unsigned int input_oil_filter=0;
unsigned int input_cleaning_tnvd=0;
unsigned int input_changing_the_injector=0;
unsigned int input_candles_of_chenge=0;
unsigned int input_set_speed_city=0;
unsigned int input_set_speed_trass=0;
unsigned int input_wheel_size_input;

extern int d;
extern long int odometr;
extern int rattling_PA_4;
extern char buf1[16];
extern char buf2[16];
unsigned long int p_one=0;
unsigned long int p_two=0;
char* stroka_one;
char* stroka_two;
extern int Vol;
extern int timer;


enum STATE_MENU state_menu=CLOCK;

void lcd_out (void)
{
  lcd_gotoxy(0,0);      //Переходим в позицию LCD x=0, y=0 позиция курсора/строка
  lcd_putsf(buf1);      //Вывод на дисплей текста из буфера buf1
  lcd_gotoxy(0,1);      //Переходим в позицию LCD x=0, y=1 позиция курсора/строка
  lcd_putsf(buf2);      //Вывод на дисплей текста из буфера buf2  
  
}
enum KEY_STATE read_kbrd (void)
{
  static enum KEY_STATE key1_state_pred=BT_NO_PRESSED;
  static enum KEY_STATE key1_state_now=BT_NO_PRESSED; 
  if(rattling_PA_4 > KEY_TIME_READ)
  {
    if(PA_IDR_bit.IDR4)
      {
        key1_state_now=BT_NO_PRESSED;           //не нажата
      }
    else
      {
        key1_state_now=BT1_PRESSED;             //нажата
      }
    if(key1_state_now==key1_state_pred)         //проверка 1
      {
      if(key1_state_now==BT_NO_PRESSED)         //если состояние одинаковы
        {
          rattling_PA_4=0;
          key1_state_pred=key1_state_now;       //пред и нынеш - не нажаты
          return BT_NO_PRESSED;                 //нет нажатия 
        }
      else
        {
        if(rattling_PA_4 > KEY_TIME_HOLDED)            //пред и нынеш - нажаты
          {
            rattling_PA_4=0;
            key1_state_pred=key1_state_now;
            return BT1_HOLDED;                  //удержание
          }
        }
      }
    else
      {
      if(key1_state_now==BT_NO_PRESSED)         //когда не одинаковы
        {
          rattling_PA_4=0;                           //key1_state_pred==BT1_PRESSED
          key1_state_pred=key1_state_now;       //key1_state_now = BT_NO_PRESSED
          return BT1_BACK;                      //отжатие
        }
      else
        {
          rattling_PA_4=0;                           //key1_state_pred==BT_NO_PRESSED
          key1_state_pred=key1_state_now;       //key1_state_now=BT1_PRESSED
          return BT1_PRESSED;
        }
      }
  }
return BT_UNDETERM;
}
//------------------------------------------------------------------------------
//***************************** Первый уровень меню ****************************
//------------------------------------------------------------------------------
void main_menu (void)
{
static enum STATE_MENU state_menu_next=CLOCK;
static unsigned int unde_vew=0;

switch(state_menu)
  {
  case CLOCK:                   //Экран с часами
    if(Vol<=1)Vol=1;
    if(Vol>=7)Vol=7;
    if(Vol==1)
    {
    read ();
    state_menu_next=FM;
    }
    if(Vol==2)
    {
    clock_out ();
    state_menu_next=CLOCK;
    }
    if(Vol==3)
    {
    stroka_one="SPEED=%3ld      ",p_one=d;
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=SPEED;
    }
    if(Vol==4)
    {
    stroka_one="ODOMETER=%7ld   ",p_one=odometr;
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=ODOMETER;
    }
    if(Vol==5)
    {
    stroka_one="INSTALLATIONS   ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(Vol==6)
    {
    stroka_one="AUX             ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=AUX;
    }
    if(Vol==7)
    {
      if(PA_ODR_bit.ODR7==1)
      {
      stroka_one="POWER ON        ";
      stroka_two="                ";
      }
      else
      {
      stroka_one="POWER OFF       ";
      stroka_two="                "; 
      }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=POWER;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Первый уровень меню (РАДИО)*********************
//------------------------------------------------------------------------------
  case FM:                   //Меню FM радио
    tuning();
    state_menu_next=CLOCK;
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Первый уровень меню (AUX)******************
//------------------------------------------------------------------------------
  case AUX:                   //Комутатор аудио сигнала
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="AUX-RADIO       ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    PC_ODR_bit.ODR5=0;
    PC_ODR_bit.ODR6=0;
    state_menu_next=CLOCK;
    }
    if(Vol==2)
    {
    stroka_one="AUX-IN          ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    PC_ODR_bit.ODR5=1;
    PC_ODR_bit.ODR6=1;
    state_menu_next=CLOCK;
    }
    if(Vol==3)
    {
    stroka_one="AUX-USB         ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    PC_ODR_bit.ODR5=1;
    PC_ODR_bit.ODR6=0;
    state_menu_next=CLOCK;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CLOCK;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Первый уровень меню (AUX)******************
//------------------------------------------------------------------------------
  case POWER:                   //Комутатор аудио сигнала
    if(Vol<=1)Vol=1;
    if(Vol>=2)Vol=2;
    if(Vol==1)
    {
    stroka_one="POWER ON        ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    if(PE_IDR_bit.IDR7==0)
    {
    PA_ODR_bit.ODR7=1;
    }
    state_menu_next=CLOCK;
    }
    if(Vol==2)
    {
    stroka_one="POWER OFF       ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    PA_ODR_bit.ODR7=0;
    state_menu_next=CLOCK;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Первый уровень меню (скорость)******************
//------------------------------------------------------------------------------
  case SPEED:                   //Показания скорости
    if(Vol<=1)Vol=1;
    if(Vol>=2)Vol=2;
    if(Vol==1)
    {
    stroka_one="MENU SPEED      ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=SPEED;
    }
    if(Vol==2)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CLOCK;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Первый уровень меню (ОДОМЕТР)*******************
//------------------------------------------------------------------------------
  case ODOMETER:                //Показания пройденого пути
    if(Vol<=1)Vol=1;
    if(Vol>=2)Vol=2;
    if(Vol==1)
    {
    stroka_one="MENU ODOMETER   ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CLOCK;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Первый уровень меню (УСТАНОВКИ)*****************
//------------------------------------------------------------------------------
  case INSTALLATIONS:           //Меню установок / настроек
    if(Vol<=1)Vol=1;
    if(Vol>=14)Vol=14;
    if(Vol==1)
    {
    stroka_one="ENGINE OIL      ";
        if(vew_engin_oil<=0)
        {
          stroka_two="WARNING         ";
        }
        else
        {
          stroka_two="%-7ld           ",p_two=vew_engin_oil;
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=ENGINE_OIL;
    }
    if(Vol==2)
    {
    stroka_one="AKPP OIL        ";
        if(vew_akpp_oil<=0)
        {
          stroka_two="WARNING         ";
        }
        else
        {
          stroka_two="%-7ld           ",p_two=vew_akpp_oil;
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=AKPP_OIL;
    }
    if(Vol==3)
    {
    stroka_one="OIL REDUCER     ";
        if(vew_oil_reducer<=0)
        {
          stroka_two="WARNING         ";
        }
        else
        {
          stroka_two="%-7ld           ",p_two=vew_oil_reducer;
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=OIL_REDUCER;
    }
    if(Vol==4)
    {
    stroka_one="FUEL FILTER     ";
        if(vew_fuel_filter<=0)
        {
          stroka_two="WARNING         ";
        }
        else
        {
          stroka_two="%-7ld           ",p_two=vew_fuel_filter;
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=FUEL_FILTER;
    }
    if(Vol==5)
    {
    stroka_one="AIR FILTER      ";
        if(vew_air_filter<=0)
        {
          stroka_two="WARNING         ";
        }
        else
        {
          stroka_two="%-7ld           ",p_two=vew_air_filter;
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=AIR_FILTER;
    }
    if(Vol==6)
    {
    stroka_one="OIL FILTER      ";
        if(vew_oil_filter<=0)
        {
          stroka_two="WARNING         ";
        }
        else
        {
          stroka_two="%-7ld           ",p_two=vew_oil_filter;
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=OIL_FILTER;
    }
    if(Vol==7)
    {
    stroka_one="CLEANING TNVD   ";
        if(vew_cleaning_tnvd<=0)
        {
          stroka_two="WARNING         ";
        }
        else
        {
          stroka_two="%-7ld           ",p_two=vew_cleaning_tnvd;
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CLEANING_TNVD;
    }
    if(Vol==8)
    {
    stroka_one="CHANGING THE    ";
        if(vew_changing_the_injector<=0)
        {
          stroka_two="INJECTOR WARNING";
        }
        else
        {
          stroka_two="INJECTOR  %-7ld ",p_two=vew_changing_the_injector; 
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CHANGING_THE_INJECTOR;
    }
    if(Vol==9)
    {
    stroka_one="CANDLES OF      ";
        if(vew_candles_of_chenge<=0)
        {
          stroka_two="CHANGE WARNING  ";
        }
        else
        {
          stroka_two="CHANGE  %-7ld   ",p_two=vew_candles_of_chenge;
        }
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CANDLES_OF_CHANGE;
    }
    if(Vol==10)
    {
    stroka_one="SET CLOCK       ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=SET_CLOCK;
    }
    if(Vol==11)
    {
    stroka_one="SET SPEED CITY  ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=SET_SPEED_CITY;
    }
    if(Vol==12)
    {
    stroka_one="SET SPEED TRASS ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=SET_SPEED_TRASS;
    }
    if(Vol==13)
    {
    stroka_one="WHEEL SIZE INPUT";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=WHEEL_SIZE_INPUT;
    }
    if(Vol==14)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CLOCK;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case ENGINE_OIL:              //Интервал замены масла в ДВГ
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING TO      ";
    stroka_two="%-7ld           ",p_two=vew_engin_oil;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_engin_oil;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=ENGINE_OIL;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_engin_oil=inst_engin_oil;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=1;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case AKPP_OIL:                //Интервал замены масла АКПП
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-7ld           ",p_two=vew_akpp_oil;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_akpp_oil;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=AKPP_OIL;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_akpp_oil=inst_akpp_oil;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=2;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case OIL_REDUCER:             //Интервал замены масла в мостах
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-7ld           ",p_two=vew_oil_reducer;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_oil_reducer;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=OIL_REDUCER;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_oil_reducer=inst_oil_reducer;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=3;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case FUEL_FILTER:             //Интервал замены топливного фильтра
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-7ld           ",p_two=vew_fuel_filter;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_fuel_filter;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=FUEL_FILTER;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_fuel_filter=inst_fuel_filter;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=4;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case AIR_FILTER:              //Интервал замены воздушного фильтра
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-7ld           ",p_two=vew_air_filter;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_air_filter;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=AIR_FILTER;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_air_filter=inst_air_filter;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=5;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case OIL_FILTER:              //Интервл замены масляного фильтра
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-7ld           ",p_two=vew_oil_filter;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_oil_filter;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=OIL_FILTER;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_oil_filter=inst_oil_filter;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=6;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case CLEANING_TNVD:           //Интервал промывки ТНВД
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-7ld           ",p_two=vew_cleaning_tnvd;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_cleaning_tnvd;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CLEANING_TNVD;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_cleaning_tnvd=inst_cleaning_tnvd;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=7;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case CHANGING_THE_INJECTOR:   //Интервал замены форсунок ТНВД
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-7ld           ",p_two=vew_changing_the_injector;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_changing_the_injector;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CHANGING_THE_INJECTOR;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_changing_the_injector=inst_changing_the_injector;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=8;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case CANDLES_OF_CHANGE:       //Интервал замены свечей накала
    if(Vol<=1)Vol=1;
    if(Vol>=4)Vol=4;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-7ld           ",p_two=vew_candles_of_chenge;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="RESET TO        ";
    stroka_two="defaults=%-7ld  ",p_two=inst_candles_of_chenge;
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=CANDLES_OF_CHANGE;//INSTALLATIONS;
        if(read_kbrd ()==KEY_CODE_HOLDED & Vol==2)
            {
              vew_candles_of_chenge=inst_candles_of_chenge;
              state_menu=state_menu_next;
              Vol=1;
            }
    }
    if(Vol==3)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=9;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case SET_CLOCK:               //Установка времени
    if(Vol<=1)Vol=1;
    if(Vol>=6)Vol=6;
    if(Vol==1)
    {
    stroka_one="INPUT HOUR      ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=13;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==2)
    {
    stroka_one="INPUT MINUTES   ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=14;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==3)
    {
    stroka_one="INPUT DAY       ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=15;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==4)
    {
    stroka_one="INPUT MONTH     ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=16;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==5)
    {
    stroka_one="INPUT YEAR      ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=17;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==6)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case SET_SPEED_CITY:          //Установка ограничения скорости в городе
    if(Vol<=1)Vol=1;
    if(Vol>=3)Vol=3;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-3ld           ",p_two=vew_set_speed_city;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=10;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==3)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case SET_SPEED_TRASS:         //Установка ограничения скорости за городом
    if(Vol<=1)Vol=1;
    if(Vol>=3)Vol=3;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="%-3ld           ",p_two=vew_set_speed_trass;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=11;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==3)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** Второй уровень меню ****************************
//------------------------------------------------------------------------------
  case WHEEL_SIZE_INPUT:         //Установка размера колеса
    if(Vol<=1)Vol=1;
    if(Vol>=3)Vol=3;
    if(Vol==1)
    {
    stroka_one="VIEWING_TO      ";
    stroka_two="R%-2ld          ",p_two=vew_wheel_size_input;
    parsing(stroka_one,stroka_two,p_one,p_two);
    }
    if(Vol==2)
    {
    stroka_one="INSTALLATION_TO ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    unde_vew=12;
    state_menu_next=INSTALLATION_TO;
    }
    if(Vol==3)
    {
    stroka_one="EXIT            ";
    stroka_two="                ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    state_menu_next=INSTALLATIONS;
    }
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=state_menu_next;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** третий уровень меню ****************************
//------------------------------------------------------------------------------
  case INSTALLATION_TO:         //Ввод цифровых значений установок
    if(unde_vew==1)             //Масло в двигателе
      {
      if(Vol<=0)Vol=0;
      if(Vol>=10)Vol=10;
      stroka_one="Input=%7ld      ",p_one=input_engin_oil=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_engin_oil=input_engin_oil;
      state_menu_next=ENGINE_OIL;
      }
    if(unde_vew==2)             //Масло в АКПП
      {
      if(Vol<=0)Vol=0;
      if(Vol>=50)Vol=50;
      stroka_one="Input=%7ld      ",p_one=input_akpp_oil=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_akpp_oil=input_akpp_oil;
      state_menu_next=AKPP_OIL;
      }
    if(unde_vew==3)             //Масло в редукторе
      {
      if(Vol<=0)Vol=0;
      if(Vol>=65)Vol=65;
      stroka_one="Input=%7ld      ",p_one=input_oil_reducer=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_oil_reducer=input_oil_reducer;
      state_menu_next=OIL_REDUCER;
      }
    if(unde_vew==4)             //Топливный фильтр
      {
      if(Vol<=0)Vol=0;
      if(Vol>=30)Vol=30;
      stroka_one="Input=%7ld      ",p_one=input_fuel_filter=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_fuel_filter=input_fuel_filter;
      state_menu_next=FUEL_FILTER;
      }
    if(unde_vew==5)             //Воздушный фильтр
      {
      if(Vol<=0)Vol=0;
      if(Vol>=10)Vol=10;
      stroka_one="Input=%7ld      ",p_one=input_air_filter=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_air_filter=input_air_filter;
      state_menu_next=AIR_FILTER;
      }
    if(unde_vew==6)             //Масляный фильтр
      {
      if(Vol<=0)Vol=0;
      if(Vol>=10)Vol=10;
      stroka_one="Input=%7ld      ",p_one=input_oil_filter=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_oil_filter=input_oil_filter;
      state_menu_next=OIL_FILTER;
      }
    if(unde_vew==7)             //Промывка ТНВД
      {
      if(Vol<=0)Vol=0;
      if(Vol>=50)Vol=50;
      stroka_one="Input=%7ld      ",p_one=input_cleaning_tnvd=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_cleaning_tnvd=input_cleaning_tnvd;
      state_menu_next=CLEANING_TNVD;
      }
    if(unde_vew==8)             //Замена форсунок
      {
      if(Vol<=0)Vol=0;
      if(Vol>=65)Vol=65;
      stroka_one="Input=%7ld      ",p_one=input_changing_the_injector=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_changing_the_injector=input_changing_the_injector;
      state_menu_next=CHANGING_THE_INJECTOR;
      }
    if(unde_vew==9)             //Замена свечей
      {
      if(Vol<=0)Vol=0;
      if(Vol>=65)Vol=65;
      stroka_one="Input=%7ld      ",p_one=input_candles_of_chenge=Vol*1000;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      inst_candles_of_chenge=input_candles_of_chenge;
      state_menu_next=CANDLES_OF_CHANGE;
      }
    if(unde_vew==10)             //Установка скорости в городе
      {
      if(Vol<=0)Vol=0;
      if(Vol>=20)Vol=20;
      stroka_one="Input=%3ld      ",p_one=input_set_speed_city=Vol*5;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      vew_set_speed_city=input_set_speed_city;
      state_menu_next=INSTALLATIONS;
      }
    if(unde_vew==11)             //Установка скорости за городом
      {
      if(Vol<=0)Vol=0;
      if(Vol>=30)Vol=30;
      stroka_one="Input=%3ld      ",p_one=input_set_speed_trass=Vol*5;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      vew_set_speed_trass=input_set_speed_trass;
      state_menu_next=INSTALLATIONS;
      }
    if(unde_vew==12)             //Установка размера колеса
      {
      if(Vol<=0)Vol=0;
      if(Vol>=30)Vol=30;
      stroka_one="Input=R%2ld     ",p_one=input_wheel_size_input=Vol;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      vew_wheel_size_input=input_wheel_size_input;
      state_menu_next=INSTALLATIONS;
      }
    if(unde_vew==13)             //Установка часов
      {
      if(Vol<=0)Vol=0;
      if(Vol>=23)Vol=23;
      stroka_one="Input=%2ld      ",p_one=Vol;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      clock_set_hour ();
      state_menu_next=SET_CLOCK;
      }
    if(unde_vew==14)             //Установка минут
      {
      if(Vol<=0)Vol=0;
      if(Vol>=59)Vol=59;
      stroka_one="Input=%2ld      ",p_one=Vol;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      clock_set_min ();
      state_menu_next=SET_CLOCK;
      }
    if(unde_vew==15)             //Установка дня месяца
      {
      if(Vol<=1)Vol=1;
      if(Vol>=31)Vol=31;
      stroka_one="Input=%2ld      ",p_one=Vol;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      clock_set_day ();
      state_menu_next=SET_CLOCK;
      }
    if(unde_vew==16)             //Установка месяца
      {
      if(Vol<=1)Vol=1;
      if(Vol>=12)Vol=12;
      stroka_one="Input=%2ld      ",p_one=Vol;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      clock_set_month ();
      state_menu_next=SET_CLOCK;
      }
    if(unde_vew==17)             //Установка ГОДА
      {
      if(Vol<=0)Vol=0;
      if(Vol>=99)Vol=99;
      stroka_one="Input=%2ld      ",p_one=Vol;
      stroka_two="      OK        ";
      parsing(stroka_one,stroka_two,p_one,p_two);
      clock_set_year ();
      state_menu_next=SET_CLOCK;
      }

    if(read_kbrd ()==KEY_CODE_SRABAT)
      {
      Vol=1;
      state_menu=state_menu_next;
      }
  break;
//------------------------------------------------------------------------------
//***************************** третий уровень меню ****************************
//------------------------------------------------------------------------------
  case EXIT:                    //Выход в экран времени
    if(read_kbrd ()==KEY_CODE_SRABAT)
    {
    state_menu=CLOCK;
    Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** МЕНЮ ГИДРОТРАНСФОРМАТРОРА **********************
//------------------------------------------------------------------------------
  case GIDROTRANSFORMATOR:                    //Выход в экран времени
    PB_ODR_bit.ODR0=1;
    stroka_one="LOCKING AKPP    ";
    stroka_two="ON              ";
    parsing(stroka_one,stroka_two,p_one,p_two);
    if (timer >=2000)
    {
      state_menu=CLOCK;
      Vol=1;
    }
  break;
//------------------------------------------------------------------------------
//***************************** КОНЕЦ МЕНЮ *************************************
//------------------------------------------------------------------------------
  default:

  break;
  } 

}