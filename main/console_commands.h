
#pragma once
#ifndef __CONSOLE_COMMANDS_H__
#define __CONSOLE_COMMANDS_H__




/*-----------------------------------------------------------------------------------------------*/
/* types                                                                                         */
/*-----------------------------------------------------------------------------------------------*/




/*-----------------------------------------------------------------------------------------------*/
/* headers                                                                                       */
/*-----------------------------------------------------------------------------------------------*/


void init_console();

static void register_restart();
static int restart(int argc, char ** argv);

static void register_ok_list();
static int ok_list(int argc, char ** argv);

static void register_nok_list();
static int nok_list(int argc, char ** argv);

static void register_last_added_device();
static int last_added_device(int argc, char ** argv);






#endif // __CONSOLE_COMMANDS_H__