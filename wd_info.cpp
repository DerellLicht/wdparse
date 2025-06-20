//**********************************************************************************
//  wdparse.cpp 
//  Read Weather Display log files and extract desired data
//  
//  Written by:  Derell Licht
//**********************************************************************************

#include <windows.h>
#include <stdio.h>

#include "common.h"
#include "wd_info.h"
#include "qualify.h"

WIN32_FIND_DATA fdata ; //  long-filename file struct

//  per Jason Hood, this turns off MinGW's command-line expansion, 
//  so we can handle wildcards like we want to.                    
//lint -e765  external '_CRT_glob' could be made static
//lint -e714  Symbol '_CRT_glob' not referenced
int _CRT_glob = 0 ;

uint filecount = 0 ;

//lint -esym(843, show_all)
bool show_all = true ;

//lint -esym(534, FindClose)  // Ignoring return value of function
//lint -esym(818, filespec, argv)  //could be declared as pointing to const
//lint -e10  Expecting '}'

static uint year_to_select = 0 ;
static uint month_to_select = 0 ;

//************************************************************
ffdata_t *ftop  = NULL;
static ffdata_t *ftail = NULL;

//**********************************************************************************
//  filename format is m[m]yyyylg.txt
//**********************************************************************************
static uint extract_month_and_year(char const *fname, char *mystr)
{
   uint len = 0 ;
   while (LOOP_FOREVER) {
      if (!IsCharNum(*fname)) {
         break ;
      }
      mystr[len] = *fname ;
      len++ ;
      fname++ ;
   }
   mystr[len] = 0 ;
   return len ;
}

//**********************************************************************************
int read_files(char *filespec)
{
   int done, fn_okay ;
   HANDLE handle;
   ffdata *ftemp;

   handle = FindFirstFile (filespec, &fdata);
   //  according to MSDN, Jan 1999, the following is equivalent
   //  to the preceding... unfortunately, under Win98SE, it's not...
   // handle = FindFirstFileEx(target[i], FindExInfoStandard, &fdata, 
   //                      FindExSearchNameMatch, NULL, 0) ;
   if (handle == INVALID_HANDLE_VALUE) {
      return -errno;
   }

   //  loop on find_next
   done = 0;
   while (!done) {
      if (!show_all) {
         if ((fdata.dwFileAttributes & 
            (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM)) != 0) {
            // fn_okay = 0 ;
            goto search_next_file;
         }
      }
      //  filter out directories if not requested
      if ((fdata.dwFileAttributes & FILE_ATTRIBUTE_VOLID) != 0)
         fn_okay = 0;
      else if ((fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
         fn_okay = 1;
      //  For directories, filter out "." and ".."
      else if (fdata.cFileName[0] != '.') //  fn=".something"
         fn_okay = 1;
      else if (fdata.cFileName[1] == 0)   //  fn="."
         fn_okay = 0;
      else if (fdata.cFileName[1] != '.') //  fn="..something"
         fn_okay = 1;
      else if (fdata.cFileName[2] == 0)   //  fn=".."
         fn_okay = 0;
      else
         fn_okay = 1;
         
      if (fn_okay  &&  strncmp(fdata.cFileName, "backup", 6) == 0) {
         fn_okay = 0;
      }

      if (fn_okay) {
         // printf("DIRECTORY %04X %s\n", fdata.attrib, fdata.cFileName) ;
         // printf("%9ld %04X %s\n", fdata.file_size, fdata.attrib, fdata.cFileName) ;

         //****************************************************
         //  allocate and initialize the structure
         //****************************************************
         // ftemp = (struct ffdata *) malloc(sizeof(ffdata)) ;
         ftemp = (struct ffdata *) new ffdata ;
         //  new does not return errno on OUT_OF_MEMORY, it just aborts
         // if (ftemp == NULL) {
         //    return -errno;
         // }
         memset((char *) ftemp, 0, sizeof(ffdata));

         //  convert filename to lower case if appropriate
         // if (!n.ucase)
         //    strlwr(fblk.name) ;

         ftemp->attrib = (uchar) fdata.dwFileAttributes;

         //  convert file time
         // if (n.fdate_option == FDATE_LAST_ACCESS)
         //    ftemp->ft = fdata.ftLastAccessTime;
         // else if (n.fdate_option == FDATE_CREATE_TIME)
         //    ftemp->ft = fdata.ftCreationTime;
         // else
         //    ftemp->ft = fdata.ftLastWriteTime;
         ftemp->ft = fdata.ftLastAccessTime;

         //  convert file size
         u64toul iconv;
         iconv.u[0] = fdata.nFileSizeLow;
         iconv.u[1] = fdata.nFileSizeHigh;
         ftemp->fsize = iconv.i;

         // ftemp->filename = (char *) malloc(strlen ((char *) fdata.cFileName) + 1);
         ftemp->filename = (char *) new char[(strlen ((char *) fdata.cFileName) + 1)];
         strcpy (ftemp->filename, (char *) fdata.cFileName);

         ftemp->dirflag = ftemp->attrib & FILE_ATTRIBUTE_DIRECTORY;
         
         //  extract month/year info from filename
         char mystr[7] ;
         uint nlen = extract_month_and_year(ftemp->filename, mystr);
         switch (nlen) {
         case 5:
            ftemp->year = (uint) atoi(&mystr[1]);
            mystr[1] = 0 ;
            ftemp->month = (uint) atoi(mystr);
            break ;
            
         case 6:
            ftemp->year = (uint) atoi(&mystr[2]);
            mystr[2] = 0 ;
            ftemp->month = (uint) atoi(mystr);
            break ;
            
         default:
            ftemp->month = 0 ;
            ftemp->year = nlen ;
            break ;
         }
         
         //  set selector from command-line options
         uint select_mode = 0 ;
         if (year_to_select  != 0)  select_mode |= 1 ;
         if (month_to_select != 0)  select_mode |= 2 ;
         
         switch (select_mode) {
         case 3:  //  month and year selected
            if (year_to_select  != ftemp->year  || 
                month_to_select != ftemp->month) {
               delete ftemp ;
               goto search_next_file;
            }
            break ;
            
         case 2:  //  month selected
            if (month_to_select != ftemp->month) {
               delete ftemp ;
               goto search_next_file;
            }

            break ;
         
         case 1:  //  year selected
            if (year_to_select != ftemp->year) {
               delete ftemp ;
               goto search_next_file;
            }
            break ;
         
         case 0:  //  show summaries over all existing log files
         default:
            break ;
         }
         
         //****************************************************
         //  add the structure to the file list
         //****************************************************
         filecount++;
         if (ftop == NULL) {
            ftop = ftemp;
         }
         else {
            ftail->next = ftemp;
         }
         ftail = ftemp;
      }  //  if file is parseable...

search_next_file:
      //  search for another file
      if (FindNextFile (handle, &fdata) == 0)
         done = 1;
   }

   FindClose (handle);
   return 0;
}

//**********************************************************************************
static void usage(void)
{
   puts("Usage: wdparse [filter_by_year] [filter_by_month] ");
   puts("-h = show usage screen");
   puts("-yYYYY = show max/min stats for given year");
   puts("-mMM = Show max/min stats for given month over all years");
   puts("");
   puts("Month and Year values may be combined; thus:");
   puts("-m8 -y2023 will show stats for August 2023.");
   puts("");
   puts("If neither year nor month are specified,");
   puts("data from all years will be collected.");
}

//**********************************************************************************
char file_spec[MAX_PATH_LEN+1] = "C:\\WeatherDisplay\\logfiles\\*lg.txt" ;

int main(int argc, char **argv)
{
   int result ;
   int idx ;
   for (idx=1; idx<argc; idx++) {
      char *p = argv[idx] ;
      if (*p == '-') {
         p++ ;
         switch (*p) {
         case 'y':
            p++ ;
            year_to_select = (uint) atoi(p);
            break ;
            
         case 'm':
            p++ ;
            month_to_select = (uint) atoi(p);
            break ;
            
         case 'h':
         default:
            usage() ;
            return 1 ;
         }
      }
      else {
         usage() ;
         return 1 ;
      }
   }

   //  09/20/23  Let's actually add support for this...
   //            This will let me look at data for a specific month+year   
   if (year_to_select != 0  &&  month_to_select != 0) {
      printf("filter on month %u in year %u\n", month_to_select, year_to_select);
   }
   else if (year_to_select != 0) {
      printf("filter on year %u\n", year_to_select);
   }
   else if (month_to_select != 0) {
      printf("filter on month %u\n", month_to_select);
   }
   else {
      printf("show data for all years\n");
   }

   // if (file_spec[0] == 0) {
   //    strcpy(file_spec, ".");
   // }

   uint qresult = qualify(file_spec) ; //lint !e732
   if (qresult == QUAL_INV_DRIVE) {
      printf("%s: 0x%X\n", file_spec, qresult);
      return 1 ;
   }
   // printf("file spec: %s\n", file_spec);

   //  Extract base path from first filespec, and strip off filename.
   //  base_path becomes useful when one wishes to perform
   //  multiple searches in one path.
   strcpy(base_path, file_spec) ;
   char *strptr = strrchr(base_path, '\\') ;
   if (strptr != 0) {
       strptr++ ;  //lint !e613  skip past backslash, to filename
      *strptr = 0 ;  //  strip off filename
   }
   base_len = strlen(base_path) ;
   // printf("base path: %s\n", base_path);
   
   wd_init_summary_data();

   //  it looks like target path is:
   // C:\WeatherDisplay\logfiles\*lg.txt
   // where filename is m[m]yyyylg.txt
   
   result = read_files(file_spec);
   if (result < 0) {
      printf("filespec: %s, %s\n", file_spec, strerror(-result));
      return -result ;
   }
   
   //  sort file list by month/year
   sort_filelist();
   
   //**********************************************************************************
   //  NOTE: execution time for this operation is fairly time-consuming.
   //  My Weather Display logfiles (m[m]yyyylg.txt) comprise the following statistics:
   //  > 125 files
   //  > 425 MBytes
   //  > 5,220,424 lines
   //  
   //  Parsing all this requires over 11 seconds on my 2022 gaming machine.
   //  It could possibly be sped up by reading each file into memory once,
   //  then reading lines out of memory, rather than using an fgets() loop
   //  as I currently do.  However, most of those log files are around 3.5MB each,
   //  so I don't think I could handle this in one read; at least in Windows 7 32-bit,
   //  There was a limit of 1MB per allocation.
   //**********************************************************************************
   printf("filespec: %s, %u found\n", file_spec, filecount);
   if (filecount > 0) {
      for (ffdata *ftemp = ftop; ftemp != NULL; ftemp = ftemp->next) {
         result = process_wd_log_file(ftemp->filename) ;  //lint !e534
         if (result != 0) {
            printf("error: %s\n", ftemp->filename);
         }
      }
   
      //  show records
      wd_show_records();
   }
   
   return 0;
}  //lint !e715

