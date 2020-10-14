/*
 *  mud_friendly.c --
 *
 *    A friendly programming interface to the MUD library
 *
 *  Copyright (c) 1996-2020 TRIUMF Cyclotron Facility
 *
 *  Author: 
 *    Ted Whidden, TRIUMF Data Acquisition Group
 *    Donald Arseneau, TRIUMF CMMS
 *    4004 Wesbrook Mall, Vancouver, BC, Canada, V6T 2A3
 *
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
 *  Modification history:
 *    26-Feb-1996  v1.0  TW   Created
 *    08-Oct-2000  v1.2  DJA  Use MUD_setSizes()
 *    19-Sep-2001  v1.3  DJA  add HistSecondsPerBin
 *    25-Sep-2001  v1.4  DJA  fixed HistSecondsPerBin (sigh)
 *    23-Oct-2001  v1.5  DJA  Major change: Add string length parameter to all
 *                            string "get" functions, and use strncpy, but 
 *                            always terminate.
 *    22-Apr-2003  v1.6  DJA  Add mud_openReadWrite
 *    25-May-2011  v1.7  DJA  Fix cast in MUD_setHistSecondsPerBin
 *    15-Oct-2020  v1.8  DF   Fix group/instance numbers in _sea_cmtgrp
 *
 *  Description:
 *
 *    This module provides a "friendly" programming interface (in C
 *    or FORTRAN) to the MUD (MuSR Data) data format.  
 *
 *    int fd :     mud-friendly file handle
 *    ing strdim : maximum length for returned strings
 * 
 *  User-callable Routines (C language):
 *    
 *    Open/close files:
 * 
 *    int MUD_openRead( char* filename, UINT32* pType )
 *    int MUD_openWrite( char* filename, UINT32 type )
 *    int MUD_openReadWrite( char* filename, UINT32* pType )
 *    int MUD_closeRead( int fd )
 *    int MUD_closeWrite( int fd )
 *    int MUD_closeWriteFile( int fd, char* filename )
 *
 *    Run description:
 *
 *    int MUD_getRunDesc( int fd, UINT32* pType )
 *    int MUD_getExptNumber( int fd, UINT32* pExptNumber )
 *    int MUD_getRunNumber( int fd, UINT32* pRunNumber )
 *    int MUD_getElapsedSec( int fd, UINT32* pElapsedSec )
 *    int MUD_getTimeBegin( int fd, UINT32* TimeBegin )
 *    int MUD_getTimeEnd( int fd, UINT32* TimeEnd )
 *    int MUD_getTitle( int fd, char* title, int strdim )
 *    int MUD_getLab( int fd, char* lab, int strdim )
 *    int MUD_getArea( int fd, char* area, int strdim )
 *    int MUD_getMethod( int fd, char* method, int strdim )
 *    int MUD_getApparatus( int fd, char* apparatus, int strdim )
 *    int MUD_getInsert( int fd, char* insert, int strdim )
 *    int MUD_getSample( int fd, char* sample, int strdim )
 *    int MUD_getOrient( int fd, char* orient, int strdim )
 *    int MUD_getDas( int fd, char* das, int strdim )
 *    int MUD_getExperimenter( int fd, char* experimenter, int strdim )
 *    Not in TRI_TI:
 *    int MUD_getTemperature( int fd, char* temperature, int strdim )
 *    int MUD_getField( int fd, char* field, int strdim )
 *    TRI_TI only:
 *    int MUD_getSubtitle( int fd, char* subtitle, int strdim )
 *    int MUD_getComment1( int fd, char* comment1, int strdim )
 *    int MUD_getComment2( int fd, char* comment2, int strdim )
 *    int MUD_getComment3( int fd, char* comment3, int strdim )
 *
 *    int MUD_setRunDesc( int fd, UINT32 type )
 *    int MUD_setExptNumber( int fd, UINT32 exptNumber )
 *    int MUD_setRunNumber( int fd, UINT32 runNumber )
 *    int MUD_setElapsedSec( int fd, UINT32 elapsedSec )
 *    int MUD_setTimeBegin( int fd, UINT32 timeBegin )
 *    int MUD_setTimeEnd( int fd, UINT32 timeEnd )
 *    int MUD_setTitle( int fd, char* title )
 *    int MUD_setLab( int fd, char* lab )
 *    int MUD_setArea( int fd, char* area )
 *    int MUD_setMethod( int fd, char* method )
 *    int MUD_setApparatus( int fd, char* apparatus )
 *    int MUD_setInsert( int fd, char* insert )
 *    int MUD_setSample( int fd, char* sample )
 *    int MUD_setOrient( int fd, char* orient )
 *    int MUD_setDas( int fd, char* das )
 *    int MUD_setExperimenter( int fd, char* experimenter )
 *    Not in TRI_TI:
 *    int MUD_setTemperature( int fd, char* temperature )
 *    int MUD_setField( int fd, char* field )
 *    TRI_TI only:
 *    int MUD_setSubtitle( int fd, char* subtitle )
 *    int MUD_setComment1( int fd, char* comment1 )
 *    int MUD_setComment2( int fd, char* comment2 )
 *    int MUD_setComment3( int fd, char* comment3 )
 *
 *    Comments:
 *
 *    int MUD_getComments( int fd, UINT32* pType, UINT32* pNum )
 *    int MUD_getCommentPrev( int fd, int num, UINT32* pPrev )
 *    int MUD_getCommentNext( int fd, int num, UINT32* pNext )
 *    int MUD_getCommentTime( int fd, int num, UINT32* pTime )
 *    int MUD_getCommentAuthor( int fd, int num, char* author, int strdim )
 *    int MUD_getCommentTitle( int fd, int num, char* title, int strdim )
 *    int MUD_getCommentBody( int fd, int num, char* body, int strdim )
 *
 *    int MUD_setComments( int fd, UINT32 type, UINT32 num )
 *    int MUD_setCommentPrev( int fd, int num, UINT32 prev )
 *    int MUD_setCommentNext( int fd, int num, UINT32 next )
 *    int MUD_setCommentTime( int fd, int num, UINT32 time )
 *    int MUD_setCommentAuthor( int fd, int num, char* author )
 *    int MUD_setCommentTitle( int fd, int num, char* title )
 *    int MUD_setCommentBody( int fd, int num, char* body )
 *
 *    Histograms:
 *
 *    int MUD_getHists( int fd, UINT32* pType, UINT32* pNum )
 *    int MUD_getHistType( int fd, int num, UINT32* pType )
 *    int MUD_getHistNumBytes( int fd, int num, UINT32* pNumBytes )
 *    int MUD_getHistNumBins( int fd, int num, UINT32* pNumBins )
 *    int MUD_getHistBytesPerBin( int fd, int num, UINT32* pBytesPerBin )
 *    int MUD_getHistFsPerBin( int fd, int num, UINT32* pFsPerBin )
 *    int MUD_getHistSecondsPerBin( int fd, int num, REAL64* pSecondsPerBin )
 *    int MUD_getHistT0_Ps( int fd, int num, UINT32* pT0_ps )
 *    int MUD_getHistT0_Bin( int fd, int num, UINT32* pT0_bin )
 *    int MUD_getHistGoodBin1( int fd, int num, UINT32* pGoodBin1 )
 *    int MUD_getHistGoodBin2( int fd, int num, UINT32* pGoodBin2 )
 *    int MUD_getHistBkgd1( int fd, int num, UINT32* pBkgd1 )
 *    int MUD_getHistBkgd2( int fd, int num, UINT32* pBkgd2 )
 *    int MUD_getHistNumEvents( int fd, int num, UINT32* pNumEvents )
 *    int MUD_getHistTitle( int fd, int num, char* title, int strdim )
 *    int MUD_getHistData( int fd, int num, void* pData )
 *    int MUD_getHistpData( int fd, int num, void** ppData )
 *    int MUD_getHistTimeData( int fd, int num, UINT32* pTimeData )
 *    int MUD_getHistpTimeData( int fd, int num, UINT32** ppTimeData )
 *
 *    int MUD_setHists( int fd, UINT32 type, UINT32 num )
 *    int MUD_setHistType( int fd, int num, UINT32 type )
 *    int MUD_setHistNumBytes( int fd, int num, UINT32 numBytes )
 *    int MUD_setHistNumBins( int fd, int num, UINT32 numBins )
 *    int MUD_setHistBytesPerBin( int fd, int num, UINT32 bytesPerBin )
 *    int MUD_setHistFsPerBin( int fd, int num, UINT32 fsPerBin )
 *    int MUD_setHistSecondsPerBin( int fd, int num, REAL64 secondsPerBin )
 *    int MUD_setHistT0_Ps( int fd, int num, UINT32 t0_ps )
 *    int MUD_setHistT0_Bin( int fd, int num, UINT32 t0_bin )
 *    int MUD_setHistGoodBin1( int fd, int num, UINT32 goodBin1 )
 *    int MUD_setHistGoodBin2( int fd, int num, UINT32 goodBin2 )
 *    int MUD_setHistBkgd1( int fd, int num, UINT32 bkgd1 )
 *    int MUD_setHistBkgd2( int fd, int num, UINT32 bkgd2 )
 *    int MUD_setHistNumEvents( int fd, int num, UINT32 numEvents )
 *    int MUD_setHistTitle( int fd, int num, char* title )
 *    int MUD_setHistData( int fd, int num, void* pData )
 *    int MUD_setHistpData( int fd, int num, void* pData )
 *    int MUD_setHistTimeData( int fd, int num, UINT32* pTimeData )
 *    int MUD_setHistpTimeData( int fd, int num, UINT32* pTimeData )
 * 
 *    int MUD_pack( int num, int inBinSize, void* inArray, int outBinSize, void* outArray )
 *    int MUD_unpack( int num, int inBinSize, void* inArray, int outBinSize, void* outArray )
 * 
 *    Scalers:
 *
 *    int MUD_getScalers( int fd, UINT32* pType, UINT32* pNum )
 *    int MUD_getScalerLabel( int fd, int num, char* label, int strdim )
 *    int MUD_getScalerCounts( int fd, int num, UINT32* pCounts )
 *
 *    int MUD_setScalers( int fd, UINT32 type, UINT32 num )
 *    int MUD_setScalerLabel( int fd, int num, char* label )
 *    int MUD_setScalerCounts( int fd, int num, UINT32* pCounts )
 *
 *    Independent variables (CAMP):
 *
 *    int MUD_getIndVars( int fd, UINT32* pType, UINT32* pNum )
 *    int MUD_getIndVarLow( int fd, int num, double* pLow )
 *    int MUD_getIndVarHigh( int fd, int num, double* pHigh )
 *    int MUD_getIndVarMean( int fd, int num, double* pMean )
 *    int MUD_getIndVarStddev( int fd, int num, double* pStddev )
 *    int MUD_getIndVarSkewness( int fd, int num, double* pSkewness )
 *    int MUD_getIndVarName( int fd, int num, char* name, int strdim )
 *    int MUD_getIndVarDescription( int fd, int num, char* description, int strdim )
 *    int MUD_getIndVarUnits( int fd, int num, char* units, int strdim )
 *    For history data:
 *    int MUD_getIndVarNumData( int fd, int num, UINT32* pNumData )
 *    int MUD_getIndVarElemSize( int fd, int num, UINT32* pElemSize )
 *    int MUD_getIndVarDataType( int fd, int num, UINT32* pDataType )
 *    int MUD_getIndVarHasTime( int fd, int num, UINT32* pHasTime )
 *    int MUD_getIndVarData( int fd, int num, void* pData )
 *    int MUD_getIndVarTimeData( int fd, int num, UINT32* pTimeData )
 *    int MUD_getIndVarpData( int fd, int num, void** ppData )
 *    int MUD_getIndVarpTimeData( int fd, int num, UINT32** ppTimeData )
 *
 *    int MUD_setIndVars( int fd, UINT32 type, UINT32 num )
 *    int MUD_setIndVarLow( int fd, int num, double low )
 *    int MUD_setIndVarHigh( int fd, int num, double high )
 *    int MUD_setIndVarMean( int fd, int num, double mean )
 *    int MUD_setIndVarStddev( int fd, int num, double stddev )
 *    int MUD_setIndVarSkewness( int fd, int num, double skewness )
 *    int MUD_setIndVarName( int fd, int num, char* name )
 *    int MUD_setIndVarDescription( int fd, int num, char* description )
 *    int MUD_setIndVarUnits( int fd, int num, char* units )
 *    For history data:
 *    int MUD_setIndVarNumData( int fd, int num, UINT32 numData )
 *    int MUD_setIndVarElemSize( int fd, int num, UINT32 elemSize )
 *    int MUD_setIndVarDataType( int fd, int num, UINT32 dataType )
 *    int MUD_setIndVarData( int fd, int num, void* pData )
 *    int MUD_setIndVarTimeData( int fd, int num, UINT32* pTimeData )
 *    int MUD_setIndVarpData( int fd, int num, void* pData )
 *    int MUD_setIndVarpTimeData( int fd, int num, UINT32* pTimeData )
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mud.h"

#define MUD_MAX_FILES 16
#define MUD_FILE_READ  1
#define MUD_FILE_WRITE 2

static FILE* mud_f[MUD_MAX_FILES] = { 0 };
static MUD_SEC_GRP* pMUD_fileGrp[MUD_MAX_FILES];

#define _strncpy( To, From, Len) strncpy( To, From, Len )[Len-1]='\0'

int 
MUD_openRead( char* filename, UINT32* pType )
{
  int fd;

  for( fd = 0; fd < MUD_MAX_FILES; fd++ ) 
  {
    if( mud_f[fd] == NULL ) break;
  }
  if( fd == MUD_MAX_FILES ) return( -1 );

  mud_f[fd] = MUD_openInput( filename );
  if( mud_f[fd] == NULL ) return( -1 );

  /*
   *  Just read the whole file
   *  Might want to make this more complicated, 
   *  i.e., only read when needed, keep track of 
   *  what's already read.
   */
  pMUD_fileGrp[fd] = (MUD_SEC_GRP*)MUD_readFile( mud_f[fd] );
  if( pMUD_fileGrp[fd] == NULL )
  {
    fclose( mud_f[fd] );
    mud_f[fd] = NULL;
    return( -1 );
  }

  *pType = MUD_instanceID( pMUD_fileGrp[fd] );

  return( fd );
}

int 
MUD_openReadWrite( char* filename, UINT32* pType )
{
  int fd;

  for( fd = 0; fd < MUD_MAX_FILES; fd++ ) 
  {
    if( mud_f[fd] == NULL ) break;
  }
  if( fd == MUD_MAX_FILES ) return( -1 );

  mud_f[fd] = MUD_openInOut( filename );
  if( mud_f[fd] == NULL ) return( -1 );

  /*
   *  Just read the whole file.  Must do so in ReadWrite version.
   */
  pMUD_fileGrp[fd] = (MUD_SEC_GRP*)MUD_readFile( mud_f[fd] );
  if( pMUD_fileGrp[fd] == NULL ) 
  {
    fclose( mud_f[fd] );
    mud_f[fd] = NULL;
    return( -1 );
  }

  *pType = MUD_instanceID( pMUD_fileGrp[fd] );

  return( fd );
}


int 
MUD_openWrite( char* filename, UINT32 type )
{
  int fd;

  for( fd = 0; fd < MUD_MAX_FILES; fd++ ) 
  {
    if( mud_f[fd] == NULL ) break;
  }
  if( fd == MUD_MAX_FILES ) return( -1 );

  mud_f[fd] = MUD_openOutput( filename );
  if( mud_f[fd] == NULL ) return( -1 );

  pMUD_fileGrp[fd] = (MUD_SEC_GRP*)MUD_new( MUD_SEC_GRP_ID, type );
  if( pMUD_fileGrp[fd] == NULL )
  {
    fclose( mud_f[fd] );
    mud_f[fd] = NULL;
    return( -1 );
  }

  return( fd );
}


int 
MUD_closeRead( int fd )
{
  if( ( fd < 0 ) || ( fd >= MUD_MAX_FILES ) || ( mud_f[fd] == NULL ) ) 
  {
    return( 0 );
  }

  /*
   *  Free the list
   */
  if( pMUD_fileGrp[fd] != NULL )
  {
    MUD_free( pMUD_fileGrp[fd] );
    pMUD_fileGrp[fd] = NULL;
  }

  fclose( mud_f[fd] );
  mud_f[fd] = NULL;

  return( 1 );
}


int 
MUD_closeWrite( int fd )
{
  if( ( fd < 0 ) || ( fd >= MUD_MAX_FILES ) || ( mud_f[fd] == NULL ) ) 
  {
    return( 0 );
  }

  /*
   *  Re-index mud groups (memSize and index.offset)
   */
  MUD_setSizes( pMUD_fileGrp[fd] );
  /*
   *  Write the file out if it's an output file
   */
  MUD_writeFile( mud_f[fd], pMUD_fileGrp[fd] ); 

  /*
   *  Free the list
   */
  if( pMUD_fileGrp[fd] != NULL )
  {
    MUD_free( pMUD_fileGrp[fd] );
    pMUD_fileGrp[fd] = NULL;
  }

  fclose( mud_f[fd] );
  mud_f[fd] = NULL;

  return( 1 );
}

int 
MUD_closeWriteFile( int fd, char* outname )
{
  if( ( fd < 0 ) || ( fd >= MUD_MAX_FILES ) || ( mud_f[fd] == NULL ) ) 
  {
    return( 0 );
  }

  /*
   * Close the input file
   */
  fclose( mud_f[fd] );

  /*
   * Open output file on same fd index
   */
  mud_f[fd] = MUD_openOutput( outname );
  if( mud_f[fd] == NULL ) return( 0 );

  /*
   *  Re-index mud groups (memSize and index.offset)
   */
  MUD_setSizes( pMUD_fileGrp[fd] );
  /*
   *  Write the file out.
   */
  MUD_writeFile( mud_f[fd], pMUD_fileGrp[fd] ); 

  /*
   *  Free the list
   */
  if( pMUD_fileGrp[fd] != NULL )
  {
    MUD_free( pMUD_fileGrp[fd] );
    pMUD_fileGrp[fd] = NULL;
  }

  fclose( mud_f[fd] );
  mud_f[fd] = NULL;

  return( 1 );
}


#define _check_fd( fd )  if( ( fd < 0 ) || \
                             ( fd >= MUD_MAX_FILES ) || \
                             ( mud_f[fd] == NULL ) ) return( 0 )

/*
 *  Run Description
 */
#define _sea_desc( fd ) \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: \
      pMUD_idesc = (MUD_SEC_TRI_TI_RUN_DESC*)MUD_search( pMUD_fileGrp[fd]->pMem,             \
                              MUD_SEC_TRI_TI_RUN_DESC_ID, (UINT32)1, \
                              (UINT32)0 ); \
      if( pMUD_idesc == NULL ) return( 0 ); \
      break; \
    case MUD_FMT_TRI_TD_ID: \
    default: \
      pMUD_desc = (MUD_SEC_GEN_RUN_DESC*)MUD_search( pMUD_fileGrp[fd]->pMem,           \
                              MUD_SEC_GEN_RUN_DESC_ID, (UINT32)1, \
                              (UINT32)0 ); \
      if( pMUD_desc == NULL ) return( 0 ); \
      break; \
  }


#define _sea_gdesc( fd ) \
  pMUD_desc = (MUD_SEC_GEN_RUN_DESC*)MUD_search( pMUD_fileGrp[fd]->pMem, \
                              MUD_SEC_GEN_RUN_DESC_ID, (UINT32)1, \
                              (UINT32)0 ); \
  if( pMUD_desc == NULL ) return( 0 )


#define _sea_idesc( fd ) \
  pMUD_idesc = (MUD_SEC_TRI_TI_RUN_DESC*)MUD_search( pMUD_fileGrp[fd]->pMem, \
                              MUD_SEC_TRI_TI_RUN_DESC_ID, (UINT32)1, \
                              (UINT32)0 ); \
  if( pMUD_idesc == NULL ) return( 0 )


#define _desc_uint_getproc( name, var ) \
  int name( int fd, UINT32* var) \
{ \
  MUD_SEC_GEN_RUN_DESC* pMUD_desc=0; \
  MUD_SEC_TRI_TI_RUN_DESC* pMUD_idesc=0; \
  _check_fd( fd ); \
  _sea_desc( fd ); \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: *var = pMUD_idesc->var; break; \
    case MUD_FMT_TRI_TD_ID: default: *var = pMUD_desc->var; break; \
  } \
  return( 1 ); \
}


#define _desc_uint_setproc( name, var ) \
int name( int fd, UINT32 var) \
{ \
  MUD_SEC_GEN_RUN_DESC* pMUD_desc=0; \
  MUD_SEC_TRI_TI_RUN_DESC* pMUD_idesc=0; \
  _check_fd( fd ); \
  _sea_desc( fd ); \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: pMUD_idesc->var = var; break; \
    case MUD_FMT_TRI_TD_ID: default: pMUD_desc->var = var; break; \
  } \
  return( 1 ); \
}


#define _desc_char_getproc( name, var ) \
int name( int fd, char* var, int strdim ) \
{ \
  MUD_SEC_GEN_RUN_DESC* pMUD_desc=0; \
  MUD_SEC_TRI_TI_RUN_DESC* pMUD_idesc=0; \
  _check_fd( fd ); \
  _sea_desc( fd ); \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: _strncpy( var, pMUD_idesc->var, strdim ); break; \
    case MUD_FMT_TRI_TD_ID: default: _strncpy( var, pMUD_desc->var, strdim ); break; \
  } \
  return( 1 ); \
}


#define _desc_char_setproc( name, var ) \
int name( int fd, char* var ) \
{ \
  MUD_SEC_GEN_RUN_DESC* pMUD_desc=0; \
  MUD_SEC_TRI_TI_RUN_DESC* pMUD_idesc=0; \
  _check_fd( fd ); \
  _sea_desc( fd ); \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: \
      _free( pMUD_idesc->var ); pMUD_idesc->var = strdup( var ); break; \
    case MUD_FMT_TRI_TD_ID: default: \
      _free( pMUD_desc->var ); pMUD_desc->var = strdup( var ); break; \
  } \
  return( 1 ); \
}


#define _gdesc_char_getproc( name, var ) \
int name( int fd, char* var, int strdim ) \
{ \
  MUD_SEC_GEN_RUN_DESC* pMUD_desc=0; \
  _check_fd( fd ); \
  _sea_gdesc( fd ); \
  _strncpy( var, pMUD_desc->var, strdim ); \
  return( 1 ); \
}


#define _gdesc_char_setproc( name, var ) \
int name( int fd, char* var ) \
{ \
  MUD_SEC_GEN_RUN_DESC* pMUD_desc=0; \
  _check_fd( fd ); \
  _sea_gdesc( fd ); \
  _free( pMUD_desc->var ); \
  pMUD_desc->var = strdup( var ); \
  return( 1 ); \
}


#define _idesc_char_getproc( name, var ) \
int name( int fd, char* var, int strdim ) \
{ \
  MUD_SEC_TRI_TI_RUN_DESC* pMUD_idesc=0; \
  _check_fd( fd ); \
  _sea_idesc( fd ); \
  _strncpy( var, pMUD_idesc->var, strdim ); \
  return( 1 ); \
}


#define _idesc_char_setproc( name, var ) \
int name( int fd, char* var ) \
{ \
  MUD_SEC_TRI_TI_RUN_DESC* pMUD_idesc=0; \
  _check_fd( fd ); \
  _sea_idesc( fd ); \
  _free( pMUD_idesc->var ); \
  pMUD_idesc->var = strdup( var ); \
  return( 1 ); \
}


int
MUD_getRunDesc( int fd, UINT32* pType )
{
  MUD_SEC_GEN_RUN_DESC* pMUD_desc=0;
  MUD_SEC_TRI_TI_RUN_DESC* pMUD_idesc=0;

  _check_fd( fd );

  switch( MUD_instanceID( pMUD_fileGrp[fd] ) )
  {
    case MUD_FMT_TRI_TI_ID:
      pMUD_idesc = (MUD_SEC_TRI_TI_RUN_DESC*)MUD_search( pMUD_fileGrp[fd]->pMem,
                              MUD_SEC_TRI_TI_RUN_DESC_ID, (UINT32)1,
                              (UINT32)0 );
      if( pMUD_idesc == NULL ) return( 0 );
      *pType = MUD_SEC_TRI_TI_RUN_DESC_ID;
      break;
    case MUD_FMT_TRI_TD_ID:
    default:
      pMUD_desc = (MUD_SEC_GEN_RUN_DESC*)MUD_search( pMUD_fileGrp[fd]->pMem,
                              MUD_SEC_GEN_RUN_DESC_ID, (UINT32)1,
                              (UINT32)0 );
      if( pMUD_desc == NULL ) return( 0 );
      *pType = MUD_SEC_GEN_RUN_DESC_ID;
      break;
  }

  return( 1 );
}

int
MUD_setRunDesc( int fd, UINT32 type )
{
  MUD_SEC_GEN_RUN_DESC* pMUD_desc=0;
  MUD_SEC_TRI_TI_RUN_DESC* pMUD_idesc=0;

  _check_fd( fd );

  switch( MUD_instanceID( pMUD_fileGrp[fd] ) )
  {
    case MUD_FMT_TRI_TI_ID:
      pMUD_idesc = (MUD_SEC_TRI_TI_RUN_DESC*)MUD_new( MUD_SEC_TRI_TI_RUN_DESC_ID, 1 );
      if( pMUD_idesc == NULL ) return( 0 );
      MUD_addToGroup( pMUD_fileGrp[fd], pMUD_idesc );
      break;
    case MUD_FMT_TRI_TD_ID:
    default:
      pMUD_desc = (MUD_SEC_GEN_RUN_DESC*)MUD_new( MUD_SEC_GEN_RUN_DESC_ID, 1 );
      if( pMUD_desc == NULL ) return( 0 );
      MUD_addToGroup( pMUD_fileGrp[fd], pMUD_desc );
      break;
  }

  return( 1 );
}

_desc_uint_getproc( MUD_getExptNumber, exptNumber )
_desc_uint_getproc( MUD_getRunNumber, runNumber )
_desc_uint_getproc( MUD_getElapsedSec, elapsedSec )
_desc_uint_getproc( MUD_getTimeBegin, timeBegin )
_desc_uint_getproc( MUD_getTimeEnd, timeEnd )
_desc_char_getproc( MUD_getTitle, title )
_desc_char_getproc( MUD_getLab, lab )
_desc_char_getproc( MUD_getArea, area )
_desc_char_getproc( MUD_getMethod, method )
_desc_char_getproc( MUD_getApparatus, apparatus )
_desc_char_getproc( MUD_getInsert, insert )
_desc_char_getproc( MUD_getSample, sample )
_desc_char_getproc( MUD_getOrient, orient )
_desc_char_getproc( MUD_getDas, das )
_desc_char_getproc( MUD_getExperimenter, experimenter )
/* not in TRI_TI */
_gdesc_char_getproc( MUD_getTemperature, temperature )
_gdesc_char_getproc( MUD_getField, field )
/* TRI_TI only */
_idesc_char_getproc( MUD_getSubtitle, subtitle )
_idesc_char_getproc( MUD_getComment1, comment1 )
_idesc_char_getproc( MUD_getComment2, comment2 )
_idesc_char_getproc( MUD_getComment3, comment3 )

_desc_uint_setproc( MUD_setExptNumber, exptNumber )
_desc_uint_setproc( MUD_setRunNumber, runNumber )
_desc_uint_setproc( MUD_setElapsedSec, elapsedSec )
_desc_uint_setproc( MUD_setTimeBegin, timeBegin )
_desc_uint_setproc( MUD_setTimeEnd, timeEnd )
_desc_char_setproc( MUD_setTitle, title )
_desc_char_setproc( MUD_setLab, lab )
_desc_char_setproc( MUD_setArea, area )
_desc_char_setproc( MUD_setMethod, method )
_desc_char_setproc( MUD_setApparatus, apparatus )
_desc_char_setproc( MUD_setInsert, insert )
_desc_char_setproc( MUD_setSample, sample )
_desc_char_setproc( MUD_setOrient, orient )
_desc_char_setproc( MUD_setDas, das )
_desc_char_setproc( MUD_setExperimenter, experimenter )
/* not in TRI_TI */
_gdesc_char_setproc( MUD_setTemperature, temperature )
_gdesc_char_setproc( MUD_setField, field )
/* TRI_TI only */
_idesc_char_setproc( MUD_setSubtitle, subtitle )
_idesc_char_setproc( MUD_setComment1, comment1 )
_idesc_char_setproc( MUD_setComment2, comment2 )
_idesc_char_setproc( MUD_setComment3, comment3 )


/*
 *  Comments
 */
#define _sea_cmtgrp( fd ) \
  pMUD_cmtGrp = (MUD_SEC_GRP*)MUD_search( pMUD_fileGrp[fd]->pMem,       \
                          MUD_SEC_GRP_ID, MUD_GRP_CMT_ID, \
                          (UINT32)0 ); \
  if( pMUD_cmtGrp == NULL ) return( 0 )


#define _sea_cmt( fd, n ) \
  pMUD_cmt = (MUD_SEC_CMT*)MUD_search( pMUD_cmtGrp->pMem, \
                         MUD_SEC_CMT_ID, (UINT32)n, \
                         (UINT32)0 ); \
  if( pMUD_cmt == NULL ) return( 0 )


#define _cmt_uint_getproc( name, var ) \
int \
name( int fd, int num, UINT32* var ) \
{ \
  MUD_SEC_GRP* pMUD_cmtGrp=0; \
  MUD_SEC_CMT* pMUD_cmt=0; \
  _check_fd( fd ); \
  _sea_cmtgrp( fd ); \
  _sea_cmt( fd, num ); \
  *var = pMUD_cmt->var; \
  return( 1 ); \
}


#define _cmt_uint_setproc( name, var ) \
int \
name( int fd, int num, UINT32 var ) \
{ \
  MUD_SEC_GRP* pMUD_cmtGrp=0; \
  MUD_SEC_CMT* pMUD_cmt=0; \
  _check_fd( fd ); \
  _sea_cmtgrp( fd ); \
  _sea_cmt( fd, num ); \
  pMUD_cmt->var = var; \
  return( 1 ); \
}


#define _cmt_char_getproc( name, var ) \
int \
name( int fd, int num, char* var, int strdim ) \
{ \
  MUD_SEC_GRP* pMUD_cmtGrp=0; \
  MUD_SEC_CMT* pMUD_cmt=0; \
  _check_fd( fd ); \
  _sea_cmtgrp( fd ); \
  _sea_cmt( fd, num ); \
  _strncpy( var, pMUD_cmt->var, strdim ); \
  return( 1 ); \
}

#define _cmt_char_setproc( name, var ) \
int \
name( int fd, int num, char* var ) \
{ \
  MUD_SEC_GRP* pMUD_cmtGrp=0; \
  MUD_SEC_CMT* pMUD_cmt=0; \
  _check_fd( fd ); \
  _sea_cmtgrp( fd ); \
  _sea_cmt( fd, num ); \
  _free( pMUD_cmt->var ); \
  pMUD_cmt->var = strdup( var ); \
  return( 1 ); \
}

int 
MUD_getComments( int fd, UINT32* pType, UINT32* pNum )
{
  MUD_SEC_GRP* pMUD_cmtGrp=0;

  _check_fd( fd );
  _sea_cmtgrp( fd );
  *pType = MUD_instanceID( pMUD_cmtGrp );
  *pNum = pMUD_cmtGrp->num;

  return( 1 );
}

int 
MUD_setComments( int fd, UINT32 type, UINT32 num )
{
  MUD_SEC_GRP* pMUD_cmtGrp=0;
  MUD_SEC_CMT* pMUD_cmt=0;
  int i;

  _check_fd( fd );

  pMUD_cmtGrp = (MUD_SEC_GRP*)MUD_new( MUD_SEC_GRP_ID, type );
  if( pMUD_cmtGrp == NULL ) return( 0 );

  for( i = 0; i < num; i++ )
  {
    pMUD_cmt = (MUD_SEC_CMT*)MUD_new( MUD_SEC_CMT_ID, i+1 );
    if( pMUD_cmt == NULL )
    {
      MUD_free( pMUD_cmtGrp );
      return( 0 );
    }
    MUD_addToGroup( pMUD_cmtGrp, pMUD_cmt );
  }

  MUD_addToGroup( pMUD_fileGrp[fd], pMUD_cmtGrp );

  return( 1 );
}

_cmt_uint_getproc( MUD_getCommentPrev, prevReplyID )
_cmt_uint_getproc( MUD_getCommentNext, nextReplyID )
_cmt_uint_getproc( MUD_getCommentTime, time )
_cmt_char_getproc( MUD_getCommentAuthor, author )
_cmt_char_getproc( MUD_getCommentTitle, title )
_cmt_char_getproc( MUD_getCommentBody, comment )

_cmt_uint_setproc( MUD_setCommentPrev, prevReplyID )
_cmt_uint_setproc( MUD_setCommentNext, nextReplyID )
_cmt_uint_setproc( MUD_setCommentTime, time )
_cmt_char_setproc( MUD_setCommentAuthor, author )
_cmt_char_setproc( MUD_setCommentTitle, title )
_cmt_char_setproc( MUD_setCommentBody, comment )


/*
 *  Histograms
 */
#define _sea_histgrp( fd ) \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: \
      pMUD_histGrp = (MUD_SEC_GRP*)MUD_search( pMUD_fileGrp[fd]->pMem,  \
                                 MUD_SEC_GRP_ID, MUD_GRP_TRI_TI_HIST_ID, \
                                 (UINT32)0 ); \
      break; \
    case MUD_FMT_TRI_TD_ID: \
    default: \
      pMUD_histGrp = (MUD_SEC_GRP*)MUD_search( pMUD_fileGrp[fd]->pMem, \
                                 MUD_SEC_GRP_ID, MUD_GRP_TRI_TD_HIST_ID, \
                                 (UINT32)0 ); \
      break; \
  } \
  if( pMUD_histGrp == NULL ) return( 0 )


#define _sea_histhdr( fd, n ) \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: \
    case MUD_FMT_TRI_TD_ID: \
    default: \
      pMUD_histHdr = (MUD_SEC_GEN_HIST_HDR*)MUD_search( pMUD_histGrp->pMem, \
                                 MUD_SEC_GEN_HIST_HDR_ID, (UINT32)n, \
                                 (UINT32)0 ); \
      break; \
  } \
  if( pMUD_histHdr == NULL ) return( 0 )


#define _hist_uint_getproc( name, var ) \
int \
name( int fd, int num, UINT32* var ) \
{ \
  MUD_SEC_GRP* pMUD_histGrp=0; \
  MUD_SEC_GEN_HIST_HDR* pMUD_histHdr=0; \
  _check_fd( fd ); \
  _sea_histgrp( fd ); \
  _sea_histhdr( fd, num ); \
  *var = pMUD_histHdr->var; \
  return( 1 ); \
}

#define _hist_uint_setproc( name, var ) \
int \
name( int fd, int num, UINT32 var ) \
{ \
  MUD_SEC_GRP* pMUD_histGrp=0; \
  MUD_SEC_GEN_HIST_HDR* pMUD_histHdr=0; \
  _check_fd( fd ); \
  _sea_histgrp( fd ); \
  _sea_histhdr( fd, num ); \
  pMUD_histHdr->var = var; \
  return( 1 ); \
}

#define _hist_char_getproc( name, var ) \
int \
name( int fd, int num, char* var, int strdim ) \
{ \
  MUD_SEC_GRP* pMUD_histGrp; \
  MUD_SEC_GEN_HIST_HDR* pMUD_histHdr; \
  _check_fd( fd ); \
  _sea_histgrp( fd ); \
  _sea_histhdr( fd, num ); \
  _strncpy( var, pMUD_histHdr->var, strdim ); \
  return( 1 ); \
}

#define _hist_char_setproc( name, var ) \
int \
name( int fd, int num, char* var ) \
{ \
  MUD_SEC_GRP* pMUD_histGrp=0; \
  MUD_SEC_GEN_HIST_HDR* pMUD_histHdr=0; \
  _check_fd( fd ); \
  _sea_histgrp( fd ); \
  _sea_histhdr( fd, num ); \
  _free( pMUD_histHdr->var ); \
  pMUD_histHdr->var = strdup( var ); \
  return( 1 ); \
}

int 
MUD_getHists( int fd, UINT32* pType, UINT32* pNum )
{
  MUD_SEC_GRP* pMUD_histGrp=0;
  _check_fd( fd );
  _sea_histgrp( fd );
  *pType = MUD_instanceID( pMUD_histGrp );
  switch( *pType )
  {
    case MUD_GRP_TRI_TI_HIST_ID:
    case MUD_GRP_TRI_TD_HIST_ID:
    default:
      /* 
       *  NOTE: two sections per histogram (header, data)
       */
      *pNum = pMUD_histGrp->num/2;   
      break;
  }
  return( 1 );
}

int 
MUD_setHists( int fd, UINT32 type, UINT32 num )
{
  MUD_SEC_GRP* pMUD_grp;
  MUD_SEC_GEN_HIST_HDR* pMUD_histHdr=0;
  MUD_SEC_GEN_HIST_DAT* pMUD_histDat=0;
  int i;

  _check_fd( fd );

  pMUD_grp = (MUD_SEC_GRP*)MUD_new( MUD_SEC_GRP_ID, type );
  if( pMUD_grp == NULL ) return( 0 );

  for( i = 0; i < num; i++ )
  {
    pMUD_histHdr = (MUD_SEC_GEN_HIST_HDR*)MUD_new( MUD_SEC_GEN_HIST_HDR_ID, i+1 );
    if( pMUD_histHdr == NULL )
    {
      MUD_free( pMUD_grp );
      return( 0 );
    }
    MUD_addToGroup( pMUD_grp, pMUD_histHdr );
    pMUD_histDat = (MUD_SEC_GEN_HIST_DAT*)MUD_new( MUD_SEC_GEN_HIST_DAT_ID, i+1 );
    if( pMUD_histDat == NULL )
    {
      MUD_free( pMUD_grp );
      return( 0 );
    }
    MUD_addToGroup( pMUD_grp, pMUD_histDat );
  }

  MUD_addToGroup( pMUD_fileGrp[fd], pMUD_grp );

  return( 1 );
}

_hist_uint_getproc( MUD_getHistType, histType )
_hist_uint_getproc( MUD_getHistNumBytes, nBytes )
_hist_uint_getproc( MUD_getHistNumBins, nBins )
_hist_uint_getproc( MUD_getHistBytesPerBin, bytesPerBin )
_hist_uint_getproc( MUD_getHistFsPerBin, fsPerBin )
_hist_uint_getproc( MUD_getHistT0_Ps, t0_ps )
_hist_uint_getproc( MUD_getHistT0_Bin, t0_bin )
_hist_uint_getproc( MUD_getHistGoodBin1, goodBin1 )
_hist_uint_getproc( MUD_getHistGoodBin2, goodBin2 )
_hist_uint_getproc( MUD_getHistBkgd1, bkgd1 )
_hist_uint_getproc( MUD_getHistBkgd2, bkgd2 )
_hist_uint_getproc( MUD_getHistNumEvents, nEvents )
_hist_char_getproc( MUD_getHistTitle, title )

_hist_uint_setproc( MUD_setHistType, histType )
_hist_uint_setproc( MUD_setHistNumBytes, nBytes )
_hist_uint_setproc( MUD_setHistNumBins, nBins )
_hist_uint_setproc( MUD_setHistBytesPerBin, bytesPerBin )
_hist_uint_setproc( MUD_setHistFsPerBin, fsPerBin )
_hist_uint_setproc( MUD_setHistT0_Ps, t0_ps )
_hist_uint_setproc( MUD_setHistT0_Bin, t0_bin )
_hist_uint_setproc( MUD_setHistGoodBin1, goodBin1 )
_hist_uint_setproc( MUD_setHistGoodBin2, goodBin2 )
_hist_uint_setproc( MUD_setHistBkgd1, bkgd1 )
_hist_uint_setproc( MUD_setHistBkgd2, bkgd2 )
_hist_uint_setproc( MUD_setHistNumEvents, nEvents )
_hist_char_setproc( MUD_setHistTitle, title )

int 
MUD_getHistpData( int fd, int num, void** ppData )
{
  MUD_SEC_GRP* pMUD_histGrp=0;
  MUD_SEC_GEN_HIST_DAT* pMUD_histDat=0;
  _check_fd( fd );
  _sea_histgrp( fd );
  
  pMUD_histDat = (MUD_SEC_GEN_HIST_DAT*)MUD_search( pMUD_histGrp->pMem,
                             MUD_SEC_GEN_HIST_DAT_ID, (UINT32)num,
                             (UINT32)0 );
  if( pMUD_histDat == NULL ) return( 0 );

  *ppData = pMUD_histDat->pData;
  return( 1 );
}

int 
MUD_setHistpData( int fd, int num, void* pData )
{
  MUD_SEC_GRP* pMUD_histGrp=0;
  MUD_SEC_GEN_HIST_DAT* pMUD_histDat=0;
  _check_fd( fd );
  _sea_histgrp( fd );
  
  pMUD_histDat = (MUD_SEC_GEN_HIST_DAT*)MUD_search( pMUD_histGrp->pMem,
                             MUD_SEC_GEN_HIST_DAT_ID, (UINT32)num,
                             (UINT32)0 );
  if( pMUD_histDat == NULL ) return( 0 );

  pMUD_histDat->pData = (caddr_t)pData;
  return( 1 );
}

int 
MUD_getHistData( int fd, int num, void* pData )
{
  MUD_SEC_GRP* pMUD_histGrp=0;
  MUD_SEC_GEN_HIST_HDR* pMUD_histHdr=0;
  MUD_SEC_GEN_HIST_DAT* pMUD_histDat=0;
  _check_fd( fd );
  _sea_histgrp( fd );
  
  pMUD_histHdr = (MUD_SEC_GEN_HIST_HDR*)MUD_search( pMUD_histGrp->pMem,
                             MUD_SEC_GEN_HIST_HDR_ID, (UINT32)num,
                             (UINT32)0 );
  if( pMUD_histHdr == NULL ) return( 0 );

  pMUD_histDat = (MUD_SEC_GEN_HIST_DAT*)MUD_search( pMUD_histGrp->pMem,
                             MUD_SEC_GEN_HIST_DAT_ID, (UINT32)num,
                             (UINT32)0 );
  if( pMUD_histDat == NULL ) return( 0 );

  /*
   *  Do unpacking/byte swapping
   */
  MUD_unpack( pMUD_histHdr->nBins, 
            pMUD_histHdr->bytesPerBin, pMUD_histDat->pData, 
    ( pMUD_histHdr->bytesPerBin == 0 ) ? 4 : pMUD_histHdr->bytesPerBin, pData );

  return( 1 );
}

int 
MUD_setHistData( int fd, int num, void* pData )
{
  MUD_SEC_GRP* pMUD_histGrp=0;
  MUD_SEC_GEN_HIST_HDR* pMUD_histHdr=0;
  MUD_SEC_GEN_HIST_DAT* pMUD_histDat=0;
  _check_fd( fd );
  _sea_histgrp( fd );
  
  pMUD_histHdr = (MUD_SEC_GEN_HIST_HDR*)MUD_search( pMUD_histGrp->pMem,
                             MUD_SEC_GEN_HIST_HDR_ID, (UINT32)num,
                             (UINT32)0 );
  if( pMUD_histHdr == NULL ) return( 0 );

  pMUD_histDat = (MUD_SEC_GEN_HIST_DAT*)MUD_search( pMUD_histGrp->pMem,
                             MUD_SEC_GEN_HIST_DAT_ID, (UINT32)num,
                             (UINT32)0 );
  if( pMUD_histDat == NULL ) return( 0 );

  _free( pMUD_histDat->pData );

  switch( pMUD_histHdr->bytesPerBin )
  {
    case 0:
      pMUD_histDat->pData = (caddr_t)zalloc( 4*pMUD_histHdr->nBins + 32 );
      break;
    default:
      pMUD_histDat->pData = (caddr_t)zalloc( 
                               pMUD_histHdr->nBins*pMUD_histHdr->bytesPerBin );
      break;
  }

  /*
   *  Do packing/byte swapping
   */
  pMUD_histDat->nBytes = pMUD_histHdr->nBytes = 
    MUD_pack( pMUD_histHdr->nBins, 
              ( pMUD_histHdr->bytesPerBin == 0 ) ? 4 : pMUD_histHdr->bytesPerBin, pData,
              pMUD_histHdr->bytesPerBin, pMUD_histDat->pData );

  return( 1 );
}

int 
MUD_getHistpTimeData( int fd, int num, UINT32** ppTimeData )
{
  /* return pointer to time history data for a histogram */
  /* not implemented */
  return( 1 );
}

int 
MUD_getHistTimeData( int fd, int num, UINT32* pTimeData )
{
  /* return pointer to time history data for a histogram */
  /* not implemented */
  return( 1 );
}

int 
MUD_setHistpTimeData( int fd, int num, UINT32* pTimeData )
{
  /* return pointer to time history data for a histogram */
  /* not implemented */
  return( 1 );
}

int 
MUD_setHistTimeData( int fd, int num, UINT32* pTimeData )
{
  /* return pointer to time history data for a histogram */
  /* not implemented */
  return( 1 );
}

/*
 *  Returns number of bytes in outArray
 *  (not success/failure)
 */
int 
MUD_pack( int num, int inBinSize, void*inArray, int outBinSize, void* outArray )
{
  return( MUD_SEC_GEN_HIST_pack( num, inBinSize, inArray,
                                 outBinSize, outArray ) );
}


int 
MUD_unpack( int num, int inBinSize, void* inArray, int outBinSize, void* outArray )
{
  return( MUD_SEC_GEN_HIST_unpack( num, inBinSize, inArray,
                                   outBinSize, outArray ) );
}


/*
 *  Scalers
 */
#define _sea_scalgrp( fd ) \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TD_ID: \
    default: \
      pMUD_scalGrp = (MUD_SEC_GRP*)MUD_search( pMUD_fileGrp[fd]->pMem,              \
                                 MUD_SEC_GRP_ID, MUD_GRP_TRI_TD_SCALER_ID, \
                                 (UINT32)0 ); \
      break; \
  } \
  if( pMUD_scalGrp == NULL ) return( 0 )


#define _sea_scal( fd, n ) \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TD_ID: \
    default: \
      pMUD_scal = (MUD_SEC_GEN_SCALER*)MUD_search( pMUD_scalGrp->pMem,                \
                                 MUD_SEC_GEN_SCALER_ID, (UINT32)n, \
                                 (UINT32)0 ); \
      break; \
  } \
  if( pMUD_scal == NULL ) return( 0 )


int 
MUD_getScalers( int fd, UINT32* pType, UINT32* pNum )
{
  MUD_SEC_GRP* pMUD_scalGrp=0;

  _check_fd( fd );
  _sea_scalgrp( fd );
  *pType = MUD_instanceID( pMUD_scalGrp );
  *pNum = pMUD_scalGrp->num;

  return( 1 );
}

int 
MUD_setScalers( int fd, UINT32 type, UINT32 num )
{
  MUD_SEC_GRP* pMUD_grp=0;
  MUD_SEC_GEN_SCALER* pMUD_scal=0;
  int i;

  _check_fd( fd );

  pMUD_grp = (MUD_SEC_GRP*)MUD_new( MUD_SEC_GRP_ID, type );
  if( pMUD_grp == NULL ) return( 0 );

  for( i = 0; i < num; i++ )
  {
    pMUD_scal = (MUD_SEC_GEN_SCALER*)MUD_new( MUD_SEC_GEN_SCALER_ID, i+1 );
    if( pMUD_scal == NULL )
    {
      MUD_free( pMUD_grp );
      return( 0 );
    }
    MUD_addToGroup( pMUD_grp, pMUD_scal );
  }

  MUD_addToGroup( pMUD_fileGrp[fd], pMUD_grp );

  return( 1 );
}

int 
MUD_getScalerLabel( int fd, int num, char* label, int strdim )
{
  MUD_SEC_GRP* pMUD_scalGrp=0;
  MUD_SEC_GEN_SCALER* pMUD_scal=0;

  _check_fd( fd );
  _sea_scalgrp( fd );
  _sea_scal( fd, num );
  _strncpy( label, pMUD_scal->label, strdim );

  return( 1 );
}

int 
MUD_setScalerLabel( int fd, int num, char* label )
{
  MUD_SEC_GRP* pMUD_scalGrp=0;
  MUD_SEC_GEN_SCALER* pMUD_scal=0;

  _check_fd( fd );
  _sea_scalgrp( fd );
  _sea_scal( fd, num );
  _free( pMUD_scal->label );
  pMUD_scal->label = strdup( label );

  return( 1 );
}

int 
MUD_getScalerCounts( int fd, int num, UINT32* pCounts )
{
  MUD_SEC_GRP* pMUD_scalGrp=0;
  MUD_SEC_GEN_SCALER* pMUD_scal=0;

  _check_fd( fd );
  _sea_scalgrp( fd );
  _sea_scal( fd, num );

  pCounts[0] = pMUD_scal->counts[0];
  pCounts[1] = pMUD_scal->counts[1];

  return( 1 );
}

int 
MUD_setScalerCounts( int fd, int num, UINT32* pCounts )
{
  MUD_SEC_GRP* pMUD_scalGrp=0;
  MUD_SEC_GEN_SCALER* pMUD_scal=0;

  _check_fd( fd );
  _sea_scalgrp( fd );
  _sea_scal( fd, num );

  pMUD_scal->counts[0] = pCounts[0];
  pMUD_scal->counts[1] = pCounts[1];

  return( 1 );
}


/*
 *  Independent variables
 */
#define _sea_indvargrp( fd ) \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: \
      pMUD_indVarGrp = (MUD_SEC_GRP*)MUD_search( pMUD_fileGrp[fd]->pMem, \
                                 MUD_SEC_GRP_ID, MUD_GRP_GEN_IND_VAR_ARR_ID, \
                                 (UINT32)0 ); \
      break; \
    case MUD_FMT_TRI_TD_ID: \
    default: \
      pMUD_indVarGrp = (MUD_SEC_GRP*)MUD_search( pMUD_fileGrp[fd]->pMem, \
                                 MUD_SEC_GRP_ID, MUD_GRP_GEN_IND_VAR_ID, \
                                 (UINT32)0 ); \
      break; \
  } \
  if( pMUD_indVarGrp == NULL ) return( 0 )


#define _sea_indvar( fd, n ) \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TD_ID: \
    case MUD_FMT_TRI_TI_ID: \
    default: \
      pMUD_indVar = (MUD_SEC_GEN_IND_VAR*)MUD_search( pMUD_indVarGrp->pMem, \
                                 MUD_SEC_GEN_IND_VAR_ID, (UINT32)n, \
                                 (UINT32)0 ); \
      break; \
  } \
  if( pMUD_indVar == NULL ) return( 0 )


#define _sea_indvardat( fd, n ) \
  switch( MUD_instanceID( pMUD_fileGrp[fd] ) ) \
  { \
    case MUD_FMT_TRI_TI_ID: \
    default: \
      pMUD_array = (MUD_SEC_GEN_ARRAY*)MUD_search( pMUD_indVarGrp->pMem, \
                               MUD_SEC_GEN_ARRAY_ID, (UINT32)n, \
                               (UINT32)0 ); \
      break; \
  } \
  if( pMUD_array == NULL ) return( 0 )


#define _indvar_doub_getproc( name, var ) \
int \
name( int fd, int num, double* var ) \
{ \
  MUD_SEC_GRP* pMUD_indVarGrp=0; \
  MUD_SEC_GEN_IND_VAR* pMUD_indVar=0; \
  _check_fd( fd ); \
  _sea_indvargrp( fd ); \
  _sea_indvar( fd, num ); \
  *var = pMUD_indVar->var; \
  return( 1 ); \
}

#define _indvar_doub_setproc( name, var ) \
int \
name( int fd, int num, double var ) \
{ \
  MUD_SEC_GRP* pMUD_indVarGrp=0; \
  MUD_SEC_GEN_IND_VAR* pMUD_indVar=0; \
  _check_fd( fd ); \
  _sea_indvargrp( fd ); \
  _sea_indvar( fd, num ); \
  pMUD_indVar->var = var; \
  return( 1 ); \
}


#define _indvar_char_getproc( name, var ) \
int \
name( int fd, int num, char* var, int strdim ) \
{ \
  MUD_SEC_GRP* pMUD_indVarGrp=0; \
  MUD_SEC_GEN_IND_VAR* pMUD_indVar=0; \
  _check_fd( fd ); \
  _sea_indvargrp( fd ); \
  _sea_indvar( fd, num ); \
  _strncpy( var, pMUD_indVar->var, strdim ); \
  return( 1 ); \
}

#define _indvar_char_setproc( name, var ) \
int \
name( int fd, int num, char* var ) \
{ \
  MUD_SEC_GRP* pMUD_indVarGrp=0; \
  MUD_SEC_GEN_IND_VAR* pMUD_indVar=0; \
  _check_fd( fd ); \
  _sea_indvargrp( fd ); \
  _sea_indvar( fd, num ); \
  _free( pMUD_indVar->var ); \
  pMUD_indVar->var = strdup( var ); \
  return( 1 ); \
}


#define _indvardat_uint_getproc( name, var ) \
int \
name( int fd, int n, UINT32* var ) \
{ \
  MUD_SEC_GRP* pMUD_indVarGrp=0; \
  MUD_SEC_GEN_ARRAY* pMUD_array=0; \
  _check_fd( fd ); \
  _sea_indvargrp( fd ); \
  _sea_indvardat( fd, n ); \
  *var = pMUD_array->var; \
  return( 1 ); \
}

#define _indvardat_uint_setproc( name, var ) \
int \
name( int fd, int n, UINT32 var ) \
{ \
  MUD_SEC_GRP* pMUD_indVarGrp=0; \
  MUD_SEC_GEN_ARRAY* pMUD_array=0; \
  _check_fd( fd ); \
  _sea_indvargrp( fd ); \
  _sea_indvardat( fd, n ); \
  pMUD_array->var = var; \
  return( 1 ); \
}


int 
MUD_getIndVars( int fd, UINT32* pType, UINT32* pNum )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0;

  _check_fd( fd );
  _sea_indvargrp( fd );

  *pType = MUD_instanceID( pMUD_indVarGrp );
  switch( *pType )
  {
    case MUD_GRP_GEN_IND_VAR_ARR_ID:
      /*
       *  NOTE: two sections per independent variable (header,array)
       */
      *pNum = pMUD_indVarGrp->num/2;
      break;
    case MUD_GRP_GEN_IND_VAR_ID:
    default:
      *pNum = pMUD_indVarGrp->num;
      break;
  }
  return( 1 );
}

int 
MUD_setIndVars( int fd, UINT32 type, UINT32 num )
{
  MUD_SEC_GRP* pMUD_grp=0;
  MUD_SEC_GEN_IND_VAR* pMUD_indVar=0;
  MUD_SEC_GEN_ARRAY* pMUD_array=0;
  int i;

  _check_fd( fd );

  pMUD_grp = (MUD_SEC_GRP*)MUD_new( MUD_SEC_GRP_ID, type );
  if( pMUD_grp == NULL ) return( 0 );

  for( i = 0; i < num; i++ )
  {
    pMUD_indVar = (MUD_SEC_GEN_IND_VAR*)MUD_new( MUD_SEC_GEN_IND_VAR_ID, i+1 );
    if( pMUD_indVar == NULL )
    {
      MUD_free( pMUD_grp );
      return( 0 );
    }
    MUD_addToGroup( pMUD_grp, pMUD_indVar );

    if( type == MUD_GRP_GEN_IND_VAR_ARR_ID )
    {
      pMUD_array = (MUD_SEC_GEN_ARRAY*)MUD_new( MUD_SEC_GEN_ARRAY_ID, i+1 );
      if( pMUD_array == NULL )
      {
        MUD_free( pMUD_grp );
        return( 0 );
      }
      MUD_addToGroup( pMUD_grp, pMUD_array );
    }
  }

  MUD_addToGroup( pMUD_fileGrp[fd], pMUD_grp );

  return( 1 );
}

_indvar_doub_getproc( MUD_getIndVarLow, low )
_indvar_doub_getproc( MUD_getIndVarHigh, high )
_indvar_doub_getproc( MUD_getIndVarMean, mean )
_indvar_doub_getproc( MUD_getIndVarStddev, stddev )
_indvar_doub_getproc( MUD_getIndVarSkewness, skewness )
_indvar_char_getproc( MUD_getIndVarName, name )
_indvar_char_getproc( MUD_getIndVarDescription, description )
_indvar_char_getproc( MUD_getIndVarUnits, units )

_indvardat_uint_getproc( MUD_getIndVarNumData, num )
_indvardat_uint_getproc( MUD_getIndVarElemSize, elemSize )
_indvardat_uint_getproc( MUD_getIndVarDataType, type )
_indvardat_uint_getproc( MUD_getIndVarHasTime, hasTime )

_indvar_doub_setproc( MUD_setIndVarLow, low )
_indvar_doub_setproc( MUD_setIndVarHigh, high )
_indvar_doub_setproc( MUD_setIndVarMean, mean )
_indvar_doub_setproc( MUD_setIndVarStddev, stddev )
_indvar_doub_setproc( MUD_setIndVarSkewness, skewness )
_indvar_char_setproc( MUD_setIndVarName, name )
_indvar_char_setproc( MUD_setIndVarDescription, description )
_indvar_char_setproc( MUD_setIndVarUnits, units )

_indvardat_uint_setproc( MUD_setIndVarNumData, num )
_indvardat_uint_setproc( MUD_setIndVarElemSize, elemSize )
_indvardat_uint_setproc( MUD_setIndVarDataType, type )

int
MUD_getIndVarpData( int fd, int num, void** ppData )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0; 
  MUD_SEC_GEN_ARRAY* pMUD_array=0; 
  _check_fd( fd ); 
  _sea_indvargrp( fd ); 
  _sea_indvardat( fd, num ); 
  *ppData = (void*)pMUD_array->pData;
  return( 1 ); 
}

int
MUD_setIndVarpData( int fd, int num, void* pData )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0; 
  MUD_SEC_GEN_ARRAY* pMUD_array=0; 
  _check_fd( fd ); 
  _sea_indvargrp( fd ); 
  _sea_indvardat( fd, num ); 
  pMUD_array->pData = (caddr_t)pData;
  return( 1 ); 
}

int
MUD_getIndVarData( int fd, int num, void* pData )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0; 
  MUD_SEC_GEN_ARRAY* pMUD_array=0; 
  _check_fd( fd ); 
  _sea_indvargrp( fd ); 
  _sea_indvardat( fd, num ); 

  switch( pMUD_array->type )
  {
    case 1:
      /*
       *  Do unpacking/byte swapping
       */
      MUD_unpack( pMUD_array->num, 
                  pMUD_array->elemSize, pMUD_array->pData, 
                  ( pMUD_array->elemSize == 0 ) ? 4 : pMUD_array->elemSize, pData );
      break;
    default:
      bcopy( pMUD_array->pData, pData, pMUD_array->num*pMUD_array->elemSize );
      break;
  }

  return( 1 ); 
}

int
MUD_setIndVarData( int fd, int num, void* pData )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0; 
  MUD_SEC_GEN_ARRAY* pMUD_array=0; 
  _check_fd( fd ); 
  _sea_indvargrp( fd ); 
  _sea_indvardat( fd, num ); 
  _free( pMUD_array->pData );
  switch( pMUD_array->elemSize )
  {
    case 0:
      pMUD_array->pData = (caddr_t)zalloc( 4*pMUD_array->num + 32 );
      break;
    default:
      pMUD_array->pData = (caddr_t)zalloc( pMUD_array->num*pMUD_array->elemSize );
      break;
  }

  switch( pMUD_array->type )
  {
    case 1:
      /*
       *  Do packing/byte swapping
       */
      MUD_pack( pMUD_array->num, 
                ( pMUD_array->elemSize == 0 ) ? 4 : pMUD_array->elemSize, pData,
                pMUD_array->elemSize, pMUD_array->pData );
      break;
    default:
      bcopy( pData, pMUD_array->pData, pMUD_array->num*pMUD_array->elemSize );
      break;
  }

  return( 1 ); 
}

int
MUD_getIndVarpTimeData( int fd, int num, UINT32** ppData )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0; 
  MUD_SEC_GEN_ARRAY* pMUD_array=0; 
  _check_fd( fd ); 
  _sea_indvargrp( fd ); 
  _sea_indvardat( fd, num ); 
  *ppData = (UINT32*)pMUD_array->pTime;
  return( 1 ); 
}

int
MUD_setIndVarpTimeData( int fd, int num, UINT32* pData )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0; 
  MUD_SEC_GEN_ARRAY* pMUD_array=0; 
  _check_fd( fd ); 
  _sea_indvargrp( fd ); 
  _sea_indvardat( fd, num ); 
  pMUD_array->pTime = (TIME*)pData;
  return( 1 ); 
}

int
MUD_getIndVarTimeData( int fd, int num, UINT32* pData )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0; 
  MUD_SEC_GEN_ARRAY* pMUD_array=0; 
  _check_fd( fd ); 
  _sea_indvargrp( fd ); 
  _sea_indvardat( fd, num ); 

  /* 
   *  Already byte swapped
   */
  bcopy( pMUD_array->pTime, pData, 4*pMUD_array->num );

  return( 1 ); 
}

int
MUD_setIndVarTimeData( int fd, int num, UINT32* pData )
{
  MUD_SEC_GRP* pMUD_indVarGrp=0; 
  MUD_SEC_GEN_ARRAY* pMUD_array=0; 

  _check_fd( fd ); 
  _sea_indvargrp( fd ); 
  _sea_indvardat( fd, num );
  _free( pMUD_array->pTime );

  pMUD_array->pTime = (TIME*)zalloc( 4*pMUD_array->num );
  /* 
   *  Don't byte swap here
   */
  bcopy( pData, pMUD_array->pTime, 4*pMUD_array->num );
  pMUD_array->hasTime = 1;

  return( 1 ); 
}

int 
MUD_getHistSecondsPerBin( int fd, int num, REAL64* pSecondsPerBin )
{
  int i;
  UINT32 fsPerBin;

  if( (i = MUD_getHistFsPerBin( fd, num, &fsPerBin )) )
  {
    if( fsPerBin < 16 )
    {
      *pSecondsPerBin = (REAL64)(0.078125e-9 * pow( (double)2.0, (double)(fsPerBin) ));
    }
    else if( fsPerBin < 29 )
    {
      *pSecondsPerBin = (REAL64)(0.048828125e-9 * pow( (double)2.0, (double)(fsPerBin - 16) ));
    }
    else
    {
      *pSecondsPerBin = (REAL64)((double)fsPerBin * 1.0e-15);
    }
  }
  return( i );
}

int 
MUD_setHistSecondsPerBin( int fd, int num, REAL64 secondsPerBin )
{
  if( secondsPerBin < 0.0 || secondsPerBin > 4294967295.0e-15 )
    return( 0 );
  return( MUD_setHistFsPerBin( fd, num, (UINT32)(1.0e15 * secondsPerBin ) ) );
}
