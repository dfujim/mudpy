import numpy as np

# =========================================================================== #
# DATA CONTAINERS
# =========================================================================== #
class mcontainer(object):
    """
        Provides common functions for data containers
        
        _get_val(): return the value needed to do the various operators. 
                    Define in child classes
    """

    def __repr__(self):
        if list(self.__slots__):
            m = max(map(len,self.__slots__)) + 1
            s = ''
            s += '\n'.join([k.rjust(m) + ': ' + repr(getattr(self,k))
                              for k in sorted(self.__slots__)])
            return s
        else:
            return self.__class__.__name__ + "()"

    # arithmatic operators
    def __add__(self,other):        return self._get_val()+self._get_oval(other)
    def __sub__(self,other):        return self._get_val()-self._get_oval(other)
    def __mul__(self,other):        return self._get_val()*self._get_oval(other)
    def __div__(self,other):        return self._get_val()/self._get_oval(other)
    def __floordiv__(self,other):   return self._get_val()//self._get_oval(other)
    def __mod__(self,other):        return self._get_val()%self._get_oval(other)
    def __divmod__(self,other):     return divmod(self._get_val(),self._get_oval(other))
    def __pow__(self,other):        return pow(self._get_val(),self._get_oval(other))
    def __lshift__(self,other):     return self._get_val()<<self._get_oval(other)
    def __rshift__(self,other):     return self._get_val()>>self._get_oval(other)
    def __neg__(self):              return -self._get_val()
    def __pos__(self):              return +self._get_val()
    def __abs__(self):              return abs(self._get_val())
    def __invert__(self):           return ~self._get_val()
    def __round__(self):            return round(self._get_val())
    
    # casting operators
    def __complex__(self):          return complex(self._get_val())
    def __int__(self):              return int(self._get_val())
    def __float__(self):            return float(self._get_val())
    def __str__(self):              return str(self._get_val())
    
    # logic operators
    def __eq__(self,other):         return self._get_val()==self._get_oval(other)
    def __lt__(self,other):         return self._get_val()<self._get_oval(other)
        
    def __le__(self,other):         return self._get_val()<=self._get_oval(other)
    def __gt__(self,other):         return self._get_val()>self._get_oval(other)
        
    def __ge__(self,other):         return self._get_val()>=self._get_oval(other)
    
    def __and__(self,other):        return self._get_val()&self._get_oval(other)
    def __xor__(self,other):        return self._get_val()^self._get_oval(other)
    def __or__(self,other):         return self._get_val()|self._get_oval(other)
    
    # reflected operators
    def __radd__(self,other):       return self.__add__(self._get_oval(other))        
    def __rsub__(self,other):       return self.__sub__(self._get_oval(other))        
    def __rmul__(self,other):       return self.__mul__(self._get_oval(other))     
    def __rdiv__(self,other):       return self.__div__(self._get_oval(other))    
    def __rfloordiv__(self,other):  return self.__floordiv__(self._get_oval(other))
    def __rmod__(self,other):       return self.__mod__(self._get_oval(other))      
    def __rdivmod__(self,other):    return self.__divmod__(self._get_oval(other))
    def __rpow__(self,other):       return self.__pow__(self._get_oval(other))      
    def __rlshift__(self,other):    return self.__lshift__(self._get_oval(other))
    def __rrshift__(self,other):    return self.__rshift__(self._get_oval(other))                          
    def __rand__(self,other):       return self.__and__(self._get_oval(other))
    def __rxor__(self,other):       return self.__xor__(self._get_oval(other))
    def __ror__(self,other):        return self.__or__(self._get_oval(other))    

    # get the value of the other
    def _get_oval(self,other):
        if hasattr(other,'_get_val'):
            return other._get_val()
        else:
            return other

# =========================================================================== #
class mdict(dict):
    """
        Provides common functions for dictionaries of data containers
    """

    def __getattr__(self, name):
        try:
            return self[name]
        except KeyError as err:
            name = name.replace('n','-').replace('p','+')
            try:
                return self[name]
            except KeyError:
                raise AttributeError(err) from None

    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__
    
    def __repr__(self):
        klist = list(self.keys())
        if klist:
            klist.sort()
            s = self.__class__.__name__+': {'
            for k in self.keys():
                s+="'"+k+"'"+', '
            s = s[:-2]
            s+='}'
            return s
        else:
            return self.__class__.__name__ + "()"
    
    def __dir__(self):
        return list(self.keys())

# =========================================================================== #
class mcomment(mcontainer):
    """
        Comment with mdata object.
        
        Data fields:
            id_number
            author
            body
            title
            time
    """
    __slots__ = ('id_number','author', 'body', 'title', 'time')
    
# =========================================================================== #
class mhist(mcontainer):
    """
        Histogram associated with mdata object.
        
        Data fields:
            id_number
            htype 
            title
            data
            n_bytes
            n_bins
            n_events
            fs_per_bin
            s_per_bin
            t0_ps
            t0_bin
            good_bin1
            good_bin2
            background1
            background2
    """
    
    __slots__ = ('id_number', 'htype', 'title', 'data', 'n_bytes', 'n_bins', 
                 'n_events', 'fs_per_bin', 's_per_bin', 't0_ps', 't0_bin', 
                 'good_bin1', 'good_bin2', 'background1', 'background2')
    
    def _get_val(self):     return self.data
    def astype(self,type):  return self.data.astype(type)
    def __len__(self):      return len(self.data)
    
# =========================================================================== #
class mscaler(mcontainer):
    """
        Scaler associated with mdata object.
        
        Data fields:
            id_number
            title
            counts_total_recent
    """
    __slots__ = ('id_number','title','counts_total_recent')
    
    def _get_val(self): return self.counts_total_recent[0]
        
# ========================================================================== #
class mlist(list):
    """
        List object from which attributes/keys are accessed from each element, 
        then returned as a list
    """
    
    # ======================================================================= #
    def __getattr__(self,name):
        """
            Get attribute of underlying data as a list.
        """
        
        # fetch from top level
        try:
            return getattr(object,name)

        # fetch from lower levels
        except AttributeError:
            out = mlist([getattr(d,name) for d in self])
            
            # if base level, return as array
            if type(out[0]) in (float,int):
                return np.array(out)
            else:
                return out
    
    # ======================================================================= #
    def __getitem__(self,name):
        """
            Get attribute of underlying data as a list.
        """
        # fetch from top level
        try:
            return list.__getitem__(self,name)
        
        # fetch from lower levels
        except TypeError:
            out = mlist([d[name] for d in self])

            # if base level, return as array
            if type(out[0]) in (float,int):
                return np.array(out)
            else:
                return out

# =========================================================================== #
class mvar(mcontainer):
    """
        Independent variable associated with bdata object.
        
        Data fields:
            id_number
            low
            high
            mean
            std
            skew
            title
            description
            units
    """
        
    __slots__ = ('id_number', 'low', 'high', 'mean', 'std', 'skew', 'title', 
                 'description', 'units')

    def _get_val(self): return self.mean
            
