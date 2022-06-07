********
Packages
********
All usable user applications and user application services are installed and 
managed as packages. Packages include not just the application binary but also 
the files required to select and run the applications.

mrhcore loads packages from the :doc:`package list <../Configurations/Package_List>` 
and from the :doc:`core configuration <../Configurations/Core_Configuration>`.

.. note::

    MRH platform package directories have to end in the **.soa** extension 
    to be valid packages.


Binaries
--------
Each package includes a binary which contains the actual code of the application 
run by the MRH platform. This binary is supplied as a shared object (.so) and 
loaded by an application parent.

.. note::

    A package can either contain a application binary, a application service binary 
    or both.
    

The binaries can be found in the SharedObject directory:

.. code-block:: c

    <Package Root>/SharedObject/App.so
    <Package Root>/SharedObject/Service.so


Configuration
-------------
The package configuration contains information about permissions, user and group 
IDs to use, application type flags and more. mrhcore loads elements from this 
configuration to know how to update the user application.

.. note::

    The package configuration is combined for both the user application and 
    user application service.


The configuration file is found in the package root directory:

.. code-block:: c

    <Package Root>/Configuration.conf


Launch Info
-----------
Each package should include support files for the package name and the launch 
triggers used to start the package. These files are not used by mrhcore directly 
but the installed launcher (home package) instead.

Both files are localized, meaning the files are actually found in a special 
directory structure. The files containing the strings itself are a simply UTF-8 
text file for the application name and a MRH launch trigger file for the launch 
commands.

Both the launch trigger files and application name files are found in their 
own folders in the package root directory:

.. code-block:: c

    <Package Root>/ApplicationName/<Language Code>/ApplicationName.txt
    <Package Root>/LaunchTrigger/<Language Code>/LaunchTrigger.mrhit


FSRoot
------
The FSRoot directory contains the actual files used by the application itself. 
Asset files like sounds and text should be stored in the FSRoot directory. 
mrhcore will not directly use this directory, although platform services will.

The FSRoot folder is found in the package root directory:

.. code-block:: c

    <Package Root>/FSRoot

