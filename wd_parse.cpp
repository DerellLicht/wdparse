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
#include <stdlib.h>
#endif

//  I don't know *why* lint thinks this??
//lint -esym(119, strtod)  Too many arguments (2) for prototype 'strtod(const char *)'

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
// barometer    windspeed   gustspeed direction  
// rainlastmin dailyrain  monthlyrain   yearlyrain  heatindex
//  
//**********************************************************************************
//lint -esym(751, wd_data_p)  local typedef not referenced
typedef struct wd_data_s {
   uint day ;
   uint month ;
   uint year ;
   uint hour ;
   uint minute ;
   double temp ;
   uint humidity ;
   double dewpoint ;
   double barometer ;
   double windspeed ;
   double gustspeed ;
   uint direction ;
   double rainlastmin ;
   double dailyrain ;
   double monthlyrain ;
   double yearlyrain ;
   double heatindex ;
   wd_data_s *next ;
} wd_data_t, *wd_data_p ;

static wd_data_t wd_totals ;

//**********************************************************************************
static wd_data_t wd_current ; //  export to caller

static int wd_parse_data_row(char *instr)
{
   // wd_data_p wdtemp = new wd_data_t ;
   ZeroMemory((char *) &wd_current, sizeof(wd_data_t));
   
// 1  7 2023  0  0 63.7  79 57.1 29.867 0.0 0.0 307  0.000 0.000 0.050 31.799 63.7
//31  7 2023  7 52 61.4  87 57.5 29.979 0.0 0.0  35  0.000 0.000 0.000 0.000 61.4

   //  pull time/data
   //  for first field, do *not* depend on next_field to work properly...
   //  If no space/tab is present, this fails
   // instr = next_field(instr);
   if (*instr == SPC) {
      instr++ ;
   }
   wd_current.day = (uint) atoi(instr);
   instr = next_field(instr);
   wd_current.month = (uint) atoi(instr);
   instr = next_field(instr);
   wd_current.year = (uint) atoi(instr);
   instr = next_field(instr);
   wd_current.hour = (uint) atoi(instr);
   instr = next_field(instr);
   wd_current.minute = (uint) atoi(instr);
   
   //  now, collect data
   instr = next_field(instr);
   wd_current.temp = (double) strtod(instr, NULL);
   instr = next_field(instr);
   wd_current.humidity = (uint) atoi(instr);
   instr = next_field(instr);
   wd_current.dewpoint = (double) strtod(instr, NULL);
   instr = next_field(instr);
   wd_current.barometer = (double) strtod(instr, NULL);
   instr = next_field(instr);
   wd_current.windspeed = (double) strtod(instr, NULL);
   instr = next_field(instr);
   wd_current.gustspeed = (double) strtod(instr, NULL);
   instr = next_field(instr);
   wd_current.direction = (uint) atoi(instr);
   instr = next_field(instr);
   wd_current.rainlastmin = (double) strtod(instr, NULL);
   instr = next_field(instr);
   wd_current.dailyrain = (double) strtod(instr, NULL);
   instr = next_field(instr);
   wd_current.monthlyrain = (double) strtod(instr, NULL);
   instr = next_field(instr);
   //  this selective filter will help us choose accurate data...
   //  Weather Display program resets annual rainfall totals
   //  on July 1st, so looking at December data for a year,
   //  may give misleading yearly results.
   if (wd_current.month <= 6) {
      wd_current.yearlyrain = (double) strtod(instr, NULL);
   }
   instr = next_field(instr);
   wd_current.heatindex = (double) strtod(instr, NULL);
   
   return 0;
}

//**********************************************************************************
static wd_data_t wd_max_temp ;
static wd_data_t wd_min_temp ;
static wd_data_t wd_max_wind ;
static wd_data_t wd_max_gust ;
static wd_data_t wd_max_rain_daily ;
static wd_data_t wd_max_rain_yearly ;

static void wd_check_records(void)
{
   if (wd_current.temp > wd_max_temp.temp) {
      wd_max_temp = wd_current ;
   }
   //  this min-temp computation wouldn't be sufficient in areas
   //  which get temps lower than zero.
   if (wd_current.temp > 1.0  &&
       wd_current.temp < wd_min_temp.temp) {
      wd_min_temp = wd_current ;
   }
   if (wd_current.windspeed > wd_max_wind.windspeed) {
      wd_max_wind = wd_current ;
   }
   if (wd_current.gustspeed > wd_max_gust.gustspeed) {
      wd_max_gust = wd_current ;
   }
   if (wd_current.dailyrain > wd_max_rain_daily.dailyrain) {
      wd_max_rain_daily = wd_current ;
   }
   //  This doesn't always give desirable results;
   //  Since rainfall totals reset on July 1st,
   //  if we got more rain in second half of year, than in first half,
   //  this gives misleading data...
   //  We need a way to monitor these values only in May/June
   if (wd_current.yearlyrain > wd_max_rain_yearly.yearlyrain) {
      wd_max_rain_yearly = wd_current ;
   }
}

//**********************************************************************************
void wd_show_records(void)
{
   puts("");
   printf("%02u/%02u/%04u, %02u:%02u  Max Temp:         %5.1f\n",
            wd_max_temp.month  , wd_max_temp.day    , wd_max_temp.year   ,
            wd_max_temp.hour   , wd_max_temp.minute ,
            wd_max_temp.temp);
   printf("%02u/%02u/%04u, %02u:%02u  Min Temp:         %5.1f\n",
            wd_min_temp.month  , wd_min_temp.day    , wd_min_temp.year   ,
            wd_min_temp.hour   , wd_min_temp.minute ,
            wd_min_temp.temp);
   printf("%02u/%02u/%04u, %02u:%02u  Max Wind Average: %5.1f\n",
            wd_max_wind.month  , wd_max_wind.day    , wd_max_wind.year   ,
            wd_max_wind.hour   , wd_max_wind.minute ,
            wd_max_wind.windspeed);
   printf("%02u/%02u/%04u, %02u:%02u  Max Wind Gust:    %5.1f\n",
            wd_max_gust.month  , wd_max_gust.day    , wd_max_gust.year   ,
            wd_max_gust.hour   , wd_max_gust.minute ,
            wd_max_gust.gustspeed);
   printf("%02u/%02u/%04u, %02u:%02u  Max Daily Rain:   %6.2f\n",
            wd_max_rain_daily.month  , wd_max_rain_daily.day    , wd_max_rain_daily.year   ,
            wd_max_rain_daily.hour   , wd_max_rain_daily.minute ,
            wd_max_rain_daily.dailyrain);
   printf("%02u/%02u/%04u, %02u:%02u  Yearly Rain:      %6.2f\n",
            wd_max_rain_yearly.month  , wd_max_rain_yearly.day    , wd_max_rain_yearly.year   ,
            wd_max_rain_yearly.hour   , wd_max_rain_yearly.minute ,
            wd_max_rain_yearly.yearlyrain);
   puts("");
}

//**********************************************************************************
void wd_init_summary_data(void)
{
   ZeroMemory((char *) &wd_totals, sizeof(wd_data_t));
   ZeroMemory((char *) &wd_max_temp, sizeof(wd_data_t));
   ZeroMemory((char *) &wd_min_temp, sizeof(wd_data_t));
   wd_min_temp.temp = 1000.0 ;
   ZeroMemory((char *) &wd_max_wind, sizeof(wd_data_t));
   ZeroMemory((char *) &wd_max_gust, sizeof(wd_data_t));
   ZeroMemory((char *) &wd_max_rain_daily, sizeof(wd_data_t));
}

//**********************************************************************************
static char const spin_chars[] = { '\\', '|', '/', '-', 0 };

static void status_spinner_update(void)
{
   static uint spin_idx = 0 ;
   printf("\r[%c]", spin_chars[spin_idx]);
   spin_idx++ ;
   if (spin_chars[spin_idx] == 0) {
      spin_idx = 0 ;
   }
}

//**********************************************************************************
int process_wd_log_file(char *filename)
{
   uint lcount ;
   status_spinner_update();
   // 10,2021 102021lg.txt
   sprintf(fpath, "%s\\%s", base_path, filename) ;
   FILE *fptr = fopen(fpath, "rt");
   if (fptr == NULL) {
      printf("%3u: %s\n", (uint) errno, fpath);
      return errno;
   }
   //  read first line, which contains labels
   int inlen = (int) fgets(inpstr, sizeof(inpstr), fptr);
   if (inlen == 0) {
      printf("%3u: %s\n", errno, filename);  //lint !e705
      return errno ;
   }
   strip_newlines(inpstr);
   uint valid = wd_parse_label_line(inpstr);
   if (valid != 0) {
      printf("line parse error [%u]: %s\n", valid, filename);
      goto exit_point ;
   }
   //  now, scan through all the rest of the lines, and collect data
   lcount = 0 ;
   while(fgets(inpstr, sizeof(inpstr), fptr) != 0) {
      strip_newlines(inpstr);
      lcount++ ;
      int result = wd_parse_data_row(inpstr);
      if (result != 0) {
         printf("parse error [L %u]: %s\n", lcount, filename);
         goto exit_point ;
      }
      
      //  next, check max/min values against static wd_current
      wd_check_records();
   }
   
exit_point:   
   fclose(fptr);

   return 0 ;
}
