import numpy as np

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
