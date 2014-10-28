#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "Fuser/ClassProfile.cpp"
#include "InstrumentProfile.cpp"

using namespace std;

#define ADDRESS "127.0.0.1"
#define PORT 8000

#define OUTPUT_BUFFER_SIZE 1024

/*
Class for facilitating communication between the Fuser and Reaper through the OSC protocol.

Generates the instruments out of class data and stores this instrument information for
use in creating notes
*/

class OSCMessenger
{
	map<string, InstrumentProfile> instrumentMap;
	private:
		int getNoteFromMap(string, string, map<string, InstrumentProfile>::const_iterator);
		void sendMIDINote(int, int, int);
		void sendAddInstrumentCommand(int, int);
	public:
		OSCMessenger();		
		map<string, InstrumentProfile> createinstrumentMap(std::vector<ClassProfile>);
		void playNote(string, string);
		void stopNote(string, string);
		
};

// REAPER OSC ACTION IDS
const int MIDI_CHANNEL_ASSIGNS[] = 
{ 
	54376, 54377, 54378, 54379, 54380, 54381, 54382, 54383, 
	54384, 54385, 54386, 54387, 54388, 54389, 54390, 54391
};

const int OMNISPHERE_TEMPLATE_ADDS[] = 
{
	53440, 53441, 53442, 53443, 53444,
	53445, 53446, 53447, 53448, 53449
};

map<string,InstrumentProfile> OSCMessenger::createinstrumentMap(std::vector<ClassProfile> listClasses){

	map<string, ClassProfile> classMap;
	for (vector<ClassProfile>::const_iterator i = listClasses.begin(); i != listClasses.end(); ++i)
	{
		classMap[(*i).getProfile] = (*i);
	}

	
	int inheritanceFamilyCounter = 0;
	
	//INHERITANCE-BASED INSTRUMENT CONSTRUCTION
	for (map<string, ClassProfile>::const_iterator i = classMap.begin(); i != classMap.end(); ++i)
	{
		vector<string> listOfInheritances = i->second.getInheritance;
		
		// focuses on classes with inheritances to start instrument construction
		if (!listOfInheritances.empty)
		{
			// starts the instrument profile of the class pointed to by the iterator 
			InstrumentProfile ip;
			ip.setClassName = i->second.getProfile;
			ip.setInheritanceFamily = inheritanceFamilyCounter;

			//iterates over focus class's list of inheritances
			for (vector<string>::const_iterator j = listOfInheritances.begin(); j != listOfInheritances.end(); ++j){
				
				// finds the class that is pointed to in the inheritance list
				map<string, ClassProfile>::const_iterator classPos = classMap.find((*j));
				
				// temporary structure for remembering new instruments
				vector<InstrumentProfile> newInstrumentMemory;

				if (classPos != classMap.end())
				{
					//checks if this class already has a created instrument (meaning some other class inherits it)
					map<string, InstrumentProfile>::const_iterator inPos = instrumentMap.find(classPos->first);
					
					// if it has an instrument, group all classes in the inheritance relationship into a inheritance family
					if (inPos != instrumentMap.end())
					{
						ip.setInheritanceFamily = inPos->second.getInheritanceFamily;
						
						if (!newInstrumentMemory.empty)
						{
							//resets the new instrument memory's inheritances
							for (vector<InstrumentProfile>::iterator k = newInstrumentMemory.begin(); k != newInstrumentMemory.end(); ++k)
							{
								k->setInheritanceFamily(inPos->second.getInheritanceFamily);
							}
						}
					}
					else{
						// creates a new instrument for the superclass and groups it
						// into the same inheritance family as its subclass
						InstrumentProfile newIP;
						newIP.setClassName(classPos->first);
						newIP.setInheritanceFamily(ip.getInheritanceFamily);

						// adds the new instrument to the temporary memory structure
						newInstrumentMemory.push_back(newIP);
					}
				}

				// add instruments from memory to larger map
				for (vector<InstrumentProfile>::const_iterator k = newInstrumentMemory.begin(); k != newInstrumentMemory.end(); ++k)
				{
					instrumentMap[k->getClassName] = (*k);
				}
			}
			
			// add focus class's instrument
			instrumentMap[ip.getClassName] = ip;			
			
			// starts a new family
			inheritanceFamilyCounter++;
		}
	}

	// iterate back through to look for classes that haven't been converted to instruments yet
	for (map<string, ClassProfile>::const_iterator i = classMap.begin(); i != classMap.end(); ++i)
	{
		vector<string> listOfInheritances = i->second.getInheritance;

		if (listOfInheritances.empty && instrumentMap.find(i->first) == instrumentMap.end())
		{		
			// creates an instrument for the class 
			InstrumentProfile ip;
			ip.setClassName = i->second.getProfile;
			ip.setInheritanceFamily = inheritanceFamilyCounter;
			instrumentMap[ip.getClassName] = ip;

			// starts a new family
			inheritanceFamilyCounter++;
		}
	}	


	// GROUPING BY DEPENDENCIES
	int dependencyFamilyCounter = 0;

	for (map<string, ClassProfile>::const_iterator i = classMap.begin(); i != classMap.end(); ++i)
	{
		map<string, InstrumentProfile>::iterator instrument = instrumentMap.find(i->first);
		
		if (instrument->second.getDependencyFamily == NULL)
		{
			instrument->second.getDependencyFamily = dependencyFamilyCounter;
		}
		
		vector<string> listOfDependencies = i->second.getDependency;

		for (vector<string>::const_iterator j = listOfDependencies.begin(); j != listOfDependencies.end(); ++j)
		{		
			map<string, InstrumentProfile>::iterator pos = instrumentMap.find((*j));
			if (pos->second.getDependencyFamily != NULL){}							
				instrument->second.getDependencyFamily = pos->second.getDependencyFamily;
		}
		for (vector<string>::const_iterator j = listOfDependencies.begin(); j != listOfDependencies.end(); ++j)
		{
			map<string, InstrumentProfile>::iterator pos = instrumentMap.find((*j));
			if (pos != instrumentMap.end())
				pos->second.setDependencyFamily(instrument->second.getDependencyFamily);
		}

		dependencyFamilyCounter++;
	}

	// TODO: CONTINUE WITH INSTRUMENT CREATION .....
	for (int i = 0; i < inheritanceFamilyCounter; i++){
		
	}

	return instrumentMap;
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
int OSCMessenger::getNoteFromMap(string clas, string func, map<string, InstrumentProfile>::const_iterator instrumentToPlay){

	map<string, int> noteMap = instrumentToPlay->second.getMethodToNoteMap;
	map<string, int>::const_iterator noteToPlay = noteMap.find(func);

	// function not found!
	if (noteToPlay == noteMap.end()){
		printf("Note not found for function: %s", func);
		return -1;
	}

	int note = noteToPlay->second;
	note += instrumentToPlay->second.getRegisterBooster;

	// invalid note!
	if (note < 0 || note > 128){
		printf("Note for function %s is out of MIDI range", func);
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
	map<string, InstrumentProfile>::const_iterator instrumentToPlay = instrumentMap.find(clas);
	// instrument not found!
	if (instrumentToPlay == instrumentMap.end()){
		printf("Instrument not found for class: %s", clas);
		return;
	}
	int note = getNoteFromMap(clas, func, instrumentToPlay);
	if (note == -1) return;
	sendMIDINote(note, 100, instrumentToPlay->second.getChannel);
}

/*
Triggers a note off event in Omnisphere
*/
void OSCMessenger::stopNote(string clas, string func)
{
	map<string, InstrumentProfile>::const_iterator instrumentToPlay = instrumentMap.find(clas);
	// instrument not found!
	if (instrumentToPlay == instrumentMap.end()){
		printf("Instrument not found for class: %s", clas);
		return;
	}
	int note = getNoteFromMap(clas, func, instrumentToPlay);
	if (note == -1) return;
	sendMIDINote(note, 0, instrumentToPlay->second.getChannel);
}

/*
int main(int argc, char* argv[])
{
    
    

}

*/