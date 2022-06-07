**************
Event Exchange
**************
All applications sent by a user application are sent with the help of 
a event queue provided by mrhcore. The core receives events from a 
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

# TODO: 1. Absatz Permissions (Note zu PERMISSION DENIED und PASSWORD REQUIRED), 2. Absatz Event Group ID

Event Reset
-----------
A reset event process has to happen before mrhcore starts the event 
exchange between the running user application and platform services. 
This event reset has to happen after every application launch and 
always works in the same way.

# TODO: Process

Receiving Events
----------------

# TODO: Diagram, explain

Sending Events
--------------

# TODO: Diagram, explain
