**************
Event Exchange
**************
All events exchanged by platform services are sent and received with the 
help of a event queue provided by mrhcore. The core receives events from 
the platform services with the :doc:`platform service pool <P_Service_Pool>` 
to hand to the current user application. Events for the platform services 
are handed to the service pool to distribute with the help of event routes.

Each update is limited by both timeout and number of events. The platform 
service pool keeps track of those limitations and updates the event exchange 
by those limits.

Event Limitations
-----------------
Only events intended to be sent from user applications are able to be sent to 
platform services. The service pool will filter events depending on their type 
to make sure no service events are sent to platform services.

.. note::

    Wrong events are discarded without notifying the current user application.
    

User Event Routes
-----------------
Events sent by user applications and user application services are assigned to 
platform services by using :doc:`user event routes <../Configurations/User_Event_Route>`.

The event routes defines which platform services are responsible for handling a 
specifc user application event. 

.. note::

    Events are duplicated if more than one service reacts to them.


Exchanging Events
-----------------
mrhcore exchanges events between the running user application, user application services 
and available platform services by performing a looped upate.

mrhcore will receive events from platform services intended for the running user application 
by retrieving the events of the services stores in the platform service pool. The events sent 
by all services are collected in one go and given to the user application as a combined group.

.. note:: 

    The order in which events are received from platform services is not guaranteed.
    

mrhcore will wait for events sent by the running user application and services and store
them until they are able to be given to the platform services they are intended for. 
The events are then given to the matching platform services depending on the user event 
route used by the platform service.

.. note:: 

    The order in which events are given to platform services is not guaranteed.
    
    
The number of events sent and received is limited by the event limits set in the 
:doc:`core configuration <../Configurations/Core_Configuration>`.