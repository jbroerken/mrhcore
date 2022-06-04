*********************
Platform Service List
*********************
mrhcore uses platform services as specified in the platform service list. This list contains 
all services to start for platform functionality as well as the conditions for their useage 
and event route information. The platform service list file uses the MRH Block File format.

File Structure
--------------
The block file stores each platform service in its own block. The block used for storing 
service information is called **PlatformService**, which always includes the same keys for 
each service.

PlatformService Block
---------------------
The PlatformService block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - BinaryPath
      - The full path to the platform service binary.
    * - RouteID
      - The event route ID to use for supplying the service 
        with events.
    * - Disabled
      - If the platform service is currently disabled.
    * - IsEssential
      - If the platform service is essential for the MRH platform 
        to operate.
        

Example
-------
The following example shows a platform service list file with two 
platform services:

.. code-block:: c

    <MRHBF_1>
    
    <PlatformService>{
        <BinaryPath></usr/local/bin/mrhpsspeech>
        <RouteID><0>
        <Disabled><0>
        <IsEssential><1>
    }
    
    <PlatformService>{
        <BinaryPath></usr/local/bin/mrhpsapplication>
        <RouteID><1>
        <Disabled><0>
        <IsEssential><1>
    }
    
