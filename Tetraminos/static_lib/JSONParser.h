#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "ClassProfile.cpp"
#include <iostream>
#include <map>
#include <cstdio>
#include <algorithm>
#include <set>	
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
//#include <windows.h>
//#include <tlhelp32.h>
//#include "Sonifier/OSCmessenger.cpp"
//#include "../../Fuser/DynamicAnalyser/DynamicRunner.h"

class JSONParser
{
public:
  int containsMatch(std::vector<std::vector<std::string> >, std::string, int&);
  void addDependencies(ClassProfile::ClassProfile, std::map<std::string, ClassProfile::ClassProfile>&, std::set<std::string>&, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::set<std::string> >&, std::set<int>&);
  int getProcIdByName(string);
};