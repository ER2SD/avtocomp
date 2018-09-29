#ifndef speed
#define speed

#define KEY_TIME_READ_SPEED 100
#define KEY_TIME_HOLDED_SPEED 2000
#define KEY_CODE_NAJATA_SPEED BT3_PRESSED
#define KEY_CODE_UDERJANIE_SPEED BT3_HOLDED

enum KEY_SPEED
{
  BT3_UNDETERM,
  BT3_NO_PRESSED,
  BT3_PRESSED,
  BT3_HOLDED,
  BT3_BACK
  
};
enum KEY_SPEED read_button_speed (void);
void speed_city (void);
void speed_trass (void);
void speed_button (void);

#endif