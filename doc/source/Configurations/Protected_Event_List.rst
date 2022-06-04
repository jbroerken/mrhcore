********************
Protected Event List
********************
The protected event list contains all events for which mrhcore requires a password 
verification to succeed before the events are usable. The protected event list file 
uses the MRH Block File format.

.. note::

    Not all events can be protected.


File Structure
--------------
The block file stores all protected events in a single block. The block containing 
all events is called **ProtectedEvent**.

ProtectedEvent Block
--------------------
The ProtectedEvent block stores password protected events. The key name is irrelevant 
here and not used, but the assigned value has to match the numerical ID of a MRH event.

.. code-block:: c

    <MRH_EVENT_LISTEN_AVAIL_U><10>


.. note:: 

    It is recommended to use the event name string as a key for the event id value.
    

Example
-------
The following example shows a protected event list with two protected events:

.. code-block:: c

    <MRHBF_1>
    
    <ProtectedEvent>{
        <MRH_EVENT_LISTEN_AVAIL_U><10>
        <MRH_EVENT_SAY_AVAIL_U><17>
    }
    
