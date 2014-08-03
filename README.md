ProtocolHandler
===============

Register a custom url protocol handler and run handler application from web browser(IE, Chrome, FF).

You can register a custom url scheme at browser, for example, "magnet:?key=value".



### 1. IE & Firefox
Please read follow document link:

http://msdn.microsoft.com/en-us/library/ie/aa767914(v=vs.85).aspx#app_reg




### 2. Chrome
 Editing value of 'excluded_schemes' in 'Local State' json file which follow path:

    %LOCAL_APPDATA%/Google/Chrome/User Data/Local State
 
 


### Used Library
* jsoncpp : https://github.com/open-source-parsers/jsoncpp
    
