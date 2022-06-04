*****************
Application Types
*****************
User applications are divided in three categories: The **home package** 
which is used as a default user application, **OS application packages** 
and all other **user application package** applications.

.. note::

    User applications are supplied as packages, which contain 
    not just the binary to run but also their configuration, 
    launch info and application ressources.
    

Home Package
------------
The home package is the default starting point for using the MRH platform. 
mrhcore will start this package after completing the initial core setup and 
if the current user application exits (either normally or because of a crash) 
and no other application to launch was set.

This package should idealy be a launcher application which allows the platform 
user to start other user applications.

.. warning::

    A crash in the home package will cause mrhcore to terminate.
    
    
The home package to use is defined in the mrhcore configuration. The full path 
to the home package is read from the configuration by searching for the following 
information:

.. code-block:: c

    <HomePackagePath></full/path/to/package.directory.soa>


User Application Package
------------------------
User applications, supplied by user application packages, are the general 
applications developed for the MRH platform. A user application might be 
a calculator, a calendar or else.

mrhcore keeps the list of all usable user application packages stored and 
checks this loaded package list for the requested user application package 
before starting.

.. note::

    User applications are only startable if they are stored in the 
    loaded package list.
    

.. warning::

    Packages have to have their directory name end in the ".soa" 
    externsion to be launchable.
    

OS Application Package
----------------------
OS application packages provide the MRH platform with changes to the configuration 
to use, the packages which are installed and more. OS application are stored like 
other user application packages. 

A application is recognized as a OS application if the OS application type value 
is greater than 0. The value checked for this is stored in the configuration file 
of a application package itself with the following key:

.. code-block:: c

    <RunAs>{
        <OSAppType><3>
    }


The following types of OS applications exist:

.. list-table::
    :header-rows: 1

    * - Type
      - ID
      - Description
    * - Utility
      - 1
      - General unspecified OS application.
    * - Launcher
      - 2
      - Application launcher.
    * - Settings
      - 4
      - Changes platform configurations.
    * - Package Manager
      - 8
      - Manage installed user application packages.
      

.. note::

    OS application types work like bit flags. An OS application can be 
    defined with multiple types.


The rules for starting, updating and stopping OS applications are almost identical 
to those of normal user applications. The only difference is that OS applications 
do not require password verification for protected events.

Stopping a OS application causes mrhcore to update itself depending on the type of 
OS application used. The following OS application types will cause the core to 
update:

.. list-table::
    :header-rows: 1

    * - Type
      - Behaviour
    * - Settings
      - Reload protected event list.
    * - Package Manager
      - Reload package list.

