from .mcontainer import mcontainer

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

    def __init__(self, 
                id_number = -1, 
                author = '', 
                body = '', 
                title = '', 
                time = 0):
        
        
        self.id_number = id_number
        self.author = author
        self.body = body
        self.title = title
        self.time = time
