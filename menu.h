#ifndef menu
#define menu

#define KEY_TIME_READ 500
#define KEY_TIME_HOLDED 5000
#define KEY_CODE_SRABAT BT1_PRESSED
#define KEY_CODE_HOLDED BT1_HOLDED

enum KEY_STATE
{
  BT_UNDETERM,
  BT_NO_PRESSED,
  BT1_PRESSED,
  BT1_HOLDED,
  BT1_BACK
  
};
enum STATE_MENU
{
  CLOCK,
  FM,
  AUX,
  POWER,
  SPEED,
  ODOMETER,
  INSTALLATIONS,
  ENGINE_OIL,
  AKPP_OIL,
  OIL_REDUCER,
  FUEL_FILTER,
  AIR_FILTER,
  OIL_FILTER,
  CLEANING_TNVD,
  CHANGING_THE_INJECTOR,
  CANDLES_OF_CHANGE,
  SET_CLOCK,
  SET_SPEED_CITY,
  SET_SPEED_TRASS,
  WHEEL_SIZE_INPUT,
  INSTALLATION_TO,
  EXIT,
  GIDROTRANSFORMATOR
};

void tuning(void);
void lcd_out (void);
void main_menu (void);
static enum KEY_STATE read_kbrd (void);
#endif