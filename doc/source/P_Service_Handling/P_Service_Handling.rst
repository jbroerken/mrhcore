*************************
Platform Service Handling
*************************
Platform services are handled by mrhcore by using individual platform 
service binaries. mrhcore starts each platform service and supplies them 
with required functionality like IPC mechanisms for a successfull operation. 
Platform services are then updated until the core is shutdown.

Platform services are stored and updated using an platform service pool. 
The platform service pool allows easy management of all individual 
service.

.. note::

    All platform services and the pool itself run on their own individual 
    threads.
    

.. toctree::
   :maxdepth: 1

   P_Service_Pool
   P_Service_Types
   P_Service_Starting
   Event_Exchange
   P_Service_Stopping