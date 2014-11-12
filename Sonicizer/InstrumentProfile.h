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