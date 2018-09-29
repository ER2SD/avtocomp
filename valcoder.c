#include "iostm8l152c6.h"
#include "lcd.h"
#include "counter.h"
#include "menu.h"
#include "set_pin.h"
#include "fm.h"

extern int upState,downState;
int Vol=0;

void up_down (void)
{
  if (Vol!=200)
  {
      if (upState >= 1) 
      {                            
      Vol++;
      upState = 0;
      }
  }
  if (Vol!=-200)
  {
      if (downState >= 1) 
      {                              
      Vol--;
      downState = 0;
      }
  }
}