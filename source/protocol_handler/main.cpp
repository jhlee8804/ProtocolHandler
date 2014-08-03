// RegisterProtocol.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <fstream>
#include "json/json.h"


#define APP_URI               L"myapp"


#ifdef _WIN32
#include <atlbase.h>
#include <ShlObj.h>

#define REG_KEY_DEFAULT_IC    APP_URI L"\\DefaultIcon"
#define REG_KEY_COMMAND       APP_URI L"\\shell\\open\\command"
#define REG_VAL_EXE_PATH      L"D:\\workspace\\example\\ProtocolHandler\\Debug\\myapp.exe"
#endif


// NOTE: 관리자 권한 필요. 프로젝트 우측 클릭 -> Properties -> Linker -> Manifest File에서
// UAC Execution level을 highestAvailable (/level='highestAvailable')으로 지정.

bool KillChromeProcess();
void LaunchChromeProcess();

int main(int argc, const char* argv[]) {

  // Register custom protocol handler - Windows  
#ifdef _WIN32	
  // 1. IE, FireFox
  {
    // http://msdn.microsoft.com/en-us/library/ie/aa767914(v=vs.85).aspx
    ATL::CRegKey reg;
    if (reg.Create(HKEY_CLASSES_ROOT, APP_URI) == ERROR_SUCCESS) {
      // set value. {default value: "URL:MyApp Protocol"}
      reg.SetStringValue(NULL, L"URL:MyApp Protocol");

      // set value. {URL Protocol: ""}
      reg.SetStringValue(L"URL Protocol", L"");

      // create subkey('DefaultIcon')
      if (reg.Create(HKEY_CLASSES_ROOT, REG_KEY_DEFAULT_IC) == ERROR_SUCCESS) {
        // set value. {default value: "'exe path', icon index"}
        std::wstring value(L"\"");
        value.append(REG_VAL_EXE_PATH);
        value.append(L"\", 0");
        reg.SetStringValue(NULL, value.c_str());
      }

      // create subkey('shell/open/command');
      if (reg.Create(HKEY_CLASSES_ROOT, REG_KEY_COMMAND) == ERROR_SUCCESS) {
        // set value. {default value: "'exe path' '%1'"}
        std::wstring value(L"\"");
        value.append(REG_VAL_EXE_PATH);
        value.append(L"\" \"%1\"");
        reg.SetStringValue(NULL, value.c_str());
      }
    }

    reg.Close();
  }

  // 2. Chrome(need to restart!!)
  {
    wchar_t buffer[MAX_PATH];
    if (::SHGetSpecialFolderPath(NULL, buffer, CSIDL_LOCAL_APPDATA, false)) {
      std::wstring filePath(buffer);
      filePath.append(L"\\Google\\Chrome\\User Data\\Local State");

      // read setting contents from 'Local State'    
      bool rewrite = false;
      Json::Value root;
      std::ifstream in(filePath);

      // update setting if exists file
      if (in.good()) {
        in >> root;

        // "protocol_handler" : {
        //   "excluded_schemes" : {
        //     ...
        //   }
        // }
        Json::Value &protocol_handler = root["protocol_handler"];
        if (protocol_handler.type() != Json::nullValue) {
          Json::Value &excluded_schemes = protocol_handler["excluded_schemes"];
          if (excluded_schemes.type() != Json::nullValue) {
            // wchar -> char
            std::wstring appUri(APP_URI);
            excluded_schemes[std::string(appUri.begin(), appUri.end())] = false;
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

  return 0;
}

bool KillChromeProcess() {
#ifdef _WIN32
  HANDLE hProcessHandle;
  ULONG nProcessID;
  
  HWND hwnd = ::FindWindow(L"Chrome_WidgetWin_1", NULL);
  ::GetWindowThreadProcessId(hwnd, &nProcessID);

  hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
  return ::TerminateProcess(hProcessHandle, 4) == TRUE ? true : false;
#endif
}

void LaunchChromeProcess() {
#ifdef _WIN32
  ::ShellExecute(NULL, L"open", 
    L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe", 
    NULL, NULL, SW_SHOW);
#endif
}