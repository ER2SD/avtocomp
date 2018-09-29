#include "iostm8l152c6.h"
#include "fm.h"
#include "lcd.h"
#include "menu.h"
#include "valcoder.h"
//#include "intrinsics.h"
//unsigned int   fgtemp=0;
//volatile int stepfm=0;
//extern int timer_aux;
extern char buf1[16];           //Буфер для LCD
extern char buf2[16];           //Буфер для LCD
char* fm_stroka_one;            //Строка для LCD
char* fm_stroka_two;            //Строка для LCD
int fm_p_one;                   //Переменная для LCD
float fm_p_two;                 //Переменная для LCD
int timefm=5;                   //Задержка для протокола LM7001
extern int Vol;                 //Счётчик валкодера
char save_fm=0;                 //Статус флага для while
int mem_fm=0;                   //Количество ячеек памяти (20 ячеек)
char *memadr=(char *)0x1030;    //Начальный адресс памяли для сохранения FM станций
unsigned int   fg=0;            //делитель
unsigned char  fgset=134;       //Конфигурация
extern int rattling_PA_SAVE_FM; //стётчик для антидребезга
int datafm[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       //массив слова для LM7001
//------------------------------------------------------------------------------
//***************************** Блок антидребезка кнопки ***********************
//------------------------------------------------------------------------------
enum KEY_SAVE_FM read_button_save_fm (void)
{
  static enum KEY_SAVE_FM key4_state_pred=BT4_NO_PRESSED;
  static enum KEY_SAVE_FM key4_state_now=BT4_NO_PRESSED; 
  if(rattling_PA_SAVE_FM > KEY_TIME_READ_SAVE_FM)
  {
    if(PC_IDR_bit.IDR7)
      {
        key4_state_now=BT4_NO_PRESSED;                  //не нажата
      }
    else
      {
        key4_state_now=BT4_PRESSED;                     //нажата
      }
    if(key4_state_now==key4_state_pred)                 //проверка 1
      {
      if(key4_state_now==BT4_NO_PRESSED)                //если состояние одинаковы
        {
          rattling_PA_SAVE_FM =0;
          key4_state_pred=key4_state_now;               //пред и нынеш - не нажаты
          return BT4_NO_PRESSED;                        //нет нажатия 
        }
      else
        {
        if(rattling_PA_SAVE_FM > KEY_TIME_HOLDED_SAVE_FM)//пред и нынеш - нажаты
          {
            rattling_PA_SAVE_FM =0;
            key4_state_pred=key4_state_now;
            return BT4_HOLDED;                          //удержание
          }
        }
      }
    else
      {
      if(key4_state_now==BT4_NO_PRESSED)                //когда не одинаковы
        {
          rattling_PA_SAVE_FM =0;                       //key1_state_pred==BT1_PRESSED
          key4_state_pred=key4_state_now;               //key1_state_now = BT_NO_PRESSED
          return BT4_BACK;                              //отжатие
        }
      else
        {
          rattling_PA_SAVE_FM=0;                        //key1_state_pred==BT_NO_PRESSED
          key4_state_pred=key4_state_now;               //key1_state_now=BT1_PRESSED
          return BT4_PRESSED;
        }
      }
  }
return BT4_UNDETERM;
}
//------------------------------------------------------------------------------
//********************** Функция плавной регулировки FM частоты ****************
//------------------------------------------------------------------------------
void tuning(void)
{
  PC_ODR_bit.ODR5=0;                    //Переключение аудиокоммутатора в положение FM
  PC_ODR_bit.ODR6=0;                    //Переключение аудиокоммутатора в положение FM
  fg=Vol+987;                           //Первая часть слова для LM7001
  if(Vol<=0)Vol=0;                      //Ограничение предела валкодера. Не менее 0
  if(Vol>=200)Vol=200;                  //Ограничение предела валкодера. Не более 200
  fm_stroka_one="FM TUNING        ";
  fm_stroka_two="%3.2f MHz        ",fm_p_two=((float)fg-987+(float)880)/10;//Формирование FM частоты
  parsingfm(fm_stroka_one,fm_stroka_two,fm_p_one,fm_p_two);    //Передача переменных в буфер массива. Сколько переменных столько и массивов
  lm7001_data (); 
  if(read_button_save_fm()==KEY_CODE_UDERJANIE) //Переход в режим записи FM частоты
  {
    memadr=(char *)0x1030;              //установка начального адреса памяти FM
    Vol=0;
    save_fm=1;                          //Установка влага для работы цикла while
    save_station();
  }
}
//------------------------------------------------------------------------------
//********************** Функция чтения FM частоты из памяти *******************
//------------------------------------------------------------------------------
void read (void)
{
    fg = *memadr+987;                   //Формируем первую часть слова для LM7001
    lm7001_data ();
    fm_stroka_one="RADIO MEMORY %-2d",fm_p_one=mem_fm;
    fm_stroka_two="%3.2f MHz        ",fm_p_two=((float)fg-987+(float)880)/10;//Формирование FM частоты
    parsingfm(fm_stroka_one,fm_stroka_two,fm_p_one,fm_p_two);
    if(read_button_save_fm()==KEY_CODE_NAJATA)  //Перемещение по каналам FM памяти
    {
      mem_fm++;                                 //Инкремент номера канала FM памяти
      *memadr++;                                //Инкремент номера ячейки памяти FM канала
    }
    if(mem_fm > 20)                     //Если номер FM канала достиг конца - начать с начала
    {
      mem_fm=0;
      memadr=(char *)0x1030;
    }
    
}
//------------------------------------------------------------------------------
//********************** Функция записи FM частоты в память ********************
//------------------------------------------------------------------------------
void save_station(void)
{ 
  while(save_fm)                        //Пока флаг равен 1 работает цикл.
  {
    if(Vol<=0)Vol=0;                    //Ограничение предела валкодера. Не менее 0
    if(Vol>=20)Vol=20;                  //Ограничение предела валкодера. Не более 20
    mem_fm=Vol;                         //Формируем номер канала памяти
    fm_stroka_one="FM CH MEMORY-%-2d",fm_p_one=mem_fm;
    fm_stroka_two="%3.2f MHz        ",fm_p_two=((float)fg-987+(float)880)/10;
    parsingfm(fm_stroka_one,fm_stroka_two,fm_p_one,fm_p_two); 
  if(read_button_save_fm()==KEY_CODE_NAJATA)//запись в память FM станции
    {
      if (FLASH_IAPSR_DUL == 0)         //Разрешаем запись в память
      {
        FLASH_DUKR = 0xAE;
        FLASH_DUKR = 0x56;
      }
      memadr=memadr+(char)Vol;          //Формируем адресс ячейки памяти
      *memadr=fg-987;                   //Записываем в ячейку памяти часть слова LM7001
      save_fm=0;                        //Переводим флаг в 0, останавливаем работу цикла while
      FLASH_IAPSR_DUL = 0;              //Запрещаем запись в память
    }
  lcd_out();                            //функция LCD
  up_down();                            //Функция валкодера
  }
}
//------------------------------------------------------------------------------
//********************** Функция формирования слова для LM7001 *****************
//------------------------------------------------------------------------------
void lm7001_data (void)
{
unsigned int mask=1;            //используем маску для определения бита в переменной fg
  for(int ch=0;ch<15;ch++)
  {
    if(fg&mask)  
    {
    PC_ODR_bit.ODR3=1;
    }
    else
    {
    PC_ODR_bit.ODR3=0;
    }
    datafm[ch]=PC_ODR_bit.ODR3; //Записываем в массив первую часть слова LM7001
    mask=mask<<1;               //Сдвигаем значение маски на 1 в право.
  }
  unsigned char maskset=1;
  for(int ch=16;ch<24;ch++)
  {
    if(fgset&maskset)  
    {
    PC_ODR_bit.ODR3=1;
    }
    else
    {
    PC_ODR_bit.ODR3=0;
    }
    datafm[ch]=PC_ODR_bit.ODR3;
    maskset=maskset<<1;
  }
  PC_ODR_bit.ODR2=1;            //Формируем сигнал CE
  for(int ch=0;ch<=23;ch++)     //Перебираем последовательно буфер
  {
    PC_ODR_bit.ODR3=datafm[ch]; //Формируем сигнал DATA
    delay(timefm);              //Функция задержки. Расположена в файле lcd.c
    PC_ODR_bit.ODR4=1;          //Формируем сигнал CL
    delay(timefm);              //Функция задержки. Расположена в файле lcd.c
    PC_ODR_bit.ODR4=0;          //Формируем сигнал CL
  }
  PC_ODR_bit.ODR2=0;            //Формируем сигнал CE
}