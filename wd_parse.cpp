//**********************************************************************************
//  wd_parse.cpp 
//  Read Weather Display log files and extract desired data
//  
//  Written by:  Derell Licht
//**********************************************************************************

#include <windows.h>
#include <stdio.h>

#include "common.h"
#include "wd_info.h"

//**********************************************************************************
int process_wd_log_file(ffdata const * const ftemp)
{
   printf("%2u,%4u %s\n", ftemp->month, ftemp->year, ftemp->filename);
   return 0 ;
}
