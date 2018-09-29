#include "iostm8l152c6.h"
#include "counter.h"
#include "flash.h"
#include "circle.h"
extern unsigned int vew_wheel_size_input;       //������� ������ � ������
double ddl;             //��������� ��������� � ��������� ������.   
unsigned int dl=0;      //���������� ��� �������������� � ����� ����� ���������� ddl
unsigned int ddll=0;    //���������� ��� TIM1
unsigned char arh=0;    //���������� ��� �������� �������� �������� TIM3.
unsigned char arl=0;    //���������� ��� �������� �������� �������� TIM3.

void diametr (void)
{
  ddl=(double)1250/((double)vew_wheel_size_input/(double)16);//�������� ��������� ����� 1250 � ���������� �������� �������� ������
  dl=(unsigned int)ddl; //�������� ���������� ddl � ����� �����.
  ddll=dl;
  arl=dl;       //�������� ������������ ����� �� ��� ������������
  dl>>=8;
  arh=dl;
}