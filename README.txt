####################
#                  #
#  mrhcore ReadMe  #
#                  #
####################

##
# About
##

The mrhcore project provides the MRH platform core which is the cental point for the MRH platform.
MRH core handles event communication, start platform and application services, handles user applications and 
handles permissions and password protection.
All event communication methods as well as event recognition and routing are supplied by MRH core.


##
# Requirements
##

Compilation:
------------
This project is built using CMake. You can find CMake here:

https://cmake.org/

Library Dependencies:
---------------------
This project requires other libraries and headers to function:

Dependency List:
libmrhbf: https://github.com/jbroerken/libmrhbf/
libmrhvt: https://github.com/jbroerken/libmrhvt/
mrhshared: https://github.com/jbroerken/mrhshared/

For more information about the requirements, check the "Building" section found in the documentation.


##
# Documentation
##

All documentation is build with sphinx-doc using the Read The Docs theme.
To build the documentation, grab the requirements for it:

sphinx-doc:
-----------
https://www.sphinx-doc.org/en/master/

Read The Docs Theme:
--------------------
https://sphinx-rtd-theme.readthedocs.io/en/stable/

##
# Licence
##

This project is licenced under the Apache 2.0 licence. 
Please read the included LICENCE.txt for the exact terms.


##
# Directories
##

This project supplies multiple directories for the development of said project. 
Their names and descriptions are as follows:

Directory List:
bin: Contains the built project executables.
build: CMake build directory.
doc: Documentation files.
src: Project source code.