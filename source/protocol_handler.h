#pragma once
#include <string>

namespace sys {

struct ProtocolInfo {
  std::string uri;                            // URI scheme. 'myapp' of 'myapp:?key=value'
  std::string displayName;                    // Display name of protocol
  std::string executablePath;                 // Your App.exe path
};

class ProtocolHandler {
public:
  virtual void Register(ProtocolInfo &info);

protected:
  bool KillChromeProcess();
  void LaunchChromeProcess();
};

} // namespace sys