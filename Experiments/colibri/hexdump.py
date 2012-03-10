import itertools

class Hexdump:
    def __init__(self, data, title = None, offset = 0):
        self._title = title
        self._data = data
        self._offset = offset

    def _row(self, it, offset):
        missing = offset % 16
        row_start = offset - missing
        displayed = 16 - missing

        row_data = bytes(itertools.islice(it, displayed))

        if not len(row_data):
            raise StopIteration

        out = []

        out.append('{:08x}'.format(offset))

        out.append(' ')
        out.extend(['   '] * missing)
        out.extend((' {:02x}'.format(x) for x in row_data[:8 - missing]))

        out.append(' ')
        out.extend((' {:02x}'.format(x) for x in row_data[8 - missing:]))
        out.extend(['   '] * (16 - len(row_data) - missing))

        out.append('  ')
        out.extend([' '] * missing)
        out.append('|')

        for char in row_data.decode('latin 1'):
            if char.isprintable():
                out.append(char)
            else:
                out.append('.')

        out.append('|')
            
        return ''.join(out), len(row_data)

    def __str__(self):
        offset = self._offset
        it = iter(self._data)
        prev_row = None
        have_star = False

        if self._title is not None:
            out = [self._title]
        else:
            out = []

        while True:
            try:
                row, length = self._row(it, offset)
            except StopIteration:
                break

            offset += length

            if row == prev_row:
                if not have_star:
                    out.append('*')
                    have_star = True
            else:
                have_star = False
                out.append(row)
                last_row = row

        return '\n'.join(out)
        
def hexprint(*args, **kwargs):
    print(Hexdump(*args, **kwargs))
