**********************
Platform Service Types
**********************
Platform services are divided up into two groups: Essential and non-essential 
platform services. Both service types provide some functionality to the MRH 
platform. 

The type of service is decided on mrhcore startup by reading the used 
:doc:`user event route <../Configurations/Platform_Service_List>`.

Essential Services
------------------
Essential services provide critical functionality to the MRH platform, like 
speech input and output or password verification. 

.. warning::

    A stopped essential service causes mrhcore to terminate.


Non-Essential Services
----------------------
Non-essential services provide optional functionality to the MRH platform, 
like debugging or tracking functionality.

.. note::

    Non-essential services will not be restartet once stopped.