/*
 *  mud_fort.c -- FORTRAN wrappers to allow basic read access
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
 *   v1.00  07-Feb-1994  [T. Whidden] Initial version
 *   v1.1      Oct-2001  [D. Arseneau] Many changes
 */

#ifndef __cplusplus /* No fortran API if using C++ compiler (cfortran.h doesn't work) */

#include <stdio.h>
#include "mud.h"

#ifdef NO_STDARG
#include <varargs.h>
#else
#include <stdarg.h>
#endif /* NO_STDARG */
/*
 *  C preprossor directives to mangle C function names to interact
 *  with fortran.  Type function names as 
 *  MANGLE(type,MixedCaseName,UPPERCASENAME,lowercasename)
 *  if GNU fileutils are expected, then use aliases to make all three
 *  underscore possibilities.
 *
 *  Control the dialect by defining...
 *    one of:  caseany, caseupper, caselower (default caselower)
 *    one of:  nounderscore, oneunderscore, twounderscores (default oneunderscore)
 *
 *    VMS automatically chooses caseany, nounderscore (for VAX fortran)
 *    f2c automatically chooses caselower, twounderscores
 *    g77 changes defaults to be caselower, twounderscores
 */
#ifdef __STDC__
#define CCT(A,B) A##B
#else
#define CCT(A,B) A/**/B
#endif

#ifdef VMS
#define caseany 1
#define nounderscore 1
#endif

#ifdef f2c
#define caselower 1
#define twounderscores 1
#endif

#ifdef g77
#ifndef caseany
#ifndef caseupper
#define caselower 1
#endif
#endif
#ifndef nounderscore
#ifndef oneunderscore
#define twounderscores 1
#endif
#endif
#endif /* g77 */


#ifdef caseany
#define MANGLE(TY,MCN,UCN,LCN) CCU(TY,MCN)
#else
#ifdef caseupper
#define MANGLE(TY,MCN,UCN,LCN) CCU(TY,UCN)
#else
#define MANGLE(TY,MCN,UCN,LCN) CCU(TY,LCN)
#endif
#endif

#if defined(__gnu_linux__) && !defined(__cplusplus)
/* Expect GNU Linux gcc and fileutils, so make aliases for underscore variants */

#define CCU(TY,NAM) \
 TY CCT(NAM,_) () __attribute__ ((weak, alias ( #NAM ))); \
 TY CCT(NAM,__) () __attribute__ ((weak, alias ( #NAM ))); \
 TY NAM

#else /* not GNU fileutils, so choose one underscoring variant */

#ifdef nounderscore
#define CCU(TY,NAM) TY NAM
#else 
#ifdef twounderscores
#define CCU(TY,NAM) TY CCT(NAM,__)
#else
#define CCU(TY,NAM) TY CCT(NAM,_)
#endif
#endif

#endif /* end GNU fileutils */


#ifdef VMS
#include descrip
typedef struct dsc$descriptor_s s_dsc;
#endif /* VMS */



#ifdef VMS
static char*
setstrtodsc( char** pStr, s_dsc* dsc )
     /*
      * Null-terminate a string in a descriptor;
      * set a string pointer to point to it.
      */
{
    *pStr = dsc->dsc$a_pointer;
    (*pStr)[dsc->dsc$w_length] = '\0';

    return( *pStr );
}


static s_dsc*
setdsctostr( s_dsc* dsc, char* str )
     /*
      * Set a string descriptor to point to a C-string
      */
{
    dsc->dsc$w_length = _strlen( str );
    dsc->dsc$a_pointer = str;
    dsc->dsc$b_class = DSC$K_CLASS_S;	/* String dsc class */
    dsc->dsc$b_dtype = DSC$K_DTYPE_T;	/* Ascii string type */

    return( dsc );
}


static s_dsc*
setdsctobuf( s_dsc* dsc, char* buf, int len )
     /*
      * Set a string descriptor to point to a non-terminated string
      */
{
    dsc->dsc$w_length = len;
    dsc->dsc$a_pointer = buf;
    dsc->dsc$b_class = DSC$K_CLASS_S;	/* String dsc class */
    dsc->dsc$b_dtype = DSC$K_DTYPE_T;	/* Ascii string type */

    return( dsc );
}


static s_dsc*
strtodsc( s_dsc* dsc, char* str )
     /*
      * Copy a C-string into a string descriptor (which must have
      * its storage space already allocated)
      */
{
    int len;

    len = _strlen( str );
    strncpy( dsc->dsc$a_pointer, str, len );
    dsc->dsc$w_length = len;
    dsc->dsc$b_class = DSC$K_CLASS_S;	/* String dsc class */
    dsc->dsc$b_dtype = DSC$K_DTYPE_T;	/* Ascii string type */

    return( dsc );
}


static char*
dsctostr( char* str, s_dsc* dsc )
     /*
      * Copy a string-descriptor string into a C string, (which
      * must already be allocated).
      */
{
    strncpy( str, dsc->dsc$a_pointer, dsc->dsc$w_length );
    str[dsc->dsc$w_length] = '\0';

    return( str );
}
#else /* not VMS */

#ifdef __gnu_linux__
#define mf_cstrnlen strnlen
#else
static int 
mf_cstrnlen( char* s, int lm )
     /*
      * "strnlen": strlen up to a limit
      */
{
  int l;
  for( l=0; l<lm; l++ )
    if( s[l] == '\0' ) return( l );
  return( lm );
}
#endif

static int 
mf_fstrnlen( char* s, unsigned lm )
     /*
      * "len_trim" strlen for a fortran (space padded) string
      */
{
  int i;
  for( i=lm-1; i>=0; i-- ) 
    if( s[i] != ' ' && s[i] != '\0' ) return( i+1 );
  return( 0 );
}

static void
cpcstrtofstr( char* cString, char* fString, unsigned lenf )
     /*
      * Copy a C string into a fortran string
      */
{
    unsigned i, l;
    l = mf_cstrnlen( cString, lenf );
    for( i=0; i<lenf; i++)
      fString[i]=(i<l? cString[i]: '\0');
    return;
}

static void
cpfstrtocstr( char* cString, char* fString, unsigned lenf )
     /*
      * Copy a fortran string into a C string, assuming the
      * C string has enough storage.
      */
{
    int l;
    l = mf_fstrnlen( fString, lenf );
    strncpy( cString, fString, l );
    cString[l]='\0';
    return;
}

static void
setcstrasfstr( char** pcString, char* fString, unsigned lenf )
     /*
      * Set C string pointer to a fortran string.  The fortran
      * string gets a null character inserted as termination.
      */
{
    unsigned i = 0;
    int l;
    l = mf_fstrnlen( fString, lenf );
    *pcString = fString;
    *pcString[ ( l<lenf ? l : lenf-1 ) ]='\0';
    return;
}

#define CBLEN 255
static char mf_chbuf[CBLEN];

#endif /* VMS */


#ifdef VMS

void
fMUD_ctofString( s_dsc* fString, char** pcString )
{
    strtodsc( fString, *pcString );
}

#else

MANGLE(void,fMUD_ctofString,FMUD_CTOFSTRING,fmud_ctofstring)
    ( char* fString, char** pcString, unsigned lenf )
{
    cpcstrtofstr( *pcString, fString, lenf );
}

#endif /* VMS */


#ifdef VMS

void
fMUD_ftocString( char** pcString, s_dsc* fString )
{
    setstrtodsc( pcString, fString );
}

#else

MANGLE(void,fMUD_ftocString,FMUD_FTOCSTRING,fmud_ftocstring)
    ( char** pcString, char* fString, unsigned lenf )
{
    setcstrasfstr( pcString, fString, lenf );
}

#endif /* VMS */


#ifdef VMS

FILE*
fMUD_openInput( s_dsc* pdsc_fname )
{
    char* fname;

    setstrtodsc( &fname, pdsc_fname );
    return( MUD_openInput( fname ) );
}

#else

MANGLE(FILE*,fMUD_openInput,FMUD_OPENINPUT,fmud_openinput)
    ( char* ffname, unsigned flen )
{
    cpfstrtocstr( mf_chbuf, ffname, (CBLEN<flen? CBLEN: flen) );
    return( MUD_openInput( mf_chbuf ) );
}

#endif /* VMS */

#ifdef VMS

FILE*
fMUD_openInOut( s_dsc* pdsc_fname )
{
    char* fname;

    setstrtodsc( &fname, pdsc_fname );
    return( MUD_openInOut( fname ) );
}

#else

MANGLE(FILE*,fMUD_openInOut,FMUD_OPENINOUT,fmud_openinout)
    ( char* ffname, unsigned flen )
{
    cpfstrtocstr( mf_chbuf, ffname, (CBLEN<flen? CBLEN: flen) );
    return( MUD_openInOut( mf_chbuf ) );
}

#endif /* VMS */


#ifdef VMS

FILE*
fMUD_openOutput( s_dsc* pdsc_fname )
{
    char* fname;

    setstrtodsc( &fname, pdsc_fname );
    return( MUD_openOutput( fname ) );
}

#else

MANGLE(FILE*,fMUD_openOutput,FMUD_OPENOUTPUT,fmud_openoutput)
    ( char* ffname, unsigned flen )
{
    cpfstrtocstr( mf_chbuf, ffname, (CBLEN<flen? CBLEN: flen) );
    return( MUD_openOutput( mf_chbuf ) );
}

#endif /* VMS */


MANGLE(void,fMUD_close,FMUD_CLOSE,fmud_close)( FILE**  pFio )
{
    fclose( *pFio );
}


MANGLE(void,fMUD_rewind,FMUD_REWIND,fmud_rewind)( FILE** pFio )
{
    rewind( *pFio );
}


MANGLE(void,fMUD_freeOne,FMUD_FREEONE,fmud_freeone)( MUD_SEC* pMUD )
{
    if( pMUD == NULL ) return;

    (*pMUD->core.proc)( MUD_FREE, NULL, pMUD );
}


MANGLE(void,fMUD_freeAll,FMUD_FREEALL,fmud_freeall)( MUD_SEC** ppMUD_head )
{
    MUD_free( *ppMUD_head );
}


MANGLE(int,fMUD_writeEnd,FMUD_WRITEEND,fmud_writeend)( FILE** pFout )
{
    return( MUD_writeEnd( *pFout ) );
}


MANGLE(int,fMUD_write,FMUD_WRITE,fmud_write)( FILE** pFout, MUD_SEC* pMUD )
{
    return( MUD_write( *pFout, pMUD, MUD_ONE ) );
}


MANGLE(int,fMUD_writeGrpStart,FMUD_WRITEGRPSTART,fmud_writegrpstart)
    ( FILE** pFout, MUD_SEC_GRP* pMUD_parentGrp, MUD_SEC_GRP* pMUD_grp, int* pNumMems )
{
    return( MUD_writeGrpStart( *pFout, pMUD_parentGrp, pMUD_grp, *pNumMems ) );
}


MANGLE(int,fMUD_writeGrpMem,FMUD_WRITEGRPMEM,fmud_writegrpmem)
    ( FILE** pFout, MUD_SEC_GRP* pMUD_grp, MUD_SEC* pMUD )
{
    return( MUD_writeGrpMem( *pFout, pMUD_grp, pMUD ) );
}


MANGLE(int,fMUD_writeGrpEnd,FMUD_WRITEGRPEND,fmud_writegrpend)
    (  FILE** pFout, MUD_SEC_GRP* pMUD_grp )
{
    return( MUD_writeGrpEnd( *pFout, pMUD_grp ) );
}


MANGLE(MUD_SEC*,fMUD_readFile,FMUD_READFILE,fmud_readfile)( FILE** pFin )
{
  return( (MUD_SEC*)MUD_readFile( *pFin ) );
}


MANGLE(int,fMUD_read,FMUD_READ,fmud_read)( FILE** pFin, MUD_SEC* pMUD_ret )
{
    MUD_SEC* pMUD_new;

    pMUD_new = (MUD_SEC*)MUD_read( *pFin, MUD_ONE );
    if( pMUD_new == NULL ) return( 0 );

    bcopy( pMUD_new, pMUD_ret, MUD_sizeOf( pMUD_new ) );
    _free( pMUD_new );
    return( 1 );
}


MANGLE(int,fMUD_search,FMUD_SEARCH,fmud_search)
#ifdef NO_STDARG
( va_alist )
    va_dcl
#else
( MUD_SEC** ppMUD_head, MUD_SEC* pMUD_ret, ... )
#endif /* NO_STDARG */
{
    va_list args;
#ifdef NO_STDARG
    MUD_SEC** ppMUD_head;
    MUD_SEC* pMUD_ret;
#endif /* NO_STDARG */ 
    UINT32 argsArray[9];
    MUD_SEC* pMUD;
    int i;

    bzero( argsArray, 9*sizeof( UINT32 ) );

#ifdef NO_STDARG
    va_start( args );
    ppMUD_head = va_arg( args, MUD_SEC** );
    pMUD_ret = va_arg( args, MUD_SEC* );
#else
    va_start( args, pMUD_ret );
#endif /* NO_STDARG */

    for( i = 0; i < 9; i++ )
	if( ( argsArray[i] = *va_arg( args, UINT32* ) ) == 0 ) break;

    va_end( args );

    pMUD = (MUD_SEC*)MUD_search( *ppMUD_head, argsArray[0], argsArray[1], argsArray[2],
		       argsArray[3], argsArray[4], argsArray[5], argsArray[6],
		       argsArray[7], argsArray[8] );
    if( pMUD == NULL ) return( 0 );

    bcopy( pMUD, pMUD_ret, MUD_sizeOf( pMUD ) );
    return( 1 );
}


MANGLE(int,fMUD_fseek,FMUD_FSEEK,fmud_fseek)
#ifdef NO_STDARG
    ( va_alist )
    va_dcl
#else
    ( FILE** pFio, ... )
#endif /* NO_STDARG */
{
    va_list args;
#ifdef NO_STDARG
    FILE** pFio;
#endif /* NO_STDARG */ 
    UINT32 argsArray[9];
    int i;
    int status;

    bzero( argsArray, 9*sizeof( UINT32 ) );

#ifdef NO_STDARG
    va_start( args );
    pFio = va_arg( args, FILE** );
#else
    va_start( args, pFio );
#endif /* NO_STDARG */

    for( i = 0; i < 9; i++ )
	if( ( argsArray[i] = *va_arg( args, UINT32* ) ) == 0 ) break;

    va_end( args );

#ifdef DEBUG
	printf( "fio = %p\n", *pFio );

	for( i = 0; i < 9; i++ )
	    printf( "arg[%d] = %X\n", i, argsArray[i] );
#endif /* DEBUG */

    status = MUD_fseek( *pFio, argsArray[0], argsArray[1], argsArray[2],
		       argsArray[3], argsArray[4], argsArray[5], argsArray[6],
		       argsArray[7], argsArray[8] );
    return( status );
}

#endif /* __cplusplus */

