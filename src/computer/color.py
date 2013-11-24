import math

"""
Usage:

Initialize:
>>> c = Color(r, g, b)

Get:
>>> c.h

Set:
>>> c.s = 100

HSV values are automatically updated when RGB values are set, and vice versa
"""

class Color(object):
    def __init__(self, r=0, g=0, b=0, h=0, s=0, v=0):
        self._r = r
        self._g = g
        self._b = b
        self.updateHSV()

    def updateHSV(self):
        mn = min(self._r, self._g, self._b)
        mx = max(self._r, self._g, self._b)
        self._v = mx
        d = mx - mn
        if d == 0:
            d = 1 # division by zero protection
        if mx != 0:
            self._s = math.floor(d * 255 / mx)
        else:
            self._s = 0
            self._h = 0
            return
        
        if self._r == mx:
            self._h = (self._g - self._b) / d
        elif self._g == mx:
            self._h = 2 + (self._b - self._r) / d
        else:
            self._h = 4 + (self._r - self._g) / d

        self._h *= 60
        if self._h < 0:
            self._h += 360

        self._h = math.floor(self._h * 255 / 360)

    def updateRGB(self):
        h = self._h * 360 / 255
        s = self._s / 255
        v = self._v / 255

        if s == 0:
            self._r = math.floor(v * 255)
            self._g = math.floor(v * 255)
            self._b = math.floor(v * 255)
            return
        h /= 60
        i = math.floor(h)
        f = h - i
        p = v * (1 - s)
        q = v * (1 - s * f)
        t = v * (1 - s * (1 - f))
        if i == 0:
            self._r = math.floor(v * 255)
            self._g = math.floor(t * 255)
            self._b = math.floor(p * 255)
        elif i == 1:
            self._r = math.floor(q * 255)
            self._g = math.floor(v * 255)
            self._b = math.floor(p * 255)
        elif i == 2:
            self._r = math.floor(p * 255)
            self._g = math.floor(v * 255)
            self._b = math.floor(t * 255)
        elif i == 3:
            self._r = math.floor(p * 255)
            self._g = math.floor(q * 255)
            self._b = math.floor(v * 255)
        elif i == 4:
            self._r = math.floor(t * 255)
            self._g = math.floor(p * 255)
            self._b = math.floor(v * 255)
        else:
            self._r = math.floor(v * 255)
            self._g = math.floor(p * 255)
            self._b = math.floor(q * 255)

    @property
    def r(self):
        return self._r
    
    @r.setter
    def r(self, value):
        self._r = value
        self.updateHSV()
    
    @property
    def g(self):
        return self._g

    @g.setter
    def g(self, value):
        self._g = value
        self.updateHSV()
    
    @property
    def b(self):
        return self._b

    @b.setter
    def b(self, value):
        self._b = value
        self.updateHSV()

    @property
    def h(self):
        return self._h
    
    @h.setter
    def h(self, value):
        self._h = value
        self.updateRGB()

    @property
    def s(self):
        return self._s

    @s.setter
    def s(self, value):
        self._s = value
        self.updateRGB()

    @property
    def v(self):
        return self._v

    @v.setter
    def v(self, value):
        self._v = value
        self.updateRGB()
