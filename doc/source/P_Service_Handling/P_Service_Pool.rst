*********************
Platform Service Pool
*********************
All platform services are managed by the platform service pool. The service pool 
is responsible for both starting and updating the currently services.

Service Pool Start
------------------
The platform service pool is started once during the setup of mrhcore. All platform 
services are started at the time at which they are added to the service pool. The 
pool itself starts updating once all services to start have been handled. 

.. note::

    All started platform services have their pid written into a service 
    pid file.


The service pool will also load the matching :doc:`user event route <../Configurations/User_Event_Route>` 
to know which events sent by user applications are intended for what service.

.. warning::

     User event routes are required for services to start successfully.


Service Pool Stop
-----------------
The service pool is stopped when mrhcore stops. Once the service pool stops 
all services stored inside the service pool will be stopped as well.

Service Update
--------------
The service pool is used to update platform services. Other components of mrhcore 
never interact with the services directly. 

.. note::

    Certain update behaviours (like event limits) are handled by the service 
    pool instead of the service process itself.
    

The service pool itself waits until one of the services signals available data to 
the service pool, at which point the service pool starts to check the currently running 
services for received data.

.. warning::

    Stopped services are not updated. Events still remaining for a 
    stopped service will not be sent or received.



