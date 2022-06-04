******
Locale
******
The locale file defines the active locale used by mrhcore and other MRH platform components. 
The locale file uses the MRH Block File format.

.. note::

    The MRH platform uses UTF-8 locales.


File Structure
--------------
The block file stores the language code for the currently used locale. All information is 
stored in the **Locale** block.

Locale Block
------------
The Locale block has a single key and value pair, the used locale:

.. code-block:: c

    <Active><Locale Code>
    

Example
-------
The following example shows a package list file with 2 packages:

.. code-block:: c

    <MRHBF_1>
    
    <Locale>{
        <Active><en_US.UTF-8>
    }
    
