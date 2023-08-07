//**********************************************************************************
//  Copyright (c) 2023 Daniel D. Miller                       
//  media_list.cpp - list info about various media files
//                                                                 
//  Written by:   Daniel D. Miller
//  
//**********************************************************************************

//lint -esym(552, base_len)   Symbol not accessed
//lint -esym(759, base_len)   header declaration for symbol could be moved from header to module
extern char base_path[PATH_MAX] ;
extern unsigned base_len ;  //  length of base_path

//lint -esym(759, total_ptime)  header declaration for symbol could be moved from header to module
extern double total_ptime ;

//************************************************************
struct ffdata {
   uchar          attrib ;
   FILETIME       ft ;
   ULONGLONG      fsize ;
   char           *filename ;
   uchar          dirflag ;
   struct ffdata  *next ;
} ;

