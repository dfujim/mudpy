/*
 *  mud_tri_ti.c -- procedures for MUD_FMT_TRI_TI sections 
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
 *   v1.0a  14-Apr-1994  [T. Whidden] operator -> experimenter
 *   v1.0b  22-Apr-1994  [T. Whidden] rename TI to TRI_TI
 *          25-Nov-2009  DA  Handle 8-byte time_t
 */

#include <time.h>
#include "mud.h"


int
MUD_SEC_TRI_TI_RUN_DESC_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_TRI_TI_RUN_DESC* pMUD )
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
	    _free( pMUD->subtitle );
	    _free( pMUD->comment1 );
	    _free( pMUD->comment2 );
	    _free( pMUD->comment3 );
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
	    decode_str( pBuf, &pMUD->subtitle );
	    decode_str( pBuf, &pMUD->comment1 );
	    decode_str( pBuf, &pMUD->comment2 );
	    decode_str( pBuf, &pMUD->comment3 );
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
	    encode_str( pBuf, &pMUD->subtitle );
	    encode_str( pBuf, &pMUD->comment1 );
	    encode_str( pBuf, &pMUD->comment2 );
	    encode_str( pBuf, &pMUD->comment3 );
	    break;
	case MUD_GET_SIZE:
	    size = 5*sizeof( UINT32 );
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
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->subtitle );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->comment1 );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->comment2 );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->comment3 );
	    return( size );
	case MUD_SHOW:
	    printf( "  MUD_SEC_TRI_TI_RUN_DESC: expt:[%lu], run:[%lu]\n",
                    (unsigned long)(pMUD->exptNumber), (unsigned long)(pMUD->runNumber) );
	    bintime = pMUD->timeBegin;
	    strncpy( tempStr1, ctime( &bintime ), sizeof(tempStr1) );
	    tempStr1[strlen(tempStr1)-1] = '\0';
	    bintime = pMUD->timeEnd;
	    strncpy( tempStr2, ctime( &bintime ), sizeof(tempStr2) );
	    tempStr2[strlen(tempStr2)-1] = '\0';
	    printf( "    timeBegin:[%s]\n", tempStr1 );
	    printf( "    timeEnd:[%s]\n", tempStr2 );
	    printf( "    elapsedSec:[%lu]\n", (unsigned long)(pMUD->elapsedSec) );
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
	    if( pMUD->subtitle ) 
		printf( "    subtitle:\"%s\"\n", pMUD->subtitle );
	    if( pMUD->comment1 ) 
		printf( "    comment1:\"%s\"\n", pMUD->comment1 );
	    if( pMUD->comment2 ) 
		printf( "    comment2:\"%s\"\n", pMUD->comment2 );
	    if( pMUD->comment3 ) 
		printf( "    comment3:\"%s\"\n", pMUD->comment3 );
	    break;
	case MUD_HEADS:
            printf( "Run number:     %lu\n", (unsigned long)(pMUD->runNumber) );
            printf( "  exper num:    %lu\n", (unsigned long)(pMUD->exptNumber) );
	    if( pMUD->experimenter ) 
	        printf( "  operator:     %s\n", pMUD->experimenter );
            if( pMUD->method ) 
                printf( "  method:       %s\n", pMUD->method );
	    bintime = pMUD->timeBegin;
	    strncpy( tempStr1, ctime( &bintime ), sizeof(tempStr1) );
	    tempStr1[strlen(tempStr1)-1] = '\0';
	    bintime = pMUD->timeEnd;
	    strncpy( tempStr2, ctime( &bintime ), sizeof(tempStr2) );
	    tempStr2[strlen(tempStr2)-1] = '\0';
	    printf( "  began:        %s\n  ended:        %s\n",
                     tempStr1, tempStr2 );
            printf( "  elapsed:      %ld:%.2d:%.2d   (%lu seconds)\n",
                    (long)(pMUD->elapsedSec/3600), ((pMUD->elapsedSec%3600)/60), (pMUD->elapsedSec%60), 
                    (unsigned long)(pMUD->elapsedSec) );
	    if( pMUD->title )  printf( "  title:        %s\n", pMUD->title );
	    if( pMUD->sample ) printf( "  sample:       %s\n", pMUD->sample );
	    if( pMUD->orient ) printf( "  orient:       %s\n", pMUD->orient );
	    if( pMUD->subtitle )
                printf( "  subtitle:     %s\n", pMUD->subtitle );
	    if( pMUD->lab )    printf( "  lab:          %s\n", pMUD->lab );
	    if( pMUD->area )   printf( "  area:         %s\n", pMUD->area );
	    if( pMUD->das )    printf( "  das:          %s\n", pMUD->das );
	    if( pMUD->apparatus ) 
		printf( "  apparatus:    %s\n", pMUD->apparatus );
	    if( pMUD->insert ) printf( "  insert:       %s\n", pMUD->insert );
	    if( pMUD->comment1 ) 
		printf( "  comment1:     %s\n", pMUD->comment1 );
	    if( pMUD->comment2 ) 
		printf( "  comment2:     %s\n", pMUD->comment2 );
	    if( pMUD->comment3 ) 
		printf( "  comment3:     %s\n", pMUD->comment3 );
	    break;
    }
    return( 1 );
}


