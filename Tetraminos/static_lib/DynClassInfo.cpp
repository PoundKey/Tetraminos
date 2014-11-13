#include "DynClassInfo.h"

DynClassInfo(time_t ts, std::string cn, std::string funcName, bool on_off) {
	timestamp = ts;
	className = cn;
	functionName = funcName;
	on = on_off;
}