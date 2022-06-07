****************************
Application Service Stopping
****************************
User application services are stopped either by the service pool shutting 
down or if the service itselfs decides to shutdown. A user does not stop 
services directly.

Stop Behaviour
--------------
Application services are given a certain amount of time for normal termination. 
This timeout is defined in the :doc:`core configuration <../Configurations/Core_Configuration>` 
with the **ForceStopProcessS** key. The user application service parent will be 
informed of the stop request by using signals.

.. note::

    The signal **SIGTERM** is used to inform of a requested termination.
    

mrhcore will forcefully terminate the user application parent in case the 
application still runs after the timeout passed.

.. warning::

    **SIGKILL** is used to force termination, which will not be received 
    by the application service parent itself.
