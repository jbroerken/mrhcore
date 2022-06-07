********************
Application Stopping
********************
User application can be stopped by mrhcore itself on user request. The 
core itself listens to all received speech input from any source and 
checks all input for stop commands.

Reading Stop Requests
---------------------
mrhcore loads a stop locale file on startup which defines the trigger 
words used to recognize a user input stop command. This file is loaded 
from a static location defined at compile time.

Every user input made will be checked for the loaded trigger words. All 
defined words must be found in the checked input to recognize the stop 
request.

.. note::

    Stop commands should be few and as explicit as possible to 
    minimize the risk of unintended application termination.


Stop Behaviour
--------------
Applications are given a certain amount of time for normal termination. 
This timeout is defined in the :doc:`core configuration <../Configurations/Core_Configuration>` 
with the **ForceStopProcessS** key. The user application parent will be 
informed of the stop request by using signals.

.. note::

    The signal **SIGTERM** is used to inform of a requested termination.
    

mrhcore will forcefully terminate the user application parent in case the 
application still runs after the timeout passed.

.. warning::

    **SIGKILL** is used to force termination, which will not be received 
    by the application parent itself.
