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
  // Register custom protocol handler to system
  virtual void Register(ProtocolInfo &info);

  // You need to Chrome restart if the your protocol must be registered
  bool IsRunningChromeProcess();
  bool KillChromeProcess();
  void LaunchChromeProcess();
};

} // namespace sys