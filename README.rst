=====================
BMML bindings for C++
=====================

:Author: Mario Lang <mlang@blind.guru>

bmmlcxx is a simple library for processing BMML (Braille Music Markup Language)
documents in C++.

BMML elements are stored in a simple DOM-like `element` base class.
All BMML elements get their own
element-specific clcasses which derive from `element` and add functionality
to make it easier for the programmer to perform common BMML tasks.

See catbmml.cxx and driver.cxx for simple examples on how to use the library.

Build requirements
------------------

You need CMake, Python, a recent C++ compiler that supports C++14, and
libstudxml_ to build this library.

.. _libstudxml: http://www.codesynthesis.com/projects/libstudxml/

Build instructions
------------------

.. code-block:: console

   $ git clone https://github.com/mlang/bmmlcxx
   $ cd bmmlcxx
   $ mkdir build
   $ (cd build && cmake ..)
   $ make -C build

BMML Files
----------

A number of BMML documents can be downloaded from the `Contrpunctus BMML library`_.

.. _`Contrpunctus BMML library`: http://www.contrapunctus.it/library

