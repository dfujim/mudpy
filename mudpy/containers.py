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
            m = max(map(len, self.__slots__)) + 1
            s = ''
            s += '\n'.join([k.rjust(m) + ': ' + repr(getattr(self, k))
                              for k in sorted(self.__slots__)])
            return s
        else:
            return self.__class__.__name__ + "()"

    # arithmatic operators
    def __abs__(self):              return np.abs(self._get_val())
    def __add__(self, other):       return self._get_val()+self._get_oval(other)
    def __divmod__(self, other):    return np.divmod(self._get_val(), self._get_oval(other))
    def __floordiv__(self, other):  return self._get_val()//self._get_oval(other)
    def __invert__(self):           return ~self._get_val()
    def __lshift__(self, other):    return self._get_val()<<self._get_oval(other)
    def __mod__(self, other):       return self._get_val()%self._get_oval(other)
    def __mul__(self, other):       return self._get_val()*self._get_oval(other)
    def __neg__(self):              return -self._get_val()
    def __pos__(self):              return +self._get_val()
    def __pow__(self, other):       return np.pow(self._get_val(), self._get_oval(other))
    def __round__(self):            return np.round(self._get_val())
    def __rshift__(self, other):    return self._get_val()>>self._get_oval(other)
    def __sub__(self, other):       return self._get_val()-self._get_oval(other)
    def __truediv__(self, other):   return self._get_val()/self._get_oval(other)

    # casting operators
    def __complex__(self):          return complex(self._get_val())
    def __int__(self):              return int(self._get_val())
    def __float__(self):            return float(self._get_val())
    def __str__(self):              return str(self._get_val())

    # logic operators
    def __and__(self, other):       return self._get_val()&self._get_oval(other)
    def __eq__(self, other):        return self._get_val()==self._get_oval(other)
    def __ge__(self, other):        return self._get_val()>=self._get_oval(other)
    def __gt__(self, other):        return self._get_val()>self._get_oval(other)
    def __le__(self, other):        return self._get_val()<=self._get_oval(other)
    def __lt__(self, other):        return self._get_val()<self._get_oval(other)
    def __ne__(self, other):        return self._get_val()!=self._get_oval(other)
    def __or__(self, other):        return self._get_val()|self._get_oval(other)
    def __xor__(self, other):       return self._get_val()^self._get_oval(other)

    # reflected operators
    def __radd__(self, other):      return self.__add__(self._get_oval(other))
    def __rand__(self, other):      return self.__and__(self._get_oval(other))
    def __rdivmod__(self, other):   return self.__divmod__(self._get_oval(other))
    def __rfloordiv__(self, other): return self.__floordiv__(self._get_oval(other))
    def __rlshift__(self, other):   return self.__lshift__(self._get_oval(other))
    def __rmod__(self, other):      return self.__mod__(self._get_oval(other))
    def __rmul__(self, other):      return self.__mul__(self._get_oval(other))
    def __ror__(self, other):       return self.__or__(self._get_oval(other))
    def __rpow__(self, other):      return self.__pow__(self._get_oval(other))
    def __rrshift__(self, other):   return self.__rshift__(self._get_oval(other))
    def __rsub__(self, other):      return self.__sub__(self._get_oval(other))
    def __rtruediv__(self, other):  return self.__truediv__(self._get_oval(other))
    def __rxor__(self, other):      return self.__xor__(self._get_oval(other))

    # assignemt operators
    def __iadd__(self, other):      self.mean += self._get_oval(other); return self
    def __iand__(self, other):      self.mean.__iand__(self._get_oval(other)); return self
    def __ifloordiv__(self, other): self.mean //= self._get_oval(other); return self
    def __ilshift__(self, other):   self.mean.__ilshift__(self._get_oval(other)); return self
    def __imod__(self, other):      self.mean %= self._get_oval(other); return self
    def __imul__(self, other):      self.mean *= self._get_oval(other); return self
    def __ior__(self, other):       self.mean.__ior__(self._get_oval(other)); return self
    def __ipow__(self, other):      self.mean **= self._get_oval(other); return self
    def __irshift__(self, other):   self.mean.__irshift__(self._get_oval(other)); return self
    def __isub__(self, other):      self.mean -= self._get_oval(other); return self
    def __itruediv__(self, other):  self.mean /= self._get_oval(other); return self
    def __ixor__(self, other):      self.mean.__ixor__(self._get_oval(other)); return self

    # get the value of the other
    def _get_oval(self, other):
        if hasattr(other, '_get_val'):
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
            name = name.replace('n', '-').replace('p', '+')
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
    __slots__ = ('id_number', 'author', 'body', 'title', 'time')

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

    def _get_val(self):                 return self.data

    # list operators
    def __contains__(self, val):        return val in self.data
    def __iter__(self):                 return self.data.__iter__()
    def __getitem__(self, i):           return self.data[i]
    def __len__(self):                  return len(self.data)
    def __setitem__(self, i, val):      self.data[i] = val

    # math operators
    def __matmul__(self, *args, **kwargs):  return self.data.__matmul__(*args, **kwargs)
    def __rmatmul__(self, *args, **kwargs): return self.data.__rmatmul__(*args, **kwargs)

    # assignemt operators
    def __iadd__(self, other):      self.data += self._get_oval(other); return self
    def __iand__(self, other):      self.data.__iand__(self._get_oval(other)); return self
    def __ifloordiv__(self, other): self.data //= self._get_oval(other); return self
    def __ilshift__(self, other):   self.data.__ilshift__(self._get_oval(other)); return self
    def __imatmul__(self, other):   self.data.__imatmul__(self._get_oval(other)); return self
    def __imod__(self, other):      self.data %= self._get_oval(other); return self
    def __imul__(self, other):      self.data *= self._get_oval(other); return self
    def __ior__(self, other):       self.data.__ior__(self._get_oval(other)); return self
    def __ipow__(self, other):      self.data **= self._get_oval(other); return self
    def __irshift__(self, other):   self.data.__irshift__(self._get_oval(other)); return self
    def __isub__(self, other):      self.data -= self._get_oval(other); return self
    def __itruediv__(self, other):  self.data /= self._get_oval(other); return self
    def __ixor__(self, other):      self.data.__ixor__(self._get_oval(other)); return self

    # copied from np.ndarray
    def all(self, *args, **kwargs):             return self.data.all(*args, **kwargs)
    def any(self, *args, **kwargs):             return self.data.any(*args, **kwargs)
    def argmax(self, *args, **kwargs):          return self.data.argmax(*args, **kwargs)
    def argmin(self, *args, **kwargs):          return self.data.argmin(*args, **kwargs)
    def argpartition(self, *args, **kwargs):    return self.data.argpartition(*args, **kwargs)
    def argsort(self, *args, **kwargs):         return self.data.argsort(*args, **kwargs)
    def astype(self, *args, **kwargs):          return self.data.astype(*args, **kwargs)
    def base(self):                             return self.data.base
    def byteswap(self, *args, **kwargs):        return self.data.byteswap(*args, **kwargs)
    def choose(self, *args, **kwargs):          return self.data.choose(*args, **kwargs)
    def clip(self, *args, **kwargs):            return self.data.clip(*args, **kwargs)
    def compress(self, *args, **kwargs):        return self.data.compress(*args, **kwargs)
    def conj(self, *args, **kwargs):            return self.data.conj(*args, **kwargs)
    def conjugate(self, *args, **kwargs):       return self.data.conjugate(*args, **kwargs)
    def copy(self, *args, **kwargs):            return self.data.copy(*args, **kwargs)
    def ctypes(self):                           return self.data.ctypes
    def cumprod(self, *args, **kwargs):         return self.data.cumprod(*args, **kwargs)
    def cumsum(self, *args, **kwargs):          return self.data.cumsum(*args, **kwargs)
    def diagonal(self, *args, **kwargs):        return self.data.diagonal(*args, **kwargs)
    def dot(self, *args, **kwargs):             return self.data.dot(*args, **kwargs)
    def dtype(self):                            return self.data.dtype
    def dump(self, *args, **kwargs):            return self.data.dump(*args, **kwargs)
    def dumps(self, *args, **kwargs):           return self.data.dumps(*args, **kwargs)
    def fill(self, *args, **kwargs):            return self.data.fill(*args, **kwargs)
    def flags(self):                            return self.data.flags
    def flat(self):                             return self.data.flat
    def flatten(self, *args, **kwargs):         return self.data.flatten(*args, **kwargs)
    def getfield(self, *args, **kwargs):        return self.data.getfield(*args, **kwargs)
    def imag(self):                             return self.data.imag
    def item(self, *args, **kwargs):            return self.data.item(*args, **kwargs)
    def itemset(self, *args, **kwargs):         return self.data.itemset(*args, **kwargs)
    def itemsize(self):                         return self.data.itemsize
    def max(self, *args, **kwargs):             return self.data.max(*args, **kwargs)
    def mean(self, *args, **kwargs):            return self.data.mean(*args, **kwargs)
    def min(self, *args, **kwargs):             return self.data.min(*args, **kwargs)
    def nbytes(self):                           return self.data.nbytes
    def ndim(selfs):                            return self.data.ndim
    def newbyteorder(self, *args, **kwargs):    return self.data.newbyteorder(*args, **kwargs)
    def nonzero(self, *args, **kwargs):         return self.data.nonzero(*args, **kwargs)
    def partition(self, *args, **kwargs):       return self.data.partition(*args, **kwargs)
    def prod(self, *args, **kwargs):            return self.data.prod(*args, **kwargs)
    def ptp(self, *args, **kwargs):             return self.data.ptp(*args, **kwargs)
    def put(self, *args, **kwargs):             return self.data.put(*args, **kwargs)
    def ravel(self, *args, **kwargs):           return self.data.ravel(*args, **kwargs)
    def real(self):                             return self.data.real
    def repeat(self, *args, **kwargs):          return self.data.repeat(*args, **kwargs)
    def reshape(self, *args, **kwargs):         return self.data.reshape(*args, **kwargs)
    def resize(self, *args, **kwargs):          return self.data.resize(*args, **kwargs)
    def round(self, *args, **kwargs):           return self.data.round(*args, **kwargs)
    def searchsorted(self, *args, **kwargs):    return self.data.searchsorted(*args, **kwargs)
    def setfield(self, *args, **kwargs):        return self.data.setfield(*args, **kwargs)
    def setflags(self, *args, **kwargs):        return self.data.setflags(*args, **kwargs)
    def shape(self):                            return self.data.shape
    def size(self):                             return self.data.size
    def sort(self, *args, **kwargs):            return self.data.sort(*args, **kwargs)
    def squeeze(self, *args, **kwargs):         return self.data.squeeze(*args, **kwargs)
    def std(self, *args, **kwargs):             return self.data.std(*args, **kwargs)
    def strides(self):                          return self.data.strides
    def sum(self, *args, **kwargs):             return self.data.sum(*args, **kwargs)
    def swapaxes(self, *args, **kwargs):        return self.data.swapaxes(*args, **kwargs)
    def take(self, *args, **kwargs):            return self.data.take(*args, **kwargs)
    def tobytes(self, *args, **kwargs):         return self.data.tobytes(*args, **kwargs)
    def tofile(self, *args, **kwargs):          return self.data.tofile(*args, **kwargs)
    def tolist(self, *args, **kwargs):          return self.data.tolist(*args, **kwargs)
    def tostring(self, *args, **kwargs):        return self.data.tostring(*args, **kwargs)
    def trace(self, *args, **kwargs):           return self.data.trace(*args, **kwargs)
    def transpose(self, *args, **kwargs):       return self.data.transpose(*args, **kwargs)
    def var(self, *args, **kwargs):             return self.data.var(*args, **kwargs)
    def view(self, *args, **kwargs):            return self.data.view(*args, **kwargs)

# =========================================================================== #
class mscaler(mcontainer):
    """
        Scaler associated with mdata object.

        Data fields:
            id_number
            title
            counts_total_recent
    """
    __slots__ = ('id_number', 'title', 'counts_total_recent')

    def _get_val(self): return self.counts_total_recent[0]

# ========================================================================== #
class mlist(list):
    """
        List object from which attributes/keys are accessed from each element,
        then returned as a list
    """

    # ======================================================================= #
    def __getattr__(self, name):
        """
            Get attribute of underlying data as a list.
        """

        # fetch from top level
        try:
            return getattr(object, name)

        # fetch from lower levels
        except AttributeError:
            out = mlist([getattr(d, name) for d in self])

            # if base level, return as array
            if type(out[0]) in (float, int):
                return np.array(out)
            else:
                return out

    # ======================================================================= #
    def __getitem__(self, name):
        """
            Get attribute of underlying data as a list.
        """
        # fetch from top level
        try:
            return list.__getitem__(self, name)

        # fetch from lower levels
        except TypeError:
            out = mlist([d[name] for d in self])

            # if base level, return as array
            if type(out[0]) in (float, int):
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
