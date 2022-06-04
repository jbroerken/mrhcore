***********
Permissions
***********
All user applications are limited by permissions which define both which 
group and type of events they are able to receive. 

.. warning:: 

    User applications cannot request extended permissions from mrhcore. 
    The permissions used must be defined by the application during 
    package installation.
    

A user application can send events limited by permissions any time, but 
missing a permission for an event will cause mrhcore to discard the event 
and return the **MRH_EVENT_PERMISSION_DENIED** event back to the user 
application.

Events sent to an user application without request will not be handed to 
the user application if the event permissions for that event are missing.

.. note:: 

    The permissions listed here are for the MRH platform 
    event version 1.
    

Defining Permissions
--------------------
User application permissions are defined and stored inside the 
application package configuration. Using the value 0 for a 
permission type will block all permissions for this type.

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
    
    
Custom Permissions
------------------
The following custom event permissions exist:

.. list-table::
    :header-rows: 1

    * - Name
      - ID
      - Description
    * - CUSTOM_USABLE
      - 1
      - Custom type events can be used.
      

Application Permissions
-----------------------
The following application event permissions exist:

.. list-table::
    :header-rows: 1

    * - Name
      - ID
      - Description
    * - APPLICATION_LAUNCH
      - 1
      - Launch application by package path.
    * - APPLICATION_CLEAR_LAUNCH
      - 2
      - Clear current application launch request.
    * - APPLICATION_LAUNCH_TIMER
      - 4
      - Timed application launch by package path.
    * - APPLICATION_CLEAR_LAUNCH_TIMER
      - 8
      - Clear all current timed application launch requests.
    * - APPLICATION_CUSTOM_COMMAND
      - 16
      - Allow sending and recieving of custom application commands.
      

Listen Permissions
------------------
The following listen event permissions exist:

.. list-table::
    :header-rows: 1

    * - Name
      - ID
      - Description
    * - LISTEN_LISTEN
      - 1
      - Receive speech input.
    * - LISTEN_GET_METHOD
      - 2
      - Get speech input method.
    * - LISTEN_CUSTOM_COMMAND
      - 4
      - Allow sending and recieving of custom listen commands.


Say Permissions
---------------
The following say event permissions exist:

.. list-table::
    :header-rows: 1

    * - Name
      - ID
      - Description
    * - SAY_SAY
      - 1
      - Send speech output.
    * - SAY_GET_METHOD
      - 2
      - Get speech output method.
    * - SAY_NOTIFICATION_APP
      - 4
      - Send notification to external device.
    * - SAY_CUSTOM_COMMAND
      - 16
      - Allow sending and recieving of custom say commands.


Password Permissions
--------------------
The following password event permissions exist:

.. list-table::
    :header-rows: 1

    * - Name
      - ID
      - Description
    * - PASSWORD_CHECK
      - 1
      - Verify user platform password.
    * - PASSWORD_SET
      - 2
      - Update user platform password.
    * - PASSWORD_CUSTOM_COMMAND
      - 4
      - Allow sending and recieving of custom password commands.
      

User Permissions
----------------
The following user event permissions exist:

.. list-table::
    :header-rows: 1

    * - Name
      - ID
      - Description
    * - USER_DOCUMENTS_ACCESS
      - 1
      - Request user documents access.
    * - USER_PICTURES_ACCESS
      - 2
      - Request user pictures access.
    * - USER_MUSIC_ACCESS
      - 4
      - Request user music access.
    * - USER_VIDEOS_ACCESS
      - 8
      - Request user videos access.
    * - USER_DOWNLOADS_ACCESS
      - 16
      - Request user downloads access.
    * - USER_CLIPBOARD_ACCESS
      - 32
      - Request user clipboard access.
    * - USER_INFO_PERSON_ACCESS
      - 64
      - Request user person info access.
    * - USER_INFO_RESIDENCE_ACCESS
      - 128
      - Request user residence info access.
    * - USER_GET_LOCATION
      - 256
      - Request user location.
    * - USER_CUSTOM_COMMAND
      - 512
      - Allow sending and recieving of custom user commands.

