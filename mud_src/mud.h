#ifndef _MUD_H_
#define _MUD_H_
/*
 * mud.h  Declarations for MUD 
 * v1.3
 *
 *   Copyright (C) 1994-2021 TRIUMF (Vancouver, Canada)
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
 * revisions:
 * 01-mar-2000   DJA  add UNKNOWN section, with no ID 
 * 11-oct-2000   DJA  add MUD_FMT_RAL_ID; MUD_setSizes
 * 22-Apr-2003   DJA  add MUD_openReadWrite, MUD_openInOut
 * 25-Nov-2009   DJA  64-bit linux
 * 25-Jun-2017   DJA  Allow use in C++ (ROOT); shared lib.
 * 14-Aug-2019   DJA  Use stdint.h, casts in printf
 * 01-Jun-2021   DJA  Add arm64 arch as little-endian
 * 26-Aug-2021   DJA  Declare caddr_t in all Win. 
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  FORMAT IDs - Must be unique!
 *               format of ID is:  0xLLFFSSSS
 *			   where:    LL       = lab identifier
 *			               FF     = format identifier
 *			                 RR   = revision
 *					   SS = section identifier
 */

/*
 *  Lab identifiers
 */
#define MUD_LAB_ALL_ID		(0x01000000)
#define MUD_LAB_TRI_ID		(0x02000000)
#define MUD_LAB_RAL_ID		(0x03000000)
#define MUD_LAB_PSI_ID		(0x04000000)
/*
 *  Format identifiers
 */
#define MUD_FMT_ALL_ID		(MUD_LAB_ALL_ID|0x00010000)
#define MUD_FMT_GEN_ID		(MUD_LAB_ALL_ID|0x00020000)
#define MUD_FMT_TRI_TD_ID	(MUD_LAB_TRI_ID|0x00010000)
#define MUD_FMT_TRI_TI_ID	(MUD_LAB_TRI_ID|0x00020000)

/*
 *  ALL Format identifiers
 */
#define	MUD_SEC_ID		(MUD_FMT_ALL_ID|0x00000001)
#define	MUD_SEC_FIXED_ID	(MUD_FMT_ALL_ID|0x00000002)
#define	MUD_SEC_GRP_ID		(MUD_FMT_ALL_ID|0x00000003)
#define	MUD_SEC_EOF_ID		(MUD_FMT_ALL_ID|0x00000004)
#define	MUD_SEC_CMT_ID		(MUD_FMT_ALL_ID|0x00000005)

#define	MUD_GRP_CMT_ID		MUD_SEC_CMT_ID
/*
 *  GEN Format identifiers
 */
#define	MUD_SEC_GEN_RUN_DESC_ID	    (MUD_FMT_GEN_ID|0x00000001)
#define	MUD_SEC_GEN_HIST_HDR_ID	    (MUD_FMT_GEN_ID|0x00000002)
#define	MUD_SEC_GEN_HIST_DAT_ID	    (MUD_FMT_GEN_ID|0x00000003)
#define	MUD_SEC_GEN_SCALER_ID	    (MUD_FMT_GEN_ID|0x00000004)
#define	MUD_SEC_GEN_IND_VAR_ID	    (MUD_FMT_GEN_ID|0x00000005)
#define MUD_SEC_GEN_ARRAY_ID        (MUD_FMT_GEN_ID|0x00000007)

#define	MUD_GRP_GEN_HIST_ID	    (MUD_FMT_GEN_ID|0x00000002)
#define	MUD_GRP_GEN_SCALER_ID	    (MUD_FMT_GEN_ID|0x00000004)
#define	MUD_GRP_GEN_IND_VAR_ID	    (MUD_FMT_GEN_ID|0x00000005)
#define	MUD_GRP_GEN_IND_VAR_ARR_ID  (MUD_FMT_GEN_ID|0x00000006)
/* 
 *  TRI_TD Format identifiers
 */
#define	MUD_SEC_TRI_TD_HIST_ID	    (MUD_FMT_TRI_TD_ID|0x00000002)


#define	MUD_GRP_TRI_TD_HIST_ID	    (MUD_FMT_TRI_TD_ID|0x00000002)
#define	MUD_GRP_TRI_TD_SCALER_ID    (MUD_FMT_TRI_TD_ID|0x00000004)
/*
 *  TRI_TI Format identifiers
 */
#define	MUD_SEC_TRI_TI_RUN_DESC_ID  (MUD_FMT_TRI_TI_ID|0x00000001)
#define	MUD_SEC_TRI_TI_HIST_ID	    (MUD_FMT_TRI_TI_ID|0x00000002)

#define	MUD_GRP_TRI_TI_HIST_ID	    MUD_SEC_TRI_TI_HIST_ID
/*
 *  RAL Format identifiers
 */
#define	MUD_SEC_RAL_RUN_DESC_ID     (MUD_FMT_RAL_ID|0x00000001)
#define	MUD_SEC_RAL_HIST_ID         (MUD_FMT_RAL_ID|0x00000002)

#define	MUD_GRP_RAL_HIST_ID         MUD_SEC_RAL_HIST_ID
/*
 *  Add new format identifiers above this comment
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>

#ifndef NOSTDINT
/* If there is no stdint.h, define NOSTDINT, as with "make NOSTDINT=1" */
#include <stdint.h>
#endif

/*
 *  MUD types
 */

#ifdef _STDINT_H
typedef int			STATUS;
typedef int8_t			INT8;
typedef uint8_t			UINT8;
typedef int16_t			INT16;
typedef uint16_t 		UINT16;
typedef int32_t			INT32;
typedef uint32_t		UINT32;
typedef float			REAL32;
typedef double			REAL64;
#else /*no stdint.h */
typedef int			STATUS;
typedef char			INT8;
typedef unsigned char		UINT8;
typedef short			INT16;
typedef unsigned short		UINT16;
#if defined(__alpha)||defined(__linux)||defined(__MACH__) || defined(__arm64)
typedef int			INT32;
typedef unsigned int		UINT32;
#else
typedef long			INT32;
typedef unsigned long		UINT32;
#endif /* __alpha || __linux || __MACH__ || __arm64 */
typedef float			REAL32;
typedef double			REAL64;
#endif /* _STDINT_H */
#if (defined(__alpha)&&defined(vms)) || defined( __CINT__ ) || defined(_WIN32)
typedef char* 			caddr_t;
#endif
typedef UINT32                  TIME;
#ifndef BOOL_DEFINED
#define BOOL_DEFINED
typedef UINT32                  BOOL;
#endif

#ifndef FALSE
#define FALSE	0
#endif /* !FALSE */
#ifndef TRUE
#define TRUE	1
#endif /* !TRUE */
#ifndef NULL
#	define NULL 0
#endif /* !NULL */

#define SUCCESS      1
#define FAILURE      0
#ifndef _success
#define _success(s)  (s&1)
#endif
#ifndef _failure
#define _failure(s)  !(s&1)
#endif
/* avoid redefinitions if c_utils.h already included */ 
/*#ifdef linux  
#include "c_utils.h"
#else
*/
/*
 *  c_utils.h,  Defines for C utilities
 */
#if defined(vms) || defined(_WIN32)
#define bcopy( b1, b2, len )		memcpy(b2,b1,len)
#define bzero( b, len )			memset(b,(char)0,len)
#endif /* vms || _WIN32 */
#ifndef _C_UTILS_H_   /* conflict with c_utils.h */
#define _max( a, b )			( ( (a) > (b) ) ? (a) : (b) )
#define _min( a, b )			( ( (a) < (b) ) ? (a) : (b) )
#define _strlen( s )			((s!=NULL)?strlen(s):0)
#define _swap32( l )                    (((UINT32)l>>16)+((UINT32)l<<16))
#define _swap16( s )                    (((UINT16)s>>8)+((UINT16)s<<8))
#endif

#define _free(objp)			if((void*)(objp)!=(void*)NULL){free((void*)(objp));objp=NULL;}
#define _roundUp( n, r )		( (r) * (int)( ((n)+(r)-1) / (r) ) )

#define zalloc( n )			memset((void*)malloc(n),0,n)
#if defined(vms) || (defined(mips)&&!defined(__sgi)) || (defined(_WIN32)&&defined(__STDC__))
#define strdup( s )			strcpy((char*)malloc(strlen(s)+1),s)
#endif /* vms || mips&&!sgi */
/*#endif */
typedef int (*MUD_PROC)();

typedef enum {
    MUD_ENCODE = 0,
    MUD_DECODE = 1,
    MUD_FREE = 2,
    MUD_GET_SIZE = 3,
    MUD_SHOW = 4,
    MUD_HEADS = 5
} MUD_OPT;

typedef enum {
    MUD_ONE = 1,
    MUD_ALL = 2,
    MUD_GRP = 3
} MUD_IO_OPT;


typedef struct {
    struct _MUD_SEC*	pNext;	    /* pointer to next section */
    UINT32	size;		    /* total size occupied by the section */
    UINT32	secID;		    /* Ident of section type */
    UINT32	instanceID;	    /* Instance ID of section type */
    UINT32	sizeOf;		    /* sizeof struct (used for FORTRAN) */
    MUD_PROC	proc;		    /* section handling procedure */
} MUD_CORE;


typedef struct _MUD_INDEX {
    struct _MUD_INDEX*	pNext;	    /* pointer to next section */
    UINT32	offset;		    /* offset from end of group section */
    UINT32	secID;		    /* Ident of section type */
    UINT32	instanceID;	    /* Instance ID of section type */
} MUD_INDEX;


typedef struct _SEEK_ENTRY {
    struct _SEEK_ENTRY* pNext;
    UINT32 secID;
    UINT32 instanceID;
} SEEK_ENTRY;


/*
 *  (Normally) for internal use only
 */
typedef struct {
    caddr_t buf;
    int     pos;
    unsigned int size;
} BUF;


typedef struct _MUD_SEC {
    MUD_CORE	core;
} MUD_SEC;

/* the stub of a section used when we hit an unknown section type */
typedef struct _MUD_SEC_UNKNOWN {
    MUD_CORE	core;
} MUD_SEC_UNKNOWN;


/* First section of all MUD format files */
typedef struct {
    MUD_CORE	core;

    UINT32	fileSize;
    UINT32	formatID;
} MUD_SEC_FIXED;


/* Generalized group section */
typedef struct _MUD_SEC_GRP {
    MUD_CORE	core;
    
    UINT32	num;		/* number of group members */
    UINT32	memSize;
    MUD_INDEX*	pMemIndex;
    MUD_SEC*	pMem;		/* pointer to list of group members */
    INT32	pos;
    struct _MUD_SEC_GRP* pParent;
} MUD_SEC_GRP;


/* Section indicating EOF */
typedef struct {
    MUD_CORE	core;
} MUD_SEC_EOF;


typedef struct {
    MUD_CORE	core;

    UINT32  ID;
    UINT32  prevReplyID;
    UINT32  nextReplyID;
    TIME    time;
    char*   author;
    char*   title;
    char*   comment;
} MUD_SEC_CMT;


typedef struct {
    MUD_CORE	core;

    UINT32	exptNumber;
    UINT32	runNumber;
    TIME	timeBegin;
    TIME	timeEnd;
    UINT32	elapsedSec;
    char*	title;
    char*	lab;
    char*	area;
    char*	method;
    char*	apparatus;
    char*	insert;
    char*	sample;
    char*	orient;
    char*	das;
    char*	experimenter;
    char*	temperature;
    char*	field;
} MUD_SEC_GEN_RUN_DESC;

/* Generic histogram header structure */
typedef struct {
    MUD_CORE	core;
    
    UINT32	histType;
    UINT32	nBytes;
    UINT32	nBins;
    UINT32	bytesPerBin;
    UINT32	fsPerBin;
    UINT32	t0_ps;
    UINT32	t0_bin;
    UINT32	goodBin1;
    UINT32	goodBin2;
    UINT32	bkgd1;
    UINT32	bkgd2;
    UINT32	nEvents;
    char*	title;
} MUD_SEC_GEN_HIST_HDR;


/* Generic (packed,integer) histogram data structure */
typedef struct {
    MUD_CORE	core;
    
    UINT32	nBytes;
    caddr_t	pData;		    /* pointer to the histogram data */
} MUD_SEC_GEN_HIST_DAT;


/* Generic (packed) array data structure */
typedef struct {
    MUD_CORE	core;

    UINT32      num;		/* number of elements */
    UINT32	elemSize;	/* size of element in bytes */
    UINT32      type;		/* 1=integer, 2=real, 3=string */
    BOOL	hasTime;	/* TRUE if there is time data */
    UINT32	nBytes;         /* bytes in pData - needed for packing */
    caddr_t	pData;		/* pointer to the array data */
    TIME*	pTime;		/* pointer to time data */
} MUD_SEC_GEN_ARRAY;


typedef struct {
    MUD_CORE	core;
    
    UINT32	counts[2];
    char*	label;
} MUD_SEC_GEN_SCALER;


typedef struct {
    MUD_CORE	core;
    
    double	low;
    double	high;
    double	mean;
    double	stddev;
    double	skewness;
    char*	name;
    char*       description;
    char*	units;
} MUD_SEC_GEN_IND_VAR;


typedef struct {
    MUD_CORE	core;

    UINT32	exptNumber;
    UINT32	runNumber;
    TIME	timeBegin;
    TIME	timeEnd;
    UINT32	elapsedSec;
    char*	title;
    char*	lab;
    char*	area;
    char*	method;
    char*	apparatus;
    char*	insert;
    char*	sample;
    char*	orient;
    char*	das;
    char*	experimenter;
    char*	subtitle;
    char*	comment1;
    char*	comment2;
    char*	comment3;
} MUD_SEC_TRI_TI_RUN_DESC;


#define MUD_pNext( pM )		(((MUD_SEC*)pM)->core.pNext)
#define MUD_sizeOf( pM )	(((MUD_SEC*)pM)->core.sizeOf)
#define MUD_size( pM )		(((MUD_SEC*)pM)->core.size)
#define MUD_secID( pM )		(((MUD_SEC*)pM)->core.secID)
#define MUD_instanceID( pM )	(((MUD_SEC*)pM)->core.instanceID)


#if defined(_WIN32) || defined(__i386__) || defined(__i586__) || defined(__i686__) || defined(vax) || defined(__alpha) || defined(__amd64) || defined(__arm64) || (defined(__mips)&&!defined(__sgi))
#define MUD_LITTLE_ENDIAN 1
#else
#define MUD_BIG_ENDIAN 1
#endif

#define _swap2bytes(s)       ((s>>8)+(s<<8))
#define _swap4bytes(l)       ((l>>24)+(l<<24)+((l&0xff0000)>>8)+((l&0xff00)<<8))

#define bdecode_obj( b, p, s )      bcopy( b, p, s )
#define bencode_obj( b, p, s )      bcopy( p, b, s )
#define bdecode_1( b, p )           bcopy( b, p, 1 )
#define bencode_1( b, p )           bcopy( p, b, 1 )

#define	_decode_obj( b, p, s )	    bcopy( &b->buf[b->pos], p, s );\
				    b->pos+=s, b->size+=s
#define	_encode_obj( b, p, s )	    bcopy( p, &b->buf[b->pos], s );\
				    b->pos+=s, b->size+=s

#define decode_2( b, p )            bdecode_2(&b->buf[b->pos],p);\
				    b->pos+=2, b->size+=2
#define encode_2( b, p )            bencode_2(&b->buf[b->pos],p);\
				    b->pos+=2, b->size+=2

#define decode_4( b, p )            bdecode_4(&b->buf[b->pos],p);\
				    b->pos+=4, b->size+=4
#define encode_4( b, p )            bencode_4(&b->buf[b->pos],p);\
				    b->pos+=4, b->size+=4

#define decode_8( b, p )            bdecode_8(&b->buf[b->pos],p);\
				    b->pos+=8, b->size+=8
#define encode_8( b, p )            bencode_8(&b->buf[b->pos],p);\
				    b->pos+=8, b->size+=8

#define decode_packed( b, p, n )    bdecode_packed(&b->buf[b->pos],p,n);\
				    b->pos+=n, b->size+=n
#define encode_packed( b, p, n )    bencode_packed(&b->buf[b->pos],p,n);\
				    b->pos+=n, b->size+=n

#define _buf_pos( b )		    b->pos
#define _buf_addr( b )		    &b->buf[b->pos]
#define _set_buf_pos( b, pos )	    b->pos = pos
#define _incr_buf_pos( b, incr )    b->pos += incr

typedef UINT16	MUD_STR_LEN_TYPE;
typedef UINT16  MUD_VAR_BIN_LEN_TYPE;
typedef UINT8   MUD_VAR_BIN_SIZ_TYPE;


#undef _ANSI_ARGS_
#if ((defined(__STDC__) || defined(SABER)) && !defined(NO_PROTOTYPE)) || defined(__cplusplus)
#define _ANSI_ARGS_(x)       x
#else 
#define _ANSI_ARGS_(x)       ()
#define NO_STDARG
#endif /* STDC */


/* mud.c */
FILE *MUD_openInput _ANSI_ARGS_(( char *inFile ));
FILE *MUD_openOutput _ANSI_ARGS_(( char *outFile ));
FILE *MUD_openInOut _ANSI_ARGS_(( char *inFile ));
void decode_str _ANSI_ARGS_(( BUF *pB , char **ps ));
void encode_str _ANSI_ARGS_(( BUF *pB , char **ps ));
void MUD_free _ANSI_ARGS_(( void* pMUD ));
BOOL MUD_encode _ANSI_ARGS_(( BUF *pBuf , void* pMUD , MUD_IO_OPT io_opt ));
void* MUD_decode _ANSI_ARGS_(( BUF *pBuf ));
int MUD_getSize _ANSI_ARGS_(( void* pMUD ));
void MUD_show _ANSI_ARGS_(( void* pMUD , MUD_IO_OPT io_opt ));
void MUD_heads _ANSI_ARGS_(( void* pMUD , MUD_IO_OPT io_opt ));
BOOL MUD_writeEnd _ANSI_ARGS_(( FILE *fout ));
BOOL MUD_writeFile _ANSI_ARGS_(( FILE *fout , void* pMUD_head ));
BOOL MUD_write _ANSI_ARGS_(( FILE *fout , void* pMUD , MUD_IO_OPT io_opt ));
BOOL MUD_writeGrpStart _ANSI_ARGS_(( FILE *fout , MUD_SEC_GRP *pMUD_parentGrp , MUD_SEC_GRP *pMUD_grp , int numMems ));
void addIndex _ANSI_ARGS_(( MUD_SEC_GRP *pMUD_grp , void* pMUD ));
BOOL MUD_writeGrpMem _ANSI_ARGS_(( FILE *fout , MUD_SEC_GRP *pMUD_grp , void* pMUD ));
BOOL MUD_writeGrpEnd _ANSI_ARGS_(( FILE *fout , MUD_SEC_GRP *pMUD_grp ));
void* MUD_readFile _ANSI_ARGS_(( FILE *fin ));
void* MUD_read _ANSI_ARGS_(( FILE *fin , MUD_IO_OPT io_opt ));
UINT32 MUD_setSizes _ANSI_ARGS_(( void* pMUD ));
MUD_SEC* MUD_peekCore _ANSI_ARGS_(( FILE *fin ));
void* MUD_search _ANSI_ARGS_(( void* pMUD_head , ...));
int MUD_fseek _ANSI_ARGS_(( FILE *fio , ...));
MUD_SEC *fseekNext _ANSI_ARGS_(( FILE *fio , MUD_SEC_GRP *pMUD_parent , UINT32 secID , UINT32 instanceID ));
int MUD_fseekFirst _ANSI_ARGS_(( FILE *fio ));
void MUD_add _ANSI_ARGS_(( void** ppMUD_head , void* pMUD_new ));
int MUD_totSize _ANSI_ARGS_(( void* pMUD ));
void MUD_addToGroup _ANSI_ARGS_(( MUD_SEC_GRP *pMUD_grp , void* pMUD ));
void MUD_assignCore _ANSI_ARGS_(( MUD_SEC *pMUD1 , MUD_SEC *pMUD2 ));
int MUD_CORE_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC *pMUD ));
int MUD_INDEX_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_INDEX *pMUD ));

/* mud_encode.c */
void bdecode_2 _ANSI_ARGS_(( void *b , void *p ));
void bencode_2 _ANSI_ARGS_(( void *b , void *p ));
void bdecode_4 _ANSI_ARGS_(( void *b , void *p ));
void bencode_4 _ANSI_ARGS_(( void *b , void *p ));
void bdecode_8 _ANSI_ARGS_(( void *b , void *p ));
void bencode_8 _ANSI_ARGS_(( void *b , void *p ));
void decode_str _ANSI_ARGS_(( BUF *pB , char **ps ));
void encode_str _ANSI_ARGS_(( BUF *pB , char **ps ));
void bencode_float _ANSI_ARGS_(( char *buf , float *fp ));
void encode_float _ANSI_ARGS_(( BUF *pBuf , float *fp ));
void bdecode_float _ANSI_ARGS_(( char *buf , float *fp ));
void decode_float _ANSI_ARGS_(( BUF *pBuf , float *fp ));
void bencode_double _ANSI_ARGS_(( char *buf , double *dp ));
void encode_double _ANSI_ARGS_(( BUF *pBuf , double *fp ));
void bdecode_double _ANSI_ARGS_(( char *buf , double *dp ));
void decode_double _ANSI_ARGS_(( BUF *pBuf , double *fp ));

/* mud_new.c */
MUD_SEC *MUD_new _ANSI_ARGS_(( UINT32 secID , UINT32 instanceID ));

/* mud_all.c */
int MUD_SEC_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC *pMUD ));
int MUD_SEC_EOF_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_EOF *pMUD ));
int MUD_SEC_FIXED_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_FIXED *pMUD ));
int MUD_SEC_GRP_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_GRP *pMUD ));
int MUD_SEC_CMT_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_CMT *pMUD ));
int MUD_SEC_UNKNOWN_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_UNKNOWN *pMUD ));

/* mud_gen.c */
int MUD_SEC_GEN_RUN_DESC_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_GEN_RUN_DESC *pMUD ));
int MUD_SEC_GEN_HIST_HDR_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_GEN_HIST_HDR *pMUD ));
int MUD_SEC_GEN_HIST_DAT_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_GEN_HIST_DAT *pMUD ));
int MUD_SEC_GEN_SCALER_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_GEN_SCALER *pMUD ));
int MUD_SEC_GEN_IND_VAR_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_GEN_IND_VAR *pMUD ));
int MUD_SEC_GEN_ARRAY_proc _ANSI_ARGS_(( MUD_OPT op, BUF *pBuf, MUD_SEC_GEN_ARRAY *pMUD ));
int MUD_SEC_GEN_HIST_pack _ANSI_ARGS_(( int num , int inBinSize , void* inHist , int outBinSize , void* outHist ));
int MUD_SEC_GEN_HIST_unpack _ANSI_ARGS_(( int num , int inBinSize , void* inHist , int outBinSize , void* outHist ));

/* mud_tri_ti.c */
int MUD_SEC_TRI_TI_RUN_DESC_proc _ANSI_ARGS_(( MUD_OPT op , BUF *pBuf , MUD_SEC_TRI_TI_RUN_DESC *pMUD ));

/* gmf_time.c */
void GMF_MKTIME _ANSI_ARGS_(( TIME* out , INT32* input ));
void GMF_TIME _ANSI_ARGS_(( TIME* out ));
void GMF_LOCALTIME _ANSI_ARGS_(( TIME* in , INT32 *out ));

/* mud_friendly.c */
int MUD_openRead _ANSI_ARGS_(( char* filename, UINT32* pType ));
int MUD_openWrite _ANSI_ARGS_(( char* filename, UINT32 type ));
int MUD_openReadWrite _ANSI_ARGS_(( char* filename, UINT32* pType ));
int MUD_closeRead _ANSI_ARGS_(( int fd ));
int MUD_closeWrite _ANSI_ARGS_(( int fd ));
int MUD_closeWriteFile _ANSI_ARGS_(( int fd, char* outfile ));

int MUD_getRunDesc _ANSI_ARGS_(( int fd, UINT32* pType ));
int MUD_getExptNumber _ANSI_ARGS_(( int fd, UINT32* pExptNumber ));
int MUD_getRunNumber _ANSI_ARGS_(( int fd, UINT32* pRunNumber ));
int MUD_getElapsedSec _ANSI_ARGS_(( int fd, UINT32* pElapsedSec ));
int MUD_getTimeBegin _ANSI_ARGS_(( int fd, UINT32* TimeBegin ));
int MUD_getTimeEnd _ANSI_ARGS_(( int fd, UINT32* TimeEnd ));
int MUD_getTitle _ANSI_ARGS_(( int fd, char* title, int strdim ));
int MUD_getLab _ANSI_ARGS_(( int fd, char* lab, int strdim ));
int MUD_getArea _ANSI_ARGS_(( int fd, char* area, int strdim ));
int MUD_getMethod _ANSI_ARGS_(( int fd, char* method, int strdim ));
int MUD_getApparatus _ANSI_ARGS_(( int fd, char* apparatus, int strdim ));
int MUD_getInsert _ANSI_ARGS_(( int fd, char* insert, int strdim ));
int MUD_getSample _ANSI_ARGS_(( int fd, char* sample, int strdim ));
int MUD_getOrient _ANSI_ARGS_(( int fd, char* orient, int strdim ));
int MUD_getDas _ANSI_ARGS_(( int fd, char* das, int strdim ));
int MUD_getExperimenter _ANSI_ARGS_(( int fd, char* experimenter, int strdim ));
int MUD_getTemperature _ANSI_ARGS_(( int fd, char* temperature, int strdim ));
int MUD_getField _ANSI_ARGS_(( int fd, char* field, int strdim ));
int MUD_getSubtitle _ANSI_ARGS_(( int fd, char* subtitle, int strdim ));
int MUD_getComment1 _ANSI_ARGS_(( int fd, char* comment1, int strdim ));
int MUD_getComment2 _ANSI_ARGS_(( int fd, char* comment2, int strdim ));
int MUD_getComment3 _ANSI_ARGS_(( int fd, char* comment3, int strdim ));

int MUD_setRunDesc _ANSI_ARGS_(( int fd, UINT32 type ));
int MUD_setExptNumber _ANSI_ARGS_(( int fd, UINT32 exptNumber ));
int MUD_setRunNumber _ANSI_ARGS_(( int fd, UINT32 runNumber ));
int MUD_setElapsedSec _ANSI_ARGS_(( int fd, UINT32 elapsedSec ));
int MUD_setTimeBegin _ANSI_ARGS_(( int fd, UINT32 timeBegin ));
int MUD_setTimeEnd _ANSI_ARGS_(( int fd, UINT32 timeEnd ));
int MUD_setTitle _ANSI_ARGS_(( int fd, char* title ));
int MUD_setLab _ANSI_ARGS_(( int fd, char* lab ));
int MUD_setArea _ANSI_ARGS_(( int fd, char* area ));
int MUD_setMethod _ANSI_ARGS_(( int fd, char* method ));
int MUD_setApparatus _ANSI_ARGS_(( int fd, char* apparatus ));
int MUD_setInsert _ANSI_ARGS_(( int fd, char* insert ));
int MUD_setSample _ANSI_ARGS_(( int fd, char* sample ));
int MUD_setOrient _ANSI_ARGS_(( int fd, char* orient ));
int MUD_setDas _ANSI_ARGS_(( int fd, char* das ));
int MUD_setExperimenter _ANSI_ARGS_(( int fd, char* experimenter ));
int MUD_setTemperature _ANSI_ARGS_(( int fd, char* temperature ));
int MUD_setField _ANSI_ARGS_(( int fd, char* field ));
int MUD_setSubtitle _ANSI_ARGS_(( int fd, char* subtitle ));
int MUD_setComment1 _ANSI_ARGS_(( int fd, char* comment1 ));
int MUD_setComment2 _ANSI_ARGS_(( int fd, char* comment2 ));
int MUD_setComment3 _ANSI_ARGS_(( int fd, char* comment3 ));

int MUD_getComments _ANSI_ARGS_(( int fd, UINT32* pType, UINT32* pNum ));
int MUD_getCommentPrev _ANSI_ARGS_(( int fd, int num, UINT32* pPrev ));
int MUD_getCommentNext _ANSI_ARGS_(( int fd, int num, UINT32* pNext ));
int MUD_getCommentTime _ANSI_ARGS_(( int fd, int num, UINT32* pTime ));
int MUD_getCommentAuthor _ANSI_ARGS_(( int fd, int num, char* author, int strdim ));
int MUD_getCommentTitle _ANSI_ARGS_(( int fd, int num, char* title, int strdim ));
int MUD_getCommentBody _ANSI_ARGS_(( int fd, int num, char* body, int strdim ));

int MUD_setComments _ANSI_ARGS_(( int fd, UINT32 type, UINT32 num ));
int MUD_setCommentPrev _ANSI_ARGS_(( int fd, int num, UINT32 prev ));
int MUD_setCommentNext _ANSI_ARGS_(( int fd, int num, UINT32 next ));
int MUD_setCommentTime _ANSI_ARGS_(( int fd, int num, UINT32 time ));
int MUD_setCommentAuthor _ANSI_ARGS_(( int fd, int num, char* author ));
int MUD_setCommentTitle _ANSI_ARGS_(( int fd, int num, char* title ));
int MUD_setCommentBody _ANSI_ARGS_(( int fd, int num, char* body ));

int MUD_getHists _ANSI_ARGS_(( int fd, UINT32* pType, UINT32* pNum ));
int MUD_getHistType _ANSI_ARGS_(( int fd, int num, UINT32* pType ));
int MUD_getHistNumBytes _ANSI_ARGS_(( int fd, int num, UINT32* pNumBytes ));
int MUD_getHistNumBins _ANSI_ARGS_(( int fd, int num, UINT32* pNumBins ));
int MUD_getHistBytesPerBin _ANSI_ARGS_(( int fd, int num, UINT32* pBytesPerBin ));
int MUD_getHistFsPerBin _ANSI_ARGS_(( int fd, int num, UINT32* pFsPerBin ));
int MUD_getHistSecondsPerBin _ANSI_ARGS_(( int fd, int num, REAL64* pSecondsPerBin ));
int MUD_getHistT0_Ps _ANSI_ARGS_(( int fd, int num, UINT32* pT0_ps ));
int MUD_getHistT0_Bin _ANSI_ARGS_(( int fd, int num, UINT32* pT0_bin ));
int MUD_getHistGoodBin1 _ANSI_ARGS_(( int fd, int num, UINT32* pGoodBin1 ));
int MUD_getHistGoodBin2 _ANSI_ARGS_(( int fd, int num, UINT32* pGoodBin2 ));
int MUD_getHistBkgd1 _ANSI_ARGS_(( int fd, int num, UINT32* pBkgd1 ));
int MUD_getHistBkgd2 _ANSI_ARGS_(( int fd, int num, UINT32* pBkgd2 ));
int MUD_getHistNumEvents _ANSI_ARGS_(( int fd, int num, UINT32* pNumEvents ));
int MUD_getHistTitle _ANSI_ARGS_(( int fd, int num, char* title, int strdim ));
int MUD_getHistData _ANSI_ARGS_(( int fd, int num, void* pData ));
int MUD_getHistpData _ANSI_ARGS_(( int fd, int num, void** ppData ));
int MUD_getHistTimeData _ANSI_ARGS_(( int fd, int num, UINT32* pTimeData ));
int MUD_getHistpTimeData _ANSI_ARGS_(( int fd, int num, UINT32** ppTimeData ));

int MUD_setHists _ANSI_ARGS_(( int fd, UINT32 type, UINT32 num ));
int MUD_setHistType _ANSI_ARGS_(( int fd, int num, UINT32 type ));
int MUD_setHistNumBytes _ANSI_ARGS_(( int fd, int num, UINT32 numBytes ));
int MUD_setHistNumBins _ANSI_ARGS_(( int fd, int num, UINT32 numBins ));
int MUD_setHistBytesPerBin _ANSI_ARGS_(( int fd, int num, UINT32 bytesPerBin ));
int MUD_setHistFsPerBin _ANSI_ARGS_(( int fd, int num, UINT32 fsPerBin ));
int MUD_setHistSecondsPerBin _ANSI_ARGS_(( int fd, int num, REAL64 secondsPerBin ));
int MUD_setHistT0_Ps _ANSI_ARGS_(( int fd, int num, UINT32 t0_ps ));
int MUD_setHistT0_Bin _ANSI_ARGS_(( int fd, int num, UINT32 t0_bin ));
int MUD_setHistGoodBin1 _ANSI_ARGS_(( int fd, int num, UINT32 goodBin1 ));
int MUD_setHistGoodBin2 _ANSI_ARGS_(( int fd, int num, UINT32 goodBin2 ));
int MUD_setHistBkgd1 _ANSI_ARGS_(( int fd, int num, UINT32 bkgd1 ));
int MUD_setHistBkgd2 _ANSI_ARGS_(( int fd, int num, UINT32 bkgd2 ));
int MUD_setHistNumEvents _ANSI_ARGS_(( int fd, int num, UINT32 numEvents ));
int MUD_setHistTitle _ANSI_ARGS_(( int fd, int num, char* title ));
int MUD_setHistData _ANSI_ARGS_(( int fd, int num, void* pData ));
int MUD_setHistpData _ANSI_ARGS_(( int fd, int num, void* pData ));
int MUD_setHistTimeData _ANSI_ARGS_(( int fd, int num, UINT32* pTimeData ));
int MUD_setHistpTimeData _ANSI_ARGS_(( int fd, int num, UINT32* pTimeData ));

int MUD_pack _ANSI_ARGS_(( int num, int inBinSize, void* inArray, int outBinSize, void* outArray ));
int MUD_unpack _ANSI_ARGS_(( int num, int inBinSize, void* inArray, int outBinSize, void* outArray ));

int MUD_getScalers _ANSI_ARGS_(( int fd, UINT32* pType, UINT32* pNum ));
int MUD_getScalerLabel _ANSI_ARGS_(( int fd, int num, char* label, int strdim ));
int MUD_getScalerCounts _ANSI_ARGS_(( int fd, int num, UINT32* pCounts ));

int MUD_setScalers _ANSI_ARGS_(( int fd, UINT32 type, UINT32 num ));
int MUD_setScalerLabel _ANSI_ARGS_(( int fd, int num, char* label ));
int MUD_setScalerCounts _ANSI_ARGS_(( int fd, int num, UINT32* pCounts ));

int MUD_getIndVars _ANSI_ARGS_(( int fd, UINT32* pType, UINT32* pNum ));
int MUD_getIndVarLow _ANSI_ARGS_(( int fd, int num, double* pLow ));
int MUD_getIndVarHigh _ANSI_ARGS_(( int fd, int num, double* pHigh ));
int MUD_getIndVarMean _ANSI_ARGS_(( int fd, int num, double* pMean ));
int MUD_getIndVarStddev _ANSI_ARGS_(( int fd, int num, double* pStddev ));
int MUD_getIndVarSkewness _ANSI_ARGS_(( int fd, int num, double* pSkewness ));
int MUD_getIndVarName _ANSI_ARGS_(( int fd, int num, char* name, int strdim ));
int MUD_getIndVarDescription _ANSI_ARGS_(( int fd, int num, char* description, int strdim ));
int MUD_getIndVarUnits _ANSI_ARGS_(( int fd, int num, char* units, int strdim ));
int MUD_getIndVarNumData _ANSI_ARGS_(( int fd, int num, UINT32* pNumData ));
int MUD_getIndVarElemSize _ANSI_ARGS_(( int fd, int num, UINT32* pElemSize ));
int MUD_getIndVarDataType _ANSI_ARGS_(( int fd, int num, UINT32* pDataType ));
int MUD_getIndVarHasTime _ANSI_ARGS_(( int fd, int num, UINT32* pHasTime ));
int MUD_getIndVarData _ANSI_ARGS_(( int fd, int num, void* pData  ));
int MUD_getIndVarTimeData _ANSI_ARGS_(( int fd, int num, UINT32* pTimeData  ));
int MUD_getIndVarpData _ANSI_ARGS_(( int fd, int num, void** ppData  ));
int MUD_getIndVarpTimeData _ANSI_ARGS_(( int fd, int num, UINT32** ppTimeData  ));

int MUD_setIndVars _ANSI_ARGS_(( int fd, UINT32 type, UINT32 num ));
int MUD_setIndVarLow _ANSI_ARGS_(( int fd, int num, double low ));
int MUD_setIndVarHigh _ANSI_ARGS_(( int fd, int num, double high ));
int MUD_setIndVarMean _ANSI_ARGS_(( int fd, int num, double mean ));
int MUD_setIndVarStddev _ANSI_ARGS_(( int fd, int num, double stddev ));
int MUD_setIndVarSkewness _ANSI_ARGS_(( int fd, int num, double skewness ));
int MUD_setIndVarName _ANSI_ARGS_(( int fd, int num, char* name ));
int MUD_setIndVarDescription _ANSI_ARGS_(( int fd, int num, char* description ));
int MUD_setIndVarUnits _ANSI_ARGS_(( int fd, int num, char* units ));
int MUD_setIndVarNumData _ANSI_ARGS_(( int fd, int num, UINT32 numData ));
int MUD_setIndVarElemSize _ANSI_ARGS_(( int fd, int num, UINT32 elemSize ));
int MUD_setIndVarDataType _ANSI_ARGS_(( int fd, int num, UINT32 dataType ));
int MUD_setIndVarHasTime _ANSI_ARGS_(( int fd, int num, UINT32 hasTime ));
int MUD_setIndVarData _ANSI_ARGS_(( int fd, int num, void* pData ));
int MUD_setIndVarTimeData _ANSI_ARGS_(( int fd, int num, UINT32* pTimeData ));
int MUD_setIndVarpData _ANSI_ARGS_(( int fd, int num, void* pData ));
int MUD_setIndVarpTimeData _ANSI_ARGS_(( int fd, int num, UINT32* pTimeData ));

#ifdef __cplusplus
}
#endif

#endif  /* _MUD_H_ */

