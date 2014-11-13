#include <string>
#include <ctime>

class DynClassInfo {
public:
	DynClassInfo(time_t ts, std::string cn, std::string funcName, bool on_off);
private:
	time_t timestamp;
	std::string className;
	std::string functionName;
	bool on;
};