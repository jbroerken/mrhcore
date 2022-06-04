*****************
User Event Routes
*****************
The user event route configuration groups events sent by user applications into groups, 
which are then used to define what kind of user events a platform service receives. The 
user event routes file uses the MRH Block File format.

File Structure
--------------
Each user event route is stored in the block file by using its own block, named **UserEventRoute**.
Each block is seen as its own event route with individual event information.

UserEventRoute Block
--------------------
The UserEventRoute block starts by listing the ID of the user event route, which is identified 
by the **RouteID** key:

.. code-block:: c

    <RouteID><0>


Following the route id come all user events which belong to this event route. The key name is 
irrelevant here and not used, but the assigned value has to match the numerical ID of a MRH event.

.. code-block:: c

    <MRH_EVENT_PS_RESET_REQUEST_U><4>
    <MRH_EVENT_LISTEN_AVAIL_U><10>


.. note:: 

    It is recommended to use the event name string as a key for the event id value.
    
Example
-------
The following example shows a user event route list file with two event routes, with one event 
found in both routes:

.. code-block:: c

    <MRHBF_1>
    
    <UserEventRoute>{
        <RouteID><0>
        <MRH_EVENT_PS_RESET_REQUEST_U><4>
        <MRH_EVENT_LISTEN_AVAIL_U><10>
        <MRH_EVENT_LISTEN_GET_METHOD_U><11>
    }
    
    <UserEventRoute>{
        <RouteID><1>
        <MRH_EVENT_PS_RESET_REQUEST_U><4>
        <MRH_EVENT_SAY_AVAIL_U><17>
        <MRH_EVENT_SAY_GET_METHOD_U><19>
    }
    
