*************************
Platform Service Stopping
*************************
Platform services are stopped either by the service pool shutting down. A user does 
not stop services directly.

.. warning::

    Platform services stopping before a service pool shutdown will cause 
    mrhcore to terminate.


Stop Behaviour
--------------
Platform services are given a certain amount of time for normal termination. 
This timeout is defined in the :doc:`core configuration <../Configurations/Core_Configuration>` 
with the **ForceStopProcessS** key. The platform service will be informed of 
the stop request by using signals.

.. note::

    The signal **SIGTERM** is used to inform of a requested termination.
    

mrhcore will forcefully terminate the platform service in case the service still 
runs after the timeout passed.

.. warning::

    **SIGKILL** is used to force termination, which will not be received 
    by the platform service itself.
