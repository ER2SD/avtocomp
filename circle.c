#include "iostm8l152c6.h"
#include "counter.h"
#include "flash.h"
#include "circle.h"
extern unsigned int vew_wheel_size_input;       //Äèàìåòğ êîëåñà â äşéìàõ
double ddl;             //Âğåìåííàÿ ïåğåìåííÿ ñ ïëàâàşùåé òî÷êîé.   
unsigned int dl=0;      //Ïåğåìåííàÿ äëÿ ïğåîáğàçîâàíèÿ â öåëîå ÷èñëî ïåğåìåííîé ddl
unsigned int ddll=0;    //Ïåğåìåííàÿ äëÿ TIM1
unsigned char arh=0;    //Ïåğåìåííàÿ äëÿ ñòàğøåãî ğåãèñòğà ñ÷¸ò÷èêà TIM3.
unsigned char arl=0;    //Ïåğåìåííàÿ äëÿ ìëàäøåãî ğåãèñòğà ñ÷¸ò÷èêà TIM3.

void diametr (void)
{
  ddl=(double)1250/((double)vew_wheel_size_input/(double)16);//Ïîëó÷àåì îòíîøåíèå ìåæäó 1250 è âûğàæåíèåì ğàçíîñòè äèàìåòğà êîëåñà
  dl=(unsigned int)ddl; //Ïåğåâîäó ïåğåìåííóş ddl â öåëîå ÷èñëî.
  ddll=dl;
  arl=dl;       //Ğàçäåëÿş äâóõáàéòîâîå ÷èñëî íà äâà îäíîáàéòîâûõ
  dl>>=8;
  arh=dl;
}