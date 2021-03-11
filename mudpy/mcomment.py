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

