//************************************************************
//  Copyright (c) 2023 Daniel D. Miller                       
//  media_list.cpp - list info about various media files
//                                                                 
//  Written by:   Daniel D. Miller
//  
//************************************************************

typedef struct ffdata {
   uchar          attrib ;
   FILETIME       ft ;
   ULONGLONG      fsize ;
   char           *filename ;
   uchar          dirflag ;
   uint           month ;  //  extracted from filename
   uint           year ;   //  extracted from filename
   struct ffdata  *next ;
} ffdata_t ;

extern ffdata_t *ftop  ;

extern char base_path[MAX_PATH_LEN+1] ;
extern unsigned base_len ;  //  length of base_path

//  wd_parse.cpp
void wd_init_summary_data(void);
int process_wd_log_file(char *filename);
void wd_show_records(void);

//  nsort.cpp
void sort_filelist(void);
