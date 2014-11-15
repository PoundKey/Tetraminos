#include "InstrumentProfile.h"

using namespace std;

void InstrumentProfile::setClassName(std::string prof) {
	className = prof;
}

void InstrumentProfile::setInheritanceFamily(int fam) {
	inheritanceFamily = fam;
}

void InstrumentProfile::setTrackTemplate(int fam) {
	trackTemplate = fam;
}

void InstrumentProfile::setRegisterBooster(int boost) {
	registerBooster = boost;
}

void InstrumentProfile::setChannel(int chan) {
	channel = chan;
}

string InstrumentProfile::getClassName() const {
	return className;
}
int InstrumentProfile::getTrackTemplate(){
	return trackTemplate;
}
int InstrumentProfile::getInheritanceFamily(){
	return inheritanceFamily;
}
int InstrumentProfile::getRegisterBooster(){
	return registerBooster;
}
int InstrumentProfile::getChannel(){
	return channel;
}
map<string, int>& InstrumentProfile::getMethodToNoteMap() {
	return methodToNoteMap;
}