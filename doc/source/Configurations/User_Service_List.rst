*****************
User Service List
*****************
The user service list contains the full paths for all user application services 
to be started by mrhcore. The user service list file uses the MRH Block File format.

File Structure
--------------
The block file stores all package paths collected in a single block. The block 
containing all package paths is called **UserService**.

UserService Block
-----------------
The UserService block stores all user application service packages by listing 
the full package path identified by the **Package** key name:

.. code-block:: c

    <Package><Full Path to Package>
    

Example
-------
The following example shows a user service list file with a single 
package:

.. code-block:: c

    <MRHBF_1>
    
    <UserService>{
        <Package></opt/mrh/de.mrh.service.example.soa>
    }
    
