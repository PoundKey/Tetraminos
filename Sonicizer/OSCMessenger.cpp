#include "OSCMessenger.h"

#define ADDRESS "127.0.0.1"
#define PORT 8000

#define OUTPUT_BUFFER_SIZE 1024

using namespace std;

/*
Class for facilitating communication between the Fuser and Reaper through the OSC protocol.

Generates the instruments out of class data and stores this instrument information for
use in creating notes
*/

// REAPER OSC ACTION IDS
const int MIDI_CHANNEL_ASSIGNS[16] = 
{ 
	54376, 54377, 54378, 54379, 54380, 54381, 54382, 54383, 
	54384, 54385, 54386, 54387, 54388, 54389, 54390, 54391
};

const int OMNISPHERE_TEMPLATE_ADDS[10] = 
{
	53440, 53441, 53442, 53443, 53444,
	53445, 53446, 53447, 53448, 53449
};

// Musicical Scales
const vector<vector<int>> SCALES =
{
	{ 2, 2, 1, 2, 2, 2, 1 }, // Ionian
	{ 2, 1, 2, 2, 2, 1, 2 }, // Dorian
	{ 1, 2, 2, 2, 1, 2, 2 }, // Phyrigian
	{ 2, 2, 2, 1, 2, 2, 1 }, // Lydian
	{ 2, 1, 2, 2, 2, 1, 2 }, // Mixolydian
	{ 2, 1, 2, 2, 1, 2, 2 }, // Aeolian
	{ 1, 2, 2, 1, 2, 2, 2 }, // Locrian
	{ 2 }, // Whole Tone
	{ 3, 2, 1, 1, 3}, // Blues
	{ 2, 1, 2, 1, 2, 1, 2 }, // Octatonic 1
	{ 1, 2, 1, 2, 1, 2, 1 }, // Octatonic 2
	{1, 3, 2, 1, 2, 3}, // Persian
	{2, 2, 3, 3, 1}, // Prometheus
	{2, 1, 3, 1, 1, 3}, // Algerian
	{1, 3, 2, 2, 1, 3}, // Flamenco
	{2, 1, 3, 1, 1, 2} // Gypsy
};

bool OSCMessenger::createInstruments(map<string, ClassProfile> classMap, vector<vector<string>> inheritanceTree, vector<set<string>> dependencyTree){

	int inheritanceFamilyCounter = 0;
	for (vector<vector<string>>::const_iterator i = inheritanceTree.begin(); i != inheritanceTree.end(); ++i)
	{
		for (vector<string>::const_iterator j = (*i).begin(); j != (*i).end(); ++j)
		{
			int registerInterval = 128/(*j).length();			 
			int registerBooster = rand() % 60;
			map<string, ClassProfile>::iterator pos = classMap.find((*j));
			if (pos != classMap.end())
			{
				InstrumentProfile ip;
				ip.setClassName(pos->first);
				ip.setChannel(inheritanceFamilyCounter);	
				ip.setRegisterBooster(registerBooster);
				
				vector<string> methods = pos->second.getMethods();
				for (vector<string>::const_iterator k = methods.begin(); k != methods.end(); ++k)
				{
					ip.getMethodToNoteMap()[(*k)] = 0;
				}

				instrumentMap[ip.getClassName()] = ip;
			}
			registerBooster += registerInterval;
			registerBooster %= 128;
		}
		inheritanceFamilyCounter++;
	}

	int dependencyFamilyCounter = 0;
	for (vector<set<string>>::const_iterator i = dependencyTree.begin(); i != dependencyTree.end(); ++i)
	{
		for (set<string>::const_iterator j = (*i).begin(); j != (*i).end(); ++j)
		{
			map<string, InstrumentProfile>::iterator pos = instrumentMap.find((*j));
			if (pos != instrumentMap.end())
			{
				pos->second.setTrackTemplate(dependencyFamilyCounter);
				int scaleIndex = 0;
				int scaleTracker = 0;
				for (map<string, int>::iterator k = pos->second.getMethodToNoteMap().begin(); k != pos->second.getMethodToNoteMap().end(); ++k)
				{
					
					k->second = (pos->second.getRegisterBooster() + scaleTracker) % 128;
					scaleTracker += SCALES.at(dependencyFamilyCounter).at(scaleIndex);
					scaleIndex++;
					scaleIndex %= SCALES.at(dependencyFamilyCounter).size();
				}
			}
		}
		dependencyFamilyCounter++;
		if (dependencyFamilyCounter > 10)
			dependencyFamilyCounter = 0;
	}

	for (map<string, InstrumentProfile>::iterator i = instrumentMap.begin(); i != instrumentMap.end(); ++i){
		//sendAddInstrumentCommand(i->second.getChannel(), i->second.getTrackTemplate());
	}

	return true;
}

/*
	Sends a command to Reaper to add a new templated track with Omnisphere pre-loaded
	and assigns it to the appropriate MIDI channel to receive notes 
*/
void OSCMessenger::sendAddInstrumentCommand(int channel, int templateNumber){
	UdpTransmitSocket transmitSocket(IpEndpointName(ADDRESS, PORT));
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

	p << osc::BeginBundleImmediate
		<< osc::BeginMessage("i/action") << OMNISPHERE_TEMPLATE_ADDS[templateNumber]
		<< osc::EndMessage 
		<< osc::BeginMessage("i/action") << MIDI_CHANNEL_ASSIGNS[channel]
		<< osc::EndBundle;

	transmitSocket.Send(p.Data(), p.Size());
}

/*
Fetches note associated with the given function in the given class
*/
int OSCMessenger::getNoteFromMap(string clas, string func, map<string, InstrumentProfile>::iterator instrumentToPlay){

	map<string, int> noteMap = instrumentToPlay->second.getMethodToNoteMap();
	map<string, int>::const_iterator noteToPlay = noteMap.find(func);

	// function not found!
	if (noteToPlay == noteMap.end()){
		printf("Note not found for function: %s", func.c_str());
		return -1;
	}

	int note = noteToPlay->second;
	note += instrumentToPlay->second.getRegisterBooster();

	// invalid note!
	if (note < 0 || note > 128){
		printf("Note for function %s is out of MIDI range", func.c_str());
		return -1;
	}
	return note;
}

/*
Sends an OSC message to Reaper with instructions to turn a note on or off on a particular MIDI channel
*/
void OSCMessenger::sendMIDINote(int note, int velocity, int channel){
	UdpTransmitSocket transmitSocket(IpEndpointName(ADDRESS, PORT));
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

	p << osc::BeginBundleImmediate
		<< osc::BeginMessage("i/vkb_midi") << channel << "note"<< note
		<< velocity << osc::EndMessage
		<< osc::EndBundle;

	transmitSocket.Send(p.Data(), p.Size());
}

/*
Triggers a note on event in Omnisphere
*/
void OSCMessenger::playNote(string clas, string func)
{
	map<string, InstrumentProfile>::iterator instrumentToPlay = instrumentMap.find(clas);
	// instrument not found!
	if (instrumentToPlay == instrumentMap.end()){
		printf("Instrument not found for class: %s", clas.c_str());
		return;
	}
	int note = getNoteFromMap(clas, func, instrumentToPlay);
	if (note == -1) return;
	sendMIDINote(note, 100, instrumentToPlay->second.getChannel());
}

/*
Triggers a note off event in Omnisphere
*/
void OSCMessenger::stopNote(string clas, string func)
{
	map<string, InstrumentProfile>::iterator instrumentToPlay = instrumentMap.find(clas);
	// instrument not found!
	if (instrumentToPlay == instrumentMap.end()){
		printf("Instrument not found for class: %s", clas.c_str());
		return;
	}
	int note = getNoteFromMap(clas, func, instrumentToPlay);
	if (note == -1) return;
	sendMIDINote(note, 0, instrumentToPlay->second.getChannel());
}

map<string, InstrumentProfile> OSCMessenger::getInstrumentMap(){
	return instrumentMap;
}
	
/**
int main(int argc, char* argv[])
{
    
	UdpTransmitSocket transmitSocket(IpEndpointName(ADDRESS, PORT));
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

	p << osc::BeginBundleImmediate
		<< osc::BeginMessage("i/action") << 40685
		<< osc::EndMessage
		<< osc::EndBundle;

	transmitSocket.Send(p.Data(), p.Size());
    

}
**/
