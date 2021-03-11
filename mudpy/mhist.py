from .mcontainer import mcontainer
import numpy as np

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

    def __init__(self, 
                id_number   = -1, 
                htype       = -1, 
                title       = '', 
                data        = None, 
                n_bytes     = -1, 
                n_bins      = 0,
                n_events    = 0, 
                fs_per_bin  = -1, 
                s_per_bin   = np.nan, 
                t0_ps       = np.nan, 
                t0_bin      = np.nan,
                good_bin1   = -1, 
                good_bin2   = -1, 
                background1 = -1, 
                background2 = -1):
                    
        self.id_number = id_number
        self.htype = htype
        self.title = title
        self.data = data
        self.n_bytes = n_bytes
        self.n_bins = n_bins
        self.n_events = n_events
        self.fs_per_bin = fs_per_bin
        self.s_per_bin = s_per_bin
        self.t0_ps = t0_ps
        self.t0_bin = t0_bin
        self.good_bin1 = good_bin1
        self.good_bin2 = good_bin2
        self.background1 = background1
        self.background2 = background2

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
