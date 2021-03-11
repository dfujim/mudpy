from .mcontainer import mcontainer

class mscaler(mcontainer):
    """
        Scaler associated with mdata object.

        Data fields:
            id_number
            title
            counts_total_recent
    """
    __slots__ = ('id_number', 'title', 'counts_total_recent')

    def __init__(self, 
                 id_number = -1,
                 title = '',
                 counts_total_recent = -1):
                     
        self.id_number = id_number
        self.title = title
        self.counts_total_recent = counts_total_recent

    def _get_val(self): return self.counts_total_recent[0]

