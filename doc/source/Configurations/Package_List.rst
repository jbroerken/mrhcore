************
Package List
************
The package list file lists all installed user and os application packages usable 
by mrhcore. Packages will only be launched if mrhcore finds them in the package list. 
The package list file uses the MRH Block File format.

.. note::

    User application packages will only be launched if mrhcore finds them in 
    the package list.


File Structure
--------------
The block file stores all packages by the id of the package and the full path to the package 
directory. All packages are stored in the **Package** block, which includes the total package 
count and package paths.

Package Block
-------------
The Package block starts by storing the total package count:

.. code-block:: c

    <Count><1>
    

.. warning::

    Only packages up to the specified package count will be stored by mrhcore!


Following the total package count come all packages in the package list which should 
be known to mrhcore. All packages are identified by their ID, which is paired with the 
full path to the package directory:

.. code-block:: c

    <Package ID><Full Package Path>
    
    
.. note::

    Package IDs start at 0.
    

Example
-------
The following example shows a package list file with 2 packages:

.. code-block:: c

    <MRHBF_1>
    
    <Package>{
        <Count><2>
        <0></opt/mrh/de.mrh.launcher.soa>
        <1></opt/mrh/de.mrh.mirrorspeech.soa>
    }
    
