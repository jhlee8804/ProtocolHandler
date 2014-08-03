// RegisterProtocol.cpp : Defines the entry point for the console application.
//

#include <protocol_handler.h>

int main(int argc, const char* argv[]) {
  sys::ProtocolInfo info;
  info.uri = "myapp";
  info.displayName = "MyApp";
  info.executablePath = "D:\\workspace\\my\\ProtocolHandler\\build\\win\\Debug\\MyApp.exe";

  sys::ProtocolHandler handler;
  handler.Register(info);

  // launch app from web browser:
  // <a href="myapp?key=value">Launch</a>
}