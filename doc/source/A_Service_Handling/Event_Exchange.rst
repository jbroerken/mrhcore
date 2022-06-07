**************
Event Exchange
**************
All events sent by a user application service are sent with the help of 
a event queue provided by mrhcore. The core receives events from a 
user application service to hand to matching platform services.

mrhcore will also check if events are allowed to sent by the current 
application service based on the application permissions and more.

Each update is limited by both timeout and number of events. mrhcore 
keeps track of those limitations and updates the event exchange by 
those limits.

.. warning::

    User application services are only able to send events.


Event Limitations
-----------------
Events sent by application services are limited by the application permissions. 
mrhcore will check the set permissions before handing off events to platform 
services, and discard the events if the permissions are missing.

.. note:: 

    User application services will not be informed if event permissions 
    are missing.


Another limitation of the events sendable by user application services are 
the events itself. User application services are limited to specific events 
which can only be sent by the services.

.. toctree::
   :maxdepth: 1

   Version 1 Events<Event_Exchange/Version_1>
   

Event Reset
-----------
User application services do not require a event reset, unlike normal user 
applications.

Exchanging Events
-----------------
mrhcore exchanges events between running user application services and available 
platform services by performing a looped upate.

mrhcore will wait for events sent by the running user application services and store
them until they are able to be given to the platform services they are intended 
for. 

.. note:: 

    There is no guarantee that all events are received completely in one update.
    

Receiving events from platform services is not allowed for user application services. 
mrhcore will not add events to a queue going to the user application services and will 
also not attempt to send anything.

The number of events sent and received is limited by the event limits set in the 
:doc:`core configuration <../Configurations/Core_Configuration>`.
