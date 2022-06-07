************************
Application Service Pool
************************
All user application services are managed by the user application service 
pool. The service pool is responsible for both starting and updating the 
currently running application services.

Service Pool Start
------------------
The user application service pool is started once during the setup of mrhcore. 
All user application services are started at the time at which they are added 
to the service pool. The pool itself starts updating once all services to start 
have been handled. 

.. note::

    All started user application services have their pid written into a service 
    pid file.


Service Pool Reload
-------------------
The user service pool can be reloaded. This causes the pool to stop all 
services of packages no longer registered as application services and 
start all new services.

The user application service pool is reloaded by three different actions:

1. On initial pool startup
2. When a package management application stops
3. If SIGHUP was received

The service pool starts the reload by first removing all services which are 
either stopped or removed from the :doc:`package list <../Configurations/Package_List>`. 
The service pool will then start all services which are listed in both the package list
and :doc:`user service list <../Configurations/User_Service_List>`.

.. note::

    Reloading the service pool will restart all stopped user application 
    services.
    
    
.. note::

    Reloading the service pool will update the service pid list.


Service Pool Stop
-----------------
The service pool is stopped when mrhcore stops. Once the service pool stops 
all services stored inside the service pool will be stopped as well.

Service Update
--------------
The service pool is used to update user application services. Other 
components of mrhcore never interact with the services directly. 

.. note::

    Certain update behaviours (like event limits) are handled by the service 
    pool instead of the service process itself.


The service pool itself waits until one of the services signals available 
data to the service pool, at which point the service pool starts to check 
the currently running services for received data.
    
.. warning::

    Stopped services are not updated. Events still remaining for a 
    stopped service will not be sent to platform services.



