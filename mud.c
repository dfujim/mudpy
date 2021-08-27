/*
 *  mud.c -- most of the utilities for reading/writing MUD format files
 *	     procedures for MUD_FMT_ALL sections 
 *
 *   Copyright (C) 1994-2009 TRIUMF (Vancouver, Canada)
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
 *   v1.0   26-Jan-1994  [T. Whidden]  Initial version
 *   v1.1   17-Feb-1994  [T. Whidden]  Groups with member index
 *   v1.1a  14-Apr-1994  [T. Whidden]  Fixes to MUD_read, MUD_search
 *   v1.2   08-Oct-2000  [D. Arseneau] Add MUD_setSizes
 *   v1.3   22-Apr-2003  [D. Arseneau] Add MUD_openInOut
 *          25-Nov-2009  [D. Arseneau] Handle larger size_t
 *          04-May-2016  [D. Arseneau] Edits for C++ use
 */


#include "mud.h"

#ifdef NO_STDARG
#include <varargs.h>
#else
#include <stdarg.h>
#endif /* NO_STDARG */

/* #define DEBUG 1 */  /* un-comment for debug */ 

FILE*
MUD_openInput( char* inFile )
{
    FILE* fin;

    fin = fopen( inFile, "rb" );
    if( fin == NULL )
    {
	return( NULL );
    }
    return( fin );
}

/* This is the same as MUD_openInput except for access mode */
FILE*
MUD_openInOut( char* inFile )
{
    FILE* fin;

    fin = fopen( inFile, "r+b" );
    if( fin == NULL )
    {
	return( NULL );
    }
    return( fin );
}


FILE* 
MUD_openOutput( char* outFile )
{
    FILE* fout;

    fout = fopen( outFile, "wb" );
    if( fout == NULL )
    {
	return( NULL );
    }
    return( fout );
}


void
MUD_free( void* pMUD )
{
    if( pMUD != NULL ) MUD_free( ((MUD_SEC*)pMUD)->core.pNext );
    else return;

#ifdef DEBUG
	printf( "  MUD_free: freeing:" );
	MUD_CORE_proc( MUD_SHOW, NULL, pMUD );
#endif /* DEBUG */

    (*((MUD_SEC*)pMUD)->core.proc)( MUD_FREE, NULL, (void*)pMUD );
    free( pMUD );
    pMUD = NULL;
}


BOOL
MUD_encode( BUF* pBuf, void* pMUD, MUD_IO_OPT io_opt )
{
    if( pMUD == NULL ) return( TRUE );

    ((MUD_SEC*)pMUD)->core.size = MUD_getSize( pMUD );

#ifdef DEBUG
    MUD_show( pMUD, MUD_ONE );
#endif /* DEBUG */

    if( pBuf->buf == NULL )
    {
	pBuf->buf = (char*)zalloc( MUD_size( pMUD ) );
    }
    else
    {
	pBuf->buf = (char*)realloc( pBuf->buf, pBuf->size + MUD_size( pMUD ) );
    }

    if( pBuf->buf == NULL ) return( FALSE );

#ifdef DEBUG
	printf( "MUD_encode:  buf.size = %d\n", pBuf->size + MUD_size( pMUD ) );
	printf( "MUD_encode:  buf.buf  = %08X\n", pBuf->buf );
#endif /* DEBUG */

    MUD_CORE_proc( MUD_ENCODE, pBuf, (MUD_SEC*)pMUD );
    (*((MUD_SEC*)pMUD)->core.proc)( MUD_ENCODE, pBuf, (void*)pMUD );

    if( ( MUD_secID( pMUD ) == MUD_SEC_GRP_ID ) && 
	( io_opt == MUD_ALL ) )
    {
	if( !MUD_encode( pBuf, ((MUD_SEC_GRP*)pMUD)->pMem, io_opt ) )
	{
	  return( FALSE );
	}
    }

    if( io_opt == MUD_ALL )
    {
	if( !MUD_encode( pBuf, ((MUD_SEC*)pMUD)->core.pNext, io_opt ) )
	{
	  return( FALSE );
	}
    }

    return( TRUE );
}


void*
MUD_decode( BUF* pBuf )
{
    MUD_SEC* pMUD;
    MUD_SEC mud;
    int pos;

    pos = pBuf->pos;

#ifdef DEBUG
    printf( "MUD_decode: decoding the core...\n" );
#endif /* DEBUG */

    /*	  
     *  Decode the core part
     */	  
    MUD_CORE_proc( MUD_DECODE, pBuf, &mud );

    pMUD = (MUD_SEC*)MUD_new( mud.core.secID, mud.core.instanceID );
    if( pMUD == NULL ) return( NULL );

    MUD_assignCore( &mud, pMUD );

#ifdef DEBUG
    printf( "MUD_decode: done  \n" );
    printf( "            decoding the section-specific...\n" );
#endif /* DEBUG */

    /*	  
     *  Decode the section-specific part
     */	  
    (*pMUD->core.proc)( MUD_DECODE, pBuf, (void*)pMUD );

#ifdef DEBUG
    printf( "MUD_decode: done\n" );
#endif /* DEBUG */

    pBuf->pos = pos + MUD_size( pMUD );

    return( pMUD );
}


int
MUD_getSize( void* pMUD )
{
    return( MUD_CORE_proc( MUD_GET_SIZE, NULL, (MUD_SEC*)pMUD ) +
	    (*((MUD_SEC*)pMUD)->core.proc)( MUD_GET_SIZE, NULL, pMUD ) );
}


void
MUD_show( void* pMUD, MUD_IO_OPT io_opt )
{
    if( pMUD == NULL ) return;

    MUD_CORE_proc( MUD_SHOW, NULL, (MUD_SEC*)pMUD );
    (*((MUD_SEC*)pMUD)->core.proc)( MUD_SHOW, NULL, pMUD );

    printf( "\n" );

    if( ( MUD_secID( pMUD ) == MUD_SEC_GRP_ID ) && 
	( io_opt == MUD_ALL ) )
    {
	MUD_show( ((MUD_SEC_GRP*)pMUD)->pMem, io_opt );
    }

    if( ( MUD_secID( pMUD ) != MUD_SEC_EOF_ID ) && 
	( io_opt == MUD_ALL ) )
    {
	MUD_show( ((MUD_SEC*)pMUD)->core.pNext, io_opt );
    }
}

void
MUD_heads( void* pMUD, MUD_IO_OPT io_opt )
{
    if( pMUD == NULL ) return;

    MUD_CORE_proc( MUD_HEADS, NULL, (MUD_SEC*)pMUD );
    (*((MUD_SEC*)pMUD)->core.proc)( MUD_HEADS, NULL, pMUD );

    if( ( MUD_secID( pMUD ) == MUD_SEC_GRP_ID ) && 
	( io_opt == MUD_ALL ) )
    {
	MUD_heads( ((MUD_SEC_GRP*)pMUD)->pMem, io_opt );
    }

    if( ( MUD_secID( pMUD ) != MUD_SEC_EOF_ID ) && 
	( io_opt == MUD_ALL ) )
    {
	MUD_heads( ((MUD_SEC*)pMUD)->core.pNext, io_opt );
    }
}


/*	  
 *  MUD_writeEnd() - called after all writes to file
 */	  
BOOL
MUD_writeEnd( FILE* fout )
{
    MUD_SEC_EOF* pMUD_eof;

    if( ( pMUD_eof = (MUD_SEC_EOF*)MUD_new( MUD_SEC_EOF_ID, 1 ) ) == NULL )
    {
      return( FALSE );
    }

    if( !MUD_write( fout, pMUD_eof, MUD_ONE ) ) return( FALSE );

    MUD_free( pMUD_eof );

    return( TRUE );
}


/*	  
 *  MUD_writeFile() - use for completely assembled groups/sections
 */	  
BOOL
MUD_writeFile( FILE* fout, void* pMUD_head )
{
    rewind( fout );
    if( !MUD_write( fout, pMUD_head, MUD_ALL ) ) return( FALSE );
    return( MUD_writeEnd( fout ) );
}


/*	  
 *  MUD_write() - use for completely assembled groups/sections
 */	  
BOOL
MUD_write( FILE* fout, void* pMUD, MUD_IO_OPT io_opt )
{
    BUF buf;

    if( pMUD == NULL ) return( TRUE );

    bzero( &buf, sizeof( BUF ) );

    if( !MUD_encode( &buf, pMUD, io_opt ) ) return( FALSE );

    fwrite( buf.buf, buf.size, 1, fout );

    _free( buf.buf );

    return( TRUE );
}


/*	  
 *  MUD_writeGrpStart() - use for writes of unassembled groups
 *  
 *  Sequence :
 *	MUD_writeGrpStart()
 *	    [ calls to MUD_writeGrpMem() 
 *	      or further blocks of MUD_writeGrpStart()-MUD_writeGrpEnd() ]
 *	MUD_writeGrpEnd()
 *	MUD_writeEnd()
 */
BOOL
MUD_writeGrpStart( FILE* fout, MUD_SEC_GRP* pMUD_parentGrp, 
		   MUD_SEC_GRP* pMUD_grp, int numMems )
{
    pMUD_grp->pParent = pMUD_parentGrp;

    pMUD_grp->pos = ftell( fout );
    pMUD_grp->memSize = 0;
    MUD_INDEX_proc( MUD_FREE, NULL, pMUD_grp->pMemIndex );
    MUD_free( pMUD_grp->pMem );

    pMUD_grp->num = numMems;
    pMUD_grp->core.size = MUD_getSize( (MUD_SEC*)pMUD_grp );
    fseek( fout, pMUD_grp->core.size, 1 );
    pMUD_grp->num = 0;

    return( TRUE );
}


void
addIndex( MUD_SEC_GRP* pMUD_grp, void* pMUD )
{
    MUD_INDEX** ppMUD_index;

    for( ppMUD_index = &pMUD_grp->pMemIndex; 
	 *ppMUD_index != NULL; 
	 ppMUD_index = &(*ppMUD_index)->pNext ) ;

    *ppMUD_index = (MUD_INDEX*)zalloc( sizeof( MUD_INDEX ) );
    (*ppMUD_index)->offset = pMUD_grp->memSize;
    (*ppMUD_index)->secID = MUD_secID( pMUD );
    (*ppMUD_index)->instanceID = MUD_instanceID( pMUD );
}


/*	  
 *  MUD_writeGrpMem() - use for writes of unassembled groups
 */	  
BOOL
MUD_writeGrpMem( FILE* fout, MUD_SEC_GRP* pMUD_grp, void* pMUD )
{
    ((MUD_SEC*)pMUD)->core.size = MUD_getSize( pMUD );

    addIndex( pMUD_grp, pMUD );
    pMUD_grp->num++;
    pMUD_grp->memSize += MUD_totSize( pMUD );

    if( MUD_secID( pMUD ) == MUD_SEC_GRP_ID )
    {
	if( !MUD_write( fout, pMUD, MUD_GRP ) ) return( FALSE );
    }
    else
    {
	if( !MUD_write( fout, pMUD, MUD_ONE ) ) return( FALSE );
    }

    return( TRUE );
}


/*	  
 *  MUD_writeGrpEnd() - use for writes of unassembled groups
 */	  
BOOL
MUD_writeGrpEnd( FILE* fout, MUD_SEC_GRP* pMUD_grp )
{
    int pos;

    pos = ftell( fout );

    fseek( fout, pMUD_grp->pos, 0 );
    if( !MUD_write( fout, pMUD_grp, MUD_ONE ) ) return( FALSE );
    fseek( fout, pos, 0 );

    if( pMUD_grp->pParent != NULL )
    {
	addIndex( pMUD_grp->pParent, pMUD_grp );
	pMUD_grp->pParent->num++;
	pMUD_grp->pParent->memSize += MUD_totSize( pMUD_grp );
    }

    return( TRUE );
}


void*
MUD_readFile( FILE* fin )
{
    rewind( fin );

    return( MUD_read( fin, MUD_ALL ) );
}


void*
MUD_read( FILE* fin, MUD_IO_OPT io_opt )
{
    BUF buf;
    MUD_SEC* pMUD_new;
    MUD_SEC* pMUD_next;
    int i;
    UINT32 size;
    int pos;

#ifdef DEBUG
    printf( "MUD_read: reading the section size...\n" );
#endif /* DEBUG */

    /*	  
     *  Read the section into a buffer
     */	  
    if( ( pos = ftell( fin ) ) == EOF ) return( NULL );
    if( fread( &size, 4, 1, fin ) == 0 ) return( NULL );
    bdecode_4( &size, &size );    /* byte ordering !!! */
    if( fseek( fin, pos, 0 ) == EOF ) return( NULL );

#ifdef DEBUG
    printf( "MUD_read: got %lu\n", (unsigned long)(size) );
    printf( "          pos = %d\n", pos );
    printf( "          reading the section ...\n" );
#endif /* DEBUG */

    bzero( &buf, sizeof( BUF ) );
    buf.buf = (char*)zalloc( (size_t)size );
    if( fread( buf.buf, (size_t)size, 1, fin ) == 0 )
    {
	_free( buf.buf );
	return( NULL );
    }

#ifdef DEBUG
    printf( "MUD_read: done\n" );
    printf( "          decoding the section...\n" );
#endif /* DEBUG */

    /*	  
     *  Decode the buffer into a structure
     */	  
    pMUD_new = (MUD_SEC*)MUD_decode( &buf );
    if( pMUD_new == NULL ) 
    {
	_free( buf.buf );
	return( NULL );
    }

    _free( buf.buf );

#ifdef DEBUG
    printf( "MUD_read: done\n" );
    MUD_show( pMUD_new, MUD_ONE );
#endif /* DEBUG */

    if(	( pMUD_new->core.secID == MUD_SEC_GRP_ID ) &&
        ( ( io_opt == MUD_ALL ) || ( io_opt == MUD_GRP ) ) )
    {
	/*	  
	 *  Read the group members
	 */	  
        for( i = 0; i < ((MUD_SEC_GRP*)pMUD_new)->num; i++ )
	{
	    pMUD_next = (MUD_SEC*)MUD_read( fin, MUD_GRP );
	    if( pMUD_next == NULL )
	    {
		return( pMUD_new );
	    }
	    else if( pMUD_next->core.secID != MUD_SEC_EOF_ID )
	    {
		MUD_add( (void**)&((MUD_SEC_GRP*)pMUD_new)->pMem, pMUD_next );
	    }
	    else
	    {
		MUD_free( pMUD_next );
		return( pMUD_new );
	    }
	}

#ifdef DEBUG
	    printf( "MUD_read: Group after reading members:\n" );
	    MUD_show( pMUD_new, MUD_ONE );
#endif
    }

    if(	( MUD_secID( pMUD_new ) != MUD_SEC_EOF_ID ) &&
        ( io_opt == MUD_ALL ) )
    {
	/*	  
	 *  Read the next section
	 */	  
	pMUD_next = (MUD_SEC*)MUD_read( fin, io_opt );
	if( pMUD_next == NULL )
	{
	    return( pMUD_new );
	}
	else if( MUD_secID( pMUD_next ) != MUD_SEC_EOF_ID )
	{
	    MUD_add( (void**)&pMUD_new->core.pNext, pMUD_next );
	}
	else
	{
	    MUD_free( pMUD_next );
	    return( pMUD_new );
	}
    }

    return( pMUD_new );
}

/*
 *  UINT32 MUD_setSizes( void* pMUD )
 *
 *  Modification history:
 *    08-Oct-2000  DJA   Created
 *
 *  Description:
 *    Go through the mud structure *pMUD recursively measuring sizes and
 *    assigning memSize and index.offset values.  This is used in the
 *    MUD-friendly routine MUD_closeWrite() before writing the file; the
 *    other friendly routines omit the picky bookkeeping needed to maintain
 *    accurate sizes and offsets.
 * 
 *  Parameter:
 *    A pointer to the root mud section to be measured (which is
 *    normally the entire mud structure for a run).
 *
 *  Return value:
 *    When pMUD points to a group, return the memSize for the group;
 *    otherwise return 0.
 */

UINT32
MUD_setSizes( void* pMUD )
{
    MUD_SEC*    pMember;
    MUD_INDEX*  pGrpIndex;
    UINT32      offset = 0;

    /*
     *  Do nothing (return 0) if mud section is not a group.
     */
    if( MUD_secID( pMUD ) != MUD_SEC_GRP_ID )
    {
        return( 0 );
    }
    /* 
     *  Loop over members of group
     */
    for( pMember = ((MUD_SEC_GRP*)pMUD)->pMem;
         pMember != NULL;
         pMember = pMember->core.pNext )
    {
        /*
         *  Look for corresponding index entry and set its offset.  If index 
         *  table is bad, just quit.
         */
        for( pGrpIndex = (MUD_INDEX*)(((MUD_SEC_GRP*)pMUD)->pMemIndex);
             pGrpIndex->instanceID != MUD_instanceID( pMember ) ||
                  pGrpIndex->secID != MUD_secID( pMember ) ;
             pGrpIndex = pGrpIndex->pNext  )
        {
            if( pGrpIndex == NULL ) return( offset );
        }
        pGrpIndex->offset = offset;

        /*
         *  If member is itself a group, recurse, measuring size.
         */
        if( MUD_secID( pMember ) == MUD_SEC_GRP_ID )
        {
            offset += MUD_setSizes( pMember );
        }
        offset += MUD_getSize( pMember );
    }

    ((MUD_SEC_GRP*)pMUD)->memSize = offset;

    return( offset );
}



MUD_SEC*
MUD_peekCore( FILE* fin )
{
    static MUD_SEC mud;
    int pos;
    BUF buf;
    int size; 

    if( ( pos = ftell( fin ) ) == EOF ) return( NULL );

    bzero( &mud, sizeof( MUD_SEC ) );
    bzero( &buf, sizeof( BUF ) );

    size = MUD_CORE_proc( MUD_GET_SIZE, 0, 0 );

    buf.buf = (char*)zalloc( size );
    if( buf.buf == NULL ) return( NULL );

    if( fread( buf.buf, size, 1, fin ) == 0 )
    {
        free( buf.buf );
	return( NULL );
    }

    fseek( fin, pos, 0 );

    MUD_CORE_proc( MUD_DECODE, &buf, &mud );

    free( buf.buf );

    return( &mud );
}


#ifdef NO_STDARG
void* 
MUD_search( va_alist )
va_dcl
#else
void* 
MUD_search( void* pMUD_head, ... )
#endif /* NO_STDARG */
{
    va_list args;
#ifdef NO_STDARG
    void* pMUD_head;
#endif /* NO_STDARG */
    MUD_SEC* pMUD;
    MUD_SEC* pMUD_start;
    UINT32 secID;
    UINT32 instanceID;
    SEEK_ENTRY* pSeekList = NULL;
    SEEK_ENTRY** ppSeekEntry;
    SEEK_ENTRY* pSeekEntry;

    /*	  
     *  Assemble seek list from varargs
     */	  
#ifdef NO_STDARG
    va_start( args );
    pMUD_head = va_arg( args, void* );
#else
    va_start( args, pMUD_head );
#endif /* NO_STDARG */
    pSeekList = NULL;
    ppSeekEntry = &pSeekList;
    while( ( ( secID = va_arg( args, UINT32 ) ) != 0 ) &&
	   ( ( instanceID = va_arg( args, UINT32 ) ) != 0 ) )
    {
	*ppSeekEntry = (SEEK_ENTRY*)zalloc( sizeof( SEEK_ENTRY ) );
	(*ppSeekEntry)->secID = secID;
	(*ppSeekEntry)->instanceID = instanceID;
	ppSeekEntry = &(*ppSeekEntry)->pNext;
    }
    va_end( args );

    pMUD_start = (MUD_SEC*)pMUD_head;
    pMUD = NULL;

    for( pSeekEntry = pSeekList; 
	 pSeekEntry != NULL; 
	 pSeekEntry = pSeekEntry->pNext )
    {
	/*
	 *  Search this level for the entry
	 */
	for( pMUD = pMUD_start; pMUD != NULL; pMUD = pMUD->core.pNext )
	    if( ( MUD_secID( pMUD ) == pSeekEntry->secID ) && 
		( MUD_instanceID( pMUD ) == pSeekEntry->instanceID ) )
		break;

	if( pMUD == NULL )
	{
	    /*
	     *  Didn't find anything at this level - search failed
	     */
	    break;
	}
	else if( pSeekEntry->pNext == NULL )
	{
	    /*	  
	     *  Found section, search is finished
	     */	  
            break;
	}
	else
	{
	    /*	  
	     *  Found section, search not finished
	     */	  
	    if( MUD_secID( pMUD ) == MUD_SEC_GRP_ID )
	    {
		/*
		 *  Search this group
		 */
		pMUD_start = ((MUD_SEC_GRP*)pMUD)->pMem;
	    }
	    else
	    {
		/*
		 *  Section is not a group - search failed
		 */
		pMUD = NULL;
		break;
	    }
	}
    }

    return( pMUD );
}


#ifdef NO_STDARG
int 
MUD_fseek( va_alist )
va_dcl
#else
int
MUD_fseek( FILE* fio, ... )
#endif /* NO_STDARG */
{
    va_list args;
#ifdef NO_STDARG
    FILE* fio;
#endif /* NO_STDARG */
    SEEK_ENTRY* pSeekList;
    SEEK_ENTRY** ppSeekEntry;
    SEEK_ENTRY* pSeekEntry;
    MUD_SEC* pMUD;
    MUD_SEC_GRP* pMUD_parent = NULL;
    UINT32 secID;
    UINT32 instanceID;
    BOOL ateof = FALSE;

    /*	  
     *  Assemble seek list from varargs
     */	  
#ifdef NO_STDARG
    va_start( args );
    fio = va_arg( args, FILE* );
#else
    va_start( args, fio );
#endif /* NO_STDARG */

    ppSeekEntry = &pSeekList;
    while( ( ( secID = va_arg( args, UINT32 ) ) != 0 ) &&
	   ( ( instanceID = va_arg( args, UINT32 ) ) != 0 ) )
    {
	*ppSeekEntry = (SEEK_ENTRY*)zalloc( sizeof( SEEK_ENTRY ) );
	(*ppSeekEntry)->secID = secID;
	(*ppSeekEntry)->instanceID = instanceID;
	ppSeekEntry = &(*ppSeekEntry)->pNext;
    }
    va_end( args );

    for( pSeekEntry = pSeekList; 
	 pSeekEntry != NULL; 
	 pSeekEntry = pSeekEntry->pNext )
    {
	/*
	 *  Search this level for the entry
	 */
	pMUD = fseekNext( fio, pMUD_parent, 
			  pSeekEntry->secID, pSeekEntry->instanceID );

	MUD_free( (MUD_SEC*)pMUD_parent );

	if( pMUD == NULL ) 
	{
	    /*
	     *  Didn't find anything at this level - search failed
	     */
	    break;
	}
	else if( pSeekEntry->pNext != NULL )
	{
	    if( MUD_secID( pMUD ) == MUD_SEC_GRP_ID )
	    {
		pMUD_parent = (MUD_SEC_GRP*)MUD_read( fio, MUD_ONE );
	    }
	    else if( MUD_secID( pMUD ) == MUD_SEC_EOF_ID )
	    {
		ateof = TRUE;
		break;
	    }
	    else 
	    {
		if( fseek( fio, MUD_size( pMUD ), 1 ) == EOF )
		{
		    ateof = TRUE;
		    break;
		}
	    }
	}
    }

    if( ateof || ( pSeekEntry != NULL ) ) return( EOF );

    return( ftell( fio ) );
}


MUD_SEC*
fseekNext( FILE* fio, MUD_SEC_GRP* pMUD_parent, 
	   UINT32 secID, UINT32 instanceID )
{
    MUD_SEC* pMUD;
    MUD_INDEX* pMUD_index;
    BOOL found = FALSE;
    BOOL ateof = FALSE;

    if( pMUD_parent != NULL )
    {
	for( pMUD_index = pMUD_parent->pMemIndex;
	     pMUD_index != NULL;
	     pMUD_index = pMUD_index->pNext )
	{
	    if( ( pMUD_index->secID == secID ) &&
		( pMUD_index->instanceID == instanceID ) )
	    {
		if( fseek( fio, pMUD_index->offset, 1 ) == EOF ) 
		{
		    ateof = TRUE;
		    break;
		}

		pMUD = MUD_peekCore( fio );
		if( pMUD == NULL )
		{
		    ateof = TRUE;
		    break;
		}
		if( ( MUD_secID( pMUD ) == secID ) && 
		    ( MUD_instanceID( pMUD ) == instanceID ) )
		    found = TRUE;

		break;
	    }
	}
    }
    else
    {
	while( !found && !ateof )
	{
	    pMUD = MUD_peekCore( fio );
	    if( pMUD == NULL )
	    {
		ateof = TRUE;
		break;
	    }
	    if( pMUD->core.secID == secID && 
		pMUD->core.instanceID == instanceID )
	    {
		found = TRUE;
	    }
	    else
	    {
		if( ( MUD_secID( pMUD ) == MUD_SEC_EOF_ID ) || feof( fio ) )
		    ateof = TRUE;
		else
		    if( fseek( fio, MUD_size( pMUD ), 1 ) == EOF ) 
			ateof = TRUE;
	    }
	}
    }

    if( ateof || !found ) return( NULL );
    return( pMUD );
}


int
MUD_fseekFirst( FILE* fio )
{
    int size;

    size = MUD_CORE_proc( MUD_GET_SIZE, NULL, NULL ) + 
	   MUD_SEC_FIXED_proc( MUD_GET_SIZE, NULL, NULL );

    if( fseek( fio, size, 0 ) == EOF ) return( EOF );

    return( ftell( fio ) );
}


void
MUD_add( void** ppMUD_head, void* pMUD_new )
{
    MUD_SEC** ppMUD;

    if( pMUD_new == NULL ) return;

    for( ppMUD = (MUD_SEC**)ppMUD_head; 
	 *ppMUD != NULL; 
	 ppMUD = &(*ppMUD)->core.pNext ) ;

    *ppMUD = (MUD_SEC*)pMUD_new;
}


int
MUD_totSize( void* pMUD )
{
    return( ( MUD_secID( pMUD ) == MUD_SEC_GRP_ID ) ? 
		MUD_size( pMUD ) + ((MUD_SEC_GRP*)pMUD)->memSize : 
		MUD_size( pMUD ) );
}


void
MUD_addToGroup( MUD_SEC_GRP* pMUD_grp, void* pMUD )
{
    MUD_SEC** ppMUD;

    if( pMUD == NULL ) return;

    /* possible addition: ((MUD_SEC*)pMUD)->core.pNext = NULL; */
    ((MUD_SEC*)pMUD)->core.size = MUD_getSize( pMUD );

    for( ppMUD = &pMUD_grp->pMem; 
	 *ppMUD != NULL; 
	 ppMUD = &(*ppMUD)->core.pNext ) ;

    *ppMUD = (MUD_SEC*)pMUD;

    addIndex( pMUD_grp, pMUD );
    pMUD_grp->num++;
    pMUD_grp->memSize += MUD_totSize( pMUD );
}


void
MUD_assignCore( MUD_SEC* pMUD1, MUD_SEC* pMUD2 )
{
    bcopy( &pMUD1->core.size, &pMUD2->core.size, 
	    MUD_CORE_proc( MUD_GET_SIZE, NULL, NULL ) );
}


int
MUD_CORE_proc( MUD_OPT op, BUF* pBuf, MUD_SEC* pMUD )
{
    int size;

    switch( op )
    {
	case MUD_FREE:
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->core.size );
            decode_4( pBuf, &pMUD->core.secID );
	    decode_4( pBuf, &pMUD->core.instanceID );
	    break;
	case MUD_ENCODE:
            encode_4( pBuf, &pMUD->core.size );
            encode_4( pBuf, &pMUD->core.secID );
            encode_4( pBuf, &pMUD->core.instanceID );
	    break;
	case MUD_GET_SIZE:
	    size = 3*sizeof(UINT32);
#ifdef DEBUG
            printf("MUD_CORE_proc: MUD_GET_SIZE returns size=%d\n",size);
#endif /* DEBUG */            
	    return( size );
	case MUD_SHOW:
	    printf( "  CORE: size=[%lu], secID=[0x%08lX], instanceID=[0x%08lX]\n",
		    (unsigned long)(pMUD->core.size), (unsigned long)(pMUD->core.secID), 
                    (unsigned long)(pMUD->core.instanceID) );
	    break;
	case MUD_HEADS:
	    break;
    }
    return( 1 );
}


int
MUD_INDEX_proc( MUD_OPT op, BUF* pBuf, MUD_INDEX* pMUD )
{
    int size;

    switch( op )
    {
	case MUD_FREE:
	    if( pMUD != NULL )
	    {
		if( pMUD->pNext != NULL ) 
		    MUD_INDEX_proc( MUD_FREE, NULL, pMUD->pNext );
		free( pMUD );
	    }
	    break;
	case MUD_DECODE:
	    decode_4( pBuf, &pMUD->offset );
            decode_4( pBuf, &pMUD->secID );
	    decode_4( pBuf, &pMUD->instanceID );
	    break;
	case MUD_ENCODE:
            encode_4( pBuf, &pMUD->offset );
            encode_4( pBuf, &pMUD->secID );
            encode_4( pBuf, &pMUD->instanceID );
	    break;
	case MUD_GET_SIZE:
	    size = 3*sizeof(UINT32);
	    return( size );
	case MUD_SHOW:
	    printf( "  INDEX: offset=[%lu], secID=[0x%08lX], instanceID=[0x%08lX]\n",
		    (unsigned long)(pMUD->offset), (unsigned long)(pMUD->secID), 
                    (unsigned long)(pMUD->instanceID) );
	    break;
	case MUD_HEADS:
	    break;
    }
    return( 1 );
}


