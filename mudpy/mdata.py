# Python base class for reading MUD files. 
# Derek Fujimoto
# Nov 2019

import mudpy.mud_friendly as mud
from mudpy.containers import mcomment, mlist, mhist, mdict, mscaler, mvar
import numpy as np
import time

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
    
    # link object attributes with mud_friendly function (attribute:function)
    # must have a title attribute
    description_attribute_functions = { 
                            'description':mud.get_description,
                            'exp':mud.get_exp_number,
                            'run':mud.get_run_number,
                            'duration':mud.get_elapsed_seconds,
                            'start_time':mud.get_start_time,
                            'end_time':mud.get_end_time,
                            'title':mud.get_title,
                            'lab':mud.get_lab,
                            'area':mud.get_area,
                            'method':mud.get_method,
                            'apparatus':mud.get_apparatus,
                            'mode':mud.get_insert,
                            'sample':mud.get_sample,
                            'orientation':mud.get_orientation,
                            'das':mud.get_das,
                            'experimenter':mud.get_experimenter,
                            'temperature':mud.get_temperature,
                            'field':mud.get_field,
                            }
    
    histogram_attribute_functions = {
                            'title':mud.get_hist_title,
                            'htype':mud.get_hist_type,
                            'data':mud.get_hist_data,
                            'n_bytes':mud.get_hist_n_bytes,
                            'n_bins':mud.get_hist_n_bins,
                            'n_events':mud.get_hist_n_events,
                            'fs_per_bin':mud.get_hist_fs_per_bin,
                            's_per_bin':mud.get_hist_sec_per_bin,
                            't0_ps':mud.get_hist_t0_ps,
                            't0_bin':mud.get_hist_t0_bin,
                            'good_bin1':mud.get_hist_good_bin1,
                            'good_bin2':mud.get_hist_good_bin2,
                            'background1':mud.get_hist_background1,
                            'background2':mud.get_hist_background2,
                            }
    
    scaler_attribute_functions = {
                            'counts':mud.get_scaler_counts,
                            'title':mud.get_scaler_label,
                            }   
    
    variable_attribute_functions = {
                            'low':mud.get_ivar_low,
                            'high':mud.get_ivar_high,
                            'mean':mud.get_ivar_mean,
                            'std':mud.get_ivar_std,
                            'skew':mud.get_ivar_skewness,
                            'title':mud.get_ivar_name,
                            'description':mud.get_ivar_description,
                            'units':mud.get_ivar_units,
                            }
    
    comment_attribute_functions = {
                            'title':mud.get_comment_title,
                            'time':mud.get_comment_time,
                            'author':mud.get_comment_author,
                            'body':mud.get_comment_body,
                            }
                
    # ======================================================================= #    
    def __init__(self,filename):
        """Constructor. Reads file."""
        
        # Open file ----------------------------------------------------------
        try:
            fh = mud.open_read(filename)
        except RuntimeError:
            raise RuntimeError("Open file %s failed. " % filename) from None
        try:
            # Read run description
            for attr,func in self.description_attribute_functions.items():
                try:
                    setattr(self,attr,func(fh))
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
    def __getattr__(self,name):
        
        try:
            # fetch from top level
            return getattr(object,name)
        except AttributeError as err:
            
            # fetching of second level
            if hasattr(self,'hist') and hasattr(self.hist,name):
                return getattr(self.hist,name)
            if hasattr(self,'ivar') and hasattr(self.ivar,name): 
                return getattr(self.ivar,name)
            if hasattr(self,'sclr') and hasattr(self.sclr,name): 
                return getattr(self.sclr,name)
                    
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
                if not hasattr(d[key],'__iter__') or d[key].__class__ == mdict:
                    items.append([key,d[key]])                
                
                # strings
                elif d[key].__class__ == str:
                    items.append([key,d[key]])                
                
                # misc objects
                else:
                    items.append([key,d[key].__class__])
                
                            
            m = max(map(len,dkeys)) + 1
            s = '\n'.join([k.rjust(m)+': '+repr(v) for k, v in sorted(items)])
            return s
        else:
            return self.__class__.__name__ + "()"
    
    # ======================================================================= #
    def _read_mdict(self,fh,get_n,attr_dict,attr_name,obj_class):
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
            setattr(self,attr_name,mdict())
            for i in range(1,n+1):
                
                obj = obj_class()
                obj.id_number = i
                
                for attr,func in attr_dict.items():
                    try:
                        setattr(obj,attr,func(fh,i))
                    except RuntimeError:
                        pass
                getattr(self,attr_name)[obj.title] = obj
        
