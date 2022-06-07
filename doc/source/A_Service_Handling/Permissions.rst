***********
Permissions
***********
All user application servicess are limited by permissions which define 
if a service event can be sent or not.

.. warning:: 

    User application servicess cannot request extended permissions from 
    mrhcore. The permissions used must be defined by the application during 
    package installation.
    

A user application service can send events limited by permissions any time, but 
missing a permission for an event will cause mrhcore to discard the event. 

.. note::

    Application services will not be informed of missing permissions!
    

.. note:: 

    The permissions listed here are for the MRH platform 
    event version 1.
    

Defining Permissions
--------------------
User application service permissions are defined and stored inside the 
application package configuration. Using the value 0 for a permission 
type will block all permissions for this type.

.. note:: 

    Event permissions work like bit flags and can be combined 
    for the total value of the needed permissions.
    

Permissions required are defined as follows:

.. code-block:: c

    <Permissions>{
        <EventCustom><0>
        <EventApplication><3>
        <EventListen><1>
        <EventSay><1>
        <EventPassword><0>
        <EventUser><0>
    }
    
    
Say Permissions
---------------
The following say event permissions exist:

.. list-table::
    :header-rows: 1

    * - Name
      - ID
      - Description
    * - SAY_NOTIFICATION_SERVICE
      - 8
      - Send notification to external device.

