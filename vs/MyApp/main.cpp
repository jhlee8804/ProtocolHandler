// MyApp.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <cstdio>
#include <regex>


int main(int argc, const char* argv[]) {

  std::string input(argv[argc - 1]);
  std::cout << "input: " << input << std::endl;

  // get key&value from query string: http://stackoverflow.com/questions/2972186/cgicc-query-string-parsing
  std::regex pattern("([\\w+%]+)=([^&]*)");
  auto words_begin = std::sregex_iterator(input.begin(), input.end(), pattern);
  auto words_end = std::sregex_iterator();

  for (std::sregex_iterator i = words_begin; i != words_end; i++) {
    std::string key = (*i)[1].str();
    std::string value = (*i)[2].str();
    
    std::cout << key << ": " << value << std::endl;
  }

  std::getchar();

  return 0;
}

