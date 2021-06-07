# Python base class for reading MUD files. 
# Derek Fujimoto
# Nov 2019

import mudpy.mud_friendly as mud
from mudpy.containers import mcomment, mlist, mhist, mdict, mscaler, mvar
import numpy as np
import time, os

__doc__="""
    mud-data module. The mdata object is a data container, designed to read out 
    the MUD data files and to provide user-friendly access to MUD data. The MUD 
    data file is read and closed on object construction. 
    
    Signature: mdata(filename)
    
    Features -----------------------------------------------------------------
    
        Representation 
    
            Representation has been nicely formatted so that typing the object 
            name into the interpreter produces nice output. 
        
        Operators
            
            mvar, mscaler, and mhist objects allow for arithmatic or logic 
            operators to be used, where the value used in the operation is the 
            mean, count, or data array respectively. 
            
        Special Rules For Attributes
        
            If an attribute is not found in mdata, it will look for the 
            attribute in the mdict objects in the order: hist, ivar, sclr
            This second-level attribute search is much slower than regular 
            access.
            
            mdict objects all allow assignment and fetching of dictionary keys 
            as if they were attributes. Note that one can replace "+" with "p", 
            and "-" with "m" to allow fetching of histograms. 
        
    Derek Fujimoto
    Nov 2019
    """

# =========================================================================== #
class mdata(object):
    """
        Data fields
            apparatus       str, name of spectrometer
            area            str, name of area data was taken
            das             str, data aquisition software
            description     str, run description
            duration        int, length of run
            end_date        str, end of run human-readable string
            end_time        int, end of run epoch time 
            exp             int, experiment number
            experimenter    str, experimenter names
            hist            mhist, histograms
            ivar            mhist, independent variables
            lab             str, facility name
            method          str, name of data collection method
            mode            str, run mode (insert)
            orientation     str, sample orientation
            run             int, run number
            sample          str, sample name
            start_date      str, start of run human-readable string 
            start_time      int, start of run epoch time 
            title           str, run title
            year            int, year at start of run
            
        Private worker functions
            __init__
            __getattr__
            __repr__
    """
    
    # link object attributes with mud_friendly function (attribute: mud.attr)
    # must have a title attribute
    # prefixed with "get_" or "set_"
    
    description_attribute_functions = { 
                            'description':  'description', 
                            'exp':          'exp_number', 
                            'run':          'run_number', 
                            'duration':     'elapsed_seconds', 
                            'start_time':   'start_time', 
                            'end_time':     'end_time', 
                            'title':        'title', 
                            'lab':          'lab', 
                            'area':         'area', 
                            'method':       'method', 
                            'apparatus':    'apparatus', 
                            'mode':         'insert', 
                            'sample':       'sample', 
                            'orientation':  'orientation', 
                            'das':          'das', 
                            'experimenter': 'experimenter', 
                            'temperature':  'temperature', 
                            'field':        'field', 
        }
    
    histogram_attribute_functions = {
                            'htype':        'hist_type', 
                            'title':        'hist_title', 
                            'n_bytes':      'hist_n_bytes', 
                            'n_bins':       'hist_n_bins', 
                            'n_events':     'hist_n_events', 
                            'fs_per_bin':   'hist_fs_per_bin', 
                            's_per_bin':    'hist_sec_per_bin', 
                            't0_ps':        'hist_t0_ps', 
                            't0_bin':       'hist_t0_bin', 
                            'good_bin1':    'hist_good_bin1', 
                            'good_bin2':    'hist_good_bin2', 
                            'background1':  'hist_background1', 
                            'background2':  'hist_background2', 
                            'data':         'hist_data', 
                            }
    
    scaler_attribute_functions = {
                            'counts_total_recent':  'scaler_counts', 
                            'title':                'scaler_label', 
                            }   
    
    variable_attribute_functions = {
                            'low':          'ivar_low', 
                            'high':         'ivar_high', 
                            'mean':         'ivar_mean', 
                            'std':          'ivar_std', 
                            'skew':         'ivar_skewness', 
                            'title':        'ivar_name', 
                            'description':  'ivar_description', 
                            'units':        'ivar_units', 
                            }
    
    comment_attribute_functions = {
                            'title':        'comment_title', 
                            'time':         'comment_time', 
                            'author':       'comment_author', 
                            'body':         'comment_body', 
                            }
                
    # default non-mdict attributes for writing if empty construction 
    default_attributes = ('apparatus', 'area', 'das', 'description', 'duration', 
                          'end_time', 'exp', 'experimenter', 'lab', 'method', 'mode', 
                          'orientation', 'run', 'sample', 'start_time', 'title', 
                          'year')
    
    # conversion from string method to int method 
    mud_method = {'TD':{'file':     mud.FMT_TRI_TD_ID, 
                        'descr':    mud.SEC_GEN_RUN_DESC_ID, 
                        'hist':     mud.GRP_TRI_TD_HIST_ID, 
                        'sclr':     mud.GRP_TRI_TD_SCALER_ID, 
                        'ivar':     mud.GRP_GEN_IND_VAR_ID, 
                        'comments': mud.GRP_CMT_ID
                       }, 
                  
                  'TI':{'file':     mud.FMT_TRI_TI_ID, 
                        'descr':    mud.SEC_TRI_TI_RUN_DESC_ID, 
                        'hist':     mud.GRP_TRI_TI_HIST_ID, 
                        'sclr':     mud.GRP_GEN_SCALER_ID, 
                        'ivar':     mud.GRP_GEN_IND_VAR_ARR_ID, 
                        'comments': mud.GRP_CMT_ID
                       }, 
                  }
    
    # ======================================================================= #    
    def __init__(self, filename=''):
        """
            Constructor. Reads file or sets file up for writing.
            
            filename: string, path to file to read. If blank, make empty object.
        """
        
        # read
        if filename:
            self._read_file(filename)
        
        # set up for writing
        else:
            for attr in self.default_attributes:
                setattr(self, attr, None)
            for attr in ('hist', 'sclr', 'ivar'):
                setattr(self, attr, mdict())
        
    # ======================================================================= #
    def __getattr__(self, name):
        
        if name in ('hist', 'ivar', 'sclr'):
            return getattr(object, name)
        
        else:
            try:
                # fetch from top level
                return getattr(object, name)
            except AttributeError as err:
                
                # fetching of second level
                if hasattr(self, 'hist') and hasattr(self.hist, name):
                    return getattr(self.hist, name)
                if hasattr(self, 'ivar') and hasattr(self.ivar, name): 
                    return getattr(self.ivar, name)
                if hasattr(self, 'sclr') and hasattr(self.sclr, name): 
                    return getattr(self.sclr, name)
                        
                # nothing worked - raise error
                raise AttributeError(err) from None
                        
    # ======================================================================= #
    def __repr__(self):
        """
            Nice printing of parameters.
        """
        
        d = self.__dict__
        dkeys = list(d.keys())
        if dkeys:
            items = []
            dkeys.sort()
            for key in dkeys:
                if key[0] == '_': continue
                
                # non iterables and mdict objects
                if not hasattr(d[key], '__iter__') or d[key].__class__ == mdict:
                    items.append([key, d[key]])                
                
                # strings
                elif d[key].__class__ == str:
                    items.append([key, d[key]])                
                
                # misc objects
                else:
                    items.append([key, d[key].__class__])
                
                            
            m = max(map(len, dkeys)) + 1
            s = '\n'.join([k.rjust(m)+': '+repr(v) for k, v in sorted(items)])
            return s
        else:
            return self.__class__.__name__ + "()"
    
    # ======================================================================= #
    def _read_file(self, filename):
        """
            Read file into memory.
        """
        
        # Open file ----------------------------------------------------------
        try:
            fh = mud.open_read(filename)
        except RuntimeError:
            raise RuntimeError("Open file %s failed. " % filename) from None
       
        try:
            # Read run description
            for attr, func_name in self.description_attribute_functions.items():
                try:
                    func = getattr(mud, "get_"+func_name)
                    setattr(self, attr, func(fh))
                except RuntimeError:
                    pass
            
            # Read histograms
            self._read_mdict(fh=fh, 
                             get_n=mud.get_hists, 
                             attr_dict=self.histogram_attribute_functions, 
                             attr_name='hist', 
                             obj_class=mhist
                             )
            
            # Read scalers
            self._read_mdict(fh=fh, 
                             get_n=mud.get_scalers, 
                             attr_dict=self.scaler_attribute_functions, 
                             attr_name='sclr', 
                             obj_class=mscaler
                             )
            
            # Read independent variables
            self._read_mdict(fh=fh, 
                             get_n=mud.get_ivars, 
                             attr_dict=self.variable_attribute_functions, 
                             attr_name='ivar', 
                             obj_class=mvar
                             )
           
            # Read comments
            self._read_mdict(fh=fh, 
                             get_n=mud.get_comments, 
                             attr_dict=self.comment_attribute_functions, 
                             attr_name='comments', 
                             obj_class=mcomment
                             )
            
        # Close file ----------------------------------------------------------
        finally:
            mud.close_read(fh)
        
        # set the date
        try:
            self.start_date = time.ctime(self.start_time)
            self.year = time.gmtime(self.start_time).tm_year
        except AttributeError:
            pass
            
        try:
            self.end_date = time.ctime(self.end_time)
        except AttributeError:
            pass      
        
    # ======================================================================= #
    def _read_mdict(self, fh, get_n, attr_dict, attr_name, obj_class):
        """
            Read all of a type of objects from MUD file and set its attributes, 
            place in mdict. 
            
            fh:         file header
            get_n:      mudpy function which gets the number of objects in the file
            attr_dict:  dictionary which links attribute name and mudpy function
            attr_name:  main attribute name. Ex: hist, scaler, or ivar
            obj_class:  object class to make
        """
        try:
            n = get_n(fh)[1]
        except RuntimeError:
            pass
        else:
            setattr(self, attr_name, mdict())
            for i in range(1, n+1):
                
                obj = obj_class()
                obj.id_number = i
                
                for attr, func_name in attr_dict.items():
                    func = getattr(mud, 'get_'+func_name)
                    try:
                        setattr(obj, attr, func(fh, i))
                    except RuntimeError:
                        pass
                getattr(self, attr_name)[obj.title] = obj
        
    # ======================================================================= #   
    def _write_mdict(self, fh, set_n, attr_dict, attr_name, typeid):
        """
            Read all of a type of objects from MUD file and set its attributes, 
            place in mdict. 
            
            fh:         file header
            set_n:      mudpy function which sets the number of objects in the file
            attr_dict:  dictionary which links attribute name and mudpy function
            attr_name:  main attribute name. Ex: hist, scaler, or ivar
            typeid:     int value specifying the type of data to write
        """
        
        # get mdict object 
        try:
            obj_dict = getattr(self, attr_name)
        except AttributeError:
            return 
            
        # get keys, length
        keys = sorted(obj_dict.keys())
        n = len(keys)
        
        # set type and number of elements
        set_n(fh, typeid, n)
        
        # get set functions
        set_fn = {k:getattr(mud, 'set_'+link) for k, link in attr_dict.items()}
        
        # set each item
        for i, key in enumerate(keys):
                
            # get the object
            obj = obj_dict[key]
            
            # set initial
            if attr_name == 'hist':
                set_fn['htype'](fh, i+1, getattr(obj, 'htype'))
            
            # iterate over attributes
            for k in attr_dict.keys():
                
                if k in ('n_bytes', 'htype', 's_per_bin'):
                    continue
                
                try:
                    set_fn[k](fh, i+1, getattr(obj, k))
                except AttributeError:
                    pass
    
    # ======================================================================= #   
    def set_description(self, mode):
        """
            Set file type. 
            
            mode is one of "TD" or "TI"
        """
        
        try:
            self.description = self.mud_method[mode]['descr']
        except KeyError:
            raise RuntimeError('Mode must be one of "TD" or "TI"')
        
    # ======================================================================= #   
    def write(self, filename):
        """
            Write object to MUD file. 
        """
        
        # check that all needed attributes are set
        for attr in self.default_attributes:
            if hasattr(self, attr):
                at = getattr(self, attr)
                if at is None:
                    del at
    
        # get file type
        try:
            if self.description == self.mud_method['TD']['descr']:
                method = self.mud_method['TD']
            elif self.description == self.mud_method['TI']['descr']:
                method = self.mud_method['TI']
            else:
                raise RuntimeError
        except RuntimeError:
            raise RuntimeError('Set description attribute with '+\
                                '"set_description(mode)", where mode is one '+\
                                'of "TD" or "TI"') from None
    
        # get file header
        fh = mud.open_write(filename, method['file'])

        # Write file body -----------------------------------------------------
        try:
            # write the run description 
            mud.set_description(fh, method['descr'])
            
            for attr, func_name in self.description_attribute_functions.items():
                
                # get the attribute
                if hasattr(self, attr):
                    val = getattr(self, attr)
                    
                    # write 
                    if val is not None:
                        func = getattr(mud, "set_"+func_name)
                        func(fh, val)
                
            # histograms
            self._write_mdict(fh=fh, 
                             set_n=mud.set_hists, 
                             attr_dict=self.histogram_attribute_functions, 
                             attr_name='hist', 
                             typeid=method['hist']
                             )
            
            # scalers
            self._write_mdict(fh=fh, 
                             set_n=mud.set_scalers, 
                             attr_dict=self.scaler_attribute_functions, 
                             attr_name='sclr', 
                             typeid=method['sclr']
                             )
            
            # independent variables
            self._write_mdict(fh=fh, 
                             set_n=mud.set_ivars, 
                             attr_dict=self.variable_attribute_functions, 
                             attr_name='ivar', 
                             typeid=method['ivar']
                             )
           
            # comments
            self._write_mdict(fh=fh, 
                             set_n=mud.set_comments, 
                             attr_dict=self.comment_attribute_functions, 
                             attr_name='comments', 
                             typeid=method['comments']
                             )
            
        # Close file with no write --------------------------------------------
        except Exception as err:
            mud.close_read(fh)
            raise(err)
            
        # Close file and write ------------------------------------------------
        else:
            mud.close_write(fh)

