/**
#include "JSONParser.h"
//#include <iterator>
// Get PID of process running http://proswdev.blogspot.ca/2012/02/get-process-id-by-name-in-linux-using-c.html
int JSONParser::getProcIdByName(string procName)
{
    int pid = -1;

    // Open the /proc directory
    DIR *dp = opendir("/proc");
    if (dp != NULL)
    {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0)
            {
                // Read contents of virtual /proc/{pid}/cmdline file
                string cmdPath = string("/proc/") + dirp->d_name + "/cmdline";
                ifstream cmdFile(cmdPath.c_str());
                string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    // Compare against requested process name
                    if (procName == cmdLine)
                        pid = id;
                }
            }
        }
    }

    closedir(dp);

    return pid;
}

// To determine if a vector contains a Matching string
int JSONParser::containsMatch(std::vector<std::vector<std::string> > block, std::string lookingFor, int& placeInt){

 for(std::vector<std::vector<string> >::size_type f = 0; f != block.size(); f++){ 
    for(std::vector<string>::size_type m = 0; m != block[f].size(); m++){
        if(block[f][m].compare(lookingFor) == 0){
            placeInt = f;
            return f; 
        }
    }
}
return -1;
}

void JSONParser::addDependencies(ClassProfile first, std::map<std::string, ClassProfile>& dmap, std::set<std::string>& tad, std::vector<std::string>& depvec, std::vector<std::string>& convec, std::vector<std::set<std::string> >& dtree, std::set<int>& pov){
    if(find(convec.begin(), convec.end(), first.getProfile()) == convec.end()){
        //std::cout << "I'm Here and first is: " << first.getProfile() << endl;
        tad.insert(first.getProfile());
        convec.push_back(first.getProfile());
        std::vector<std::string> deps = first.getDependency();
        for(std::vector<string>::size_type i = 0; i != deps.size(); i++){
            ClassProfile newFirst = dmap[deps[i]]; 
            JSONParser::addDependencies(newFirst, dmap, tad, depvec, convec, dtree, pov);

        }
    }else{
        // Loops to see if already in a different vector in the tree, if so we need to add the position so we can add this array to it. 
        for(std::vector<std::set<std::string> >::size_type k = 0; k < dtree.size(); k++){
            if(find(dtree[k].begin(), dtree[k].end(), first.getProfile()) != dtree[k].end())
                pov.insert(k);
        }
    }

}

int main(int argc, char * argv[]) {
    std::map<std::string,std::string> classmap;
    std::map<std::string,std::string>::iterator it;
    std::map<std::string,ClassProfile> profilemap;

    JSONParser jp;
    int pid = jp.getProcIdByName(argv[1]);
    cout << "pid: " << pid << endl;
    std::map<int, std::vector<ClassProfile> > inheritanceCounts;
    std::vector<ClassProfile>::iterator icit;
    int maxInheritances = 0; 

    std::vector<std::string> dependencyVector;


    // Name of JSON File to be parsed
    // std::string jsonName = argv[2];


    //TODO: IMPORT JSON FILE
    // http://stackoverflow.com/questions/18107079/rapidjson-working-code-for-reading-document-from-file
    FILE * pFile = fopen(argv[2] , "r");
    rapidjson::FileStream is(pFile);
    rapidjson::Document d;
    d.ParseStream<0>(is);

    // This is test data
    // const char myJson[] = "{\"staticInfo\":[{\"className\""cBlood\", \"method\":[\"cBlood\", \"update\", \"newBlood\", \"oneNewBlood\", \"reset\"],\"field\"  \"bloodparts\",\"BLOODcount\"],\"inheritance\":[],\"dependency\":[]},{\"className\""cBlood\", \"method\":[\"cBlood\", \"update\", \"newBlood\", \"oneNewBlood\", \"reset\"],\"field\"  \"bloodparts\",\"BLOODcount\"],\"inheritance\":[],\"dependency\":[]}]}";


    rapidjson::Document child;
    const rapidjson::Value& b = d["staticInfo"];
    assert(b.IsArray());
    std::vector<ClassProfile> listClasses;
    for (rapidjson::SizeType i = 0; i < b.Size(); i++){ // rapidjson uses SizeType instead of size_t.

            //New cl object
        ClassProfile cl;
        const rapidjson::Value& temp = b[i];
        std::string className = temp["className"].GetString();
        std::string tempinheritance = temp["inheritance"].GetString();
        if (tempinheritance.compare("N/A") != 0){
          cl.setInheritance(tempinheritance);
      }
      std::cout << className << "\n";
            // Add classname key to profile name
      profilemap[className];
            // Create Profile
      cl.setProfile(className);
      dependencyVector.push_back(className);
            // Line space for instrument creationtempField[h].GetString() to access field

      const rapidjson::Value& tempMethod = temp["method"];
      for (rapidjson::SizeType j = 0; j < tempMethod.Size(); j++){
        printf("Method[%d] = %s\n", j, tempMethod[j].GetString());

        cl.setMethod(tempMethod[j].GetString());

                // Add method to and class to hashmap
        classmap[tempMethod[j].GetString()] = temp["className"].GetString();

                // Line space for instrument creationtempField[h].GetString() to access field

    }
    const rapidjson::Value& tempField = temp["field"];
    for (rapidjson::SizeType h = 0; h < tempField.Size(); h++){
        printf("Field[%d] = %s\n", h, tempField[h].GetString());

        cl.setField(tempField[h].GetString());
                // Line space for instrument creationtempField[h].GetString() to access field
    }
    const rapidjson::Value& tempDep = temp["dependency"];
    for (rapidjson::SizeType k = 0; k < tempDep.Size(); k++){
        printf("dependency[%d] = %s\n", k, tempDep[k].GetString());

        cl.setDependency(tempDep[k].GetString());
                // Line space for instrument creationtempField[h].GetString() to access field
    }


    listClasses.push_back(cl);
    profilemap[className] = cl;

            // Used Inheritance Groupings below
    if (!cl.getInheritance().empty()){
        maxInheritances = 1;
        inheritanceCounts[1].push_back(cl);
    } else {
        inheritanceCounts[0].push_back(cl);
    }

}



    // Construct Inheritance Groupings to pass for Instrument Creation
std::vector<std::vector<std::string> > inheritanceTree;
std::vector<ClassProfile> leftovers;
for (int i = 0; i <= maxInheritances; i++){
    if (i==0){
        for(std::vector<ClassProfile>::size_type h = 0; h != inheritanceCounts[i].size(); h++){
            std::vector<std::string> tempClassName;
            tempClassName.push_back(inheritanceCounts[i][h].getProfile());
            inheritanceTree.push_back(tempClassName);
        }
    }else {
        for(std::vector<ClassProfile>::size_type m = 0; m != inheritanceCounts[i].size(); m++){
            int placeInt2;
            if(0 < jp.containsMatch(inheritanceTree, inheritanceCounts[i][m].getInheritance(), placeInt2)){
                inheritanceTree[placeInt2].push_back(inheritanceCounts[i][m].getProfile());
            } else {
                leftovers.push_back(inheritanceCounts[i][m]);
            }
            
        }

        for(std::vector<ClassProfile>::size_type l = 0; l != leftovers.size(); l++){
            int placeInt1;
            if(0 < jp.containsMatch(inheritanceTree, leftovers[l].getInheritance(), placeInt1)){
                inheritanceTree[placeInt1].push_back(leftovers[l].getProfile());
                leftovers.erase(leftovers.begin()+l);
            }
        }
        
    }
}

     // This is to check that all Inheritances are grouped together properly    
std::cout << "This is the Inheritances" << endl;
for( std::vector<std::vector<std::string> >::const_iterator i = inheritanceTree.begin(); i != inheritanceTree.end(); ++i){
    std::cout << "Vector: ";
    std::cout << i - inheritanceTree.begin() << endl;
    for( std::vector<string>::const_iterator j = (*i).begin(); j != (*i).end(); ++j){
       std::cout << *j << ' ' << std::endl;
   }
}



    // Construct Dependency Groupings
    // std::map<std::string, ClassProfile> dependencyMap = profilemap;
std::vector<std::set<std::string> > dependencyTree;
std::vector<std::string> containerVec;
    //printf("Vector size: %lu", dependencyVector.size());
int count = 0;
for (std::vector<std::string>::const_iterator i = dependencyVector.begin(); i != dependencyVector.end(); ++i){

    std::map<std::string, ClassProfile>::iterator searchit = profilemap.find(*i);
    ClassProfile first = profilemap[(*i)];
    std::set<std::string> toAdd;
    if(find(containerVec.begin(), containerVec.end(), first.getProfile()) == containerVec.end()){
        std::set<int> posOfVecs;
        jp.addDependencies(first, profilemap, toAdd, dependencyVector, containerVec, dependencyTree, posOfVecs);
        std::cout << "Vector: " << count << endl;
        count = count + 1;
        for( std::set<string>::iterator p = toAdd.begin(); p != toAdd.end(); ++p){
           std::cout << *p << ' ' << std::endl;
       }

         // Merge Step 
       if(posOfVecs.size() > 0){
        std::set<int>::iterator setit = posOfVecs.begin();
        dependencyTree[*setit].insert(toAdd.begin(), toAdd.end());
        if(posOfVecs.size() > 1){
            for (std::set<int>::iterator k = std::next(posOfVecs.begin()); k != posOfVecs.end(); ++k){
                dependencyTree[*setit].insert(dependencyTree[*k].begin(), dependencyTree[*k].end());
                dependencyTree[*k].clear();
            }
        }
    }else{
        dependencyTree.push_back(toAdd);
    }
}
}
    //std::cout << "here?\n";
int counter = 0;
    //for(std::vector<std::set<std::string> >::const_iterator dtit = dependencyTree.begin(); dtit != dependencyTree.end(); ++dtit){
     //   if ((*dtit).size() == 0) { 
     //        dependencyTree.erase(dtit);
     //   }
//}
for(std::vector<std::set<std::string> >::const_iterator dtit = dependencyTree.begin(); dtit != dependencyTree.end(); ++dtit){
    std::cout << endl << "FinVector " << counter << endl;
    counter = counter + 1;
    for(std::set<std::string>::const_iterator sdtit = (*dtit).begin(); sdtit != (*dtit).end(); ++sdtit){
        std::cout << *sdtit << endl; 
    }
}

    //createInstruments();


      /*  // Not Working
        if(searchit != dependencyMap.end())
            {
        std::string depProfName = dependencyMap[(*i)].getProfile();
        std::vector<std::string> depDepNames = dependencyMap[(*i)].getDependency();
        depDepNames.insert(depDepNames.begin(), depProfName);
        dependencyMap.erase(*i);

        for(std::vector<std::string>::const_iterator j = depDepNames.begin() + 1; j != depDepNames.end(); ++j){
            std::vector<std::string> toAdd = dependencyMap[(*j)].getDependency();
            depDepNames.insert( depDepNames.end(), toAdd.begin(), toAdd.end() );
            dependencyMap.erase(*j);
        }

    }*/









    // TODO: Pass listClasses too Dyninst.
    //This is for Dyninst to use for iterating over it's functions
   /* std::cout << "This is printing from the list of Class" << endl;
    for( std::vector<ClassProfile>::const_iterator i = listClasses.begin(); i != listClasses.end(); ++i){
        std::cout << "This is profile: ";
        std::cout << (*i).getProfile() << endl;
        vector<std::string> methods = (*i).getMethods();
        for( std::vector<string>::const_iterator j = methods.begin(); j != methods.end(); ++j){
             std::cout << *j << ' ' << std::endl;
         }
        }*/




/*
    std::cout << "This is the Dependencies" << endl;
    for(std::vector<std::set<std::string> >::const_iterator k = dependencyTree.begin(); k != dependencyTree.end(); ++k){
        std::cout << "Vector: " << k - dependencyTree.begin() << endl;
        for( std::set<std::string>::const_iterator q = (*k).begin(); q != (*k).end(); ++k){
            std::cout << *q << ' '<< std::endl;
        }
    }
*/




    


// WINDOWS: GET Process ID of launched exe  http://stackoverflow.com/questions/865152/how-can-i-get-a-process-handle-by-its-name-in-c

    /*std::string exeName = argv[1];
    std::string pid;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (_stricmp(entry.szExeFile, exeName) == 0)
            {  
                //HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

                pid = entry.th32ProcessID;

                //CloseHandle(hProcess);
            }
        }
    }
*/
/**
    OSCMessenger messenger;
    std::cout << "ProfileMap size: " << profilemap.size() << "\n";
    std::cout << "InheritanceTree size: " << inheritanceTree.size() << "\n";
    std::cout << "dependencyTree size: " << dependencyTree.size() << "\n";
    if (messenger.createInstruments(profilemap, inheritanceTree, dependencyTree)){
        printf("Successfully created Instruments\n");
    } else {
        printf("Failed Instrument Creation\n");
    }
    std::map<std::string, InstrumentProfile> iMap = messenger.getInstrumentMap();
    typedef std::map<std::string, InstrumentProfile>::iterator itype;
   //for (itype i = iMap.begin(); i != iMap.end(); i++) {
    //std::cout << "hey!\n";
   // InstrumentProfile ip = i->second;
   // std::cout << "IP NAME: " << ip.getClassName() << "\n";
    //std::string name = i->first;
    //std::cout << "Name: " << name << "\n";
   //}
    
    **/
    /**

    // CloseHandle(snapshot);

    /** Let's just assume this has run and output a file for now **/
    //DynamicRunner *dr = new DynamicRunner(pid, listClasses);
    //dr->analyze();
    

    /**
    typedef std::map<std::string, std::vector<DynClassInfo> >::iterator it_type;
    for (it_type iterator = dynMap.begin(); iterator != dynMap.end(); iterator++) {
        std::vector<DynClassInfo> info = iterator->second;
        for (int i = 0; i < info.size(); i++) {
            // get the instrument profile
            DynClassInfo tinfo = info[i];
            tinfo.printClassInfo();
        }
    }
    **/


/* Experimental
    std::cout << "cBLood => " << classmap.find("cBlood") << '\n';
    std::cout << "loadlevel => " << classmap.find("loadlevel") << '\n';
    it = classmap.find("playerTouchRight");
    std::string s = it->second;
    std::cout << s << endl;
    */
    
    // Reference Code That You Can Ignore
    // assert(a.IsArray());
    // rapidjson::SizeType s = 0;
    // std::cout << a[s].GetString() << "\n";

    // //assert(d[\"1\"].IsObject()); 
    // const rapidjson::Value& b = d[\"classDate\"];
    // assert(b[\"1\"].IsObject());
    // const rapidjson::Value& a = b[\"1\"];
    // const rapidjson::Value& fields = a[\"fields\"];
    // assert(fields.IsArray());
    // for (rapidjson::SizeType i = 0; i < fields.Size(); i++) // rapidjson uses SizeType instead of size_t.    
    //         printf(\"a[%d] = %s\n\", i, fields[i].GetString());
//}

/////////////////



#include "JSONParser.h"
//#include <iterator>
// Get PID of process running http://proswdev.blogspot.ca/2012/02/get-process-id-by-name-in-linux-using-c.html
int JSONParser::getProcIdByName(std::string procName)
{
    int pid = -1;

// Open the /proc directory
    DIR *dp = opendir("/proc");
    if (dp != NULL)
    {
// Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
// Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0)
            {
// Read contents of virtual /proc/{pid}/cmdline file
                std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                ifstream cmdFile(cmdPath.c_str());
                std::string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
// Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(0, pos);
// Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
// Compare against requested process name
                    if (procName == cmdLine)
                        pid = id;
                }
            }
        }
    }

    closedir(dp);

    return pid;
}

// To determine if a vector contains a Matching std::string
int JSONParser::containsMatch(std::vector<std::vector<std::string> > block, std::string lookingFor, int& placeInt){

    for(std::vector<std::vector<std::string> >::size_type f = 0; f != block.size(); f++){
        for(std::vector<std::string>::size_type m = 0; m != block[f].size(); m++){
            if(block[f][m].compare(lookingFor) == 0){
                placeInt = f;
                return f;
            }
        }
    }
    return -1;
}

void JSONParser::addDependencies(ClassProfile first, std::map<std::string, ClassProfile>& dmap, std::set<std::string>& tad, std::vector<std::string>& depvec, std::vector<std::string>& convec, std::vector<std::set<std::string> >& dtree, std::set<int>& pov){
    if(find(convec.begin(), convec.end(), first.getProfile()) == convec.end()){
//std::cout << "I'm Here and first is: " << first.getProfile() << std::endl;
        tad.insert(first.getProfile());
        convec.push_back(first.getProfile());
        std::vector<std::string> deps = first.getDependency();
        for(std::vector<std::string>::size_type i = 0; i != deps.size(); i++){
            ClassProfile newFirst = dmap[deps[i]];
            JSONParser::addDependencies(newFirst, dmap, tad, depvec, convec, dtree, pov);

        }
    }else{
// Loops to see if already in a different vector in the tree, if so we need to add the position so we can add this array to it.
        for(std::vector<std::set<std::string> >::size_type k = 0; k < dtree.size(); k++){
            if(find(dtree[k].begin(), dtree[k].end(), first.getProfile()) != dtree[k].end())
                pov.insert(k);
        }
    }

}

void JSONParser::parseJSON(const rapidjson::Value& b,std::vector<ClassProfile>& listClasses,std::map<std::string,ClassProfile>& profilemap,std::map<int, std::vector<ClassProfile> >& inheritanceCounts,int& maxInheritances, std::vector<std::string>& dependencyVector, std::map<std::string,std::string>& classmap){
for (rapidjson::SizeType i = 0; i < b.Size(); i++){ // rapidjson uses SizeType instead of size_t.

//New cl object
    ClassProfile cl;
    const rapidjson::Value& temp = b[i];
    std::string className = temp["className"].GetString();
    std::string tempinheritance = temp["inheritance"].GetString();
    if (tempinheritance.compare("N/A") != 0){
        cl.setInheritance(tempinheritance);
    }
    std::cout << className << "\n";
// Add classname key to profile name
    profilemap[className];
// Create Profile
    cl.setProfile(className);
    dependencyVector.push_back(className);
// Line space for instrument creationtempField[h].GetString() to access field

    const rapidjson::Value& tempMethod = temp["method"];
    for (rapidjson::SizeType j = 0; j < tempMethod.Size(); j++){
        printf("Method[%d] = %s\n", j, tempMethod[j].GetString());

        cl.setMethod(tempMethod[j].GetString());

// Add method to and class to hashmap
        classmap[tempMethod[j].GetString()] = temp["className"].GetString();

// Line space for instrument creationtempField[h].GetString() to access field

    }
    const rapidjson::Value& tempField = temp["field"];
    for (rapidjson::SizeType h = 0; h < tempField.Size(); h++){
        printf("Field[%d] = %s\n", h, tempField[h].GetString());

        cl.setField(tempField[h].GetString());
// Line space for instrument creationtempField[h].GetString() to access field
    }
    const rapidjson::Value& tempDep = temp["dependency"];
    for (rapidjson::SizeType k = 0; k < tempDep.Size(); k++){
        printf("dependency[%d] = %s\n", k, tempDep[k].GetString());

        cl.setDependency(tempDep[k].GetString());
// Line space for instrument creationtempField[h].GetString() to access field
    }


    listClasses.push_back(cl);
    profilemap[className] = cl;

// Used Inheritance Groupings below
    if (!cl.getInheritance().empty()){
        maxInheritances = 1;
        inheritanceCounts[1].push_back(cl);
    } else {
        inheritanceCounts[0].push_back(cl);
    }

}
}
std::vector<std::vector<std::string> > JSONParser::constructInheritance(int maxInheritances,std::map<int, std::vector<ClassProfile> > inheritanceCounts){
    std::vector<std::vector<std::string> > inheritanceTree;
    std::vector<ClassProfile> leftovers;
    JSONParser jp;
    for (int i = 0; i <= maxInheritances; i++){
        if (i==0){
            for(std::vector<ClassProfile>::size_type h = 0; h != inheritanceCounts[i].size(); h++){
                std::vector<std::string> tempClassName;
                tempClassName.push_back(inheritanceCounts[i][h].getProfile());
                inheritanceTree.push_back(tempClassName);
            }
        }else {
            for(std::vector<ClassProfile>::size_type m = 0; m != inheritanceCounts[i].size(); m++){
                int placeInt2;
                if(0 < jp.containsMatch(inheritanceTree, inheritanceCounts[i][m].getInheritance(), placeInt2)){
                    inheritanceTree[placeInt2].push_back(inheritanceCounts[i][m].getProfile());
                } else {
                    leftovers.push_back(inheritanceCounts[i][m]);
                }

            }

            for(std::vector<ClassProfile>::size_type l = 0; l != leftovers.size(); l++){
                int placeInt1;
                if(0 < jp.containsMatch(inheritanceTree, leftovers[l].getInheritance(), placeInt1)){
                    inheritanceTree[placeInt1].push_back(leftovers[l].getProfile());
                    leftovers.erase(leftovers.begin()+l);
                }
            }

        }
    }
    return inheritanceTree;
}

void JSONParser::createInstrumentCommands(OSCMessenger messenger){
    DynamicParser parser;
    std::map<std::string, std::vector<DynClassInfo> > *dynMap = parser.parseFile("/home/james00794/Downloads/spongebob_doodleJump/dynamicOutput.txt");
    //if (!dynMap)
    //    std::cout << "NULL!\n";
    //std::cout << "Am I here?\n";
    typedef std::map<std::string, std::vector<DynClassInfo> >::iterator it_type;
    std::map<std::string, InstrumentProfile> instrumentMap = messenger.getInstrumentMap();
    //std::cout << "Am I here?\n";

    for (it_type i = dynMap->begin(); i != dynMap->end(); i++){
        //std::cout << "Am I here?\n";
        std::vector<DynClassInfo> classVec = i->second;
        for (int s = 0; s < classVec.size(); s++) {
            DynClassInfo oninfo = classVec.at(s);
            boost::posix_time::ptime onts = oninfo.getTimeStamp();
            DynClassInfo offinfo = classVec.at(s+1);
            boost::posix_time::ptime offts = offinfo.getTimeStamp();
            boost::posix_time::time_duration timeDuration = offts-onts;
//std::cout << "Total Time duration: " << timeDuration.total_milliseconds();
            std::string className = oninfo.getClassName();
            InstrumentProfile iprof = instrumentMap.at(className);
            std::string fullFuncName = oninfo.getFunctionName();
            int needle = fullFuncName.find("::");
            std::string ss = fullFuncName.substr(needle+2, fullFuncName.size());
            while (ss.find("::") != std::string::npos) {
                needle = ss.find("::");
                ss = ss.substr(needle+2, ss.size());
            }
//std::cout << " Func Name: " << ss;
            std::map<std::string, int> mtnm = iprof.getMethodToNoteMap();
//for (std::map<std::string, int>::iterator ite = mtnm.begin(); ite !=mtnm.end(); ite++) {
// std::string key = ite->first;
// std::cout << " instrument key: " << key << "\n";
//}
            int note = mtnm.at(ss);
//std::cout << " Note: " << note << "\n";
            s++;

            long totalTime = timeDuration.total_milliseconds();
            if (totalTime < 10) {
                totalTime = 10;
            }

// <note> <velocity> <channel> <duration> <track template>
            std::cout << note << ",100," << iprof.getChannel() << "," << totalTime <<
            "," << iprof.getTrackTemplate() << "," << onts << "\n";
        }
    }
}

std::vector<std::set<std::string> > JSONParser::constructDependency(std::map<std::string,ClassProfile>& profilemap, std::vector<std::string>& dependencyVector){
    std::vector<std::set<std::string> > dependencyTree;
    std::vector<std::string> containerVec;
    JSONParser jp;
//printf("Vector size: %lu", dependencyVector.size());
    int count = 0;
    for (std::vector<std::string>::const_iterator i = dependencyVector.begin(); i != dependencyVector.end(); ++i){

        std::map<std::string, ClassProfile>::iterator searchit = profilemap.find(*i);
        ClassProfile first = profilemap[(*i)];
        std::set<std::string> toAdd;
        if(find(containerVec.begin(), containerVec.end(), first.getProfile()) == containerVec.end()){
            std::set<int> posOfVecs;
            jp.addDependencies(first, profilemap, toAdd, dependencyVector, containerVec, dependencyTree, posOfVecs);
            std::cout << "Vector: " << count << std::endl;
            count = count + 1;
            for( std::set<std::string>::iterator p = toAdd.begin(); p != toAdd.end(); ++p){
                std::cout << *p << ' ' << std::endl;
            }

// Merge Step
            if(posOfVecs.size() > 0){
                std::set<int>::iterator setit = posOfVecs.begin();
                dependencyTree[*setit].insert(toAdd.begin(), toAdd.end());
                if(posOfVecs.size() > 1){
                    for (std::set<int>::iterator k = std::next(posOfVecs.begin()); k != posOfVecs.end(); ++k){
                        dependencyTree[*setit].insert(dependencyTree[*k].begin(), dependencyTree[*k].end());
                        dependencyTree[*k].clear();
                    }
                }
            }else{
                dependencyTree.push_back(toAdd);
            }
        }
    }
    return dependencyTree;
}

int main(int argc, char * argv[]) {
    std::map<std::string,std::string> classmap;
    std::map<std::string,std::string>::iterator it;
    std::map<std::string,ClassProfile> profilemap;
    JSONParser jp;
    int pid = jp.getProcIdByName(argv[1]);
    std::cout << "pid: " << pid << std::endl;
    std::map<int, std::vector<ClassProfile> > inheritanceCounts;
    std::vector<ClassProfile>::iterator icit;
    int maxInheritances = -1;
    std::vector<std::string> dependencyVector;
// Name of JSON File to be parsed
// std::string jsonName = argv[2];

//Import JSON File
// http://stackoverflow.com/questions/18107079/rapidjson-working-code-for-reading-document-from-file
    FILE * pFile = fopen(argv[2] , "r");
    rapidjson::FileStream is(pFile);
    rapidjson::Document d;
    d.ParseStream<0>(is);

// Parse complete JSON File filling all require data structures
// This is test data
// const char myJson[] = "{\"staticInfo\":[{\"className\""cBlood\", \"method\":[\"cBlood\", \"update\", \"newBlood\", \"oneNewBlood\", \"reset\"],\"field\" \"bloodparts\",\"BLOODcount\"],\"inheritance\":[],\"dependency\":[]},{\"className\""cBlood\", \"method\":[\"cBlood\", \"update\", \"newBlood\", \"oneNewBlood\", \"reset\"],\"field\" \"bloodparts\",\"BLOODcount\"],\"inheritance\":[],\"dependency\":[]}]}";
    rapidjson::Document child;
    const rapidjson::Value& b = d["staticInfo"];
    assert(b.IsArray());
    std::vector<ClassProfile> listClasses;
    jp.parseJSON(b,listClasses, profilemap, inheritanceCounts, maxInheritances, dependencyVector, classmap);


// Construct Inheritance Groupings to pass for Instrument Creation
    std::vector<std::vector<std::string> > inheritanceTree = jp.constructInheritance(maxInheritances, inheritanceCounts);

// Construct Dependency Groupings
// std::map<std::string, ClassProfile> dependencyMap = profilemap;
    std::vector<std::set<std::string> > dependencyTree = jp.constructDependency(profilemap, dependencyVector);



    OSCMessenger messenger;
    if (messenger.createInstruments(profilemap, inheritanceTree, dependencyTree)){
        printf("Successfully created Instruments\n");
    } else {
        printf("Failed Instrument Creation\n");
    }


    /** Let's just assume this has run and output a file for now
    DynamicRunner *dr = new DynamicRunner(pid, listClasses);
    dr->analyze();
    **/
    jp.createInstrumentCommands(messenger);

/**

    DynamicParser parser;
    std::map<std::string, std::vector<DynClassInfo> > *dynMap = parser.parseFile("/home/james00794/Downloads/spongebob_doodleJump/dynamicOutput.txt");
    typedef std::map<std::string, std::vector<DynClassInfo> >::iterator it_type;
    std::map<std::string, InstrumentProfile> instrumentMap = messenger.getInstrumentMap();
    boost::posix_time::ptime earliestTime(boost::posix_time::second_clock::local_time());
    for (it_type i = dynMap->begin(); i != dynMap->end(); i++){
        std::vector<DynClassInfo> classVec = i->second;
        for (int s = 0; s < classVec.size(); s++) {
            DynClassInfo oninfo = classVec[s];
            boost::posix_time::ptime thisTime = oninfo.getTimeStamp();
            if (earliestTime > thisTime) {
                earliestTime = thisTime;
            }
        }
    }

    for (it_type i = dynMap->begin(); i != dynMap->end(); i++){
        std::vector<DynClassInfo> classVec = i->second;
        //std::cout << classVec.size() << "\n"; 
        for (int s = 0; s < classVec.size(); s++) {
            //std::cout << "S: " << s << "\n";
            DynClassInfo oninfo = classVec[s];
            //if (!oninfo.getOn()) {
            //    std::cout << "BAD! Function: " << oninfo.getFunctionName() << "\n";
            //}
            boost::posix_time::ptime onts = oninfo.getTimeStamp();
            DynClassInfo offinfo = classVec.at(s+1);
            boost::posix_time::ptime offts = offinfo.getTimeStamp();
            boost::posix_time::time_duration timeDuration = offts-onts;
            //std::cout << "Total Time duration: " << timeDuration.total_milliseconds();
            std::string className = oninfo.getClassName();
            //std::cout << "Before mapAt\n";
            InstrumentProfile iprof = instrumentMap.at(className);
           // std::cout << "After mapAt\n";
            std::string fullFuncName = oninfo.getFunctionName();
            int needle = fullFuncName.find("::");
            std::string ss = fullFuncName.substr(needle+2, fullFuncName.size());
            //std::cout << "HERE?\n";
            while (ss.find("::") != std::string::npos) {
                needle = ss.find("::");
                ss = ss.substr(needle+2, ss.size());
            }
            //std::cout << " Func Name: " << ss;
            std::map<std::string, int> mtnm = iprof.getMethodToNoteMap();
            //for (std::map<std::string, int>::iterator ite = mtnm.begin(); ite !=mtnm.end(); ite++) {
            //    std::string key = ite->first;
            //    std::cout << " instrument key: " << key << "\n";
            //}
            int note = mtnm.at(ss);
            //std::cout << " Note: " << note << "\n";
            s++;

            long totalTime = timeDuration.total_milliseconds();
            if (totalTime < 10) {
                totalTime = 10;
            }

            boost::posix_time::time_duration relativeTimeStamp = onts - earliestTime;

            // <note> <velocity> <channel> <duration> <track template>
            std::cout << note << ",100," << iprof.getChannel() << "," << totalTime <<
            "," << iprof.getTrackTemplate() << "," << relativeTimeStamp.total_milliseconds() <<
            "\n";
            //<< "," << oninfo.getFunctionName() << "\n";
        }
    }

**/

/**
typedef std::map<std::string, std::vector<DynClassInfo> >::iterator it_type;
for (it_type iterator = dynMap.begin(); iterator != dynMap.end(); iterator++) {
std::vector<DynClassInfo> info = iterator->second;
for (int i = 0; i < info.size(); i++) {
// get the instrument profile
DynClassInfo tinfo = info[i];
tinfo.printClassInfo();
}
}
**/

// TESTING ----------------------

/*
// This is to check that all Inheritances are grouped together properly
std::cout << "This is the Inheritances" << std::endl;
for( std::vector<std::vector<std::string> >::const_iterator i = inheritanceTree.begin(); i != inheritanceTree.end(); ++i){
std::cout << "Vector: ";
std::cout << i - inheritanceTree.begin() << std::endl;
for( std::vector<std::string>::const_iterator j = (*i).begin(); j != (*i).end(); ++j){
std::cout << *j << ' ' << std::endl;
}
}
*/

//This is for Dyninst to use for iterating over it's functions
/* std::cout << "This is printing from the list of Class" << std::endl;
for( std::vector<ClassProfile>::const_iterator i = listClasses.begin(); i != listClasses.end(); ++i){
std::cout << "This is profile: ";
std::cout << (*i).getProfile() << std::endl;
vector<std::string> methods = (*i).getMethods();
for( std::vector<std::string>::const_iterator j = methods.begin(); j != methods.end(); ++j){
std::cout << *j << ' ' << std::endl;
}
}*/




/*
std::cout << "This is the Dependencies" << std::endl;
for(std::vector<std::set<std::string> >::const_iterator k = dependencyTree.begin(); k != dependencyTree.end(); ++k){
std::cout << "Vector: " << k - dependencyTree.begin() << std::endl;
for( std::set<std::string>::const_iterator q = (*k).begin(); q != (*k).end(); ++k){
std::cout << *q << ' '<< std::endl;
}
}
*/
/*
//std::cout << "here?\n";
int counter = 0;
//for(std::vector<std::set<std::string> >::const_iterator dtit = dependencyTree.begin(); dtit != dependencyTree.end(); ++dtit){
// if ((*dtit).size() == 0) {
// dependencyTree.erase(dtit);
// }
//}
for(std::vector<std::set<std::string> >::const_iterator dtit = dependencyTree.begin(); dtit != dependencyTree.end(); ++dtit){
std::cout << std::endl << "FinVector " << counter << std::endl;
counter = counter + 1;
for(std::set<std::string>::const_iterator sdtit = (*dtit).begin(); sdtit != (*dtit).end(); ++sdtit){
std::cout << *sdtit << std::endl;
}
}
*/
/* Experimental
std::cout << "cBLood => " << classmap.find("cBlood") << '\n';
std::cout << "loadlevel => " << classmap.find("loadlevel") << '\n';
it = classmap.find("playerTouchRight");
std::string s = it->second;
std::cout << s << std::endl;
*/

// Reference Code That You Can Ignore
// assert(a.IsArray());
// rapidjson::SizeType s = 0;
// std::cout << a[s].GetString() << "\n";

// //assert(d[\"1\"].IsObject());
// const rapidjson::Value& b = d[\"classDate\"];
// assert(b[\"1\"].IsObject());
// const rapidjson::Value& a = b[\"1\"];
// const rapidjson::Value& fields = a[\"fields\"];
// assert(fields.IsArray());
// for (rapidjson::SizeType i = 0; i < fields.Size(); i++) // rapidjson uses SizeType instead of size_t.
// printf(\"a[%d] = %s\n\", i, fields[i].GetString());
}

