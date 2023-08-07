//*****************************************************************
//  NSORT.CPP -   linked-list sorting routines                       
//                                                                 
//  Written by:   Daniel D. Miller
//*****************************************************************

#include <windows.h>

#include "common.h"
#include "wd_info.h"

//lint -e1013  Symbol 'LowPart' not a member of class '_LARGE_INTEGER'
//lint -e40    Undeclared identifier 'LowPart'
//lint -e63    Expected an lvalue

static struct ffdata *merge(struct ffdata *a, struct ffdata *b);
//************************************************************
//  the following object is a dummy point structure
//  which is used by merge_sort.  The main code must
//  allocate a strucure for this to point to.
//  
//  A global function pointer is also required by the
//  sort routine.  This will point to a function which
//  accepts two structure pointers as arguments, and
//  returns:
//  
//     >0 if a > b
//    ==0 if a == b
//     <0 if a < b
//  
//************************************************************
static struct ffdata *z = NULL ;
static int (*sort_fcn) (struct ffdata *a, struct ffdata *b) ;

//****************************************************
//  allocate a dummy structure for merge_sort()
//****************************************************
static int init_sort(void) 
{
   // z = (struct ffdata *) malloc(sizeof(ffdata)) ;
   //  new does not return errno on OUT_OF_MEMORY, it just aborts
   z = (struct ffdata *) new ffdata ;
   // if (z == NULL)
   //    error_exit(OUT_OF_MEMORY, NULL) ;
   z->next = NULL ;
   return 0 ;
}

//****************************************************
// void free_file_structs(void)
//    {
//    if (z != NULL)  delete z ;
//    }

//*********************************************************
static int sort_month(struct ffdata *a, struct ffdata *b)
{
   if (a->month > b->month)  return(1) ;
   else if (b->month > a->month)  return(-1) ;
   else return(0) ;
}  //lint !e818

//*********************************************************
static int sort_year(struct ffdata *a, struct ffdata *b)
{
   if (a->year > b->year)  return(1) ;
   else if (b->year > a->year)  return(-1) ;
   else return(0) ;
}  //lint !e818

//***************************************************************************
//  This routine recursively splits linked lists into two parts, 
//  passing the divided lists to merge() to merge the two sorted lists.
//***************************************************************************
static struct ffdata *merge_sort(struct ffdata *c)
   {
   struct ffdata *a, *b, *prev ;
   int pcount = 0 ;
   int j = 0 ;

   if ((c != NULL) && (c->next != NULL))
      {
      a = c ;
      while (a != NULL)
         {
         pcount++ ;
         a = a->next  ;
         }
      a = c ;
      b = c ;
      prev = b ;
      while (j <  pcount/2)
         {
         j++ ;
         prev = b ;
         b = b->next ;
         }
      prev->next = NULL ;  //lint !e771

      return merge(merge_sort(a), merge_sort(b)) ;
      }
   return c ;
   }

//*********************************************************
//  This routine merges two sorted linked lists.
//*********************************************************
static struct ffdata *merge(struct ffdata *a, struct ffdata *b)
   {
   struct ffdata *c ;
   c = z ;

   do
      {
      int x = sort_fcn(a, b) ;
      if (x <= 0)
         {
         c->next = a ;
         c = a ;
         a = a->next ;
         }
      else
         {
         c->next = b ;
         c = b ;
         b = b->next ;  //lint !e613
         }
      }
   while ((a != NULL) && (b != NULL));

   if (a == NULL)  c->next = b ;  //lint !e613
             else  c->next = a ;  //lint !e613
   return z->next ;
   }

//*********************************************************
//  This intermediate function is used because I want
//  merge_sort() to accept a passed parameter,
//  but in this particular application the initial
//  list is global.  This function sets up the global
//  comparison-function pointer and passes the global
//  list pointer to merge_sort().
//*********************************************************
static void sort_files(int (*current_sort)(struct ffdata *a, struct ffdata *b))
{
   sort_fcn = current_sort ;
   ftop = merge_sort(ftop) ;
}

//*****************************************************************
void sort_filelist(void)
{
   if (z == 0) {
      init_sort() ;  //lint !e534
   }

   sort_files(sort_month) ;
   sort_files(sort_year) ;
   
}

//lint +e1013  Symbol 'LowPart' not a member of class '_LARGE_INTEGER'
//lint +e40    Undeclared identifier 'LowPart'
//lint +e63    Expected an lvalue


