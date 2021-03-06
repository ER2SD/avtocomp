/*
****************************************************************
* �������� ���� ��� � ��������� ��� ���� ������� ������ �� LCD *
* =============================================================*
* lcd_init2();                                                 *
* lcd_init();                                                  *
* lcd_gotoxy(0,0);//��������� � ������� LCD x=0, y=0           *
* lcd_putsf("Stroka_1");//����� �� ������� ������ "Stroka_1"   *
* lcd_gotoxy(0,1);//��������� � ������� LCD x=0, y=0           *
* lcd_putsf("Stroka_2");//����� �� ������� ������ "Stroka_2"   *
****************************************************************
*/

#include "iostm8l152c6.h"
#include "stdio.h"
#include "menu.h"
#include "fm.h"
#include "clock.h"

#define RS  PE_ODR_bit.ODR0     //0-������ ������, 1-������ ������ ������ RD
#define E   PE_ODR_bit.ODR1     //���� ������ �� ���������� ������ ������ �
#define DB4 PE_ODR_bit.ODR3     //����������� ����� 4-� ������� ���������� DB4
#define DB5 PE_ODR_bit.ODR2     //����������� ����� 4-� ������� ���������� DB5
#define DB6 PE_ODR_bit.ODR5     //����������� ����� 4-� ������� ���������� DB6
#define DB7 PE_ODR_bit.ODR4     //����������� ����� 4-� ������� ���������� DB7

char buf1[16];
char buf2[16];

//*****************************************************************************
char time=30;                   //������� �������� ������� ��� LCD(������� �� �������� ������� MCU)
//����������� ������
void lcd_init2(void)
{
PE_DDR_bit.DDR0=1;              //��������� ������� RS ��� 7 ����� D �� ����� 
PE_CR1_bit.C10=1;               //��������� �������� CR1 ��� 7 ����� D ��� "����������� �����"(��. Datasheet)
PE_DDR_bit.DDR1=1;              //��������� ������� � ��� 5 ����� D �� ����� 
PE_CR1_bit.C11=1;               //��������� �������� CR1 ��� 5 ����� D ��� "����������� �����"(��. Datasheet)
PE_DDR_bit.DDR3=1;              //��������� ������� DB4 ��� 6 ����� D �� �����  
PE_CR1_bit.C13=1;               //��������� �������� CR1 ��� 6 ����� D ��� "����������� �����"(��. Datasheet)
PE_DDR_bit.DDR2=1;              //��������� ������� DB5 ��� 4 ����� D �� ����� 
PE_CR1_bit.C12=1;               //��������� �������� CR1 ��� 4 ����� D ��� "����������� �����"(��. Datasheet)
PE_DDR_bit.DDR5=1;              //��������� ������� DB5 ��� 2 ����� D �� ����� 
PE_CR1_bit.C15=1;               //��������� �������� CR1 ��� 2 ����� D ��� "����������� �����"(��. Datasheet)
PE_DDR_bit.DDR4=1;              //��������� ������� DB6 ��� 0 ����� D �� ����� 
PE_CR1_bit.C14=1;               //��������� �������� CR1 ��� 0 ����� D ��� "����������� �����"(��. Datasheet)
}
//*****************************************************************************
void delay(int a)               //������� ��������
{
  int cnt;
  for (cnt=a; cnt>0; cnt--);
}
//*****************************************************************************
void lcd(char d)                //������� ������ ������/������ � LCD
 {E=1;
  DB7=(d&0x80)>>7;              //������� ������� �����
  DB6=(d&0x40)>>6;
  DB5=(d&0x20)>>5;
  DB4=(d&0x10)>>4;
  delay(time);                  //��������
  E=0;                          //������
  delay(time);                  //��������
  E=1;                          //enable = 1
  DB7=(d&0x08)>>3;              //������� ������� �����
  DB6=(d&0x04)>>2;
  DB5=(d&0x02)>>1;
  DB4=(d&0x01);
  delay(time);                  //��������
  E=0;                          //������
  delay(time);                  //����� ��� ���������� �������
 }
//*****************************************************************************
void lcd_com(char c)            //������ ������ ������ � LCD
{
  RS=0;                         //����� ������ "�������"
  lcd(c);                       //������
}
//*****************************************************************************
void lcd_clear(void)            //������� ������� LCD
{
  lcd_com(0x01); delay(time*100);//������� LCD
}
//*****************************************************************************
void lcd_init(void)             //������� ������������� LCD
{delay(time*10);                //��������������� ��������
 lcd_com(0x33); delay(time*50); //����������
 lcd_com(0x32); lcd_com(0x28);  //4 ����, 2 ������
 lcd_com(0x08);                 //���������� LCD
 lcd_clear();                   //������� LCD
 lcd_com(0x06);                 //����� ������� ������
 lcd_com(0x0C);                 //��������� ��� �������
 delay(time);                   //����� ��� ���������� �������
}
//*****************************************************************************
void lcd_putchar(char d)        //������� ������ ������ � LCD
{
  RS=1;                         //����� ������ "������"
  lcd(d);                       //������
}
//*****************************************************************************
void lcd_putsf(char *_str)      //������� ������ ������ �� LCD
{
  char data;
  while (*_str) {data=*_str++; lcd_putchar(data);}
}
//*****************************************************************************
void lcd_gotoxy(char x, char y) //������� �������� � ��������� �������
{
  lcd_com((0x80+y*64)+x);       //������� �������. ������ 0�80 - ���. �����. ����
}
//*****************************************************************************
void parsing (char* stroka_one,char* stroka_two,long int con1,long int con2)//������� �������������� ������
{
  sprintf(buf1, stroka_one, con1); //������ ���������� �� ������� buf1  
  sprintf(buf2, stroka_two, con2);   //������ ���������� �� ������� buf2
}
//*****************************************************************************
void parsingfm (char* fm_stroka_one,char* fm_stroka_two,int fm_con1,float fm_con2)//������� �������������� ������
{
  sprintf(buf1, fm_stroka_one, fm_con1); //������ ���������� �� ������� buf1  
  sprintf(buf2, fm_stroka_two, fm_con2);   //������ ���������� �� ������� buf2
}
//*****************************************************************************
void parsing_clock (char* clock_one,char* clock_two,unsigned int clock1,unsigned int clock2,unsigned int clock3,unsigned int data_1,unsigned int data_2,unsigned int data_3)//������� �������������� ������
{
  sprintf(buf1, clock_one, clock3,clock2,clock1); //������ ���������� �� ������� buf1  
  sprintf(buf2, clock_two, data_1,data_2,data_3);   //������ ���������� �� ������� buf2
}