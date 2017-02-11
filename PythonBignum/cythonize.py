#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

modules = \
    [
        'PBN',
    ]

ext_sources = \
    [name + '.py' for name in modules]

extensions = []
for i, mod in enumerate(modules):
    extensions.append(Extension(mod, [ext_sources[i]]))

setup(
    ext_modules = cythonize(extensions)
)
