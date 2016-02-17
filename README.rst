=====================
BMML bindings for C++
=====================

:Author: Mario Lang <mlang@blind.guru>

bmmlcxx is a simple library for processing BMML (Braille Music Markup Language)
documents in C++.  It uses a local copy of libstudxml for
parsing and serialising XML.

BMML elements are stored in a simple DOM-like `element` class.
Certain frequently used BMML elements get their own
element-specific clcasses which add functionality to make it
easier for the programmer to perform common BMML tasks.

See driver.cxx for a simple example on how to use the library.

BMML Files
----------

A number of BMML documents can be downloaded from the `Contrpunctus BMML library`_.

.. _`Contrpunctus BMML library`: http://www.contrapunctus.it/library

