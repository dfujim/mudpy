import numpy as np

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
    def __pow__(self, other):       return np.power(self._get_val(), self._get_oval(other))
    def __round__(self):            return np.round(self._get_val())
    def __rshift__(self, other):    return self._get_val()>>self._get_oval(other)
    def __sub__(self, other):       return self._get_val()-self._get_oval(other)
    def __truediv__(self, other):   return self._get_val()/self._get_oval(other)

    # casting operators
    def __bool__(self):             return bool(self._get_val())
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

