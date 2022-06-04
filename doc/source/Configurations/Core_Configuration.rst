******************
Core Configuration
******************
The core configuration is used by mrhcore to know about required binaries and packages as 
well as event and process limitations. The core configuration file uses the MRH Block File 
format.

File Structure
--------------
The block file stores all core configuration values in a single combined block, which is 
called the **Core** block.

Core Block
----------
The Core block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - AppParentBinaryPath
      - The full path to the user application parent.
    * - AppServiceParentBinaryPath
      - The full path to the user appliction service parent.
    * - ForceStopProcessS
      - The time in seconds until a child process is force 
        stopped by mrhcore.
    * - ThreadWaitSleepMS
      - The time in milliseconds to sleep if necessary.
    * - UserAppRecieveTimeoutMS
      - The time in milliseconds to wait for receiving a user 
        application event.
    * - UserServiceRecieveTimeoutMS
      - The time in milliseconds to wait for receiving a user 
        application service event.
    * - PlatformServiceRecieveTimeoutMS
      - The time in milliseconds to wait for receiving a platform 
        service event.
    * - UserAppEventLimit
      - The maximum amount of events to both sent and receive in 
        a update cycle for a user application.
    * - UserServiceEventLimit
      - The maximum amount of events to both sent and receive in 
        a update cycle for a user application service.
    * - PlatformServiceEventLimit
      - The maximum amount of events to both sent and receive in 
        a update cycle for a platform service.
    * - HomePackagePath
      - The full path to the user application package to use as 
        the default home package.
    * - HomePackageDefaultLaunchCommandID
      - The launch command ID to use when relaunching the home 
        package.
    * - HomePackageStartupLaunchCommandID
      - The launch command ID to use when launching the home 
        package for the first time.
        
        
.. note:: 

    The UserAppEventLimit, UserServiceEventLimit and PlatformServiceEventLimit 
    values set here will also be given to the components themselves.
    

Example
-------
The following example shows a core configuration file with default values:

.. code-block:: c

    <MRHBF_1>
    
    <Core>{
        <AppParentBinaryPath></usr/local/bin/mrhuapp>
        <AppServiceParentBinaryPath></usr/local/bin/mrhuservice>
        <ForceStopProcessS><3>
        <ThreadWaitSleepMS><100>
        <UserAppRecieveTimeoutMS><20>
        <UserServiceRecieveTimeoutMS><20>
        <PlatformServiceRecieveTimeoutMS><20>
        <UserAppEventLimit><100>
        <UserServiceEventLimit><100>
        <PlatformServiceEventLimit><100>
        <HomePackagePath></opt/mrh/de.mrh.launcher.soa>
        <HomePackageDefaultLaunchCommandID><0>
        <HomePackageStartupLaunchCommandID><1>
    }
    
