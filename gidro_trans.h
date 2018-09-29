#ifndef gidro_trams
#define gidro_trans

#define KEY_TIME_READ_TRANS 100
#define KEY_TIME_HOLDED_TRANS 2000
#define KEY_CODE_NAJATA2 BT2_PRESSED
#define KEY_CODE_UDERJANIE2 BT2_HOLDED
enum KEY_TRANS
{
  BT2_UNDETERM,
  BT2_NO_PRESSED,
  BT2_PRESSED,
  BT2_HOLDED,
  BT2_BACK
  
};

enum KEY_TRANS read_button (void);
void gidrotransformator_on (void);
void gidrotransformator_off (void);
void delay_audio(void);

#endif