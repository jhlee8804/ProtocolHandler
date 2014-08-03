#include "protocol_handler.h"

#include <string>
#include <iostream>
#include <fstream>
#include <json/json.h>


#ifdef _WIN32

// release UNICODE, _UNICODE
#ifdef UNICODE
#undef UNICODE

#ifdef _UNICODE
#undef _UNICODE

#include <atlbase.h>
#include <ShlObj.h>

#endif  // _UNICODE
#endif  // UNICODE
#endif  // _WIN32


// NOTE: 관리자 권한 필요. 프로젝트 우측 클릭 -> Properties -> Linker -> Manifest File에서
// UAC Execution level을 highestAvailable (/level='highestAvailable')으로 지정.


namespace sys {

void ProtocolHandler::Register(ProtocolInfo &info) {
  // Register custom protocol handler - Windows
#ifdef _WIN32
  // 1. IE, FireFox
  {
    // http://msdn.microsoft.com/en-us/library/ie/aa767914(v=vs.85).aspx
    ATL::CRegKey reg;
    if (reg.Create(HKEY_CLASSES_ROOT, info.uri.c_str()) == ERROR_SUCCESS) {
      // set value. {default: "URL:MyApp Protocol"}
      {
        std::string value("URL:");
        value.append(info.displayName);
        value.append(" Protocol");
        reg.SetStringValue(NULL, value.c_str());
      }
      
      // set value. {URL Protocol: ""}
      reg.SetStringValue("URL Protocol", "");

      // create subkey('DefaultIcon') and set value. {default: "'exe path', icon index"}
      std::string key(info.uri);
      key.append("\\DefaultIcon");

      if (reg.Create(HKEY_CLASSES_ROOT, key.c_str()) == ERROR_SUCCESS) {
        std::string value("\"");
        value.append(info.executablePath);
        value.append("\", 0");
        reg.SetStringValue(NULL, value.c_str());
      }

      // create subkey('shell/open/command') and set value. {default: "'exe path' '%1'"}
      key.clear();
      key.assign(info.uri);
      key.append("\\shell\\open\\command");

      if (reg.Create(HKEY_CLASSES_ROOT, key.c_str()) == ERROR_SUCCESS) {
        std::string value("\"");
        value.append(info.executablePath);
        value.append("\" \"%1\"");
        reg.SetStringValue(NULL, value.c_str());
      }
    }

    reg.Close();
  }

  // 2. Chrome(need to restart!!)
  {
    char buffer[MAX_PATH];
    if (::SHGetSpecialFolderPath(NULL, buffer, CSIDL_LOCAL_APPDATA, false)) {
      std::string filePath(buffer);
      filePath.append("\\Google\\Chrome\\User Data\\Local State");

      // read setting contents from 'Local State'
      bool rewrite = false;
      Json::Value root;
      std::ifstream in(filePath);

      // update setting if exists file
      if (in.good()) {
        in >> root;

        // "protocol_handler" : {
        //   "excluded_schemes" : {
        //     "some_protocol_name": false      // allowed protocol
        //   }
        // }
        Json::Value &protocol_handler = root["protocol_handler"];
        if (protocol_handler.type() != Json::nullValue) {
          Json::Value &excluded_schemes = protocol_handler["excluded_schemes"];
          if (excluded_schemes.type() != Json::nullValue) {
            excluded_schemes[info.uri.c_str()] = false;
            rewrite = true;
          }
        }
      }

      in.close();

      if (rewrite) {
        bool killed = KillChromeProcess();

        std::ofstream out(filePath);
        out << root;
        out.close();

        if (killed)
          LaunchChromeProcess();
      }
    }
  }
#endif
}

bool ProtocolHandler::KillChromeProcess() {
#ifdef _WIN32
  HANDLE hProcessHandle;
  ULONG nProcessID;

  HWND hwnd = ::FindWindow("Chrome_WidgetWin_1", NULL);
  ::GetWindowThreadProcessId(hwnd, &nProcessID);

  hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
  return ::TerminateProcess(hProcessHandle, 4) == TRUE ? true : false;
#endif
}

void ProtocolHandler::LaunchChromeProcess() {
#ifdef _WIN32
  ::ShellExecute(NULL, "open",
                 "C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe",
                 NULL, NULL, SW_SHOW);
#endif
}

} // namespace sys


