********************
Application Handling
********************
User applications are handled by mrhcore with the help of a user 
application parent. mrhcore starts user applications and supplies 
them with required functionality like IPC mechanisms for a successfull 
operation. Applications are then updated until the application is stopped.

.. note::

    Only one user application can be run at a time.
    

.. toctree::
   :maxdepth: 1
   :caption: Actions:

   Application_Types
   Application_Starting
   Event_Exchange
   Permissions
   Application_Stopping