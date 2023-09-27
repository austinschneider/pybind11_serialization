import pyABC
from pyABC import ABC


def log(*msg):
    print('[Python]', *msg)


class PyDerived(ABC):

    _id = 0
    data = None

    @classmethod
    def id(cls):
        cls._id += 1
        return cls._id

    def __init__(self):
        self._id = PyDerived.id()
        ABC.__init__(self)
        log("__init__ called on", self, self._id)

    def method(self):
        log("method called on", self, self._id)
        log("data =", self.data)

    def __del__(self):
        log(" __del__ called on", self, self._id)
