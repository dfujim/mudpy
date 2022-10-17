# Cython functions for running the cpp wrapper. 
# Derek Fujimoto
# July 2017
"""
Cython wrapper functions for friendly MUD file reading.
MUD source description can be found on the musr webpage: 
    http://musr.ca/mud/mud_friendly.html

Function list: 

    FILE IO
        open_read
        close_read
            
        open_write
        open_readwrite
        close_write
        close_writefile
        
    RUN DESCRIPTION 
        get_description
        get_exp_number
        get_run_number
        get_elapsed_seconds
        get_start_time
        get_end_time
        get_title
        get_lab
        get_area
        get_method
        get_apparatus
        get_insert
        get_sample
        get_orientation
        get_das
        get_experimenter
        get_temperature
        get_field

        set_description
        set_exp_number
        set_run_number
        set_elapsed_seconds
        set_start_time
        set_end_time
        set_title
        set_lab
        set_area
        set_method
        set_apparatus
        set_insert
        set_sample
        set_orientation
        set_das
        set_experimenter
        set_temperature
        set_field
    
    COMMENTS
        get_comments
        get_comment_prev
        get_comment_next
        get_comment_time
        get_comment_author
        get_comment_title
        get_comment_body
        
        set_comments
        set_comment_prev
        set_comment_next
        set_comment_time
        set_comment_author
        set_comment_title
        set_comment_body
        
    HISTOGRAMS
        get_hists
        get_hist_type
        get_hist_n_bytes
        get_hist_n_bins
        get_hist_bytes_per_bin
        get_hist_fs_per_bin
        get_hist_t0_ps
        get_hist_t0_bin
        get_hist_good_bin1
        get_hist_good_bin2
        get_hist_background1
        get_hist_background2
        get_hist_n_events
        get_hist_title
        get_hist_sec_per_bin
        get_hist_data
        get_hist_data_pointer
        
        set_hists
        set_hist_type
        set_hist_n_bytes
        set_hist_n_bins
        set_hist_bytes_per_bin
        set_hist_fs_per_bin
        set_hist_t0_ps
        set_hist_t0_bin
        set_hist_good_bin1
        set_hist_good_bin2
        set_hist_background1
        set_hist_background2
        set_hist_n_events
        set_hist_title
        set_hist_sec_per_bin
        set_hist_data
        set_hist_data_pointer
        
    SCALARS
        get_scalers
        get_scaler_label
        get_scaler_counts
        
        set_scalers
        set_scaler_label
        set_scaler_counts
        
    INDEPENDENT VARIABLES
        get_ivars
        get_ivar_low
        get_ivar_high
        get_ivar_mean
        get_ivar_std
        get_ivar_skewness
        get_ivar_name
        get_ivar_description
        get_ivar_units
        get_ivar_n_data
        get_ivar_element_size
        get_ivar_data_type
        get_ivar_has_time
        get_ivar_data
        get_ivar_time_data
        
        set_ivars
        set_ivar_low
        set_ivar_high
        set_ivar_mean
        set_ivar_std
        set_ivar_skewness
        set_ivar_name
        set_ivar_description
        set_ivar_units
        set_ivar_n_data
        set_ivar_element_size
        set_ivar_data_type
        set_ivar_data
        set_ivar_time_data
                
Derek Fujimoto 
July 2017
"""

import numpy as np
cimport numpy as np
from cpython cimport array
import array

### ======================================================================= ###
# CONSTANTS
### ======================================================================= ###
character_encoding = "latin1"
DEF TITLE_CHAR_SIZE = 256
DEF COMMENT_CHAR_SIZE = 8192

cdef extern from 'mud.h':
    
    # Lab identifiers
    cdef int MUD_LAB_ALL_ID
    cdef int MUD_LAB_TRI_ID
    cdef int MUD_LAB_RAL_ID
    cdef int MUD_LAB_PSI_ID

    # Format identifiers
    cdef int MUD_FMT_ALL_ID
    cdef int MUD_FMT_GEN_ID
    cdef int MUD_FMT_TRI_TD_ID
    cdef int MUD_FMT_TRI_TI_ID
    
    # All format identifiers
    cdef int MUD_SEC_ID
    cdef int MUD_SEC_FIXED_ID
    cdef int MUD_SEC_GRP_ID
    cdef int MUD_SEC_EOF_ID
    cdef int MUD_SEC_CMT_ID
    
    cdef int MUD_GRP_CMT_ID
    
    # GEN format identifiers
    cdef int MUD_SEC_GEN_RUN_DESC_ID
    cdef int MUD_SEC_GEN_HIST_HDR_ID
    cdef int MUD_SEC_GEN_HIST_DAT_ID
    cdef int MUD_SEC_GEN_SCALER_ID
    cdef int MUD_SEC_GEN_IND_VAR_ID
    cdef int MUD_SEC_GEN_ARRAY_ID
    cdef int MUD_GRP_GEN_HIST_ID
    cdef int MUD_GRP_GEN_SCALER_ID
    cdef int MUD_GRP_GEN_IND_VAR_ID
    cdef int MUD_GRP_GEN_IND_VAR_ARR_ID
    
    # TRI_TD format identifiers
    cdef int MUD_SEC_TRI_TD_HIST_ID
    cdef int MUD_GRP_TRI_TD_HIST_ID
    cdef int MUD_GRP_TRI_TD_SCALER_ID
    
    # TRI_TI format identifiers
    cdef int MUD_SEC_TRI_TI_RUN_DESC_ID
    cdef int MUD_SEC_TRI_TI_HIST_ID
    cdef int MUD_GRP_TRI_TI_HIST_ID

# RAL format identifiers
#~     cdef int MUD_SEC_RAL_RUN_DESC_ID
#~     cdef int MUD_SEC_RAL_HIST_ID
#~     cdef int MUD_GRP_RAL_HIST_ID

# Lab identifiers
LAB_ALL_ID = MUD_LAB_ALL_ID
LAB_TRI_ID = MUD_LAB_TRI_ID
LAB_RAL_ID = MUD_LAB_RAL_ID
LAB_PSI_ID = MUD_LAB_PSI_ID   

# Format identifiers
FMT_ALL_ID = MUD_FMT_ALL_ID
FMT_GEN_ID = MUD_FMT_GEN_ID
FMT_TRI_TD_ID = MUD_FMT_TRI_TD_ID
FMT_TRI_TI_ID = MUD_FMT_TRI_TI_ID

# All format identifiers
SEC_ID = MUD_SEC_ID
SEC_FIXED_ID = MUD_SEC_FIXED_ID
SEC_GRP_ID = MUD_SEC_GRP_ID
SEC_EOF_ID = MUD_SEC_EOF_ID
SEC_CMT_ID = MUD_SEC_CMT_ID

GRP_CMT_ID = MUD_GRP_CMT_ID

# GEN format identifiers
SEC_GEN_RUN_DESC_ID = MUD_SEC_GEN_RUN_DESC_ID
SEC_GEN_HIST_HDR_ID = MUD_SEC_GEN_HIST_HDR_ID
SEC_GEN_HIST_DAT_ID = MUD_SEC_GEN_HIST_DAT_ID
SEC_GEN_SCALER_ID = MUD_SEC_GEN_SCALER_ID
SEC_GEN_IND_VAR_ID = MUD_SEC_GEN_IND_VAR_ID
SEC_GEN_ARRAY_ID = MUD_SEC_GEN_ARRAY_ID
GRP_GEN_HIST_ID = MUD_GRP_GEN_HIST_ID
GRP_GEN_SCALER_ID = MUD_GRP_GEN_SCALER_ID
GRP_GEN_IND_VAR_ID = MUD_GRP_GEN_IND_VAR_ID
GRP_GEN_IND_VAR_ARR_ID = MUD_GRP_GEN_IND_VAR_ARR_ID

# TRI_TD format identifiers
SEC_TRI_TD_HIST_ID = MUD_SEC_TRI_TD_HIST_ID
GRP_TRI_TD_HIST_ID = MUD_GRP_TRI_TD_HIST_ID
GRP_TRI_TD_SCALER_ID = MUD_GRP_TRI_TD_SCALER_ID
    
# TRI_TI format identifiers
SEC_TRI_TI_RUN_DESC_ID = MUD_SEC_TRI_TI_RUN_DESC_ID
SEC_TRI_TI_HIST_ID = MUD_SEC_TRI_TI_HIST_ID
GRP_TRI_TI_HIST_ID = MUD_GRP_TRI_TI_HIST_ID

# RAL format identifiers
#~ SEC_RAL_RUN_DESC_ID = MUD_SEC_RAL_RUN_DESC_ID
#~ SEC_RAL_HIST_ID = MUD_SEC_RAL_HIST_ID
#~ GRP_RAL_HIST_ID = MUD_GRP_RAL_HIST_ID

### ======================================================================= ###
# READ FILE IO
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_openRead(char* file_name, unsigned int* pType)
    void MUD_closeRead(int file_handle)
    
cpdef open_read(str file_name):
    """Open file for reading. Returns file handle."""
    cdef unsigned int file_type = 0
    cdef int fh = MUD_openRead(file_name.encode(character_encoding), &file_type)
    
    if fh < 0:  raise RuntimeError('MUD_openRead failed.')
    return <int>fh

cpdef close_read(int file_handle):
    """Closes open file without writing anything."""
    MUD_closeRead(file_handle)
    
### ======================================================================= ###
# WRITE FILE IO
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_openWrite(char* file_name, unsigned int pType)
    int MUD_openReadWrite(char* file_name, unsigned int* pType)
    void MUD_closeWrite(int file_handle)
    void MUD_closeWriteFile(int file_handle, char* file_name)
    
cpdef open_write(str file_name, unsigned int file_type):
    """
        Open file for writing a new MUD file. 
        file_name:      string, file name 
        file_type:      int, corresponds to one of the format identifiers
                            FMT_ALL_ID
                            FMT_GEN_ID
                            FMT_TRI_TD_ID (TD-MuSR)
                            FMT_TRI_TI_ID (I-MuSR)                           
        Returns file handle.
    """
    cdef int fh = MUD_openWrite(file_name.encode(character_encoding),file_type)
    if fh < 0:  raise RuntimeError('MUD_openWrite failed.')
    return <int>fh
    
cpdef open_readwrite(str file_name):
    """
        Open file for writing changes to an existing MUD file. 
        file_name:      string, file name 
        Returns file handle.
    """
    cdef unsigned int file_type = 0
    cdef int fh = MUD_openReadWrite(file_name.encode(character_encoding), &file_type)
    if fh < 0:  raise RuntimeError('MUD_openWrite failed.')
    return <int>fh
    
cpdef close_write(int file_handle):
    """Writes changes to file and closes."""
    MUD_closeWrite(file_handle)
    
cpdef close_writefile(int file_handle, str file_name):
    """Writes changes to a new file and close both"""
    MUD_closeWriteFile(file_handle, file_name.encode(character_encoding))
    

### ======================================================================= ###
# READ RUN DESCRIPTION
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_getRunDesc(int fh, unsigned int* pType)
    int MUD_getExptNumber(int fh, unsigned int* pExpNumber)
    int MUD_getRunNumber(int fh, unsigned int* pRunNumber)
    int MUD_getElapsedSec(int fh, unsigned int* pElapsedSec)
    int MUD_getTimeBegin(int fh, unsigned int* pTimeBegin)
    int MUD_getTimeEnd(int fh, unsigned int* pTimeEnd)
    int MUD_getTitle(int fh, char* string, int strdim) 
    int MUD_getLab(int fh, char* string, int strdim)
    int MUD_getArea(int fh, char* string, int strdim)
    int MUD_getMethod(int fh, char* string, int strdim)
    int MUD_getApparatus(int fh, char* string, int strdim)
    int MUD_getInsert(int fh, char* string, int strdim)
    int MUD_getSample(int fh, char* string, int strdim)
    int MUD_getOrient(int fh, char* string, int strdim)
    int MUD_getDas(int fh, char* string, int strdim)
    int MUD_getExperimenter(int fh, char* string, int strdim)
    int MUD_getTemperature(int fh, char* string, int strdim )
    int MUD_getField(int fh, char* string, int strdim )

cpdef get_description(int file_handle):
    """Returns int, related to file description."""
    cdef unsigned int type = 0
    if not MUD_getRunDesc(file_handle,&type):
        raise RuntimeError('MUD_getRunDesc failed.')
    return <int>type

cpdef get_exp_number(int file_handle):
    """Returns int, experiment number."""
    cdef unsigned int expNumber = 0
    if not MUD_getExptNumber(file_handle,&expNumber):
        raise RuntimeError('MUD_getExptNumber failed.')
    return <int>expNumber

cpdef get_run_number(int file_handle):
    
    """Returns int, run number."""
    cdef unsigned int runNumber = 0
    if not MUD_getRunNumber(file_handle,&runNumber):
        raise RuntimeError('MUD_getRunNumber failed.')
    return <int>runNumber
    
cpdef get_elapsed_seconds(int file_handle):
    """Returns int, time elapsed during run in seconds."""
    cdef unsigned int time  = 0
    if not MUD_getElapsedSec(file_handle,&time):
        raise RuntimeError('MUD_getElapsedSec failed.')
    return <int>time
    
cpdef get_start_time(int file_handle):
    """Returns int, unix epoch timestamp associated with run start."""
    cdef unsigned int time  = 0
    if not MUD_getTimeBegin(file_handle,&time):
        raise RuntimeError('MUD_getTimeBegin failed.')
    return <int>time
    
cpdef get_end_time(int file_handle):
    """Returns int, unix epoch timestamp associated with run termination."""
    cdef unsigned int time  = 0
    if not MUD_getTimeEnd(file_handle,&time):
        raise RuntimeError('MUD_getTimeEnd failed.')
    return <int>time
    
cpdef get_title(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getTitle(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getTitle failed.')
    return <object>(title.decode(character_encoding))
    
cpdef get_lab(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getLab(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getLab failed.')
    return <object>(title.decode(character_encoding))
 
cpdef get_area(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getArea(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getArea failed.')
    return <object>(title.decode(character_encoding))
    
cpdef get_method(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getMethod(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getMethod failed.')
    return <object>(title.decode(character_encoding))
    
cpdef get_apparatus(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getApparatus(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getApparatus failed.')
    return <object>(title.decode(character_encoding))
    
cpdef get_insert(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getInsert(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getInsert failed.')
    return <object>(title.decode(character_encoding))

cpdef get_sample(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getSample(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getSample failed.')
    return <object>(title.decode(character_encoding))
    
cpdef get_orientation(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getOrient(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getOrient failed.')
    return <object>(title.decode(character_encoding))

cpdef get_das(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getDas(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getDas failed.')
    return <object>(title.decode(character_encoding))

cpdef get_experimenter(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE. """
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getExperimenter(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getExperimenter failed.')
    return <object>(title.decode(character_encoding))

cpdef get_temperature(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE. """
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getTemperature(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getTemperature failed.')
    return <object>(title.decode(character_encoding))

cpdef get_field(int file_handle):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE. """
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getField(file_handle, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getField failed.')
    return <object>(title.decode(character_encoding))

### ======================================================================= ###
# WRITE RUN DESCRIPTION
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_setRunDesc(int fh, unsigned int pType)
    int MUD_setExptNumber(int fh, unsigned int pExpNumber)
    int MUD_setRunNumber(int fh, unsigned int pRunNumber)
    int MUD_setElapsedSec(int fh, unsigned int pElapsedSec)
    int MUD_setTimeBegin(int fh, unsigned int pTimeBegin)
    int MUD_setTimeEnd(int fh, unsigned int pTimeEnd)
    int MUD_setTitle(int fh, char* string) 
    int MUD_setLab(int fh, char* string)
    int MUD_setArea(int fh, char* string)
    int MUD_setMethod(int fh, char* string)
    int MUD_setApparatus(int fh, char* string)
    int MUD_setInsert(int fh, char* string)
    int MUD_setSample(int fh, char* string)
    int MUD_setOrient(int fh, char* string)
    int MUD_setDas(int fh, char* string)
    int MUD_setExperimenter(int fh, char* string)
    int MUD_setTemperature(int fh, char* string)
    int MUD_setField(int fh, char* string)
       
cpdef set_description(int file_handle, unsigned int pType):
    """
        Initializes a run description section of type pType. This must be done 
        before specifying any of the other parts of the run description. Valid 
        types are:
            SEC_GEN_RUN_DESC_ID      (TD-MuSR),
            SEC_TRI_TI_RUN_DESC_ID   (I-MuSR). 
    """
    if not MUD_setRunDesc(file_handle, pType):
        raise RuntimeError('MUD_setRunDesc failed.')
    return
  
cpdef set_exp_number(int file_handle, unsigned int expNumber):
    """Set experiment number."""
    if not MUD_setExptNumber(file_handle,expNumber):
        raise RuntimeError('MUD_setExptNumber failed.')
    return
    
cpdef set_run_number(int file_handle, unsigned int runNumber):
    """Set run number."""
    if not MUD_setRunNumber(file_handle, runNumber):
        raise RuntimeError('MUD_setRunNumber failed.')
    return
    
cpdef set_elapsed_seconds(int file_handle, unsigned int elapsedSec):
    """Set time elapsed during run in seconds."""
    if not MUD_setElapsedSec(file_handle, elapsedSec):
        raise RuntimeError('MUD_setElapsedSec failed.')
    return
    
cpdef set_start_time(int file_handle, unsigned int timeBegin):
    """Set unix epoch timestamp associated with run start."""
    if not MUD_setTimeBegin(file_handle, timeBegin):
        raise RuntimeError('MUD_setTimeBegin failed.')
    return
    
cpdef set_end_time(int file_handle, unsigned int timeEnd):
    """Set unix epoch timestamp associated with run termination."""
    if not MUD_setTimeEnd(file_handle, timeEnd):
        raise RuntimeError('MUD_setTimeEnd failed.')
    return
    
cpdef set_title(int file_handle, str title):
    """title: string"""
    if not MUD_setTitle(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setTitle failed.')
    return
    
cpdef set_lab(int file_handle, str title):
    """title: string"""
    if not MUD_setLab(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setLab failed.')
    return
 
cpdef set_area(int file_handle, str title):
    """title: string"""
    if not MUD_setArea(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setArea failed.')
    return
 
cpdef set_method(int file_handle, str title):
    """title: string"""
    if not MUD_setMethod(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setMethod failed.')
    return
 
cpdef set_apparatus(int file_handle, str title):
    """title: string"""
    if not MUD_setApparatus(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setApparatus failed.')
    return
 
cpdef set_insert(int file_handle, str title):
    """title: string"""
    if not MUD_setInsert(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setInsert failed.')
    return
 
cpdef set_sample(int file_handle, str title):
    """title: string"""
    if not MUD_setSample(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setSample failed.')
    return

cpdef set_orientation(int file_handle, str title):
    """title: string"""
    if not MUD_setOrient(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setOrient failed.')
    return
 
cpdef set_das(int file_handle, str title):
    """title: string"""
    if not MUD_setDas(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setDas failed.')
    return
 
cpdef set_experimenter(int file_handle, str title):
    """title: string"""
    if not MUD_setExperimenter(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setExperimenter failed.')
    return
 
cpdef set_temperature(int file_handle, str title):
    """
        Not in I-MuSR
        title: string
    """
    if not MUD_setTemperature(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setTemperature failed.')
    return
 
cpdef set_field(int file_handle, str title):
    """
        Not in I-MuSR
        title: string
    """
    if not MUD_setField(file_handle, title.encode(character_encoding)):
        raise RuntimeError('MUD_setField failed.')
    return
           
### ======================================================================= ###
# READ COMMENTS
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_getComments(int fh, unsigned int* pType, \
                        unsigned int* n_comments)
    int MUD_getCommentPrev(int fh, int num, unsigned int* pPrev )
    int MUD_getCommentNext(int fh, int num, unsigned int* pNext )
    int MUD_getCommentTime(int fh, int num, unsigned int* pTime )
    int MUD_getCommentAuthor(int fh, int num, char* author, int strdim )
    int MUD_getCommentTitle(int fh, int num, char* title, int strdim )
    int MUD_getCommentBody(int fh, int num, char* body, int strdim )
       
cpdef get_comments(int file_handle, unsigned int pType = GRP_CMT_ID):
    """Returns numpy array of int32: [data type, number of comments]."""
    cdef unsigned int n_comments
    if not MUD_getComments(file_handle, &pType, &n_comments):
        raise RuntimeError('MUD_getComments failed.')
    return np.array([<int>pType,<int>n_comments])
    
cpdef get_comment_prev(int file_handle, int id_number):
    """Returns int, id of previous comment."""
    cdef unsigned int value
    if not MUD_getCommentPrev(file_handle, id_number, &value):
        raise RuntimeError('MUD_getCommentPrev failed.')
    return value

cpdef get_comment_next(int file_handle, int id_number):
    """Returns int, id of next comment."""
    cdef unsigned int value
    if not MUD_getCommentNext(file_handle, id_number, &value):
        raise RuntimeError('MUD_getCommentNext failed.')
    return value

cpdef get_comment_time(int file_handle, int id_number):
    """Returns int, unix epoch timestamp of comment."""
    cdef unsigned int value
    if not MUD_getCommentTime(file_handle, id_number,&value):
        raise RuntimeError('MUD_getCommentTime failed.')
    return value

cpdef get_comment_author(int file_handle, int id_number):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getCommentAuthor(file_handle, id_number, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getCommentAuthor failed.')
    return <object>(title.decode(character_encoding))

cpdef get_comment_title(int file_handle, int id_number):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getCommentTitle(file_handle, id_number, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getCommentTitle failed.')
    return <object>(title.decode(character_encoding))
    
cpdef get_comment_body(int file_handle, int id_number):
    """Returns python string. Max number of characters: COMMENT_CHAR_SIZE."""
    cdef char title[COMMENT_CHAR_SIZE]
    if not MUD_getCommentBody(file_handle, id_number, title, COMMENT_CHAR_SIZE):
        raise RuntimeError('MUD_getCommentBody failed.')
    return <object>(title.decode(character_encoding))
        
### ======================================================================= ###
# WRITE COMMENTS
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_setComments(int fh, unsigned int pType, \
                        unsigned int n_comments)
    int MUD_setCommentPrev(int fh, int num, unsigned int pPrev )
    int MUD_setCommentNext(int fh, int num, unsigned int pNext )
    int MUD_setCommentTime(int fh, int num, unsigned int pTime )
    int MUD_setCommentAuthor(int fh, int num, char* author )
    int MUD_setCommentTitle(int fh, int num, char* title )
    int MUD_setCommentBody(int fh, int num, char* body )
       
cpdef set_comments(int file_handle, unsigned int pType, unsigned int n_comments):
    """ 
        Initializes a comment group of type type with numCom comments. This 
        must be done before defining the comments. The only valid type is 
        MUD_GRP_CMT_ID. 
    """
    if not MUD_setComments(file_handle, pType, n_comments):
        raise RuntimeError('MUD_setComments failed.')
    return
    
cpdef set_comment_prev(int file_handle, int id_number, unsigned int value):
    """Set id of previous comment."""
    if not MUD_setCommentPrev(file_handle, id_number, np.int32(value)):
        raise RuntimeError('MUD_setCommentPrev failed.')
    return 

cpdef set_comment_next(int file_handle, int id_number, unsigned int value):
    """Set id of next comment."""
    if not MUD_setCommentNext(file_handle, id_number, np.int32(value)):
        raise RuntimeError('MUD_setCommentNext failed.')
    return 

cpdef set_comment_time(int file_handle, int id_number, unsigned int value):
    """Set unix epoch timestamp of comment."""
    if not MUD_setCommentTime(file_handle, id_number, np.int32(value)):
        raise RuntimeError('MUD_setCommentTime failed.')
    return

cpdef set_comment_author(int file_handle, int id_number, str title):
    """Set string"""
    if not MUD_setCommentAuthor(file_handle, id_number, title.encode(character_encoding)):
        raise RuntimeError('MUD_setCommentAuthor failed.')
    return 

cpdef set_comment_title(int file_handle, int id_number, str title):
    """Set string"""
    if not MUD_setCommentTitle(file_handle, id_number, title.encode(character_encoding)):
        raise RuntimeError('MUD_setCommentTitle failed.')
    return 

cpdef set_comment_body(int file_handle, int id_number, str title):
    """Set string"""
    if not MUD_setCommentBody(file_handle, id_number, title.encode(character_encoding)):
        raise RuntimeError('MUD_setCommentBody failed.')
    return 

### ======================================================================= ###
# READ HISTOGRAMS
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_getHists( int fh, unsigned int* pType, unsigned int* pNum )
    int MUD_getHistType( int fh, int num, unsigned int* pType )
    int MUD_getHistNumBytes( int fh, int num, unsigned int* pNumBytes )
    int MUD_getHistNumBins( int fh, int num, unsigned int* pNumBins )
    int MUD_getHistBytesPerBin( int fh, int num, unsigned int* pBytesPerBin )
    int MUD_getHistFsPerBin( int fh, int num, unsigned int* pFsPerBin )
    int MUD_getHistT0_Ps( int fh, int num, unsigned int* pT0_ps )
    int MUD_getHistT0_Bin( int fh, int num, unsigned int* pT0_bin )
    int MUD_getHistGoodBin1( int fh, int num, unsigned int* pGoodBin1 )
    int MUD_getHistGoodBin2( int fh, int num, unsigned int* pGoodBin2 )
    int MUD_getHistBkgd1( int fh, int num, unsigned int* pBkgd1 )
    int MUD_getHistBkgd2( int fh, int num, unsigned int* pBkgd2 )
    int MUD_getHistNumEvents( int fh, int num, unsigned int* pNumEvents )
    int MUD_getHistTitle( int fh, int num, char* title, int strdim )

    int MUD_getHistSecondsPerBin( int fh, int num, double* pSecondsPerBin )

    int MUD_getHistData( int fh, int num, void* pData )
    int MUD_getHistpData( int fh, int num, void** ppData )
        
cpdef get_hists(int file_handle):
    """Returns numpy array of int32: [data type, number of histograms]."""
    cdef unsigned int pType
    cdef unsigned int number_of_histograms
    if not MUD_getHists(file_handle,&pType,&number_of_histograms):
        raise RuntimeError('MUD_getHists failed.')
    return np.array([<int>pType,<int>number_of_histograms])
    
cpdef get_hist_type(int file_handle, int id_number):
    """Returns int, histogram data type, probably."""
    cdef unsigned int value
    if not MUD_getHistType(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistType failed.')
    return value

cpdef get_hist_n_bytes(int file_handle, int id_number):
    """Returns int."""
    cdef unsigned int value
    if not MUD_getHistNumBytes(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistNumBytes failed.')
    return value

cpdef get_hist_n_bins(int file_handle, int id_number):
    """Returns int. Number of bins in histogram."""
    cdef unsigned int value
    if not MUD_getHistNumBins(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistNumBins failed.')
    return value

cpdef get_hist_bytes_per_bin(int file_handle, int id_number):
    """Returns int."""
    cdef unsigned int value
    if not MUD_getHistBytesPerBin(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistBytesPerBin failed.')
    return value

cpdef get_hist_fs_per_bin(int file_handle, int id_number):
    """Returns int."""
    cdef unsigned int value
    if not MUD_getHistFsPerBin(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistFsPerBin failed.')
    return value
    
cpdef get_hist_t0_ps(int file_handle, int id_number):
    """Returns int. Initial time of first bin, given in picoseconds."""
    cdef unsigned int value
    if not MUD_getHistT0_Ps(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistT0_Ps failed.')
    return value
    
cpdef get_hist_t0_bin(int file_handle, int id_number):
    """Returns int. Bin label of bin associated with time t0."""
    cdef unsigned int value
    if not MUD_getHistT0_Bin(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistT0_Bin failed.')
    return value

cpdef get_hist_good_bin1(int file_handle, int id_number):
    """Returns int."""
    cdef unsigned int value
    if not MUD_getHistGoodBin1(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistGoodBin1 failed.')
    return value

cpdef get_hist_good_bin2(int file_handle, int id_number):
    """Returns int."""
    cdef unsigned int value
    if not MUD_getHistGoodBin2(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistGoodBin2 failed.')
    return value
    
cpdef get_hist_background1(int file_handle, int id_number):
    """Returns int."""
    cdef unsigned int value
    if not MUD_getHistBkgd1(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistBkgd1 failed.')
    return value    

cpdef get_hist_background2(int file_handle, int id_number):
    """Returns int."""
    cdef unsigned int value
    if not MUD_getHistBkgd2(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistBkgd2 failed.')
    return value    

cpdef get_hist_n_events(int file_handle, int id_number):
    """Returns int."""
    cdef unsigned int value
    if not MUD_getHistNumEvents(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistNumEvents failed.')
    return value    
    
cpdef get_hist_title(int file_handle, int id_number):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getHistTitle(file_handle, id_number, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getHistTitle failed.')
    return <object>(title.decode(character_encoding))
    
cpdef get_hist_sec_per_bin(int file_handle, int id_number):
    """Returns float."""
    cdef double value
    if not MUD_getHistSecondsPerBin(file_handle, id_number,&value):
        raise RuntimeError('MUD_getHistSecondsPerBin failed.')
    return <float>value    

cpdef get_hist_data(int file_handle, int id_number):
    """Returns numpy array of ints: values contained in each histogram bin."""
    nbins = get_hist_n_bins(file_handle, id_number)
    cdef array.array a = array.array('i',[0]*nbins)
    if not MUD_getHistData(file_handle, id_number,&a.data.as_ints[0]):
        raise RuntimeError('MUD_getHistData failed.')
    cdef int[:] ca = a
    return np.array(ca,dtype=int)

cpdef get_hist_data_pointer(int file_handle, int id_number):
    raise RuntimeError("Pointers not available in python. Please use get_hist_data.")

### ======================================================================= ###
# WRITE HISTOGRAMS
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_setHists( int fh, unsigned int pType, unsigned int numHists )
    int MUD_setHistType( int fh, int num, unsigned int pType )
    int MUD_setHistNumBytes( int fh, int num, unsigned int pNumBytes )
    int MUD_setHistNumBins( int fh, int num, unsigned int pNumBins )
    int MUD_setHistBytesPerBin( int fh, int num, unsigned int pBytesPerBin )
    int MUD_setHistFsPerBin( int fh, int num, unsigned int pFsPerBin )
    int MUD_setHistT0_Ps( int fh, int num, unsigned int pT0_ps )
    int MUD_setHistT0_Bin( int fh, int num, unsigned int pT0_bin )
    int MUD_setHistGoodBin1( int fh, int num, unsigned int pGoodBin1 )
    int MUD_setHistGoodBin2( int fh, int num, unsigned int pGoodBin2 )
    int MUD_setHistBkgd1( int fh, int num, unsigned int pBkgd1 )
    int MUD_setHistBkgd2( int fh, int num, unsigned int pBkgd2 )
    int MUD_setHistNumEvents( int fh, int num, unsigned int pNumEvents )
    int MUD_setHistTitle( int fh, int num, char* title)

    int MUD_setHistSecondsPerBin( int fh, int num, double pSecondsPerBin )

    int MUD_setHistData( int fh, int num, void* pData )
    int MUD_setHistpData( int fh, int num, void** ppData )
        
cpdef set_hists(int file_handle, unsigned int pType, unsigned int number_histograms):
    """
        Initializes a histogram group of type type with numHists histograms. 
        This must be done before defining the individual histograms. Valid 
        types are:
            GRP_GEN_HIST_ID ,
            GRP_TRI_TD_HIST_ID    (TD-MuSR),
            GRP_TRI_TI_HIST_ID    (I-MuSR).
    """
    if not MUD_setHists(file_handle, pType, number_histograms):
        raise RuntimeError('MUD_getHists failed.')
    return 
    
cpdef set_hist_type(int file_handle, int id_number, unsigned int pType):
    """Set histogram data type"""
    if not MUD_setHistType(file_handle, id_number, pType):
        raise RuntimeError('MUD_setHistType failed.')
    return

cpdef set_hist_n_bytes(int file_handle, int id_number, unsigned int value):
    """Set number of bytes in histogram"""
    if not MUD_setHistNumBytes(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistNumBytes failed.')
    return 

cpdef set_hist_n_bins(int file_handle, int id_number, unsigned int value):
    """Set number of bins in histogram."""
    if not MUD_setHistNumBins(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistNumBins failed.')
    return

cpdef set_hist_bytes_per_bin(int file_handle, int id_number, unsigned int value):
    """Set number of bytes per bin in histogram"""
    if not MUD_setHistBytesPerBin(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistBytesPerBin failed.')
    return

cpdef set_hist_fs_per_bin(int file_handle, int id_number, unsigned int value):
    """Set number of femptoseconds per bin"""
    if not MUD_setHistFsPerBin(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistFsPerBin failed.')
    return
    
cpdef set_hist_t0_ps(int file_handle, int id_number, unsigned int value):
    """Set initial time of first bin, given in picoseconds."""
    if not MUD_setHistT0_Ps(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistT0_Ps failed.')
    return
    
cpdef set_hist_t0_bin(int file_handle, int id_number, unsigned int value):
    """Set bin label of bin associated with time t0."""
    if not MUD_setHistT0_Bin(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistT0_Bin failed.')
    return 

cpdef set_hist_good_bin1(int file_handle, int id_number, unsigned int value):
    """"""
    if not MUD_setHistGoodBin1(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistGoodBin1 failed.')
    return 

cpdef set_hist_good_bin2(int file_handle, int id_number, unsigned int value):
    """"""
    if not MUD_setHistGoodBin2(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistGoodBin2 failed.')
    return 
    
cpdef set_hist_background1(int file_handle, int id_number, unsigned int value):
    """"""
    if not MUD_setHistBkgd1(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistBkgd1 failed.')
    return

cpdef set_hist_background2(int file_handle, int id_number, unsigned int value):
    """Returns int."""
    if not MUD_setHistBkgd2(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistBkgd2 failed.')
    return

cpdef set_hist_n_events(int file_handle, int id_number, unsigned int value):
    if not MUD_setHistNumEvents(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistNumEvents failed.')
    return
    
cpdef set_hist_title(int file_handle, int id_number, str title):
    if not MUD_setHistTitle(file_handle, id_number, title.encode(character_encoding)):
        raise RuntimeError('MUD_setHistTitle failed.')
    return
    
cpdef set_hist_sec_per_bin(int file_handle, id_number, double value):
    if not MUD_setHistSecondsPerBin(file_handle, id_number, value):
        raise RuntimeError('MUD_setHistSecondsPerBin failed.')
    return

cpdef set_hist_data(int file_handle, int id_number, long[:] data_array):
    """
        Set data: numpy array of ints for values contained in each histogram bin.
    
        Note that you pass an array of 32 bit integers to the routine 
        MUD_setHistData. This routine will pack the array, if necessary, 
        depending on the previous definition of bytesPerBin. Note that a value 
        of 0 (zero) in bytesPerBin indicates a packed array.

        For C usage, it might be desireable to pass a pointer to the array 
        using MUD_setHistpData. In this case, it is left to the programmer to 
        pack the array (if necessary).     
    """
    
    # set dtype and contiguous
    cdef np.uint32_t[::1] buff = np.ascontiguousarray(data_array, dtype=np.uint32)
    
    if not MUD_setHistData(file_handle, id_number, &buff[0]):
        raise RuntimeError('MUD_setHistData failed.')
    return 

cpdef set_hist_data_pointer(int file_handle, int id_number):
    raise RuntimeError("Pointers not available in python. Please use set_hist_data.")
    
### ======================================================================= ###
# READ SCALARS
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_getScalers( int fh, unsigned int* pType, unsigned int* pNum )
    int MUD_getScalerLabel( int fh, int num, char* label, int strdim )
    int MUD_getScalerCounts( int fh, int num, void* pCounts )
    
cpdef get_scalers(int file_handle):
    """Returns numpy array of int32: [data type, number of scalars]."""
    cdef unsigned int pType
    cdef unsigned int number_of_scalars
    if not MUD_getScalers(file_handle,&pType,&number_of_scalars):
        raise RuntimeError('MUD_getScalers failed.')
    return np.array([<int>pType,<int>number_of_scalars])
    
cpdef get_scaler_label(int file_handle, int id_number):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getScalerLabel(file_handle,id_number, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getScalerLabel failed.')
    return <object>(title.decode('Latin-1'))
    
cpdef get_scaler_counts(int file_handle, int id_number):
    """
        Returns array of two 4-byte integers: the scaler total, and the most 
        recent rate
    """ 
    cdef array.array value = array.array('i',[0]*2) 
    if not MUD_getScalerCounts(file_handle, id_number, &value.data.as_ints[0]):
        raise RuntimeError('MUD_getScalerCounts failed.')
    cdef int[:] ca = value
    return np.array(ca,dtype=int)
        
### ======================================================================= ###
# WRITE SCALARS
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_setScalers( int fh, unsigned int pType, unsigned int pNum )
    int MUD_setScalerLabel( int fh, int num, char* label)
    int MUD_setScalerCounts( int fh, int num, unsigned int* pCounts )
    
cpdef set_scalers(int file_handle, unsigned int pType, unsigned int number_scalars):
    """
        Intializes a scaler group of type type with numScal scalers. Valid 
        types are:
   
            GRP_GEN_SCALER_ID ,
            GRP_TRI_TD_SCALER_ID    (TD-MuSR).
    """
    if not MUD_setScalers(file_handle, pType, number_scalars):
        raise RuntimeError('MUD_setScalers failed.')
    return
    
cpdef set_scaler_label(int file_handle, int id_number, str title):
    if not MUD_setScalerLabel(file_handle, id_number, title.encode(character_encoding)):
        raise RuntimeError('MUD_setScalerLabel failed.')
    return
    
cpdef set_scaler_counts(int file_handle, int id_number, long[:] value):
    """
        Scaler counts are passed in an array of two four-byte unsigned 
        integers. The first element is the scaler total, and the second is the 
        most recent rate. 
    """
    
    # set dtype and contiguous
    cdef np.uint32_t[::1] buff = np.ascontiguousarray(value, dtype=np.uint32)
    
    if not MUD_setScalerCounts(file_handle, id_number, &buff[0]):
        raise RuntimeError('MUD_setScalerCounts failed.')
    return
        
### ======================================================================= ###
# READ INDEPENDENT VARIABLES
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_getIndVars(int fh, unsigned int* pType, unsigned int* number_vars)
    int MUD_getIndVarLow( int fh, int num, double* pLow )
    int MUD_getIndVarHigh( int fh, int num, double* pHigh )
    int MUD_getIndVarMean( int fh, int num, double* pMean )
    int MUD_getIndVarStddev( int fh, int num, double* pStddev )
    int MUD_getIndVarSkewness( int fh, int num, double* pSkewness )
    int MUD_getIndVarName( int fh, int num, char* name, int strdim )
    int MUD_getIndVarDescription( int fh, int num, char* description, int strdim)
    int MUD_getIndVarUnits( int fh, int num, char* units, int strdim )
    
    int MUD_getIndVarNumData( int fh, int num, unsigned int* pNumData )
    int MUD_getIndVarElemSize( int fh, int num, unsigned int* pElemSize )
    int MUD_getIndVarDataType( int fh, int num, unsigned int* pDataType )
    int MUD_getIndVarHasTime( int fh, int num, unsigned int* pHasTime )
    int MUD_getIndVarData( int fh, int num, void* pData )
    int MUD_getIndVarTimeData( int fh, int num, void* pTimeData )
    
cpdef get_ivars(int file_handle):
    """Returns numpy array of int32: [data type, number of independent variables]."""
    cdef unsigned int pType
    cdef unsigned int number_of_variables
    if not MUD_getIndVars(file_handle, &pType, &number_of_variables):
        raise RuntimeError('MUD_getIndVars failed.')
    return np.array([<int>pType, <int>number_of_variables])
    
cpdef get_ivar_low(int file_handle, int id_number):
    """Returns float."""
    cdef double value
    if not MUD_getIndVarLow(file_handle, id_number,&value):
        raise RuntimeError('MUD_getIndVarLow failed.')
    return <double>value

cpdef get_ivar_high(int file_handle, int id_number):
    """Returns float."""
    cdef double value
    if not MUD_getIndVarHigh(file_handle, id_number,&value):
        raise RuntimeError('MUD_getIndVarHigh failed.')
    return <double>value

cpdef get_ivar_mean(int file_handle, int id_number):
    """Returns float."""
    cdef double value
    if not MUD_getIndVarMean(file_handle, id_number,&value):
        raise RuntimeError('MUD_getIndVarMean failed.')
    return <double>value
    
cpdef get_ivar_std(int file_handle, int id_number):
    """Returns float."""
    cdef double value
    if not MUD_getIndVarStddev(file_handle, id_number,&value):
        raise RuntimeError('MUD_getIndVarStddev failed.')
    return <double>value  
    
cpdef get_ivar_skewness(int file_handle, int id_number):
    """Returns float."""
    cdef double value
    if not MUD_getIndVarSkewness(file_handle, id_number,&value):
        raise RuntimeError('MUD_getIndVarSkewness failed.')
    return <double>value

cpdef get_ivar_name(int file_handle, int id_number):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getIndVarName(file_handle, id_number, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getIndVarName failed.')
    return <object>(title.decode(character_encoding))

cpdef get_ivar_description(int file_handle, int id_number):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getIndVarDescription(file_handle, id_number, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getIndVarDescription failed.')
    return <object>(title.decode(character_encoding)) 
    
cpdef get_ivar_units(int file_handle, int id_number):
    """Returns python string. Max number of characters: TITLE_CHAR_SIZE."""
    cdef char title[TITLE_CHAR_SIZE]
    if not MUD_getIndVarUnits(file_handle, id_number, title, TITLE_CHAR_SIZE):
        raise RuntimeError('MUD_getIndVarUnits failed.')
    return <object>(title.decode(character_encoding))

cpdef get_ivar_n_data(int file_handle, int id_number):
    """Returns number of history data points """ 
    cdef unsigned int n_data
    if not MUD_getIndVarNumData(file_handle, id_number, &n_data):
        raise RuntimeError('MUD_getIndVarNumData failed.')
    return <int>n_data
    
cpdef get_ivar_element_size(int file_handle, int id_number):
    """Returns size of elements in array in bytes per element""" 
    cdef unsigned int value
    if not MUD_getIndVarElemSize(file_handle, id_number, &value):
        raise RuntimeError('MUD_getIndVarElemSize failed.')
    return value
    
cpdef get_ivar_data_type(int file_handle, int id_number):
    """Returns data type of elements in array""" 
    cdef unsigned int value
    if not MUD_getIndVarDataType(file_handle, id_number, &value):
        raise RuntimeError('MUD_getIndVarDataType failed.')
    return value
    
cpdef get_ivar_has_time(int file_handle, int id_number):
    """Indicates whether or not there is time data""" 
    cdef unsigned int value
    if not MUD_getIndVarHasTime(file_handle, id_number, &value):
        raise RuntimeError('MUD_getIndVarHasTime failed.')
    return value
  
cpdef get_ivar_data(int file_handle, int id_number):
    """Returns array of saved data"""

    data_type = get_ivar_data_type(file_handle, id_number)
    elem_size = get_ivar_element_size(file_handle, id_number)
    n_data = get_ivar_n_data(file_handle, id_number)

    cdef np.uint8_t[::1] buff_int_8
    cdef np.uint16_t[::1] buff_int_16
    cdef np.uint32_t[::1] buff_int_32
    cdef np.float32_t[::1] buff_float_32
    cdef np.float64_t[::1] buff_float_64

    if data_type == 1:
        if elem_size == 1:
            buff_int_8 = np.ascontiguousarray(np.zeros(n_data, dtype=np.uint8), dtype=np.uint8)
            if not MUD_getIndVarData(file_handle, id_number, &buff_int_8[0]):
                raise RuntimeError('MUD_getIndVarData failed.')
            return np.array(buff_int_8, dtype=np.uint8)
        if elem_size == 2:
            buff_int_16 = np.ascontiguousarray(np.zeros(n_data, dtype=np.uint16), dtype=np.uint16)
            if not MUD_getIndVarData(file_handle, id_number, &buff_int_16[0]):
                raise RuntimeError('MUD_getIndVarData failed.')
            return np.array(buff_int_16, dtype=np.uint16)
        if elem_size == 4:
            buff_int_32 = np.ascontiguousarray(np.zeros(n_data, dtype=np.uint32), dtype=np.uint32)
            if not MUD_getIndVarData(file_handle, id_number, &buff_int_32[0]):
                raise RuntimeError('MUD_getIndVarData failed.')
            return np.array(buff_int_32, dtype=np.uint32)

    if data_type == 2:
        if elem_size == 4:
            buff_float_32 = np.ascontiguousarray(np.zeros(n_data, dtype=np.float32), dtype=np.float32)
            if not MUD_getIndVarData(file_handle, id_number, &buff_float_32[0]):
                raise RuntimeError('MUD_getIndVarData failed.')
            return np.array(buff_float_32, dtype=np.float32)
        if elem_size == 8:
            buff_float_64 = np.ascontiguousarray(np.zeros(n_data, dtype=np.float64), dtype=np.float64)
            if not MUD_getIndVarData(file_handle, id_number, &buff_float_64[0]):
                raise RuntimeError('MUD_getIndVarData failed.')
            return np.array(buff_float_64, dtype=np.float64)
    return

cpdef get_ivar_time_data(int file_handle, int id_number):
    """Returns array of saved time data"""
    ndata = get_ivar_n_data(file_handle, id_number)
    cdef array.array a = array.array('i', [0]*ndata)
    if not MUD_getIndVarTimeData(file_handle, id_number, &a.data.as_ints[0]):
        raise RuntimeError('MUD_getIndVarTimeData failed.')
    cdef int[:] ca = a
    return np.array(ca, dtype=int)

### ======================================================================= ###
# WRITE INDEPENDENT VARIABLES
### ======================================================================= ###
cdef extern from "mud_friendly.c":
    int MUD_setIndVars(int fh, unsigned int pType, unsigned int number_variables)
    int MUD_setIndVarLow( int fh, int id_number, double value )
    int MUD_setIndVarHigh( int fh, int id_number, double value )
    int MUD_setIndVarMean( int fh, int id_number, double value )
    int MUD_setIndVarStddev( int fh, int id_number, double value )
    int MUD_setIndVarSkewness( int fh, int id_number, double value )
    int MUD_setIndVarName( int fh, int id_number, char* name )
    int MUD_setIndVarDescription( int fh, int id_number, char* description)
    int MUD_setIndVarUnits( int fh, int id_number, char* units )

    int MUD_setIndVarNumData( int fh, int num, unsigned int value )
    int MUD_setIndVarElemSize( int fh, int num, unsigned int value )
    int MUD_setIndVarDataType( int fh, int num, unsigned int pType )
    int MUD_setIndVarData( int fh, int num, void* pData )
    int MUD_setIndVarTimeData( int fh, int num, void* pTimeData )

cpdef set_ivars(int file_handle, unsigned int pType, unsigned int n_vars):
    """
        Initializes a independent variable group of type type with n_vars
        independent variables. Valid types are:

            MUD_GRP_GEN_IND_VAR_ID     (TD-MuSR -- statistics data only),
            MUD_GRP_GEN_IND_VAR_ARR_ID (I-MuSR -- statistics data, history data,
                                                    and possibly time data).
    """

    if not MUD_setIndVars(file_handle, pType, n_vars):
        raise RuntimeError('MUD_setIndVars failed.')
    return

cpdef set_ivar_low(int file_handle, int id_number, double value):
    if not MUD_setIndVarLow(file_handle, id_number, value):
        raise RuntimeError('MUD_setIndVarLow failed.')
    return

cpdef set_ivar_high(int file_handle, int id_number, double value):
    if not MUD_setIndVarHigh(file_handle, id_number, value):
        raise RuntimeError('MUD_setIndVarHigh failed.')
    return

cpdef set_ivar_mean(int file_handle, int id_number, double value):
    if not MUD_setIndVarMean(file_handle, id_number, value):
        raise RuntimeError('MUD_setIndVarMean failed.')
    return

cpdef set_ivar_std(int file_handle, int id_number, double value):
    if not MUD_setIndVarStddev(file_handle, id_number, value):
        raise RuntimeError('MUD_setIndVarStddev failed.')
    return

cpdef set_ivar_skewness(int file_handle, int id_number, double value):
    if not MUD_setIndVarSkewness(file_handle, id_number, value):
        raise RuntimeError('MUD_setIndVarSkewness failed.')
    return

cpdef set_ivar_name(int file_handle, int id_number, str title):
    if not MUD_setIndVarName(file_handle, id_number, title.encode(character_encoding)):
        raise RuntimeError('MUD_setIndVarName failed.')
    return

cpdef set_ivar_description(int file_handle, int id_number, str title):
    if not MUD_setIndVarDescription(file_handle, id_number, title.encode(character_encoding)):
        raise RuntimeError('MUD_setIndVarDescription failed.')
    return

cpdef set_ivar_units(int file_handle, int id_number, str title):
    if not MUD_setIndVarUnits(file_handle, id_number, title.encode(character_encoding)):
        raise RuntimeError('MUD_setIndVarUnits failed.')
    return

cpdef set_ivar_n_data(int file_handle, int id_number, unsigned int value):
    """Set number of history data points """
    if not MUD_setIndVarNumData(file_handle, id_number, value):
        raise RuntimeError('MUD_setIndVarNumData failed.')
    return

cpdef set_ivar_element_size(int file_handle, int id_number, unsigned int value):
    """Set size of elements in array in bytes per element"""
    if not MUD_setIndVarElemSize(file_handle, id_number, value):
        raise RuntimeError('MUD_setIndVarElemSize failed.')
    return

cpdef set_ivar_data_type(int file_handle, int id_number, unsigned int value):
    """
        Set data type of elements in array
        value =
            1 for integer (3 bytes/element),
            2 for real (4 bytes/element), and
            3 for string
    """
    if not MUD_setIndVarDataType(file_handle, id_number, value):
        raise RuntimeError('MUD_setIndVarDataType failed.')
    return

cpdef set_ivar_data(int file_handle, int id_number, data_array):
    """Set array of saved data"""

    # get which data type was set
    data_type = get_ivar_data_type(file_handle, id_number)
    elem_size = get_ivar_element_size(file_handle, id_number)

    cdef np.uint8_t[::1] buff_int_8
    cdef np.uint16_t[::1] buff_int_16
    cdef np.uint32_t[::1] buff_int_32
    cdef np.float32_t[::1] buff_float_32
    cdef np.float64_t[::1] buff_float_64

    # set dtype and contiguous
    if data_type == 1:
        if elem_size == 1:
            buff_int_8 = np.ascontiguousarray(data_array, dtype=np.uint8)
            if not MUD_setIndVarData(file_handle, id_number, &buff_int_8[0]):
                raise RuntimeError('MUD_setIndVarData failed.')
            return
        if elem_size == 2:
            buff_int_16 = np.ascontiguousarray(data_array, dtype=np.uint16)
            if not MUD_setIndVarData(file_handle, id_number, &buff_int_16[0]):
                raise RuntimeError('MUD_setIndVarData failed.')
            return
        if elem_size == 4:
            buff_int_32 = np.ascontiguousarray(data_array, dtype=np.uint32)
            if not MUD_setIndVarData(file_handle, id_number, &buff_int_32[0]):
                raise RuntimeError('MUD_setIndVarData failed.')
            return

    elif data_type == 2:
        if elem_size == 4:
            buff_float_32 = np.ascontiguousarray(data_array, dtype=np.float32)
            if not MUD_setIndVarData(file_handle, id_number, &buff_float_32[0]):
                raise RuntimeError('MUD_setIndVarData failed.')
            return
        if elem_size == 8:
            buff_float_64 = np.ascontiguousarray(data_array, dtype=np.float64)
            if not MUD_setIndVarData(file_handle, id_number, &buff_float_64[0]):
                raise RuntimeError('MUD_setIndVarData failed.')
            return

    elif data_type == 3:
            raise RuntimeError('Setting strings not defined in set_ivar_data.')
    else:
        raise RuntimeError('Need to set ivar data type')

cpdef set_ivar_time_data(int file_handle, int id_number, long[:] data_array):
    """Set array of saved time data""" 
    # set dtype and contiguous
    cdef np.uint32_t[::1] buff = np.ascontiguousarray(data_array, dtype=np.uint32)
    
    if not MUD_setIndVarTimeData(file_handle, id_number, &buff[0]):
        raise RuntimeError('MUD_getIndVarTimeData failed.')
    return 
