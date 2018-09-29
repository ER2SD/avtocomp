#ifndef fm
#define fm
void delay(int);
void tuning(void);
void save_station(void);
void lm7001_data (void);
void read (void);

#define KEY_TIME_READ_SAVE_FM   100
#define KEY_TIME_HOLDED_SAVE_FM 2000
#define KEY_CODE_NAJATA         BT4_PRESSED
#define KEY_CODE_UDERJANIE      BT4_HOLDED
enum KEY_SAVE_FM
{
  BT4_UNDETERM,
  BT4_NO_PRESSED,
  BT4_PRESSED,
  BT4_HOLDED,
  BT4_BACK
};
enum KEY_SAVE_FM read_button_save_fm (void);

#endif