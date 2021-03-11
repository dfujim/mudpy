from .mcontainer import mcontainer
import numpy as np

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

    def __init__(self, 
                id_number   = -1, 
                low         = np.nan, 
                high        = np.nan, 
                mean        = np.nan, 
                std         = np.nan, 
                skew        = np.nan, 
                title       = '',
                description = '', 
                units       = ''):
                    
        self.id_number = id_number
        self.low = low
        self.high = high
        self.mean = mean
        self.std = std
        self.skew = skew
        self.title = title
        self.description = description
        self.units = units

    def _get_val(self): return self.mean
