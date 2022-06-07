****************************
Application Service Handling
****************************
User application servicess are handled by mrhcore with the help of a user 
application service parent. mrhcore starts each user application service 
and supplies them with required functionality like IPC mechanisms for a 
successfull operation. Application servicess are then updated until the 
core is shutdown or a application service stops on its own.

Application services are stored and updated using an application service 
pool. The application service pool allows easy management of all individual 
service.

.. note::

    All application services and the pool itself run on their own individual 
    thread.
    

.. toctree::
   :maxdepth: 1

   A_Service_Pool
   A_Service_Starting
   Event_Exchange
   Permissions
   A_Service_Stopping