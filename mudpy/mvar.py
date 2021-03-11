from .mcontainer import mcontainer

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
