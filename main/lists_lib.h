
#pragma once
#ifndef __LISTS_LIB_H__
#define __LISTS_LIB_H__



/*-----------------------------------------------------------------------------------------------*/
/* extern variables                                                                              */
/*-----------------------------------------------------------------------------------------------*/
volatile extern bool device_ok;
volatile extern bool device_nok;



/*-----------------------------------------------------------------------------------------------*/
/* functions headers                                                                             */
/*-----------------------------------------------------------------------------------------------*/
void init_spiffs();


/**
 * @brief if ok_pressed => add device mac to ok_list ; same for nok_list
 */
void update_lists();

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