#pragma once
#include <string>

namespace sys {

struct ProtocolInfo {
  std::string uri;                            // URI scheme
  std::string displayName;                    // Display name of Protocol
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