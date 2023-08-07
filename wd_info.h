//************************************************************
//  Copyright (c) 2023 Daniel D. Miller                       
//  media_list.cpp - list info about various media files
//                                                                 
//  Written by:   Daniel D. Miller
//  
//************************************************************

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
int process_wd_log_file(ffdata const * const ftemp);

//  nsort.cpp
void sort_filelist(void);
