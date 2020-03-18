/*
 *  mud_encode.c  --  encode/decode, 
 *  Routines for converting encoding and byte order between the MUD standard and the
 *  local machine standard.
 *  
 *   Copyright (C) 1994 TRIUMF (Vancouver, Canada)
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
 *  Float conversions adapted from Sun RPC xdr_float.c, which is covered by a separate
 *  license (unrestricted use).
 */

#include "mud.h"


void
bdecode_2( void* b, void* p )
{
#ifdef MUD_BIG_ENDIAN
  UINT16 i;
  bcopy( b, &i, 2 );
  i = _swap2bytes(i);
  bcopy( &i, p, 2 );
#else
  bcopy( b, p, 2 );
#endif /* MUD_BIG_ENDIAN */
}

void
bencode_2( void* b, void* p )
{
#ifdef MUD_BIG_ENDIAN
  UINT16 i;
  bcopy( p, &i, 2 );
#ifdef DEBUG
  printf( "bencode_2: got %04X,", i );
#endif 
  i = _swap2bytes(i);
#ifdef DEBUG
  printf( "coded %04X\n", i );
#endif
  bcopy( &i, b, 2 );
#else
  bcopy( p, b, 2 );
#endif /* MUD_BIG_ENDIAN */
}

void
bdecode_4( void* b,void*  p )
{
#ifdef MUD_BIG_ENDIAN
  UINT32 i;
  bcopy( b, &i, 4 );
  i = _swap4bytes(i);
  bcopy( &i, p, 4 );
#else
  bcopy( b, p, 4 );
#endif /* MUD_BIG_ENDIAN */
}

void
bencode_4( void* b, void* p )
{
#ifdef MUD_BIG_ENDIAN
  UINT32 i;
  bcopy( p, &i, 4 );
  i = _swap4bytes(i);
  bcopy( &i, b, 4 );
#else
  bcopy( p, b, 4 );
#endif /* MUD_BIG_ENDIAN */
}

void
bdecode_8( void* b, void* p )
{
#ifdef MUD_BIG_ENDIAN
  UINT32 i[2], i2[2];
  bcopy( b, i, 8 );
  i2[1] = _swap4bytes(i[0]);
  i2[0] = _swap4bytes(i[1]);
  bcopy( i2, p, 8 );
#else
  bcopy( b, p, 8 );
#endif /* MUD_BIG_ENDIAN */
}

void
bencode_8( void* b, void* p )
{
#ifdef MUD_BIG_ENDIAN
  UINT32 i[2], i2[2];
  bcopy( p, i, 8 );
  i2[1] = _swap4bytes(i[0]);
  i2[0] = _swap4bytes(i[1]);
  bcopy( i2, b, 8 );
#else
  bcopy( p, b, 8 );
#endif /* MUD_BIG_ENDIAN */
}


void
decode_str( BUF* pB, char** ps )
{
    MUD_STR_LEN_TYPE len;

#ifdef DEBUG
    printf( "buf pos = %d\n", pB->pos );
    printf( "decoding string length...\n" );
#endif /* DEBUG */
    bdecode_2( (void*)&(pB->buf[pB->pos]), (void*)&len );
#ifdef DEBUG
    printf( "string len %d\n", len );
#endif /* DEBUG */
    pB->pos += 2;
    pB->size += 2;
    if( *ps == NULL ) *ps = (char*)zalloc( len+1 );
    strncpy( *ps, &pB->buf[pB->pos], len );
    pB->pos += len;
    pB->size += len;
#ifdef DEBUG
    printf( "string:\"%s\"\n", *ps );
#endif /* DEBUG */
}


void
encode_str( BUF* pB, char** ps )
{
    MUD_STR_LEN_TYPE len;

    len = _strlen( *ps );
    bencode_2( (void*)&pB->buf[pB->pos], (void*)&len );
    pB->pos += 2;
    pB->size += 2;
    strncpy( &pB->buf[pB->pos], *ps, len );
    pB->pos += len;
    pB->size += len;
}


#ifndef VMS

#ifdef MUD_LITTLE_ENDIAN
struct	ieee_single {
	unsigned int	mantissa2 : 16;
	unsigned int	mantissa1 : 7;
	unsigned int	exp       : 8;
	unsigned int	sign      : 1;
};
struct	vax_single {
	unsigned int	mantissa1 : 7;
	unsigned int	exp       : 8;
	unsigned int	sign      : 1;
	unsigned int	mantissa2 : 16;
};
static struct sgl_limits {
	struct vax_single s;
	struct ieee_single ieee;
} sgl_limits[2] = {
	{{ 0x7f, 0xff, 0x0,  0xffff },	/* Max Vax */
	{  0x0,  0x0,  0xff, 0x0 }},	/* Max IEEE */
	{{ 0x0,  0x0,  0x0,  0x0 },	/* Min Vax */
	{  0x0,  0x0,  0x0,  0x0 }}	/* Min IEEE */
};
#else
struct	ieee_single {
	unsigned int	sign      : 1;
	unsigned int	exp       : 8;
	unsigned int	mantissa1 : 7;
	unsigned int	mantissa2 : 16;
};
struct	vax_single {
	unsigned int	mantissa2 : 16;
	unsigned int	sign      : 1;
	unsigned int	exp       : 8;
	unsigned int	mantissa1 : 7;
};
static struct sgl_limits {
	struct vax_single s;
	struct ieee_single ieee;
} sgl_limits[2] = {
	{{ 0xffff, 0x0, 0xff,  0x7f },	/* Max Vax */
	{  0x0,  0xff,  0x0, 0x0 }},	/* Max IEEE */
	{{ 0x0,  0x0,  0x0,  0x0 },	/* Min Vax */
	{  0x0,  0x0,  0x0,  0x0 }}	/* Min IEEE */
};
#endif /* ENDIAN */

#define VAX_SNG_BIAS	0x81
#define IEEE_SNG_BIAS	0x7f

#endif /* !VMS */

void
bencode_float( char* buf, float* fp )
{
#ifndef VMS
  struct ieee_single is;
  struct vax_single vs;
  struct sgl_limits *lim;
  int i;
#endif /* !VMS */

#ifdef VMS
  bencode_4( buf, fp );
#else
  is = *((struct ieee_single*)fp);

  for( i = 0, lim = sgl_limits;
       i < sizeof(sgl_limits)/sizeof(struct sgl_limits);
       i++, lim++) 
  {
    if( ( is.mantissa1 == lim->ieee.mantissa1 ) &&
        ( is.mantissa2 == lim->ieee.mantissa2 ) && 
        ( is.exp == lim->ieee.exp ) )
    {
      vs = lim->s;
      goto shipit;
    }
  }
  vs.exp = is.exp - IEEE_SNG_BIAS + VAX_SNG_BIAS;
  vs.mantissa2 = is.mantissa2;
  vs.mantissa1 = is.mantissa1;
shipit:
  vs.sign = is.sign;
  bencode_4( (void*)buf, (void*)&vs );
#endif /* VMS */
}

void
encode_float( BUF* pBuf, float* fp )
{
  bencode_float( &(pBuf->buf[pBuf->pos]), fp );
  pBuf->pos += 4;
  pBuf->size += 4;
}

void
bdecode_float( char* buf, float* fp )
{
#ifndef VMS
  struct ieee_single *isp;
  struct vax_single vs;
  struct sgl_limits *lim;
  int i;
#endif /* !VMS */

#ifdef VMS
  bdecode_4( (void*)buf, (void*)fp );
#else
  bdecode_4( (void*)buf, (void*)&vs );

#ifdef DEBUG
  printf( "man1:%d man2:%d exp:%d sign:%d\n", 
          vs.mantissa1, vs.mantissa2, vs.exp, vs.sign );
#endif /* DEBUG */

  isp = (struct ieee_single*)fp;
  for( i = 0, lim = sgl_limits;
       i < sizeof(sgl_limits)/sizeof(struct sgl_limits);
       i++, lim++ ) 
  {
    if( ( vs.mantissa2 == lim->s.mantissa2 ) &&
        ( vs.exp == lim->s.exp ) &&
        ( vs.mantissa1 == lim->s.mantissa1 ) ) 
    {
      *isp = lim->ieee;
      goto shipit;
    }
  }
  isp->exp = vs.exp - VAX_SNG_BIAS + IEEE_SNG_BIAS;
  isp->mantissa1 = vs.mantissa1;
  isp->mantissa2 = vs.mantissa2;
shipit:
  isp->sign = vs.sign;
#endif
}

void
decode_float( BUF* pBuf, float* fp )
{
  bdecode_float( &(pBuf->buf[pBuf->pos]), fp );
  pBuf->pos += 4;
  pBuf->size += 4;
}

/*
 * This routine works on Suns (Sky / 68000's) and Vaxen.
 */

#ifndef VMS

#ifdef MUD_LITTLE_ENDIAN
struct	ieee_double {
	unsigned int	mantissa4 : 16;
	unsigned int	mantissa3 : 16;
	unsigned int	mantissa2 : 16;
	unsigned int	mantissa1 : 4;
	unsigned int	exp       : 11;
	unsigned int	sign      : 1;
};
struct  vax_double {
	unsigned int	mantissa1 : 7;
	unsigned int	exp       : 8;
	unsigned int	sign      : 1;
	unsigned int	mantissa2 : 16;
	unsigned int	mantissa3 : 16;
	unsigned int	mantissa4 : 16;
};
static struct dbl_limits {
	struct	vax_double d;
	struct	ieee_double ieee;
} dbl_limits[2] = {
	{{ 0x7f, 0xff, 0x0, 0xffff, 0xffff, 0xffff },	/* Max Vax */
	 { 0x0, 0x0, 0x0, 0x0, 0x7ff, 0x0 }},		/* Max IEEE */
	{{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},		/* Min Vax */
	 { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }}		/* Min IEEE */
};
#else
struct	ieee_double {
	unsigned int	sign      : 1;
	unsigned int	exp       : 11;
	unsigned int	mantissa1 : 4;
	unsigned int	mantissa2 : 16;
	unsigned int	mantissa3 : 16;
	unsigned int	mantissa4 : 16;
};
struct  vax_double {
	unsigned int	mantissa4 : 16;
	unsigned int	mantissa3 : 16;
	unsigned int	mantissa2 : 16;
	unsigned int	sign      : 1;
	unsigned int	exp       : 8;
	unsigned int	mantissa1 : 7;
};
static struct dbl_limits {
	struct	vax_double d;
	struct	ieee_double ieee;
} dbl_limits[2] = {
	{{ 0xffff, 0xffff, 0xffff, 0x0, 0xff, 0x7f },	/* Max Vax */
	 { 0x0, 0x7ff, 0x0, 0x0, 0x0, 0x0 }},		/* Max IEEE */
	{{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},		/* Min Vax */
	 { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }}		/* Min IEEE */
};
#endif /* ENDIAN */

#define VAX_DBL_BIAS	0x81
#define IEEE_DBL_BIAS	0x3ff
#define MASK(nbits)	((1 << nbits) - 1)

#endif /* !VMS */


void
bencode_double( char* buf, double* dp )
{
#ifndef VMS
  struct ieee_double id;
  struct vax_double vd;
  register struct dbl_limits *lim;
  int i;
#endif /* !VMS */

#ifdef VMS
  bencode_8( buf, dp );
#else
  id = *((struct ieee_double*)dp);
  for( i = 0, lim = dbl_limits;
       i < sizeof(dbl_limits)/sizeof(struct dbl_limits);
       i++, lim++) 
  {
    if( ( id.mantissa2 == lim->ieee.mantissa2 ) &&
        ( id.mantissa1 == lim->ieee.mantissa1 ) &&
        ( id.mantissa3 == lim->ieee.mantissa3 ) &&
        ( id.mantissa4 == lim->ieee.mantissa4 ) &&
        ( id.exp == lim->ieee.exp ) ) 
    {
      vd = lim->d;
      goto shipit;
    }
  }
  vd.exp = id.exp - IEEE_DBL_BIAS + VAX_DBL_BIAS;
  vd.mantissa1 = ( id.mantissa1 << 3 ) | (id.mantissa2 >> 13);
  vd.mantissa2 = (id.mantissa2 << 3) | (id.mantissa3 >> 13);
  vd.mantissa3 = (id.mantissa3 << 3) | (id.mantissa4 >> 13);
  vd.mantissa4 = (id.mantissa4 << 3);
shipit:
  vd.sign = id.sign;
  bencode_8( buf, &vd );
#endif /* VMS */
}

void
encode_double( BUF* pBuf, double* fp )
{
  bencode_double( &(pBuf->buf[pBuf->pos]), fp );
  pBuf->pos += 8;
  pBuf->size += 8;
}

void
bdecode_double( char* buf, double* dp )
{
#ifndef VMS
  struct ieee_double *idp;
  struct vax_double vd;
  register struct dbl_limits *lim;
  int i;
#endif /* !VMS */

#ifdef VMS
  bdecode_8( buf, dp );
#else
  bdecode_8( buf, &vd );
  idp = (struct ieee_double*)dp;
  for( i = 0, lim = dbl_limits;
       i < sizeof(dbl_limits)/sizeof(struct dbl_limits);
       i++, lim++) 
  {
    if( ( vd.mantissa4 == lim->d.mantissa4 ) &&
        ( vd.mantissa3 == lim->d.mantissa3 ) &&
        ( vd.mantissa2 == lim->d.mantissa2 ) &&
        ( vd.mantissa1 == lim->d.mantissa1 ) &&
        ( vd.exp == lim->d.exp ) )
    {
      *idp = lim->ieee;
      goto shipit;
    }
  }
  idp->exp = vd.exp - VAX_DBL_BIAS + IEEE_DBL_BIAS;
  idp->mantissa1 = vd.mantissa1 >> 3;
  idp->mantissa2 = (vd.mantissa1 << 13) | (vd.mantissa2 >> 3);
  idp->mantissa3 = (vd.mantissa2 << 13) | (vd.mantissa3 >> 3);
  idp->mantissa4 = (vd.mantissa3 << 13) | (vd.mantissa4 >> 3);
shipit:
  idp->sign = vd.sign;
#endif /* VMS */
}

void
decode_double( BUF* pBuf, double* fp )
{
  bdecode_double( &(pBuf->buf[pBuf->pos]), fp );
  pBuf->pos += 8;
  pBuf->size += 8;
}

