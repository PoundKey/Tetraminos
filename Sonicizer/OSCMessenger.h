#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "Fuser/ClassProfile.cpp"
#include "InstrumentProfile.h"
#include <vector>
#include <map>


using namespace std;

class OSCMessenger
{
	map<string, InstrumentProfile> instrumentMap;

private:
	int getNoteFromMap(string, string, map<string, InstrumentProfile>::iterator);
	void sendMIDINote(int, int, int);
	void sendAddInstrumentCommand(int, int);
public:
	OSCMessenger();
	bool createInstruments(map<string, ClassProfile>, vector<vector<string>>, vector<vector<string>>);
	void playNote(string, string);
	void stopNote(string, string);

};
