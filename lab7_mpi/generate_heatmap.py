import matplotlib.pyplot as plt


class Heatmap:

    def __init__(self, n, value=0):

        if not 0 <= value <= 100:
            raise ValueError("Values must be in range [0, 100]")

        self._n = n
        self._map = [[0] * (n + 2) for _ in range(n + 2)]  # nxn matrix

        for i in range(n):
            for j in range(n):
                self._map[i + 1][j + 1] = value

    def set_left_wall(self, value):
        if isinstance(value, list):
            if len(value) != self._n:
                raise ValueError("Value not of size n")
            for i in range(1, self._n + 1):
                self._map[i][0] = value[i - 1]
        else:
            for i in range(1, self._n + 1):
                self._map[i][0] = value

    def set_top_wall(self, value):
        if isinstance(value, list):
            if len(value) != self._n:
                raise ValueError("Value not of size n")
            for i in range(1, self._n + 1):
                self._map[0][i] = value[i - 1]
        else:
            for i in range(1, self._n + 1):
                self._map[0][i] = value

    def set_right_wall(self, value):
        if isinstance(value, list):
            if len(value) != self._n:
                raise ValueError("Value not of size n")
            for i in range(1, self._n + 1):
                self._map[i][-1] = value[i - 1]
        else:
            for i in range(1, self._n + 1):
                self._map[i][-1] = value

    def set_bottom_wall(self, value):
        if isinstance(value, list):
            if len(value) != self._n:
                raise ValueError("Value not of size n")
            for i in range(1, self._n + 1):
                self._map[-1][i] = value[i - 1]
        else:
            for i in range(1, self._n + 1):
                self._map[-1][i] = value

    def set_rect(self, i, j, w, h, value):
        if not (0 <= i < self._n and 0 <= j < self._n and 0 <= w <= self._n - i and 0 <= h <= self._n - j):
            raise ValueError("Rectangle must fit in the nxn map")

        for _w in range(w):
            for _h in range(h):
                self._map[i + 1 + _w][j + 1 + _h] = value

    def set_circle(self, i, j, r, value):
        if not (0 <= i < self._n and 0 <= j < self._n and r >= 0):
            raise ValueError("Circle must have a valid center and a non-negative radius")

        for _i in range(self._n):
            for _j in range(self._n):
                if (i - _i) ** 2 + (j - _j) ** 2 <= r ** 2:
                    self._map[_i + 1][_j + 1] = value

    def get_map(self):
        return self._map

    def export(self, filename):
        with open(filename, mode="w") as f:
            f.write(self.__str__())

    def __str__(self):
        return '\n'.join([' '.join(map(str, self._map[i])) for i in range(self._n + 2)])


if __name__ == '__main__':
    n = 1000
    hm = Heatmap(n)

    hm.set_left_wall(60)
    hm.set_top_wall(30)
    hm.set_right_wall(10)
    hm.set_bottom_wall(20)

    hm.set_circle(n / 2, n / 2, 3 * n / 20, 50)
    # hm.set_rect(0, 0, 30, 30, 50)

    plt.matshow(hm.get_map())
    plt.show()

    hm.export("heatmap" + str(n) + ".txt")
