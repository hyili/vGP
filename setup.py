#!/usr/bin/python3

from distutils.core import setup, Extension

setup(name="vGP", version="1.0", ext_modules=[Extension("vGP", ["vGP.cpp"])])
