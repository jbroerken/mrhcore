**************
Event Exchange
**************
All events exchanged by a user application are sent and received with the 
help of a event queue provided by mrhcore. The core receives events from a 
user application to hand to matching platform services and send platform 
service events to the user application.

mrhcore will also check if events are allowed to sent and received by 
the current application based on the application permissions and more. 
mrhcore will inform the user application if necessary about missing 
permissions.

Each update is limited by both timeout and number of events. mrhcore 
keeps track of those limitations and updates the event exchange by 
those limits.

Event Limitations
-----------------
Events are limited by the application permissions and password verification. 
mrhcore will check the set permissions before handing off events in either 
direction discard them if permissions are missing.

mrhcore will notifiy the running user application if permissions for an event 
are missing. mrhcore will send the event **MRH_EVENT_PERMISSION_DENIED** if 
event permissions are missing and the event **MRH_EVENT_PASSWORD_REQUIRED** if 
password verification is required for the event.

.. note:: 

    Only events sent by the user application will cause response events 
    to be sent to the user application.
    
    
Certain events are not able to be password protected to limit their use. 
This is done to ensure nonsensical protections like protecting the password 
verification and important system events are always accessible.

.. toctree::
   :maxdepth: 1

   Version 1 Events<Event_Exchange/Version_1>
    

Events are also limited by the event group ID in use. User applications 
can only receive events intended for the running application by checking 
the event group ID of the event.

.. note::

    A certain few events have no group ID set and will always be given 
    to the user application.


Event Reset
-----------
A reset event process has to happen before mrhcore starts the event 
exchange between the running user application and platform services. 
This event reset has to happen after every application launch and 
always works in the same way.

The reset event process begins by receiving **MRH_EVENT_PS_RESET_REQUEST_U** 
from the currently running application, which will be given to all services 
to inform them of the currently running application package. The event 
has to contain the full path to the package directory. 

mrhcore will send the **MRH_EVENT_PS_RESET_ACKNOLEDGED_U** event after receiving 
the reset request, which tells the application that the reset succeeded and that 
the services are now informed of the current package.This event also signals that 
sending normal events is now allowed.

.. note:: 

    mrhcore will discard all other events until the event reset process has 
    been completed.


Exchanging Events
-----------------
mrhcore exchanges events between the running user application and available 
platform services by performing a looped upate.

.. note::

    The number of events sent and received is limited by the event limits set in the 
    :doc:`core configuration <../Configurations/Core_Configuration>`.


mrhcore will first wait for events sent by the running user application and stores 
them until they are able to be given to the platform services they are intended 
for. 

Sending events to the user application happens after events were either received or 
the timeout for receiving events expired. The core will send all events received from 
platform services which were added between the last send and the current send.

.. warning::

    Events are sent in the order they were added to the send queue. Sending will 
    stop if the current event to send failed to do so.
