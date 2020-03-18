/*
 *  mud_misc.c -- some handy things
 *
 *   Copyright (C) 1994-2010 TRIUMF (Vancouver, Canada)
 *
 *   Authors: T. Whidden, D. Arseneau
 *   
 *   Released under the GNU LGPL - see http://www.gnu.org/licenses
 *
 *   This program is free software; you can distribute it and/or modify it under 
 *   the terms of the Lesser GNU General Public License as published by the Free 
 *   Software Foundation; either version 2 of the License, or any later version. 
 *   Accordingly, this program is distributed in the hope that it will be useful, 
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *   or FITNESS FOR A PARTICULAR PURPOSE. See the Lesser GNU General Public License 
 *   for more details.
 *
 *  Revision history:
 *   v1.0  03-Feb-1994  [T. Whidden] Initial version
 *   v1.1  12-Mar-1994  [T. Whidden] Added mailbox utils
 *   v2.0  07-Jul-1994  [TW] Separated from vaxc_utils.c
 *   v3.0  20-Feb-1996  TW  Added gmf_time.c, renamed to mud_misc.c
 *         04-Mar-1996  TW  Removed memory allocation routines
 *   v4.0  02-Dec-2009  DA  Use mud TIME type, not system time_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include "mud.h"


/* 
 *  GMF_TIME - FORTRAN/C time interface by Michael Montour. Last
 *  modified 6/29/93. See GMF_TIME.README for more info. 
 */

/* 
 *  GMF_MKTIME calls C's mktime() function. 
 *  The input is an array of longwords in the order Y-M-D-H-M-S. The Month 
 *  field is from 1-12, instead of C's 0-11. Year is years from 1900, and
 *  everything else is normal.  
 *  OLD COMMENT: 
 *  Currently, no Daylight Savings information is supported - standard 
 *  time is assumed.   ** This will be fixed. ** HA
 *  2001-May-4 DA fixes DST by using Tm.tm_isdst = -1, instead of 0 (rtfm)
 */
void 
GMF_MKTIME( TIME  *out, INT32 *input )
{

  struct tm Tm;

  Tm.tm_year = input[0];
  Tm.tm_mon  = input[1]-1;
  Tm.tm_mday = input[2];
  Tm.tm_hour = input[3];
  Tm.tm_min  = input[4];
  Tm.tm_sec  = input[5];
  Tm.tm_isdst = -1;
  
  *out = (TIME)mktime (&Tm);

}


/* 
 *  Returns a longword containing the current system time. 
 */
void 
GMF_TIME( TIME* out )
{
    *out = (TIME)time( NULL );
}


/* 
 *  Calls C's localtime(), writes the info into a 6-longword array (as in the
 *  input to GMF_MKTIME). Currently the extra stuff is ignored, but this is
 *  easy to change. 
 */
void 
GMF_LOCALTIME( TIME *in, INT32 *out )
{
  struct tm *Tm;
  time_t timet;

  timet = *in;
  Tm = localtime( &timet );
  out[0] = Tm->tm_year;
  out[1] = Tm->tm_mon+1;
  out[2] = Tm->tm_mday;
  out[3] = Tm->tm_hour;
  out[4] = Tm->tm_min;
  out[5] = Tm->tm_sec;
}

