#include "iostm8l152c6.h"
#include "fm.h"
#include "lcd.h"
#include "menu.h"
#include "valcoder.h"
//#include "intrinsics.h"
//unsigned int   fgtemp=0;
//volatile int stepfm=0;
//extern int timer_aux;
extern char buf1[16];           //����� ��� LCD
extern char buf2[16];           //����� ��� LCD
char* fm_stroka_one;            //������ ��� LCD
char* fm_stroka_two;            //������ ��� LCD
int fm_p_one;                   //���������� ��� LCD
float fm_p_two;                 //���������� ��� LCD
int timefm=5;                   //�������� ��� ��������� LM7001
extern int Vol;                 //������� ���������
char save_fm=0;                 //������ ����� ��� while
int mem_fm=0;                   //���������� ����� ������ (20 �����)
char *memadr=(char *)0x1030;    //��������� ������ ������ ��� ���������� FM �������
unsigned int   fg=0;            //��������
unsigned char  fgset=134;       //������������
extern int rattling_PA_SAVE_FM; //������ ��� ������������
int datafm[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       //������ ����� ��� LM7001
//------------------------------------------------------------------------------
//***************************** ���� ������������ ������ ***********************
//------------------------------------------------------------------------------
enum KEY_SAVE_FM read_button_save_fm (void)
{
  static enum KEY_SAVE_FM key4_state_pred=BT4_NO_PRESSED;
  static enum KEY_SAVE_FM key4_state_now=BT4_NO_PRESSED; 
  if(rattling_PA_SAVE_FM > KEY_TIME_READ_SAVE_FM)
  {
    if(PC_IDR_bit.IDR7)
      {
        key4_state_now=BT4_NO_PRESSED;                  //�� ������
      }
    else
      {
        key4_state_now=BT4_PRESSED;                     //������
      }
    if(key4_state_now==key4_state_pred)                 //�������� 1
      {
      if(key4_state_now==BT4_NO_PRESSED)                //���� ��������� ���������
        {
          rattling_PA_SAVE_FM =0;
          key4_state_pred=key4_state_now;               //���� � ����� - �� ������
          return BT4_NO_PRESSED;                        //��� ������� 
        }
      else
        {
        if(rattling_PA_SAVE_FM > KEY_TIME_HOLDED_SAVE_FM)//���� � ����� - ������
          {
            rattling_PA_SAVE_FM =0;
            key4_state_pred=key4_state_now;
            return BT4_HOLDED;                          //���������
          }
        }
      }
    else
      {
      if(key4_state_now==BT4_NO_PRESSED)                //����� �� ���������
        {
          rattling_PA_SAVE_FM =0;                       //key1_state_pred==BT1_PRESSED
          key4_state_pred=key4_state_now;               //key1_state_now = BT_NO_PRESSED
          return BT4_BACK;                              //�������
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
//********************** ������� ������� ����������� FM ������� ****************
//------------------------------------------------------------------------------
void tuning(void)
{
  PC_ODR_bit.ODR5=0;                    //������������ ���������������� � ��������� FM
  PC_ODR_bit.ODR6=0;                    //������������ ���������������� � ��������� FM
  fg=Vol+987;                           //������ ����� ����� ��� LM7001
  if(Vol<=0)Vol=0;                      //����������� ������� ���������. �� ����� 0
  if(Vol>=200)Vol=200;                  //����������� ������� ���������. �� ����� 200
  fm_stroka_one="FM TUNING        ";
  fm_stroka_two="%3.2f MHz        ",fm_p_two=((float)fg-987+(float)880)/10;//������������ FM �������
  parsingfm(fm_stroka_one,fm_stroka_two,fm_p_one,fm_p_two);    //�������� ���������� � ����� �������. ������� ���������� ������� � ��������
  lm7001_data (); 
  if(read_button_save_fm()==KEY_CODE_UDERJANIE) //������� � ����� ������ FM �������
  {
    memadr=(char *)0x1030;              //��������� ���������� ������ ������ FM
    Vol=0;
    save_fm=1;                          //��������� ����� ��� ������ ����� while
    save_station();
  }
}
//------------------------------------------------------------------------------
//********************** ������� ������ FM ������� �� ������ *******************
//------------------------------------------------------------------------------
void read (void)
{
    fg = *memadr+987;                   //��������� ������ ����� ����� ��� LM7001
    lm7001_data ();
    fm_stroka_one="RADIO MEMORY %-2d",fm_p_one=mem_fm;
    fm_stroka_two="%3.2f MHz        ",fm_p_two=((float)fg-987+(float)880)/10;//������������ FM �������
    parsingfm(fm_stroka_one,fm_stroka_two,fm_p_one,fm_p_two);
    if(read_button_save_fm()==KEY_CODE_NAJATA)  //����������� �� ������� FM ������
    {
      mem_fm++;                                 //��������� ������ ������ FM ������
      *memadr++;                                //��������� ������ ������ ������ FM ������
    }
    if(mem_fm > 20)                     //���� ����� FM ������ ������ ����� - ������ � ������
    {
      mem_fm=0;
      memadr=(char *)0x1030;
    }
    
}
//------------------------------------------------------------------------------
//********************** ������� ������ FM ������� � ������ ********************
//------------------------------------------------------------------------------
void save_station(void)
{ 
  while(save_fm)                        //���� ���� ����� 1 �������� ����.
  {
    if(Vol<=0)Vol=0;                    //����������� ������� ���������. �� ����� 0
    if(Vol>=20)Vol=20;                  //����������� ������� ���������. �� ����� 20
    mem_fm=Vol;                         //��������� ����� ������ ������
    fm_stroka_one="FM CH MEMORY-%-2d",fm_p_one=mem_fm;
    fm_stroka_two="%3.2f MHz        ",fm_p_two=((float)fg-987+(float)880)/10;
    parsingfm(fm_stroka_one,fm_stroka_two,fm_p_one,fm_p_two); 
  if(read_button_save_fm()==KEY_CODE_NAJATA)//������ � ������ FM �������
    {
      if (FLASH_IAPSR_DUL == 0)         //��������� ������ � ������
      {
        FLASH_DUKR = 0xAE;
        FLASH_DUKR = 0x56;
      }
      memadr=memadr+(char)Vol;          //��������� ������ ������ ������
      *memadr=fg-987;                   //���������� � ������ ������ ����� ����� LM7001
      save_fm=0;                        //��������� ���� � 0, ������������� ������ ����� while
      FLASH_IAPSR_DUL = 0;              //��������� ������ � ������
    }
  lcd_out();                            //������� LCD
  up_down();                            //������� ���������
  }
}
//------------------------------------------------------------------------------
//********************** ������� ������������ ����� ��� LM7001 *****************
//------------------------------------------------------------------------------
void lm7001_data (void)
{
unsigned int mask=1;            //���������� ����� ��� ����������� ���� � ���������� fg
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
    datafm[ch]=PC_ODR_bit.ODR3; //���������� � ������ ������ ����� ����� LM7001
    mask=mask<<1;               //�������� �������� ����� �� 1 � �����.
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
  PC_ODR_bit.ODR2=1;            //��������� ������ CE
  for(int ch=0;ch<=23;ch++)     //���������� ��������������� �����
  {
    PC_ODR_bit.ODR3=datafm[ch]; //��������� ������ DATA
    delay(timefm);              //������� ��������. ����������� � ����� lcd.c
    PC_ODR_bit.ODR4=1;          //��������� ������ CL
    delay(timefm);              //������� ��������. ����������� � ����� lcd.c
    PC_ODR_bit.ODR4=0;          //��������� ������ CL
  }
  PC_ODR_bit.ODR2=0;            //��������� ������ CE
}