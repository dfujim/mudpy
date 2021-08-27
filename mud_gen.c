/*
 *  mud_gen.c -- procedures for MUD_FMT_GEN sections 
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
 *   v1.0   26-Jan-1994  [T. Whidden] Initial version
 *   v1.0a  15-Feb-1994  [T. Whidden] Split ...GEN_HIST to ...GEN_HIST_HDR
 *                       and ...GEN_HIST_DAT
 *   v1.0b  14-Apr-1994  [T. Whidden] operator -> experimenter
 *   v1.0c  17-Apr-1994  [T. Whidden] added temperature and field to RUN_DESC
 *   v1.0d  11-Jul-1994  [TW] Fixed "unaligned data access" messages in
 *			 MUD_SEC_GEN_HIST_pack()
 *          25-Nov-2009  DA  Handle 8-byte time_t
 */

#include <time.h>
#include <math.h>
#include "mud.h"

/* #define DEBUG 1 */ /*  (un)comment for debug */  
#define PACK_OP 1
#define UNPACK_OP 2
static int pack_op;

static int MUD_SEC_GEN_HIST_dopack _ANSI_ARGS_(( int num, int inBinSize, void* inHist, int outBinSize, void* outHist ));
static int n_bytes_needed _ANSI_ARGS_(( UINT32 val ));
static UINT32 varBinArray _ANSI_ARGS_(( void* pHistData, int binSize, int index ));
static void next_few_bins _ANSI_ARGS_(( int num_tot, int inBinSize, void* pHistData, int outBinSize_now, MUD_VAR_BIN_LEN_TYPE *pNum_next, MUD_VAR_BIN_SIZ_TYPE *pOutBinSize_next ));


int
MUD_SEC_GEN_RUN_DESC_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_GEN_RUN_DESC* pMUD )
{
    int size;
    char tempStr1[32];
    char tempStr2[32];
    time_t bintime;

    switch( op )
    {
	case MUD_FREE:
	    _free( pMUD->title );
	    _free( pMUD->lab );
	    _free( pMUD->area );
	    _free( pMUD->method );
	    _free( pMUD->apparatus );
	    _free( pMUD->insert );
	    _free( pMUD->sample );
	    _free( pMUD->orient );
	    _free( pMUD->das );
	    _free( pMUD->experimenter );
	    _free( pMUD->temperature );
	    _free( pMUD->field );
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->exptNumber );
	    decode_4( pBuf, &pMUD->runNumber );
	    decode_4( pBuf, &pMUD->timeBegin );
	    decode_4( pBuf, &pMUD->timeEnd );
	    decode_4( pBuf, &pMUD->elapsedSec );
	    decode_str( pBuf, &pMUD->title );
	    decode_str( pBuf, &pMUD->lab );
	    decode_str( pBuf, &pMUD->area );
	    decode_str( pBuf, &pMUD->method );
	    decode_str( pBuf, &pMUD->apparatus );
	    decode_str( pBuf, &pMUD->insert );
	    decode_str( pBuf, &pMUD->sample );
	    decode_str( pBuf, &pMUD->orient );
	    decode_str( pBuf, &pMUD->das );
	    decode_str( pBuf, &pMUD->experimenter );
	    decode_str( pBuf, &pMUD->temperature );
	    decode_str( pBuf, &pMUD->field );
	    break;
	case MUD_ENCODE:
	    encode_4( pBuf, &pMUD->exptNumber );
	    encode_4( pBuf, &pMUD->runNumber );
	    encode_4( pBuf, &pMUD->timeBegin );
	    encode_4( pBuf, &pMUD->timeEnd );
	    encode_4( pBuf, &pMUD->elapsedSec );
	    encode_str( pBuf, &pMUD->title );
	    encode_str( pBuf, &pMUD->lab );
	    encode_str( pBuf, &pMUD->area );
	    encode_str( pBuf, &pMUD->method );
	    encode_str( pBuf, &pMUD->apparatus );
	    encode_str( pBuf, &pMUD->insert );
	    encode_str( pBuf, &pMUD->sample );
	    encode_str( pBuf, &pMUD->orient );
	    encode_str( pBuf, &pMUD->das );
	    encode_str( pBuf, &pMUD->experimenter );
	    encode_str( pBuf, &pMUD->temperature );
	    encode_str( pBuf, &pMUD->field );
	    break;
	case MUD_GET_SIZE:
	    size = 3*sizeof( UINT32 ) + 2*sizeof( TIME );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->title );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->lab );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->area );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->method );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->apparatus );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->insert );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->sample );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->orient );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->das );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->experimenter );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->temperature );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->field );
	    return( size );
	case MUD_SHOW:
	    printf( "  MUD_SEC_GEN_RUN_DESC: expt:[%ld], run:[%ld]\n",
                    (long)(pMUD->exptNumber), (long)(pMUD->runNumber) );
	    bintime = pMUD->timeBegin;
	    strncpy( tempStr1, ctime( &bintime ), sizeof(tempStr1) );
	    tempStr1[strlen(tempStr1)-1] = '\0';
	    bintime = pMUD->timeEnd;
	    strncpy( tempStr2, ctime( &bintime ), sizeof(tempStr2) );
	    tempStr2[strlen(tempStr2)-1] = '\0';
	    printf( "    timeBegin:[%s] [%lu]\n", tempStr1, (unsigned long)(pMUD->timeBegin) );
	    printf( "    timeEnd:[%s] [%lu]\n", tempStr2, (unsigned long)(pMUD->timeEnd) );
	    printf( "    elapsedSec:[%ld]\n", (long)(pMUD->elapsedSec) );
	    if( pMUD->title ) printf( "    title:\"%s\"\n", pMUD->title );
	    if( pMUD->lab ) printf( "    lab:\"%s\"\n", pMUD->lab );
	    if( pMUD->area ) printf( "    area:\"%s\"\n", pMUD->area );
	    if( pMUD->method ) printf( "    method:\"%s\"\n", pMUD->method );
	    if( pMUD->apparatus ) 
		printf( "    apparatus:\"%s\"\n", pMUD->apparatus );
	    if( pMUD->insert ) printf( "    insert:\"%s\"\n", pMUD->insert );
	    if( pMUD->sample ) printf( "    sample:\"%s\"\n", pMUD->sample );
	    if( pMUD->orient ) printf( "    orient:\"%s\"\n", pMUD->orient );
	    if( pMUD->das ) printf( "    das:\"%s\"\n", pMUD->das );
	    if( pMUD->experimenter ) 
		printf( "    experimenter:\"%s\"\n", pMUD->experimenter );
	    if( pMUD->temperature ) 
		printf( "    temperature:\"%s\"\n", pMUD->temperature );
	    if( pMUD->field ) 
		printf( "    field:\"%s\"\n", pMUD->field );
	    break;
	case MUD_HEADS:
            printf( "Run number:    %ld\n", (long)(pMUD->runNumber) );
            printf( "  exper num:   %ld\n", (long)(pMUD->exptNumber) );
	    if( pMUD->experimenter ) 
	        printf( "  operator:    %s\n", pMUD->experimenter );
	    if( pMUD->method ) printf( "  method:      %s\n", pMUD->method );
	    bintime = pMUD->timeBegin;
	    strncpy( tempStr1, ctime( &bintime ), sizeof(tempStr1) );
	    tempStr1[strlen(tempStr1)-1] = '\0';
	    bintime = pMUD->timeEnd;
	    strncpy( tempStr2, ctime( &bintime ), sizeof(tempStr2) );
	    tempStr2[strlen(tempStr2)-1] = '\0';
	    printf( "  began:       %s\n  ended:       %s\n",
                    tempStr1, tempStr2 );
            printf( "  elapsed:     %ld:%.2d:%.2d   (%lu seconds)\n",
                    (long)(pMUD->elapsedSec/3600), ((pMUD->elapsedSec%3600)/60), (pMUD->elapsedSec%60),
                    (unsigned long)(pMUD->elapsedSec) );
            if( pMUD->title )  
            {
                if( strlen( pMUD->title ) > 63 && strlen( pMUD->title ) < 80 )
                {
                    printf( "  title:\n%s\n", pMUD->title );
                }
                else
                {
                    printf( "  title:       %s\n", pMUD->title );
                }
            }
	    if( pMUD->sample ) printf( "  sample:      %s\n", pMUD->sample );
	    if( pMUD->orient ) printf( "  orient:      %s\n", pMUD->orient );
	    if( pMUD->temperature ) 
		printf( "  temperature: %s\n", pMUD->temperature );
	    if( pMUD->field ) printf( "  field:       %s\n", pMUD->field );
	    if( pMUD->lab )  printf( "  lab:         %s\n", pMUD->lab );
	    if( pMUD->area ) printf( "  area:        %s\n", pMUD->area );
	    if( pMUD->das )  printf( "  das:         %s\n", pMUD->das );
	    if( pMUD->apparatus ) 
		printf( "  apparatus:   %s\n", pMUD->apparatus );
	    if( pMUD->insert ) printf( "  insert:      %s\n", pMUD->insert );
	    break;
    }
    return( 1 );
}


int
MUD_SEC_GEN_HIST_HDR_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_GEN_HIST_HDR* pMUD )
{
    int size;
    UINT32 fsBin;
    double nsBin;

    switch( op )
    {
	case MUD_FREE:
	    _free( pMUD->title );
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->histType );
	    decode_4( pBuf, &pMUD->nBytes );
	    decode_4( pBuf, &pMUD->nBins );
	    decode_4( pBuf, &pMUD->bytesPerBin );
	    decode_4( pBuf, &pMUD->fsPerBin );
	    decode_4( pBuf, &pMUD->t0_ps );
	    decode_4( pBuf, &pMUD->t0_bin );
	    decode_4( pBuf, &pMUD->goodBin1 );
	    decode_4( pBuf, &pMUD->goodBin2 );
	    decode_4( pBuf, &pMUD->bkgd1 );
	    decode_4( pBuf, &pMUD->bkgd2 );
	    decode_4( pBuf, &pMUD->nEvents );
	    decode_str( pBuf, &pMUD->title );
	    break;
	case MUD_ENCODE:
	    encode_4( pBuf, &pMUD->histType );
	    encode_4( pBuf, &pMUD->nBytes );
	    encode_4( pBuf, &pMUD->nBins );
	    encode_4( pBuf, &pMUD->bytesPerBin );
	    encode_4( pBuf, &pMUD->fsPerBin );
	    encode_4( pBuf, &pMUD->t0_ps );
	    encode_4( pBuf, &pMUD->t0_bin );
	    encode_4( pBuf, &pMUD->goodBin1 );
	    encode_4( pBuf, &pMUD->goodBin2 );
	    encode_4( pBuf, &pMUD->bkgd1 );
	    encode_4( pBuf, &pMUD->bkgd2 );
	    encode_4( pBuf, &pMUD->nEvents );
	    encode_str( pBuf, &pMUD->title );
	    break;
	case MUD_GET_SIZE:
	    size = 12*sizeof( UINT32 );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->title );
	    return( size );
	case MUD_SHOW:
            printf( "  MUD_SEC_GEN_HIST_HDR: histType:[0x%08lX]\n", (unsigned long)(pMUD->histType) );
	    printf( "    nBytes:[%ld], nBins:[%ld], nEvents:[%lu]\n", 
                    (long)(pMUD->nBytes), (long)(pMUD->nBins), (unsigned long)(pMUD->nEvents) );
	    printf( "    bytesPerBin:[%ld], fsPerBin:[%ld], t0_ps:[%ld], t0_bin:[%ld]\n", 
                    (long)(pMUD->bytesPerBin), (long)(pMUD->fsPerBin), (long)(pMUD->t0_ps), (long)(pMUD->t0_bin) );
	    printf( "    goodBin1:[%ld], goodBin2:[%ld], bkgd1:[%ld], bkgd2:[%ld]\n", 
                    (long)(pMUD->goodBin1), (long)(pMUD->goodBin2), (long)(pMUD->bkgd1), (long)(pMUD->bkgd2) );
	    if( pMUD->title ) printf( "    title:\"%s\"\n", pMUD->title );
	    break;
	case MUD_HEADS:
	    if( pMUD->title ) printf( "Hist title: %s\n", pMUD->title );
	    printf( "  nBins: %ld,  nEvents: %lu,  ",
                    (long)(pMUD->nBins), (unsigned long)(pMUD->nEvents) );
	    fsBin = pMUD->fsPerBin;
	    if( fsBin < 16 )
	    {
		nsBin = 0.078125 * pow( (double)2.0, (double)fsBin );
		printf( "ns_per_Bin: %.6lf\n", nsBin );
	    }
	    else if( fsBin < 29 )
	    {
                nsBin = 0.048828125 * pow( (double)2.0, (double)(fsBin - 16) );
		printf( "ns_per_Bin: %.9lf\n", nsBin );
	    }
	    else
	    {
		nsBin = (double)fsBin * 0.000001;
		printf( "ns_per_Bin: %lf\n", nsBin );
	    }

	    printf( "  t0_ps: %ld,  t0_bin: %ld\n", 
                    (long)(pMUD->t0_ps), (long)(pMUD->t0_bin) );
	    printf( "  goodBin1: %ld,  goodBin2: %ld,  bkgd1: %ld,  bkgd2: %ld\n", 
                    (long)(pMUD->goodBin1), (long)(pMUD->goodBin2), (long)(pMUD->bkgd1), (long)(pMUD->bkgd2) );
	    break;
    }
    return( 1 );
}


int
MUD_SEC_GEN_HIST_DAT_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_GEN_HIST_DAT* pMUD )
{
    int size;

    switch( op )
    {
	case MUD_FREE:
	    _free( pMUD->pData );
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->nBytes );
	    pMUD->pData = (char*)zalloc( pMUD->nBytes );
	    _decode_obj( pBuf, pMUD->pData, pMUD->nBytes );
	    break;
	case MUD_ENCODE:
	    encode_4( pBuf, &pMUD->nBytes );
	    _encode_obj( pBuf, pMUD->pData, pMUD->nBytes );
	    break;
	case MUD_GET_SIZE:
	    size = sizeof( UINT32 );
	    size += pMUD->nBytes;
	    return( size );
	case MUD_SHOW:
          printf( "  MUD_SEC_GEN_HIST_DAT: nBytes:[%ld]\n", (long)(pMUD->nBytes) );
	    break;
	case MUD_HEADS:
	    break;
    }
    return( 1 );
}


int 
MUD_SEC_GEN_SCALER_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_GEN_SCALER* pMUD )
{
    int size;

    switch( op )
    {
	case MUD_FREE:
	    _free( pMUD->label );
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->counts[0] );
	    decode_4( pBuf, &pMUD->counts[1] );
	    decode_str( pBuf, &pMUD->label );
	    break;
	case MUD_ENCODE:
	    encode_4( pBuf, &pMUD->counts[0] );
	    encode_4( pBuf, &pMUD->counts[1] );
	    encode_str( pBuf, &pMUD->label );
	    break;
	case MUD_GET_SIZE:
	    size = 2*sizeof( UINT32 );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->label );
	    return( size );
	case MUD_SHOW:
	    printf( "  MUD_SEC_GEN_SCALER: counts[1]:[%lu], counts[0]:[%lu]\n",
                    (unsigned long)(pMUD->counts[1]), (unsigned long)(pMUD->counts[0]) );
	    if( pMUD->label ) printf( "    label:\"%s\"\n", pMUD->label );
	    break;
	case MUD_HEADS:
	    printf( "  Scaler " );
	    if( pMUD->label ) printf( "%8s", pMUD->label );
	    else  printf( "???     " );
	    printf( "  total: %11lu,  recent: %lu\n", 
                    (unsigned long)(pMUD->counts[0]), (unsigned long)(pMUD->counts[1]) );
	    break;
    }
    return( 1 );
}


int
MUD_SEC_GEN_IND_VAR_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_GEN_IND_VAR* pMUD )
{
    int size;

    switch( op )
    {
	case MUD_FREE:
	    _free( pMUD->name );
	    _free( pMUD->description );
	    _free( pMUD->units );
	    break;
	case MUD_DECODE:
	    decode_double( pBuf, &pMUD->low );
	    decode_double( pBuf, &pMUD->high );
	    decode_double( pBuf, &pMUD->mean );
	    decode_double( pBuf, &pMUD->stddev );
	    decode_double( pBuf, &pMUD->skewness );
	    decode_str( pBuf, &pMUD->name );
	    decode_str( pBuf, &pMUD->description );
	    decode_str( pBuf, &pMUD->units );
	    break;
	case MUD_ENCODE:
	    encode_double( pBuf, &pMUD->low );
	    encode_double( pBuf, &pMUD->high );
	    encode_double( pBuf, &pMUD->mean );
	    encode_double( pBuf, &pMUD->stddev );
	    encode_double( pBuf, &pMUD->skewness );
	    encode_str( pBuf, &pMUD->name );
	    encode_str( pBuf, &pMUD->description );
	    encode_str( pBuf, &pMUD->units );
	    break;
	case MUD_GET_SIZE:
	    size =  5*sizeof( double );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->name );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->description );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->units );
	    return( size );
	case MUD_SHOW:
	    printf( "  MUD_SEC_GEN_IND_VAR: \n" );
	    printf( "    mean:[%.10f], stddev:[%.10f], skewness:[%.10f]\n", 
			pMUD->mean, pMUD->stddev, pMUD->skewness );
	    printf( "    low:[%.10f], high:[%.10f]\n", pMUD->low, pMUD->high );
	    if( pMUD->name ) printf( "    name:\"%s\"\n", pMUD->name );
	    if( pMUD->description ) printf( "    description:\"%s\"\n", pMUD->description );
	    if( pMUD->units ) printf( "    units:\"%s\"\n", pMUD->units );
	    break;
	case MUD_HEADS:
	    printf( "Independent variable: " );
	    if( pMUD->name ) printf( "%s\n", pMUD->name );
	    if( pMUD->description ) 
		printf( "  description: %s\n", pMUD->description );
	    printf( "  mean: %.9lf,  stddev: %.9lf,  skewness: %.9lf\n", 
			pMUD->mean, pMUD->stddev, pMUD->skewness );
	    printf( "  low:  %.9lf,  high:   %.9lf,  ", pMUD->low, pMUD->high );
	    if( pMUD->units ) printf( "units: %s\n", pMUD->units );
	    break;
    }
    return( 1 );
}


int 
MUD_SEC_GEN_ARRAY_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_GEN_ARRAY* pMUD )
{
    int size;
    int i;

    switch( op )
    {
	case MUD_FREE:
	    _free( pMUD->pData );
	    if( pMUD->hasTime ) _free( pMUD->pTime );
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->num );
	    decode_4( pBuf, &pMUD->elemSize );
	    decode_4( pBuf, &pMUD->type );
	    decode_4( pBuf, &pMUD->hasTime );
	    decode_4( pBuf, &pMUD->nBytes );
	    pMUD->pData = (caddr_t)zalloc( pMUD->num*pMUD->elemSize );
            switch( pMUD->type )
            {
              case 1:
                _decode_obj( pBuf, pMUD->pData, pMUD->nBytes );
                break;
              case 2:
                switch( pMUD->elemSize )
                {
                  case 4:
                    for( i = 0; i < pMUD->num; i++ )
                    {
                      decode_float( pBuf, &(((float*)pMUD->pData)[i]) );
                    }
                    break;
                  case 8:
                    for( i = 0; i < pMUD->num; i++ )
                    {
                      decode_double( pBuf, &(((double*)pMUD->pData)[i]) );
                    }
                    break;
                }
                break;
              case 3:
                _decode_obj( pBuf, pMUD->pData, pMUD->nBytes );
                break;
            }
            if( pMUD->hasTime )
            {
	      pMUD->pTime = (TIME*)zalloc( pMUD->num*sizeof(TIME) );
              for( i = 0; i < pMUD->num; i++ )
              {
                decode_4( pBuf, &(((UINT32*)pMUD->pTime)[i]) );
              }
            }
	    break;
	case MUD_ENCODE:
	    encode_4( pBuf, &pMUD->num );
	    encode_4( pBuf, &pMUD->elemSize );
	    encode_4( pBuf, &pMUD->type );
	    encode_4( pBuf, &pMUD->hasTime );
	    encode_4( pBuf, &pMUD->nBytes );
            switch( pMUD->type )
            {
              case 1:
                _encode_obj( pBuf, pMUD->pData, pMUD->nBytes );
                break;
              case 2:
                switch( pMUD->elemSize )
                {
                  case 4:
                    for( i = 0; i < pMUD->num; i++ )
                    {
                      encode_float( pBuf, &(((float*)pMUD->pData)[i]) );
                    }
                    break;
                  case 8:
                    for( i = 0; i < pMUD->num; i++ )
                    {
                      encode_double( pBuf, &(((double*)pMUD->pData)[i]) );
                    }
                    break;
                }
                break;
              case 3:
                _encode_obj( pBuf, pMUD->pData, pMUD->nBytes );
                break;
            }
            if( pMUD->hasTime )
            {
              for( i = 0; i < pMUD->num; i++ )
              {
                encode_4( pBuf, &(((UINT32*)pMUD->pTime)[i]) );
              }
            }
	    break;
	case MUD_GET_SIZE:
	    size =  4*sizeof( UINT32 ) + sizeof( BOOL );
	    size += pMUD->nBytes;
            if( pMUD->hasTime )
            {
	      size += pMUD->num*sizeof(TIME);
            }
	    return( size );
	case MUD_SHOW:
	    printf( "  MUD_SEC_GEN_ARRAY: \n" );
	    printf( "    num:[%ld], elemSize:[%ld], type:[%ld], hasTime:[%ld], nBytes:[%ld]\n", 
                    (long)(pMUD->num), (long)(pMUD->elemSize), (long)(pMUD->type), (long)(pMUD->hasTime),
                    (long)(pMUD->nBytes) );
	    break;
	case MUD_HEADS:
	    break;
    }
    return( 1 );
}


int
MUD_SEC_GEN_HIST_pack( int num, int inBinSize, void* inHist, int outBinSize, void* outHist )
{
  pack_op = PACK_OP;
  return( MUD_SEC_GEN_HIST_dopack( num, inBinSize, inHist, outBinSize, outHist ) );
}

int
MUD_SEC_GEN_HIST_unpack( int num, int inBinSize, void* inHist, int outBinSize, void* outHist )
{
  pack_op = UNPACK_OP;
  return( MUD_SEC_GEN_HIST_dopack( num, inBinSize, inHist, outBinSize, outHist ) );
}

static int
MUD_SEC_GEN_HIST_dopack( int num, int inBinSize, void* inHist, int outBinSize, void* outHist )
{
    int i;
    int outLen = 0;
    MUD_VAR_BIN_LEN_TYPE num_temp;
    int bin, inLoc, outLoc;
    int outLen_temp;
    MUD_VAR_BIN_SIZ_TYPE outBinSize_now, outBinSize_next;
    MUD_VAR_BIN_SIZ_TYPE inBinSize_temp;
    UINT8  c, *pc;
    UINT16 s, *ps, *ps2;
    UINT32 l, *pl, *pl2;


#ifdef DEBUG

        switch( pack_op )
        {
          case PACK_OP:
            printf("MUD_SEC_GEN_HIST_dopack (pack) starting with \n");
            break;
          case UNPACK_OP:
            printf("MUD_SEC_GEN_HIST_dopack (unpack) starting with \n");
            break;
        }
       printf("     num: %d, inBinSize: %d, outBinSize: %d\n",
              num,inBinSize,outBinSize);

#endif /* DEBUG */
    
    if( inBinSize == 1 && outBinSize == 1 )
    {
	bcopy( inHist, outHist, num );
	outLen = num*outBinSize;
    }
    else if( inBinSize == 2 && outBinSize == 2 )
    {
        ps = (UINT16*)inHist;
        ps2 = (UINT16*)outHist;
        switch( pack_op )
        {
          case PACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bencode_2( ps2, ps ); ps2++; ps++;
            }
            break;
          case UNPACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bdecode_2( ps, ps2 ); ps++; ps2++;
            }
            break;
        }
	outLen = num*outBinSize;
    }
    else if( inBinSize == 4 && outBinSize == 4 )
    {
        pl = (UINT32*)inHist;
        pl2 = (UINT32*)outHist;
        switch( pack_op )
        {
          case PACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bencode_4( pl2, pl ); pl2++; pl++;
            }
            break;
          case UNPACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bdecode_4( pl, pl2 ); pl++; pl2++;
            }
            break;
        }
	outLen = num*outBinSize;
    }
    else if( inBinSize == 1 && outBinSize == 2 )
    {
        pc = (UINT8*)inHist;
        ps = (UINT16*)outHist;
        switch( pack_op )
        {
          case PACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bcopy( pc, &c, 1 );
              s = (UINT16)c;
              bencode_2( ps, &s ); 
              ps++; pc++;
            }
            break;
          case UNPACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bdecode_1( pc, &c );
              s = (UINT16)c;  
              bcopy( &s, ps, 2 );
              ps++; pc++;
            }
            break;
        }
	outLen = num*outBinSize;
    }
    else if( inBinSize == 1 && outBinSize == 4 )
    {
        pc = (UINT8*)inHist;
        pl = (UINT32*)outHist;
        switch( pack_op )
        {
          case PACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bcopy( pc, &c, 1 );
              l = (UINT32)c;
              bencode_4( pl, &l ); 
              pl++; pc++;
            }
            break;
          case UNPACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bdecode_1( pc, &c );
              l = (UINT32)c;  
              bcopy( &l, pl, 4 );
              pl++; pc++;
            }
            break;
        }
	outLen = num*outBinSize;
    }
    else if( inBinSize == 2 && outBinSize == 1 )
    {
        ps = (UINT16*)inHist;
        pc = (UINT8*)outHist;
        switch( pack_op )
        {
          case PACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bcopy( ps, &s, 2 );
              c = (UINT8)s;
              bencode_1( pc, &c ); 
              ps++; pc++;
            }
            break;
          case UNPACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bdecode_2( ps, &s );
              c = (UINT8)s;   
              bcopy( &c, pc, 1 );
              ps++; pc++;
            }
            break;
        }
	outLen = num*outBinSize;
    }
    else if( inBinSize == 2 && outBinSize == 4 )
    {
        ps = (UINT16*)inHist;
        pl = (UINT32*)outHist;
        switch( pack_op )
        {
          case PACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bcopy( ps, &s, 2 );
              l = (UINT32)s;
              bencode_4( pl, &l ); 
              pl++; ps++;
            }
            break;
          case UNPACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bdecode_2( ps, &s );
              l = (UINT32)s;  
              bcopy( &l, pl, 4 );
              pl++; ps++;
            }
            break;
        }
	outLen = num*outBinSize;
    }
    else if( inBinSize == 4 && outBinSize == 1 )
    {
        pl = (UINT32*)inHist;
        pc = (UINT8*)outHist;
        switch( pack_op )
        {
          case PACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bcopy( pl, &l, 4 );
              c = (UINT8)l;
              bencode_1( pc, &c ); 
              pl++; pc++;
            }
            break;
          case UNPACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bdecode_4( pl, &l );
              c = (UINT8)l;
              bcopy( &c, pc, 1 );
              pl++; pc++;
            }
            break;
        }
	outLen = num*outBinSize;
    }
    else if( inBinSize == 4 && outBinSize == 2 )
    {
        pl = (UINT32*)inHist;
        ps = (UINT16*)outHist;
        switch( pack_op )
        {
          case PACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bcopy( pl, &l, 4 );
              s = (UINT16)l;
              bencode_2( ps, &s ); 
              pl++; ps++;
            }
            break;
          case UNPACK_OP:
	    for( i = 0; i < num; i++ )
            {
              bdecode_4( pl, &l );
              s = (UINT16)l;
              bcopy( &s, ps, 2 );
              pl++; ps++;
            }
            break;
        }
	outLen = num*outBinSize;
    }
    else if( inBinSize == 0 )
    {
	bin = 0;
	inLoc = 0;
	outLoc = 0;
	while( bin < num )
	{
            switch( pack_op )
            {
              case PACK_OP:
	        bcopy( &((char*)inHist)[inLoc], &num_temp, 2 );
                break;
              case UNPACK_OP:
                bdecode_2( &((char*)inHist)[inLoc], &num_temp );
                break;
            }
	    inLoc += 2;

	    bcopy( &((char*)inHist)[inLoc], &inBinSize_temp, 1 );
	    inLoc += 1;

#ifdef DEBUG
            printf( "unpacking %d bins of %d bytes\n", num_temp, 
                    inBinSize_temp );
#endif /* DEBUG */

	    outLen_temp = num_temp*outBinSize;

	    if( inBinSize_temp == 0 )
	    {
		bzero( &((char*)outHist)[outLoc], outLen_temp );
	    }
	    else
	    {
              switch( pack_op )
              {
               case PACK_OP:
 		MUD_SEC_GEN_HIST_pack( num_temp, 
		    inBinSize_temp, (void*)&((char*)inHist)[inLoc],
		    outBinSize, (void*)&((char*)outHist)[outLoc] );
                break;
               case UNPACK_OP:
		MUD_SEC_GEN_HIST_unpack( num_temp, 
		    inBinSize_temp, (void*)&((char*)inHist)[inLoc],
		    outBinSize, (void*)&((char*)outHist)[outLoc] );
                break;
              }
              inLoc += num_temp*inBinSize_temp;
	    }

	    outLoc += outLen_temp;
	    bin += num_temp;
	}
	outLen = num*outBinSize;
    }
    else if( outBinSize == 0 )
    {
	bin = 0;
	inLoc = 0;
	outLoc = 0;
        outBinSize_now = n_bytes_needed( varBinArray( inHist, inBinSize, 0 ) );

	while( bin < num )
	{
	    next_few_bins( num - bin, inBinSize, &((char*)inHist)[inLoc],
			   outBinSize_now, &num_temp, &outBinSize_next );

#ifdef DEBUG
            printf("\n");
            printf("MUD_SEC_GEN_HIST_DOPACK - packing %d bins of %d bytes\n", 
                   num_temp, outBinSize_now );
            printf("\n");
#endif /* DEBUG */

            switch( pack_op )
            {
              case PACK_OP:
                bencode_2( &((char*)outHist)[outLoc], &num_temp );
                break;
              case UNPACK_OP:
	        bcopy( &num_temp, &((char*)outHist)[outLoc], 2 );
                break;
            }
            outLoc += 2;

	    bcopy( &outBinSize_now, &((char*)outHist)[outLoc], 1 );
            outLoc += 1;

	    if( outBinSize_now != 0 )
	    {
              switch( pack_op )
              {
               case PACK_OP:
		MUD_SEC_GEN_HIST_pack( num_temp, 
		    inBinSize, (void*)&((char*)inHist)[inLoc],
		    outBinSize_now, (void*)&((char*)outHist)[outLoc] );
                break;
               case UNPACK_OP:
		MUD_SEC_GEN_HIST_unpack( num_temp, 
		    inBinSize, (void*)&((char*)inHist)[inLoc],
		    outBinSize_now, (void*)&((char*)outHist)[outLoc] );
                break;
              }
              outLoc += num_temp*outBinSize_now;
	    }

	    outBinSize_now = outBinSize_next;
	    inLoc += num_temp*inBinSize;
	    bin += num_temp;
	}
	outLen = outLoc;
    }

    return( outLen );
}


static int
n_bytes_needed( UINT32 val )
{
    if( val & 0xFFFF0000 ) return( 4 );
    else if( val & 0x0000FF00 ) return( 2 );
    else if( val & 0x000000FF ) return( 1 );
    else return( 0 );
}


static UINT32
varBinArray( void* pHistData, int binSize, int index )
{
  UINT8  c;
  UINT16 s;
  UINT32 l;

  switch( pack_op )
  {
    case PACK_OP:
      switch( binSize )
      {
        case 1:
          bcopy( &((UINT8*)pHistData)[index], &c, 1 );
	  return( c );
	case 2:
          bcopy( &((UINT16*)pHistData)[index], &s, 2 );
	  return( s );
	case 4:
          bcopy( &((UINT32*)pHistData)[index], &l, 4 );
	  return( l );
      }
      break;
    case UNPACK_OP:
      switch( binSize )
      {
        case 1:
          bdecode_1( &((UINT8*)pHistData)[index], &c );
	  return( c );
	case 2:
          bdecode_2( &((UINT16*)pHistData)[index], &s );
	  return( s );
	case 4:
          bdecode_4( &((UINT32*)pHistData)[index], &l );
	  return( l );
      }
      break;
  }
  return( 0 );
}


static void
next_few_bins( int num_tot, int inBinSize, void* pHistData, int outBinSize_now,
               MUD_VAR_BIN_LEN_TYPE* pNum_next, MUD_VAR_BIN_SIZ_TYPE* pOutBinSize_next )
{
    int val;
    MUD_VAR_BIN_LEN_TYPE num_next;
    MUD_VAR_BIN_LEN_TYPE num_nextLower;
    MUD_VAR_BIN_SIZ_TYPE outBinSize_next;
    MUD_VAR_BIN_SIZ_TYPE outBinSize_nextLower;
    MUD_VAR_BIN_SIZ_TYPE outBinSize_nextLowerFirst;
    int bytesNextLower;
    int bytesNextLowerDoNow;

/* Maximum of 16 bits for num_next; must break up storage into pieces 
   of maximum length 65535 or num_next becomes negative */
 
    MUD_VAR_BIN_LEN_TYPE temp_next; /* needed to check for overflow */

#ifdef DEBUG
    printf("Next_few_bins starting with num_tot: %d\n",num_tot); 
#endif /* DEBUG */

    num_next = num_nextLower = 0;
    outBinSize_nextLowerFirst = outBinSize_nextLower = outBinSize_next = 
	outBinSize_now;
    bytesNextLower = bytesNextLowerDoNow = 0;


    while( num_next < num_tot )
    {
      /* check for overflow of short int. Turns negative if > 65535 
         Have to store data in chunks of maximum length 65535  */
      temp_next = num_next + 1 ;
      if (temp_next <  num_next ) {

#ifdef DEBUG
        printf ("Next_few_bins: num_next will overflow; num_next: %d\n",
                       num_next);
        printf ("               Breaking out of Next_few_bins ... \n");
#endif /* DEBUG */

        break;
      } 

	val = varBinArray( pHistData, inBinSize, num_next );
	outBinSize_next = n_bytes_needed( val );
	if( outBinSize_next == outBinSize_now ) 
	{
	    num_next++;
	    if( num_nextLower != 0 )
	    {
		num_nextLower = 0;
		outBinSize_nextLowerFirst = outBinSize_nextLower = outBinSize_now;
		bytesNextLower = bytesNextLowerDoNow = 0;
	    }
	}
	else if( outBinSize_next < outBinSize_now )
	{
	    if( outBinSize_nextLowerFirst == outBinSize_now )
		outBinSize_nextLowerFirst = outBinSize_next;

	    if( outBinSize_nextLower != outBinSize_next )
	    {
		bytesNextLower += sizeof( MUD_VAR_BIN_LEN_TYPE ) +
				  sizeof( MUD_VAR_BIN_SIZ_TYPE ) +
				  outBinSize_next;
		outBinSize_nextLower = outBinSize_next;
	    }
	    else
	    {
		bytesNextLower += outBinSize_next;
	    }
	    bytesNextLowerDoNow += outBinSize_now;

	    if( bytesNextLowerDoNow < ( bytesNextLower + 
					sizeof( MUD_VAR_BIN_LEN_TYPE ) +
					sizeof( MUD_VAR_BIN_SIZ_TYPE ) ) )
	    {
		num_next++;
		num_nextLower++;
	    }
	    else
	    {
		num_next -= num_nextLower;
		outBinSize_next = outBinSize_nextLowerFirst;
		break;
	    }
	}
	else break;
    }

    *pOutBinSize_next = outBinSize_next;
    *pNum_next = num_next;
}


