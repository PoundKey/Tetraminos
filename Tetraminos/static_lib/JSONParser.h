#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "ClassProfile.cpp"
#include <iostream>
#include <map>
#include <cstdio>
//#include <windows.h>
//#include <tlhelp32.h>
// #include "Sonifier/OSCmessenger.cpp"
// #include "../../Fuser/DynamicAnalyser/DynamicRunner.h"

class JSONParser
{
public:
  int containsMatch(std::vector<std::vector<std::string> >, std::string, int&);
};