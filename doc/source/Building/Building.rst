********
Building
********
The project files are already prepared and include everything except the 
required dependencies. This document lists both dependencies as well as the 
build process to create the library from source.

Supported Platforms
-------------------
The supported platforms for mrhcore are UNIX-likes, primarily 
Debian-based GNU/Linux Distributions like the Rapsberry Pi OS.

Dependencies
------------
This project has the following dependencies:

* mrhshared: https://github.com/jbroerken/mrhshared/
* libmrhbf: https://github.com/jbroerken/libmrhbf/
* libmrhvt: https://github.com/jbroerken/libmrhvt/

Build Tools
-----------
This release includes a CMake script (CMakeLists.txt) for a simplified build 
process. The minimal required version for CMake is 3.1.
Also needed is the GNU C++ Compiler. Full C++14 support is required.

Changing Pre-defined Settings
-----------------------------
Listed in the CMakeLists.txt file are preprocessor macros used to specify 
default file paths and other information. Change these to fit the requirements 
of the target use case.

.. list-table::
    :header-rows: 1

    * - Macro
      - Description
    * - MRH_CORE_LOG_FILE_PATH
      - The full path to the log file to use.
    * - MRH_CORE_BACKTRACE_FILE_PATH
      - The full path to the backtrace file to use.
    * - MRH_LOGGER_PRINT_CLI
      - If logging should be printed on the cli.
    * - MRH_LOGGER_LINE_LIMIT
      - The max amount of lines allowed in a log file.
    * - MRH_CORE_DAEMON_MODE
      - If mrhcore should be started as a daemon.
    * - MRH_CORE_EVENT_LOG_FILE_PATH
      - The full path to the event exchange log file to 
        use.
    * - MRH_EVENT_LOGGER_PRINT_CLI
      - If event logging should be printed on the cli.
    * - MRH_CORE_EVENT_LOGGING
      - If the core should log incoming and outgoing events.
    * - MRH_LOCALE_FILE_PATH
      - The full path to the MRH locale file to use.
    * - MRH_CORE_CONFIGURATION_FILE_PATH
      - The full path to the core configuration file to use.
    * - MRH_USER_SERVICE_LIST_FILE_PATH
      - The full path to the user service whitelist file to 
        use.
    * - MRH_PACKAGE_LIST_FILE_PATH
      - The full path to the package list file to use.
    * - MRH_PLATFORM_SERVICE_LIST_FILE_PATH
      - The full path to the platform service list file to 
        use.
    * - MRH_PROTECTED_EVENT_LIST_FILE_PATH
      - The full path to the password protected event list file 
        to use.
    * - MRH_USER_EVENT_ROUTE_FILE_PATH
      - The full path to the user to platform service event route 
        list file to use.
    * - MRH_CORE_LAUNCH_INPUT_DIR
      - The full path to the directory to store used launch input 
        in.
    * - MRH_CORE_LAUNCH_INPUT_FILE
      - The name of the text file to store used launch input in.
    * - MRH_CORE_INPUT_STOP_TRIGGER_PATH
      - The full path to the directory where the user application 
        quit command localizations are stored in.
    * - MRH_CORE_INPUT_STOP_TRIGGER_FILE
      - The name of the user application quit localization file.
    * - MRH_CORE_LOG_FILE_DIR
      - The full path to the MRH platform default log file directory.
    * - MRH_CORE_PID_FILE_DIR
      - The full path to the directory where mrhcore will store 
        running processes pid files in.
    * - MRH_CORE_USER_SERVICE_PID_FILE
      - The name of the stored user application service pid file.
    * - MRH_CORE_PLATFORM_SERVICE_PID_FILE
      - The name of the stored platform service pid file.
    * - MRH_CORE_USER_PID_FILE
      - The name of the stored user application pid file.
      

Build Process
-------------
The build process should be relatively straightforward:

1. Aqquire dependencies.
2. Move into the project "build" folder.
3. Compile Makefiles with the included CMakeLists.txt.
4. Run "make" to compile the binary.
5. Install the compiled binary.

Shell Commands
--------------
The following shell commands will create makefiles with the 
provided CMakeLists.txt, compile the project with the created 
makefiles and install the binary:

.. code-block::

    cd <Project Root Folder>/build
    cmake ..
    make
    sudo make install
