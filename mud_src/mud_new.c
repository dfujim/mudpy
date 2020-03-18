/*
 *  mud_new.c -- Procedure for mallocing and initializing a new MUD section.
 *		 This routine is used for _all_ section types, and so a
 *		 CASE entry must be added for each unique type.
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
 *   v1.0   26-Jan-1994  [TW] Initial version
 *   v1.0a  08-Feb-1994  [TW] Added sizeOf to core
 *   v1.0b  15-Feb-1994  [TW] Split ...GEN_HIST to ...GEN_HIST_HDR
 *                       and ...GEN_HIST_DAT
 *   v1.0c  25-Apr-1994  [TW] Added CAMP sections
 *   v1.1   21-Feb-1996  TW   Remove CAMP sections, add GEN_ARRAY
 *   v1.2a  01-Mar-2000  DA   Add handling of unidentified sections (don't quit)
 */


#include "mud.h"

MUD_SEC*
MUD_new( UINT32 secID, UINT32 instanceID )
{
    MUD_SEC* pMUD_new;
    MUD_PROC proc;
    int sizeOf;

    switch( secID )
    {
	case MUD_SEC_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC ) );
	    proc = (MUD_PROC)MUD_SEC_proc;
	    sizeOf = sizeof( MUD_SEC );
	    break;
	case MUD_SEC_FIXED_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_FIXED ) );
	    proc = (MUD_PROC)MUD_SEC_FIXED_proc;
	    sizeOf = sizeof( MUD_SEC_FIXED );
	    break;
	case MUD_SEC_GRP_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_GRP ) );
	    proc = (MUD_PROC)MUD_SEC_GRP_proc;
	    sizeOf = sizeof( MUD_SEC_GRP );
	    break;
	case MUD_SEC_EOF_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_EOF ) );
	    proc = (MUD_PROC)MUD_SEC_EOF_proc;
	    sizeOf = sizeof( MUD_SEC_EOF );
	    break;
	case MUD_SEC_CMT_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_CMT ) );
	    proc = (MUD_PROC)MUD_SEC_CMT_proc;
	    sizeOf = sizeof( MUD_SEC_CMT );
	    break;
	case MUD_SEC_GEN_RUN_DESC_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_GEN_RUN_DESC ) );
	    proc = (MUD_PROC)MUD_SEC_GEN_RUN_DESC_proc;
	    sizeOf = sizeof( MUD_SEC_GEN_RUN_DESC );
	    break;
	case MUD_SEC_GEN_HIST_HDR_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_GEN_HIST_HDR ) );
	    proc = (MUD_PROC)MUD_SEC_GEN_HIST_HDR_proc;
	    sizeOf = sizeof( MUD_SEC_GEN_HIST_HDR );
	    break;
	case MUD_SEC_GEN_HIST_DAT_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_GEN_HIST_DAT ) );
	    proc = (MUD_PROC)MUD_SEC_GEN_HIST_DAT_proc;
	    sizeOf = sizeof( MUD_SEC_GEN_HIST_DAT );
	    break;
	case MUD_SEC_GEN_SCALER_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_GEN_SCALER ) );
	    proc = (MUD_PROC)MUD_SEC_GEN_SCALER_proc;
	    sizeOf = sizeof( MUD_SEC_GEN_SCALER );
	    break;
	case MUD_SEC_GEN_IND_VAR_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_GEN_IND_VAR ) );
	    proc = (MUD_PROC)MUD_SEC_GEN_IND_VAR_proc;
	    sizeOf = sizeof( MUD_SEC_GEN_IND_VAR );
	    break;
	case MUD_SEC_GEN_ARRAY_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_GEN_ARRAY ) );
	    proc = (MUD_PROC)MUD_SEC_GEN_ARRAY_proc;
	    sizeOf = sizeof( MUD_SEC_GEN_ARRAY );
	    break;
	case MUD_SEC_TRI_TI_RUN_DESC_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_TRI_TI_RUN_DESC ) );
	    proc = (MUD_PROC)MUD_SEC_TRI_TI_RUN_DESC_proc;
	    sizeOf = sizeof( MUD_SEC_TRI_TI_RUN_DESC );
	    break;
/*
	case MUD_SEC_CAMP_NUM_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_CAMP_NUM ) );
	    proc = (MUD_PROC)MUD_SEC_CAMP_NUM_proc;
	    sizeOf = sizeof( MUD_SEC_CAMP_NUM );
	    break;
	case MUD_SEC_CAMP_STR_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_CAMP_STR ) );
	    proc = (MUD_PROC)MUD_SEC_CAMP_STR_proc;
	    sizeOf = sizeof( MUD_SEC_CAMP_STR );
	    break;
	case MUD_SEC_CAMP_SEL_ID:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_CAMP_SEL ) );
	    proc = (MUD_PROC)MUD_SEC_CAMP_SEL_proc;
	    sizeOf = sizeof( MUD_SEC_CAMP_SEL );
	    break;
*/

/* add action for unknown */
	default:
	    pMUD_new = (MUD_SEC*)zalloc( sizeof( MUD_SEC_UNKNOWN ) );
	    proc = (MUD_PROC)MUD_SEC_UNKNOWN_proc;
	    sizeOf = sizeof( MUD_SEC_UNKNOWN );
	    break;

    }

    if( pMUD_new == NULL ) return( NULL );

    pMUD_new->core.sizeOf = sizeOf;
    pMUD_new->core.secID = secID;
    pMUD_new->core.instanceID = instanceID;
    pMUD_new->core.proc = proc;

    return( pMUD_new );
}

