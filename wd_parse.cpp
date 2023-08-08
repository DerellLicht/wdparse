//**********************************************************************************
//  wd_parse.cpp 
//  Read Weather Display log files and extract desired data
//  
//  Written by:  Derell Licht
//**********************************************************************************

#include <windows.h>
#include <stdio.h>
#ifdef _lint
#include <errno.h>
#endif

#include "common.h"
#include "wd_info.h"

static char fpath[1024] ;
static char inpstr[260];

//**********************************************************************************
//  file line 1: format line:
// day month year hour minute 
// temperature   humidity     dewpoint   
// barometer    windspeed   gustspeed direction  rainlastmin    
// dailyrain  monthlyrain   yearlyrain  heatindex
//**********************************************************************************
static uint wd_parse_label_line(char *instr)
{
   if (strncmp(instr, "day", 3) != 0) {
      return 1 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "month", 5) != 0) {
      return 2 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "year", 4) != 0) {
      return 3 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "hour", 4) != 0) {
      return 4 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "minute", 6) != 0) {
      return 5 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "temperature", 11) != 0) {
      return 6 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "humidity", 8) != 0) {
      return 7 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "dewpoint", 8) != 0) {
      return 8 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "barometer", 9) != 0) {
      return 9 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "windspeed", 9) != 0) {
      return 10 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "gustspeed", 9) != 0) {
      return 11 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "direction", 9) != 0) {
      return 12 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "rainlastmin", 11) != 0) {
      return 13 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "dailyrain", 9) != 0) {
      return 14 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "monthlyrain", 11) != 0) {
      return 15 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "yearlyrain", 10) != 0) {
      return 16 ;
   }
   instr = next_field(instr);
   if (strncmp(instr, "heatindex", 9) != 0) {
      return 17 ;
   }
   return 0 ;
}

//**********************************************************************************
//  file line 1: format line:
// day month year hour minute 
// temperature   humidity     dewpoint   
// barometer    windspeed   gustspeed direction  rainlastmin    
// dailyrain  monthlyrain   yearlyrain  heatindex
//  
//  for first pass, just collect/display *all* of these values...
// typedef enum {
// WD_UNKNOWN=0,
// WD_MAX_TEMP,
// WD_MIN_TEMP,
// WD_MAX_WIND,
// WD_MAX_GUST,
// WD_MAX_RAIN_DAILY
// } data_req_t ;
//**********************************************************************************


//**********************************************************************************
int process_wd_log_file(ffdata const * const ftemp)
{
   uint lcount ;
   // 10,2021 102021lg.txt
   // printf("%2u,%4u %s\n", ftemp->month, ftemp->year, ftemp->filename);
   sprintf(fpath, "%s\\%s", base_path, ftemp->filename) ;
   FILE *fptr = fopen(fpath, "rt");
   if (fptr == NULL) {
      // printf("%3u: %2u,%4u %s\n", errno, ftemp->month, ftemp->year, ftemp->filename);
      return errno;
   }
   //  read first line, which contains labels
   int inlen = (int) fread(inpstr, 1, sizeof(inpstr), fptr);
   if (inlen == 0) {
      printf("%3u: %2u,%4u %s\n", errno, ftemp->month, ftemp->year, ftemp->filename);  //lint !e705
      return errno ;
   }
   uint valid = wd_parse_label_line(inpstr);
   if (valid != 0) {
      printf("nope [%u]: %2u,%4u %s\n", valid, ftemp->month, ftemp->year, ftemp->filename);
      goto exit_point ;
   }
   //  now, scan through all the rest of the lines, and collect data
   lcount = 0 ;
   while(fread(inpstr, 1, sizeof(inpstr), fptr) != 0) {
      lcount++ ;
   }
   printf("okay: lcount: %u, %2u,%4u %s\n", lcount, ftemp->month, ftemp->year, ftemp->filename);
   
exit_point:   
   fclose(fptr);

   return 0 ;
}
