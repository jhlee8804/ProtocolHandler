// RegisterProtocol.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>

#ifdef _WIN32
#include <atlbase.h>
#endif

#define APP_URI             _T("myapp")

#define REG_KEY_DEFAULT_IC  APP_URI _T("\\DefaultIcon")
#define REG_KEY_COMMAND     APP_URI _T("\\shell\\open\\command")
#define REG_VAL_EXE_PATH    _T("D:\\workspace\\example\\CustomProtocol\\Debug\\myapp.exe")


// NOTE: 관리자 권한 필요. 프로젝트 우측 클릭 -> Properties -> Linker -> Manifest File에서
// UAC Execution level을 highestAvailable (/level='highestAvailable')으로 지정.


int main(int argc, const char* argv[]) {

  // Register to IE
  // http://msdn.microsoft.com/en-us/library/ie/aa767914(v=vs.85).aspx
#ifdef _WIN32	
  // set key if not exists
  ATL::CRegKey reg;
  if (reg.Open(HKEY_CLASSES_ROOT, APP_URI) != ERROR_SUCCESS) {
    if (reg.Create(HKEY_CLASSES_ROOT, APP_URI) == ERROR_SUCCESS) {
      // set value. {default value: "URL:MyApp Protocol"}
      reg.SetStringValue(NULL, _T("URL:MyApp Protocol"));

      // set value. {URL Protocol: ""}
      reg.SetStringValue(_T("URL Protocol"), _T(""));

      // create subkey('DefaultIcon')
      if (reg.Create(HKEY_CLASSES_ROOT, REG_KEY_DEFAULT_IC) == ERROR_SUCCESS) {
        // set value. {default value: "'exe path', icon index"}
        std::wstring value(_T("\""));
        value.append(REG_VAL_EXE_PATH);
        value.append(_T("\", 0"));
        reg.SetStringValue(NULL, value.c_str());
      }

      // create subkey('shell/open/command');
      if (reg.Create(HKEY_CLASSES_ROOT, REG_KEY_COMMAND) == ERROR_SUCCESS) {
        // set value. {default value: "'exe path' '%1'"}
        std::wstring value(_T("\""));
        value.append(REG_VAL_EXE_PATH);
        value.append(_T("\" \"%1\""));
        reg.SetStringValue(NULL, value.c_str());
      }
    }

    reg.Close();
  }
#endif

  return 0;
}
