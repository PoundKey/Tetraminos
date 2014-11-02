#include <vector>
#include <map>

using namespace std;

class InstrumentProfile {
	std::string className;
	std::map<string, int> methodToNoteMap;
	int inheritanceFamily;
	int trackTemplate;
	int registerBooster;
	int channel;

public:
	map<string, int> getMethodToNoteMap() const;

	string getClassName() const;
	int getInheritanceFamily();
	int getTrackTemplate();
	int getRegisterBooster();
	int getChannel();

	void setClassName(string);
	void setInheritanceFamily(int);
	void setTrackTemplate(int);
	void setRegisterBooster(int);
	void setChannel(int);

};

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
map<string, int> InstrumentProfile::getMethodToNoteMap() const{
	return methodToNoteMap;
}