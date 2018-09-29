#include "iostm8l152c6.h"
#include "menu.h"
#include "circle.h"
//ÏÐÎÌÅÆÓÒÎ×ÍÛÅ ÏÅÐÅÌÅÍÍÛÅ******************************************************
//Ïåðåìåííûå ñ òåêóùèìè ïîêàçàíèÿìè
//******************************************************************************
unsigned int shift_vew_engin_oil;
unsigned int shift_vew_akpp_oil;
unsigned int shift_vew_oil_reducer;
unsigned int shift_vew_fuel_filter;
unsigned int shift_vew_air_filter;
unsigned int shift_vew_oil_filter;
unsigned int shift_vew_cleaning_tnvd;
unsigned int shift_vew_changing_the_injector;
unsigned int shift_vew_candles_of_chenge;
unsigned int shift_vew_set_speed_city;
unsigned int shift_vew_set_speed_trass;
unsigned int shift_vew_wheel_size_input;
//Ïåðåìåííûå õðàíÿùèå êîíñòàíòû.
unsigned int shift_inst_engin_oil;
unsigned int shift_inst_akpp_oil;
unsigned int shift_inst_oil_reducer;
unsigned int shift_inst_fuel_filter;
unsigned int shift_inst_air_filter;
unsigned int shift_inst_oil_filter;
unsigned int shift_inst_cleaning_tnvd;
unsigned int shift_inst_changing_the_injector;
unsigned int shift_inst_candles_of_chenge;
unsigned int shift_inst_set_speed_city;
unsigned int shift_inst_set_speed_trass;
unsigned int shift_inst_wheel_size_input;

long int shift_c;                                      //Ïðîéäåíûé ïóòü

//ÊÎÍÅ×ÍÛÅ ÏÅÐÅÌÅÍÍÛÅ***********************************************************
//Ïåðåìåííûå ñ òåêóùèìè ïîêàçàíèÿìè
//******************************************************************************
extern unsigned int vew_engin_oil;
extern unsigned int vew_akpp_oil;
extern unsigned int vew_oil_reducer;
extern unsigned int vew_fuel_filter;
extern unsigned int vew_air_filter;
extern unsigned int vew_oil_filter;
extern unsigned int vew_cleaning_tnvd;
extern unsigned int vew_changing_the_injector;
extern unsigned int vew_candles_of_chenge;
extern unsigned int vew_set_speed_city;
extern unsigned int vew_set_speed_trass;
extern unsigned int vew_wheel_size_input;
//Ïåðåìåííûå õðàíÿùèå êîíñòàíòû.
extern unsigned int inst_engin_oil;
extern unsigned int inst_akpp_oil;
extern unsigned int inst_oil_reducer;
extern unsigned int inst_fuel_filter;
extern unsigned int inst_air_filter;
extern unsigned int inst_oil_filter;
extern unsigned int inst_cleaning_tnvd;
extern unsigned int inst_changing_the_injector;
extern unsigned int inst_candles_of_chenge;
extern unsigned int inst_set_speed_city;
extern unsigned int inst_set_speed_trass;
extern unsigned int inst_wheel_size_input;
extern long int odometr;                                      //Ïðîéäåíûé ïóòü
int i=0;
int j=0;
int k=0;

unsigned char byte[4]; 

void save_flash (void)
{
  char *address = (char *) 0x1000;
   if (FLASH_IAPSR_DUL == 0)
    {
      FLASH_DUKR = 0xAE;
      FLASH_DUKR = 0x56;
    }
     //*************************************************************************
     *address++ = odometr >>24;
     *address++ = odometr >>16;
     *address++ = odometr >>8;
     *address++ = odometr ;
     //***************----ÇÀÏÈÑÜ ÒÅÊÙÈÕ ÇÍÀ×ÅÍÈÉ----****************************
     *address++ = vew_engin_oil >>8;
     *address++ = vew_engin_oil ;
     //*************************************************************************
     *address++ = vew_akpp_oil >>8;
     *address++ = vew_akpp_oil ;
     //*************************************************************************
     *address++ = vew_oil_reducer >>8;
     *address++ = vew_oil_reducer ;
     //*************************************************************************
     *address++ = vew_fuel_filter >>8;
     *address++ = vew_fuel_filter ;
     //*************************************************************************
     *address++ = vew_air_filter >>8;
     *address++ = vew_air_filter ;
     //*************************************************************************
     *address++ = vew_oil_filter >>8;
     *address++ = vew_oil_filter ;
     //*************************************************************************
     *address++ = vew_cleaning_tnvd >>8;
     *address++ = vew_cleaning_tnvd ;
     //*************************************************************************
     *address++ = vew_changing_the_injector >>8;
     *address++ = vew_changing_the_injector ;
     //*************************************************************************
     *address++ = vew_candles_of_chenge >>8;
     *address++ = vew_candles_of_chenge ;
     //*************************************************************************
     *address++ = vew_set_speed_city >>8;
     *address++ = vew_set_speed_city ;
     //*************************************************************************
     *address++ = vew_set_speed_trass >>8;
     *address++ = vew_set_speed_trass ;
     //*************************************************************************
     *address++ = vew_wheel_size_input >>8;
     *address++ = vew_wheel_size_input ;
     //*************************************************************************
     //***************----ÇÀÏÈÑÜ ÊÎÍÑÒÀÍÒ----***********************************
     //*************************************************************************
     *address++ = inst_engin_oil >>8;
     *address++ = inst_engin_oil ;
     //*************************************************************************
     *address++ = inst_akpp_oil >>8;
     *address++ = inst_akpp_oil ;
     //*************************************************************************
     *address++ = inst_oil_reducer >>8;
     *address++ = inst_oil_reducer ;
     //*************************************************************************
     *address++ = inst_fuel_filter >>8;
     *address++ = inst_fuel_filter ;
     //*************************************************************************
     *address++ = inst_air_filter >>8;
     *address++ = inst_air_filter ;
     //*************************************************************************
     *address++ = inst_oil_filter >>8;
     *address++ = inst_oil_filter ;
     //*************************************************************************
     *address++ = inst_cleaning_tnvd >>8;
     *address++ = inst_cleaning_tnvd ;
     //*************************************************************************
     *address++ = inst_changing_the_injector >>8;
     *address++ = inst_changing_the_injector ;
     //*************************************************************************
     *address++ = inst_candles_of_chenge >>8;
     *address++ = inst_candles_of_chenge ;
     //*************************************************************************
     *address++ = inst_set_speed_city >>8;
     *address++ = inst_set_speed_city ;
     //*************************************************************************
     *address++ = inst_set_speed_trass >>8;
     *address++ = inst_set_speed_trass ;
     //*************************************************************************
     *address++ = inst_wheel_size_input >>8;
     *address++ = inst_wheel_size_input ;
     
  FLASH_IAPSR_DUL = 0;
}

void load_flash (void)
{
  char *address = (char *) 0x1000;
     //*************************************************************************
      shift_c = byte[3] = *address++ ;
      shift_c <<=8;
      shift_c |= byte[2] = *address++ ;
      shift_c <<=8;
      shift_c |= byte[1] = *address++ ;
      shift_c <<=8;
      shift_c |= byte[0] = *address++ ;
      odometr=shift_c;
     //***************----ÇÀÏÈÑÜ ÒÅÊÙÈÕ ÇÍÀ×ÅÍÈÉ----****************************
     shift_vew_engin_oil                = *address++ <<8;
     shift_vew_engin_oil                |= *address++;
        vew_engin_oil = shift_vew_engin_oil;
     //*************************************************************************
     shift_vew_akpp_oil                 = *address++ <<8;
     shift_vew_akpp_oil                 |= *address++;
        vew_akpp_oil = shift_vew_akpp_oil;
     //*************************************************************************
     shift_vew_oil_reducer              = *address++ <<8;
     shift_vew_oil_reducer              |= *address++;
        vew_oil_reducer = shift_vew_oil_reducer;
     //*************************************************************************
     shift_vew_fuel_filter              = *address++ <<8;
     shift_vew_fuel_filter              |= *address++;
        vew_fuel_filter = shift_vew_fuel_filter;
     //*************************************************************************
     shift_vew_air_filter               = *address++ <<8;
     shift_vew_air_filter               |= *address++;
        vew_air_filter = shift_vew_air_filter;
     //*************************************************************************
     shift_vew_oil_filter               = *address++ <<8;
     shift_vew_oil_filter               |= *address++;
        vew_oil_filter = shift_vew_oil_filter;
     //*************************************************************************
     shift_vew_cleaning_tnvd            = *address++ <<8;
     shift_vew_cleaning_tnvd            |= *address++;
        vew_cleaning_tnvd = shift_vew_cleaning_tnvd;
     //*************************************************************************
     shift_vew_changing_the_injector    = *address++ <<8;
     shift_vew_changing_the_injector    |= *address++;
        vew_changing_the_injector = shift_vew_changing_the_injector;
     //*************************************************************************
     shift_vew_candles_of_chenge        = *address++ <<8;
     shift_vew_candles_of_chenge        |= *address++;
        vew_candles_of_chenge = shift_vew_candles_of_chenge;
     //*************************************************************************
     shift_vew_set_speed_city           = *address++ <<8;
     shift_vew_set_speed_city           |= *address++;
        vew_set_speed_city = shift_vew_set_speed_city;
     //*************************************************************************
     shift_vew_set_speed_trass          = *address++ <<8;
     shift_vew_set_speed_trass          |= *address++;
        vew_set_speed_trass = shift_vew_set_speed_trass;
        //*************************************************************************
     shift_vew_wheel_size_input          = *address++ <<8;
     shift_vew_wheel_size_input          |= *address++;
        vew_wheel_size_input = shift_vew_wheel_size_input;
     //*************************************************************************
     //***************----ÇÀÏÈÑÜ ÊÎÍÑÒÀÍÒ----***********************************
     //*************************************************************************
     shift_inst_engin_oil               = *address++ <<8;
     shift_inst_engin_oil               |= *address++;
        inst_engin_oil = shift_inst_engin_oil;
     //*************************************************************************
     shift_inst_akpp_oil                = *address++ <<8;
     shift_inst_akpp_oil                |= *address++;
        inst_akpp_oil = shift_inst_akpp_oil;
     //*************************************************************************
     shift_inst_oil_reducer             = *address++ <<8;
     shift_inst_oil_reducer             |= *address++;
        inst_oil_reducer = shift_inst_oil_reducer;
     //*************************************************************************
     shift_inst_fuel_filter             = *address++ <<8;
     shift_inst_fuel_filter             |= *address++;
        inst_fuel_filter = shift_inst_fuel_filter;
     //*************************************************************************
     shift_inst_air_filter              = *address++ <<8;
     shift_inst_air_filter              |= *address++;
        shift_inst_air_filter = shift_inst_air_filter;
     //*************************************************************************
     shift_inst_oil_filter              = *address++ <<8;
     shift_inst_oil_filter              |= *address++;
        inst_oil_filter = shift_inst_oil_filter;
     //*************************************************************************
     shift_inst_cleaning_tnvd           = *address++ <<8;
     shift_inst_cleaning_tnvd           |= *address++;
        inst_cleaning_tnvd = shift_inst_cleaning_tnvd;
     //*************************************************************************
     shift_inst_changing_the_injector   = *address++ <<8;
     shift_inst_changing_the_injector   |= *address++;
        inst_changing_the_injector = shift_inst_changing_the_injector;
     //*************************************************************************
     shift_inst_candles_of_chenge       = *address++ <<8;
     shift_inst_candles_of_chenge       |= *address++;
        inst_candles_of_chenge = shift_inst_candles_of_chenge;
     //*************************************************************************
     shift_inst_set_speed_city          = *address++ <<8;
     shift_inst_set_speed_city          |= *address++;
        inst_set_speed_city = shift_inst_set_speed_city;
     //*************************************************************************
     shift_inst_set_speed_trass         = *address++ <<8;
     shift_inst_set_speed_trass         |= *address++;
        inst_set_speed_trass = shift_inst_set_speed_trass;
     //*************************************************************************
     shift_inst_wheel_size_input         = *address++ <<8;
     shift_inst_wheel_size_input         |= *address++;
        inst_wheel_size_input = shift_inst_wheel_size_input;
     //*************************************************************************
}

