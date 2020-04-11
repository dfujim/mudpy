# Package Overview

MUon (MUD) Data file reader package.

## Setup 

### Dependencies needed pre-install: 

* python development version: `sudo apt-get install python3-dev` (on Ubuntu, for example)
* Cython: `pip3 install Cython`
* numpy: `pip3 install numpy`

### Install 

* Install using pip: `pip3 install mud-py`

Note that the import statement after install is `import mudpy`

## Contents

* [`mdata`](#mdata) [object]: access general MUD files, provides special containers
* [`mud_friendly`](#mud_friendly) [C wrapper]: python access to MUD C functions

# [mdata](https://github.com/dfujim/mudpy/blob/master/mudpy/mdata.py)
mud-data object. The mdata object is a data container designed for easy reading of any MUD file, regardless of experiment type or mesurement method. 

## Object Map

**Signature**: 

`mdata(filename)`

Example: `mud = mdata('filename.msr')`


## Misc Features

### Representation

   Representation has been nicely formatted so that typing the object 
   name into the interpreter produces nice output. 

### Operators

   mvar, mscaler, and mhist objects allow for arithmatic or logic 
   operators to be used, where the value used in the operation is the 
   mean, count, or data array respectively. 

   Example:    `mud.ivar['BNMR:HVBIAS:POS:RDVOL'] + 1`
   is equivalent to 
               `mud.ivar['BNMR:HVBIAS:POS:RDVOL'].mean + 1`

### Special Rules For Attributes

   If an attribute is not found in mdata, it will look for the 
   attribute in the bdict objects in the order: hist, ivar.
   This second-level attribute search is much slower than regular 
   access.

   mdict objects all allow assignment and fetching of dictionary keys 
   as if they were attributes. Note that one can replace `+` with `p`,
   and `-` with `m` to allow fetching of histograms. 

   Example: `mud.hist.Bp`, `mud.hist['B+']`, `mud.Bp` all have the 
            exact same output, with the last being much slower than 
            the others.


# [mud_friendly](https://github.com/dfujim/mudpy/blob/master/mudpy/mud_friendly.pyx)

Wrap the [mud_friendly.c](http://musr.ca/mud/mud_friendly.html) functions using Cython. Gives low-level access to mud files.

**Functions**: 

File IO

|Cython Function (python-accessible) |C Function (wrapped) |
| -------- | -------- |
|`open_read(str file_name)` |`MUD_openRead(char* file_name, unsigned int* pType)` |
|`open_write(str file_name, unsigned int file_type)` |`MUD_openWrite(char* file_name, unsigned int pType)` |
|`open_readwrite(str file_name)` |`MUD_openReadWrite(char* file_name, unsigned int* pType)` |
|`close_read(int file_handle)` |`MUD_closeRead(int file_handle)` |
|`close_write(int file_handle)` |`MUD_closeWrite(int file_handle)` |
|`close_writefile(int file_handle, str file_name)` |`MUD_closeWriteFile(int file_handle, char* file_name)` |

Headers

|Cython Function (python-accessible) |C Function (wrapped) |
| -------- | -------- |
|`get_description(int file_handle)` |`int MUD_getRunDesc(int fh, unsigned int* pType)` |
|`get_exp_number(int file_handle)` |`int MUD_getExptNumber(int fh, unsigned int* pExpNumber)` |
|`get_run_number(int file_handle)` |`int MUD_getRunNumber(int fh, unsigned int* pRunNumber)` |
|`get_elapsed_seconds(int file_handle)` |`int MUD_getElapsedSec(int fh, unsigned int* pElapsedSec)` |
|`get_start_time(int file_handle)` |`int MUD_getTimeBegin(int fh, unsigned int* pTimeBegin)` |
|`get_end_time(int file_handle)` |`int MUD_getTimeEnd(int fh, unsigned int* pTimeEnd)` |
|`get_title(int file_handle)` |`int MUD_getTitle(int fh, char* string, int strdim)` |
|`get_lab(int file_handle)` |`int MUD_getLab(int fh, char* string, int strdim)` |
|`get_area(int file_handle)` |`int MUD_getArea(int fh, char* string, int strdim)` |
|`get_method(int file_handle)` |`int MUD_getMethod(int fh, char* string, int strdim)` |
|`get_apparatus(int file_handle)` |`int MUD_getApparatus(int fh, char* string, int strdim)` |
|`get_insert(int file_handle)` |`int MUD_getInsert(int fh, char* string, int strdim)` |
|`get_sample(int file_handle)` |`int MUD_getSample(int fh, char* string, int strdim)` |
|`get_orientation(int file_handle)` |`int MUD_getOrient(int fh, char* string, int strdim)` |
|`get_das(int file_handle)` |`int MUD_getDas(int fh, char* string, int strdim)` |
|`get_experimenter(int file_handle)` |`int MUD_getExperimenter(int fh, char* string, int strdim)` |
|`get_temperature(int file_handle)` |`int MUD_getTemperature(int fh, char* string, int strdim )` |
|`get_field(int file_handle)` |`int MUD_getField(int fh, char* string, int strdim )` |

|Cython Function (python-accessible) |C Function (wrapped) |
| -------- | -------- |
|`set_description(int file_handle, unsigned int pType)` |`MUD_setRunDesc(int fh, unsigned int pType)` |
|`set_exp_number(int file_handle, unsigned int expNumber)` |`int MUD_setExptNumber(int fh, unsigned int pExpNumber)` |
|`set_run_number(int file_handle, unsigned int runNumber)` |`int MUD_setRunNumber(int fh, unsigned int pRunNumber)` |
|`set_elapsed_seconds(int file_handle, unsigned int elapsedSec)` |`int MUD_setElapsedSec(int fh, unsigned int pElapsedSec)` |
|`set_start_time(int file_handle, unsigned int timeBegin)` |`int MUD_setTimeBegin(int fh, unsigned int pTimeBegin)` |
|`set_end_time(int file_handle, unsigned int timeEnd)` |`int MUD_setTimeEnd(int fh, unsigned int pTimeEnd)` |
|`set_title(int file_handle, str title)` |`int MUD_setTitle(int fh, char* string )` |
|`set_lab(int file_handle, str title)` |`int MUD_setLab(int fh, char* string )` |
|`set_area(int file_handle, str title)` |`int MUD_setArea(int fh, char* string )` |
|`set_method(int file_handle, str title)` |`int MUD_setMethod(int fh, char* string )` |
|`set_apparatus(int file_handle, str title)` |`int MUD_setApparatus(int fh, char* string )` |
|`set_insert(int file_handle, str title)` |`int MUD_setInsert(int fh, char* string )` |
|`set_sample(int file_handle, str title)` |`int MUD_setSample(int fh, char* string )` |
|`set_orientation(int file_handle, str title)` |`int MUD_setOrient(int fh, char* string )` |
|`set_das(int file_handle, str title)` |`int MUD_setDas(int fh, char* string )` |
|`set_experimenter(int file_handle, str title)` |`int MUD_setExperimenter(int fh, char* string )` |
|`set_temperature(int file_handle, str title)` |`int MUD_setTemperature(int fh, char* string )` |
|`set_field(int file_handle, str title)` |`int MUD_setField(int fh, char* string )` |

Comments

|Cython Function (python-accessible) |C Function (wrapped) |
| -------- | -------- |
|`get_comments(file_handle)` |`int MUD_getComments(int fh, unsigned int* pType, unsigned int* number_of_comments)` |
|`get_comment_prev(file_handle,id_number)` |`int MUD_getCommentPrev(int fh, int num, unsigned int* pPrev )` |
|`get_comment_next(file_handle,id_number)` |`int MUD_getCommentNext(int fh, int num, unsigned int* pNext )` |
|`get_comment_time(file_handle,id_number)` |`int MUD_getCommentTime(int fh, int num, unsigned int* pTime )` |
|`get_comment_author(file_handle,id_number)` |`int MUD_getCommentAuthor(int fh, int num, char* author, int strdim )` |
|`get_comment_title(file_handle,id_number)` |`int MUD_getCommentTitle(int fh, int num, char* title, int strdim )` |
|`get_comment_body(file_handle,id_number)` |`int MUD_getCommentBody(int fh, int num, char* body, int strdim )` |

|Cython Function (python-accessible) |C Function (wrapped) |
| -------- | -------- |
|`set_comments(int file_handle, unsigned int pType, unsigned int n_comments)` |`MUD_setComments(int fh, unsigned int pType, unsigned int n_comments)` |
|`set_comment_prev(int file_handle,id_number,unsigned int value)` |`MUD_setCommentPrev(int fh, int num, unsigned int pPrev )` |
|`set_comment_next(int file_handle,id_number,unsigned int value)` |`MUD_setCommentNext(int fh, int num, unsigned int pNext )` |
|`set_comment_time(int file_handle,id_number,unsigned int value)` |`MUD_setCommentTime(int fh, int num, unsigned int pTime )` |
|`set_comment_author(int file_handle,id_number, str title)` |`MUD_setCommentAuthor(int fh, int num, char* author )` |
|`set_comment_title(int file_handle,id_number, str title)` |`MUD_setCommentTitle(int fh, int num, char* title )` |
|`set_comment_body(int file_handle,id_number, str title)` |`MUD_setCommentBody(int fh, int num, char* body )` |


Histograms

|Cython Function (python-accessible) |C Function (wrapped) |
| -------- | -------- |
|`get_hists(file_handle)` |`int MUD_getHists( int fh, unsigned int* pType, unsigned int* pNum )` |
|`get_hist_type(file_handle,hist_id_number)` |`int MUD_getHistType( int fh, int num, unsigned int* pType )` |
|`get_hist_n_bytes(file_handle,hist_id_number)` |`int MUD_getHistNumBytes( int fh, int num, unsigned int* pNumBytes )` |
|`get_hist_n_bins(file_handle,hist_id_number)` |`int MUD_getHistNumBins( int fh, int num, unsigned int* pNumBins )` |
|`get_hist_bytes_per_bin(file_handle,hist_id_number)` |`int MUD_getHistBytesPerBin( int fh, int num, unsigned int* pBytesPerBin )` |
|`get_hist_fs_per_bin(file_handle,hist_id_number)` |`int MUD_getHistFsPerBin( int fh, int num, unsigned int* pFsPerBin )` |
|`get_hist_t0_ps(file_handle,hist_id_number)` |`int MUD_getHistT0_Ps( int fh, int num, unsigned int* pT0_ps )` |
|`get_hist_t0_bin(file_handle,hist_id_number)` |`int MUD_getHistT0_Bin( int fh, int num, unsigned int* pT0_bin )` |
|`get_hist_good_bin1(file_handle,hist_id_number)` |`int MUD_getHistGoodBin1( int fh, int num, unsigned int* pGoodBin1 )` |
|`get_hist_good_bin2(file_handle,hist_id_number)` |`int MUD_getHistGoodBin2( int fh, int num, unsigned int* pGoodBin2 )` |
|`get_hist_background1(file_handle,hist_id_number)` |`int MUD_getHistBkgd1( int fh, int num, unsigned int* pBkgd1 )` |
|`get_hist_background2(file_handle,hist_id_number)` |`int MUD_getHistBkgd2( int fh, int num, unsigned int* pBkgd2 )` |
|`get_hist_n_events(file_handle,hist_id_number)` |`int MUD_getHistNumEvents( int fh, int num, unsigned int* pNumEvents )` |
|`get_hist_title(file_handle,hist_id_number)` |`int MUD_getHistTitle( int fh, int num, char* title, int strdim )` |
|`get_hist_sec_per_bin(file_handle,hist_id_number)` |`int MUD_getHistSecondsPerBin( int fh, int num, double* pSecondsPerBin )` |
|`get_hist_data(file_handle,hist_id_number)` |`int MUD_getHistData( int fh, int num, void* pData )` |
|`get_hist_data_pointer(file_handle,hist_id_number)` |`int MUD_getHistpData( int fh, int num, void** ppData )` |

Scalers

|Cython Function (python-accessible) |C Function (wrapped) |
| -------- | -------- |
|`get_scalers(file_handle)` |`int MUD_getScalers( int fh, unsigned int* pType, unsigned int* pNum )` |
|`get_scaler_label(file_handle,scalar_id_number)` |`int MUD_getScalerLabel( int fh, int num, char* label, int strdim )` |
|`get_scaler_counts(file_handle,scalar_id_number)` |`int MUD_getScalerCounts( int fh, int num, unsigned int* pCounts )` |

Independent Variables

|Cython Function (python-accessible) |C Function (wrapped) |
| -------- | -------- |
|`get_ivars(file_handle)` |`int MUD_getIndVars(int fh, unsigned int* pType, unsigned int* number_of_variables)` |
|`get_ivar_low(file_handle,variable_id_number)` |`int MUD_getIndVarLow( int fh, int num, double* pLow )` |
|`get_ivar_high(file_handle,variable_id_number)` |`int MUD_getIndVarHigh( int fh, int num, double* pHigh )` |
|`get_ivar_mean(file_handle,variable_id_number)` |`int MUD_getIndVarMean( int fh, int num, double* pMean )` |
|`get_ivar_std(file_handle,variable_id_number)` |`int MUD_getIndVarStddev( int fh, int num, double* pStddev )` |
|`get_ivar_skewness(file_handle,variable_id_number)` |`int MUD_getIndVarSkewness( int fh, int num, double* pSkewness )` |
|`get_ivar_name(file_handle,variable_id_number)` |`int MUD_getIndVarName( int fh, int num, char* name, int strdim )` |
|`get_ivar_description(file_handle,variable_id_number)` |`int MUD_getIndVarDescription( int fh, int num, char* description,int strdim)` |
|`get_ivar_units(file_handle,variable_id_number)` |`int MUD_getIndVarUnits( int fh, int num, char* units, int strdim )` |
