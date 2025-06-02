
#pragma once
#ifndef __LISTS_LIB_H__
#define __LISTS_LIB_H__



/*-----------------------------------------------------------------------------------------------*/
/* extern variables                                                                              */
/*-----------------------------------------------------------------------------------------------*/
volatile extern char device_ok;
volatile extern char device_nok;
volatile extern char last_added_device_ok_nok[48];

volatile extern char last_device_in_ok_list;


/*-----------------------------------------------------------------------------------------------*/
/* functions headers                                                                             */
/*-----------------------------------------------------------------------------------------------*/
void init_spiffs();


/**
 * @brief automatically classifies device as ok or nok ↓
 */
void auto_classify_ok_nok();

/**
 * @brief check if device mac is in ok_list.csv , if not, write it there 
 * @param _mac_u 
 */
void add_to_ok_list(char * _mac_u); //_mac_u ends with '\0' naturally => no problem in using strstr


/**
 * @brief check if device mac is in nok_list.csv , if not, write it there 
 * @param _mac_u 
 */
void add_to_nok_list(char * _mac_u); 










#endif // __LISTS_LIB_H__