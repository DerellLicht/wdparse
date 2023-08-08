//************************************************************
//  Copyright (c) 2023 Daniel D. Miller                       
//  media_list.cpp - list info about various media files
//                                                                 
//  Written by:   Daniel D. Miller
//  
//************************************************************

//lint -esym(769, data_req_t::WD_MAX_TEMP, data_req_t::WD_MIN_TEMP, data_req_t::WD_MAX_WIND)
//lint -esym(769, data_req_t::WD_MAX_GUST, data_req_t::WD_MAX_RAIN_DAILY)
//lint -esym(759, data_req)

typedef enum {
WD_UNKNOWN=0,
WD_MAX_TEMP,
WD_MIN_TEMP,
WD_MAX_WIND,
WD_MAX_GUST,
WD_MAX_RAIN_DAILY
} data_req_t ;

extern data_req_t data_req ;

struct ffdata {
   uchar          attrib ;
   FILETIME       ft ;
   ULONGLONG      fsize ;
   char           *filename ;
   uchar          dirflag ;
   uint           month ;  //  extracted from filename
   uint           year ;   //  extracted from filename
   struct ffdata  *next ;
} ;
extern ffdata *ftop  ;

//  wd_parse.cpp
void wd_init_summary_data(void);
int process_wd_log_file(ffdata const * const ftemp);

//  nsort.cpp
void sort_filelist(void);
