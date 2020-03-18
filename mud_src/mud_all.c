/*
 *  mud_all.c -- procedures for MUD_FMT_ALL sections 
 *
 *   Copyright (C) 1994-2010 TRIUMF (Vancouver, Canada)
 *
 *   Authors: T. Whidden, D. Arseneau, S. Daviel
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
 *   v1.00  15-Feb-1994  [T. Whidden] Initial version
 *   v1.10  17-Feb-1994  [T. Whidden] Groups with member index
 *   v1.2a  01-Mar-2000  DA  Proc for unknown sections
 *          25-Nov-2009  DA  Handle 8-byte time_t
 */

#include <time.h>
#include "mud.h"


int
MUD_SEC_proc( MUD_OPT op, BUF* pBuf, MUD_SEC* pMUD )
{
    switch( op )
    {
	case MUD_FREE:
	    break;
	case MUD_DECODE:
	    break;
	case MUD_ENCODE:
	    break;
	case MUD_GET_SIZE:
	    return( 0 );
	case MUD_SHOW:
	    printf( "  MUD_SEC: \n" );
	    break;
	case MUD_HEADS:
	    break;
    }
    return( 1 );
}


int
MUD_SEC_EOF_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_EOF* pMUD )
{
    switch( op )
    {
	case MUD_FREE:
	    break;
	case MUD_DECODE:
	    break;
	case MUD_ENCODE:
	    break;
	case MUD_GET_SIZE:
	    return( 0 );
	case MUD_SHOW:
	    printf( "  MUD_EOF: \n" );
	    break;
	case MUD_HEADS:
	    break;
    }
    return( 1 );
}


int
MUD_SEC_FIXED_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_FIXED* pMUD )
{
    int size;

    switch( op )
    {
	case MUD_FREE:
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->fileSize );
	    decode_4( pBuf, &pMUD->formatID );
	    break;
	case MUD_ENCODE:
	    encode_4( pBuf, &pMUD->fileSize );
	    encode_4( pBuf, &pMUD->formatID );
	    break;
	case MUD_GET_SIZE:
	    size = 2*sizeof( UINT32 );
	    return( size );
	case MUD_SHOW:
	    printf( "  MUD_SEC_FIXED: fileSize=[%lu], formatID=[0x%08lX]\n",
		    (unsigned long)(pMUD->fileSize), (unsigned long)(pMUD->formatID) );
	    break;
	case MUD_HEADS:
	    break;
    }
    return( 1 );
}


int
MUD_SEC_GRP_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_GRP* pMUD )
{
    int size;
    int i;
    MUD_INDEX** ppMUD_index;
    MUD_INDEX* pMUD_index;

    switch( op )
    {
	case MUD_FREE:
	    MUD_INDEX_proc( MUD_FREE, NULL, pMUD->pMemIndex );
	    MUD_free( pMUD->pMem );
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->num );
	    decode_4( pBuf, &pMUD->memSize );
	    ppMUD_index = &pMUD->pMemIndex;
	    for( i = 0; i < pMUD->num; i++ )
	    {
		pMUD_index = (MUD_INDEX*)zalloc( sizeof( MUD_INDEX ) );
		MUD_INDEX_proc( MUD_DECODE, pBuf, pMUD_index );
		*ppMUD_index = pMUD_index;
		ppMUD_index = &(*ppMUD_index)->pNext;
	    }
	    break;
	case MUD_ENCODE:
	    encode_4( pBuf, &pMUD->num );
	    encode_4( pBuf, &pMUD->memSize );

	    for( pMUD_index = pMUD->pMemIndex;
		 pMUD_index != NULL; 
		 pMUD_index = pMUD_index->pNext )
		MUD_INDEX_proc( MUD_ENCODE, pBuf, pMUD_index );
	    break;
	case MUD_GET_SIZE:
	    size = 2*sizeof( UINT32 );
	    size += pMUD->num*MUD_INDEX_proc( MUD_GET_SIZE, NULL, NULL );
	    return( size );
	case MUD_SHOW:
	    printf( "  MUD_SEC_GRP: num=[%ld], memSize=%ld\n", 
		    (long int)(pMUD->num), (long int)(pMUD->memSize) );
	    for( pMUD_index = pMUD->pMemIndex;
		 pMUD_index != NULL;
		 pMUD_index = pMUD_index->pNext )
		MUD_INDEX_proc( MUD_SHOW, NULL, pMUD_index );
	    break;
	case MUD_HEADS:
	    for( pMUD_index = pMUD->pMemIndex;
		 pMUD_index != NULL;
		 pMUD_index = pMUD_index->pNext )
		MUD_INDEX_proc( MUD_HEADS, NULL, pMUD_index );
	    break;
    }
    return( 1 );
}


int
MUD_SEC_CMT_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_CMT* pMUD )
{
    int size;
    char tempStr1[32];
    time_t bintime;

    switch( op )
    {
	case MUD_FREE:
	    _free( pMUD->author );
	    _free( pMUD->title );
	    _free( pMUD->comment );
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->ID );
	    decode_4( pBuf, &pMUD->prevReplyID );
	    decode_4( pBuf, &pMUD->nextReplyID );
	    decode_4( pBuf, &pMUD->time );
	    decode_str( pBuf, &pMUD->author );
	    decode_str( pBuf, &pMUD->title );
	    decode_str( pBuf, &pMUD->comment );
	    break;
	case MUD_ENCODE:
	    encode_4( pBuf, &pMUD->ID );
	    encode_4( pBuf, &pMUD->prevReplyID );
	    encode_4( pBuf, &pMUD->nextReplyID );
	    encode_4( pBuf, &pMUD->time );
	    encode_str( pBuf, &pMUD->author );
	    encode_str( pBuf, &pMUD->title );
	    encode_str( pBuf, &pMUD->comment );
	    break;
	case MUD_GET_SIZE:
	    size = 3*sizeof( UINT32 );
	    size += 1*sizeof( TIME );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->author );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->title );
	    size += sizeof( MUD_STR_LEN_TYPE ) + _strlen( pMUD->comment );
	    return( size );
	case MUD_SHOW:
	    printf( "  MUD_SEC_CMT: \n" );
	    printf( "    number:[%lu],  prevReply:[%lu],  nextReply:[%lu]\n", 
                    (unsigned long)(pMUD->ID), (unsigned long)(pMUD->prevReplyID),
                    (unsigned long)(pMUD->nextReplyID) );
	    bintime = pMUD->time;
	    strncpy( tempStr1, ctime( &bintime ), sizeof(tempStr1) );
	    tempStr1[strlen(tempStr1)-1] = '\0';
	    printf( "    time:[%s]\n", tempStr1 );
	    if( pMUD->author ) printf( "    author:\"%s\"\n", pMUD->author );
	    if( pMUD->title ) printf( "    title:\"%s\"\n", pMUD->title );
	    if( pMUD->comment ) printf( "    comment:\"%s\"\n", pMUD->comment );
	    break;
	case MUD_HEADS:
            printf( "Comment number %lu.     ", (unsigned long)(pMUD->ID) );
            if( pMUD->prevReplyID > 0 )
              printf("  Re: #%lu.    ", (unsigned long)(pMUD->prevReplyID) );
            if( pMUD->nextReplyID > 0 )
              printf("  Next: #%lu.", (unsigned long)(pMUD->nextReplyID) );
            printf( "\n" );
	    bintime = pMUD->time;
	    strncpy( tempStr1, ctime( &bintime ), sizeof(tempStr1) );
	    tempStr1[strlen(tempStr1)-1] = '\0';
	    if( pMUD->author ) printf( "    author: %s,     time: %s\n", pMUD->author, tempStr1 );
	    if( pMUD->title ) printf( "    title: %s\n", pMUD->title );
	    if( pMUD->comment ) printf( "%s\n", pMUD->comment );
	    break;
    }
    return( 1 );
}


int
MUD_SEC_UNKNOWN_proc( MUD_OPT op, BUF* pBuf, MUD_SEC_UNKNOWN* pMUD )
{
    switch( op )
    {
	case MUD_FREE:
	    break;
	case MUD_DECODE:
	    break;
	case MUD_ENCODE:
	    break;
	case MUD_GET_SIZE:
	    return( 0 );
	case MUD_SHOW:
	    printf( "  UNKNOWN MUD SECTION: [] \n" );
	    break;
	case MUD_HEADS:
	    break;
    }
    return( 1 );
}


