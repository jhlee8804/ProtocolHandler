// RegisterProtocol.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <algorithm>
#include <protocol_handler.h>


int main(int argc, const char* argv[]) {
  sys::ProtocolInfo info;
  info.uri = "myapp";
  info.displayName = "MyApp";
  info.executablePath = "F:\\workspace\\my\\ProtocolHandler\\build\\win\\Debug\\MyApp.exe";

  sys::ProtocolHandler handler;
  handler.Register(info);

  if (handler.IsRunningChromeProcess()) {
    std::cout << "You need to restart Google Chrome. Are you sure? (y / n)" << std::endl;

    std::string input("");
    std::cin >> input;

    // make lowercase
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (input.compare("y") == 0) {
      handler.KillChromeProcess();
      handler.LaunchChromeProcess();
    }
  }

  // launch MyApp from web browser:
  // <a href="myapp?key=value">Launch</a>

  return 0;
}