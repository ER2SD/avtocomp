#include "iostm8l152c6.h"
#include "lcd.h"
#include "menu.h"
#include "valcoder.h"

unsigned int num_s=0;
unsigned int num_m=0;
unsigned int num_h=0;
unsigned int clock_1;
unsigned int clock_2;
unsigned int clock_3;
unsigned int data_day=0;
unsigned int data_month=0;
unsigned int data_year=0;
unsigned int data_1;
unsigned int data_2;
unsigned int data_3;
char* clock_one;
char* clock_two;
extern int Vol;

char GetNum( char i )
{
  return (char)(i/16*10 + i%16);
}
char CodNum( char i )
{
  return (char)(i/10*16 + i%10);
}

void clock_out (void)
{
long int data[6];
while ( !RTC_ISR1_bit.RSF ); // ������� �������� ������
//������ ��� ��������
data[0] =GetNum(RTC_TR1); 
data[1] =GetNum(RTC_TR2);
data[2] =GetNum(RTC_TR3);
data[3] =GetNum(RTC_DR1);
data[4] =GetNum(RTC_DR2);
data[5] =GetNum(RTC_DR3);
num_s           =data[2];
num_m           =data[1];
num_h           =data[0];
data_day        =data[3];
data_month      =data[4];
data_year       =data[5];

clock_one="%02u:%02u:%02u        ",clock_1=num_h, clock_2=num_m, clock_3=num_s;
clock_two="%02u-%02u-20%02u  ",data_1=data_day, data_2=data_month, data_3=data_year;
parsing_clock(clock_one, clock_two, clock_1, clock_2, clock_3, data_1, data_2, data_3);
}


void clock_ini (void)//������������� RTC
{
  //CLK_CKDIVR_bit.CKM            = 0;    //��������� �������� ������������
  CLK_PCKENR2_bit.PCKEN22       = 1;    //����� ������������ �� RTC
  RTC_WPR                       = 0xCA; //������ ������
  RTC_WPR                       = 0x53;
  RTC_ISR1_bit.INIT             = 1;    //���� �������� ��� ����������
  //while(RTC_ISR1_bit.INITF==0);         //��������, ���� ��� �������� �����
  CLK_CRTCR                     =194;
  RTC_APRER                     =125;
  RTC_SPRERH                    = 7;
  RTC_SPRERL                    = 208;
  RTC_CR1_bit.FMT               =0;
  RTC_ISR1_bit.INIT = 0;                //���� �������� ��� ����������
  RTC_WPR = 0x00;
}

void clock_set_hour (void)
{
RTC_WPR                       = 0xCA; //������ ������
RTC_WPR                       = 0x53;
RTC_ISR1_bit.INIT             = 1;    //���� �������� ��� ����������
while(RTC_ISR1_bit.INITF==0);         //��������, ���� ��� �������� �����
RTC_TR3 = CodNum( Vol );
RTC_ISR1_bit.INIT = 0;                //���� �������� ��� ����������
RTC_WPR = 0x00;
}

void clock_set_min (void)
{
RTC_WPR                       = 0xCA; //������ ������
RTC_WPR                       = 0x53;
RTC_ISR1_bit.INIT             = 1;    //���� �������� ��� ����������
while(RTC_ISR1_bit.INITF==0);         //��������, ���� ��� �������� �����
RTC_TR2 = CodNum( Vol );
RTC_ISR1_bit.INIT = 0;                //���� �������� ��� ����������
RTC_WPR = 0x00;
}

void clock_set_day (void)
{
RTC_WPR                       = 0xCA; //������ ������
RTC_WPR                       = 0x53;
RTC_ISR1_bit.INIT             = 1;    //���� �������� ��� ����������
while(RTC_ISR1_bit.INITF==0);         //��������, ���� ��� �������� �����
RTC_DR1 = CodNum( Vol );
RTC_ISR1_bit.INIT = 0;                //���� �������� ��� ����������
RTC_WPR = 0x00;
}

void clock_set_month (void)
{
RTC_WPR                       = 0xCA; //������ ������
RTC_WPR                       = 0x53;
RTC_ISR1_bit.INIT             = 1;    //���� �������� ��� ����������
while(RTC_ISR1_bit.INITF==0);         //��������, ���� ��� �������� �����
RTC_DR2 = CodNum( Vol );
RTC_ISR1_bit.INIT = 0;                //���� �������� ��� ����������
RTC_WPR = 0x00;
}

void clock_set_year (void)
{
RTC_WPR                       = 0xCA; //������ ������
RTC_WPR                       = 0x53;
RTC_ISR1_bit.INIT             = 1;    //���� �������� ��� ����������
while(RTC_ISR1_bit.INITF==0);         //��������, ���� ��� �������� �����
RTC_DR3 = CodNum( Vol );
RTC_ISR1_bit.INIT = 0;                //���� �������� ��� ����������
RTC_WPR = 0x00;
}