
#include <vector>
#include <map>
using namespace std;

class InstrumentProfile {
	std::string className;
	std::map<string, int> methodToNoteMap;
	int inheritanceFamily;
	int dependencyFamily;
	int registerBooster;
	int channel;
public:
	map<string, int> getMethodToNoteMap() const;

	string getClassName() const;
	int getInheritanceFamily();
	int getDependencyFamily();
	int getRegisterBooster();
	int getChannel();

	void setClassName(string);
	void setInheritanceFamily(int);
	void setDependencyFamily(int);
	void setRegisterBooster(int);
	void setChannel(int);

};

void InstrumentProfile::setClassName(std::string prof) {
	className = prof;
}

void InstrumentProfile::setInheritanceFamily(int fam) {
	inheritanceFamily = fam;
}

void InstrumentProfile::setDependencyFamily(int fam) {
	dependencyFamily = fam;
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
int InstrumentProfile::getDependencyFamily(){
	return dependencyFamily;
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