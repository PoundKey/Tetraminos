#include "DynClassInfo.h"
DynClassInfo::DynClassInfo() {

}

DynClassInfo::DynClassInfo(boost::posix_time::ptime ts, std::string cn, std::string funcName, bool on_off) {
	timestamp = ts;
	className = cn;
	functionName = funcName;
	on = on_off;
}

std::string DynClassInfo::getFunctionName() {
	return functionName;
}

boost::posix_time::ptime DynClassInfo::getTimeStamp() {
	return timestamp;
}

std::string DynClassInfo::getClassName() {
	return className;
}

bool DynClassInfo::getOn() {
	return on;
}

void DynClassInfo::printClassInfo() {
	std::string timestampAsString = boost::posix_time::to_simple_string(timestamp);
	std::cout << "Timestamp: " << timestampAsString << " - Class: " << className <<
	" - Function: " << functionName << " - on: " << on << "\n";
}