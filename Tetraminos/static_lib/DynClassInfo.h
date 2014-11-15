#include <string>
#include <ctime>
#include "boost/date_time/posix_time/posix_time.hpp"

class DynClassInfo {
public:
	DynClassInfo();
	DynClassInfo(boost::posix_time::ptime ts, std::string cn, std::string funcName, bool on_off);
	std::string getFunctionName();
	boost::posix_time::ptime getTimeStamp();
	std::string getClassName();
	bool getOn();
	void printClassInfo();

private:
	boost::posix_time::ptime timestamp;
	std::string className;
	std::string functionName;
	bool on;
};