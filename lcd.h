#ifndef LCD
#define LCD

#define RS  PD_ODR_bit.ODR7//0-запись команд, 1-запись данных сигнал RD
#define E   PD_ODR_bit.ODR5//Ввод данных по спадающему фронту сигнал Е
#define DB4 PD_ODR_bit.ODR6//Определение линий 4-х битного интерфейса DB4
#define DB5 PD_ODR_bit.ODR4//Определение линий 4-х битного интерфейса DB5
#define DB6 PD_ODR_bit.ODR2//Определение линий 4-х битного интерфейса DB6
#define DB7 PD_ODR_bit.ODR0//Определение линий 4-х битного интерфейса DB7
void lcd_init2(void);
void delay(int);
void lcd(char);
void lcd_clear(void);
void lcd_init(void);
void lcd_com(char);
void lcd_putchar(char);
void lcd_putsf(char*);
void lcd_gotoxy(char,char);
void parsing (char*,char*,long int,long int);
void parsingfm (char*,char*,int,float);
void parsing_clock (char* clock_one,char* clock_two,unsigned int clock1,unsigned int clock2,unsigned int clock3,unsigned int data_1,unsigned int data_2,unsigned int data_3);
#endif