#include "JSONParser.h"

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

int main(int argc, char * argv[]) {
    std::map<std::string,std::string> classmap;
    std::map<std::string,std::string>::iterator it;
    std::map<std::string,ClassProfile::ClassProfile> profilemap;
    std::map<std::string,ClassProfile::ClassProfile>::iterator pmit;

    JSONParser jp;

    std::map<int, std::vector<ClassProfile::ClassProfile> > inheritanceCounts;
    std::vector<ClassProfile::ClassProfile>::iterator icit;
    int maxInheritances = 0; 

    // Name of JSON File to be parsed
    std::string jsonName = argv[2];


    //TODO: IMPORT JSON FILE
    // http://stackoverflow.com/questions/18107079/rapidjson-working-code-for-reading-document-from-file
    FILE * pFile = fopen(argv[2] , "r");
    rapidjson::FileStream is(pFile);
    rapidjson::Document d;
    d.ParseStream<0>(is);

    // This is test data
	// const char myJson[] = "{\"staticInfo\":[{\"className\":\"cBlood\", \"method\":[\"cBlood\", \"update\", \"newBlood\", \"oneNewBlood\", \"reset\"],\"field\":[  \"bloodparts\",\"BLOODcount\"],\"inheritance\":[],\"dependency\":[]},{\"className\":\"cBlood\", \"method\":[\"cBlood\", \"update\", \"newBlood\", \"oneNewBlood\", \"reset\"],\"field\":[  \"bloodparts\",\"BLOODcount\"],\"inheritance\":[],\"dependency\":[]}]}";


    rapidjson::Document child;
    const rapidjson::Value& b = d["staticInfo"];
    assert(b.IsArray());
    std::vector<ClassProfile::ClassProfile> listClasses;
    for (rapidjson::SizeType i = 0; i < b.Size(); i++){ // rapidjson uses SizeType instead of size_t.
            
            //New cl object
            ClassProfile::ClassProfile cl;
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
            if (!cl.getInheritance().empty()){
                maxInheritances = 1;
                inheritanceCounts[1].push_back(cl);
            } else {
            inheritanceCounts[0].push_back(cl);
        }

    }

    // Construct Inheritance "Trees" to pass for Instrument Creation
    std::vector<std::vector<std::string> > inheritanceTree;
    std::vector<ClassProfile::ClassProfile> leftovers;
    for (int i = 0; i <= maxInheritances; i++){
        if (i==0){
        for(std::vector<ClassProfile>::size_type h = 0; h != inheritanceCounts[i].size(); h++){
            std::vector<std::string> tempClassName;
            tempClassName.push_back(inheritanceCounts[i][h].getProfile());
            inheritanceTree.push_back(tempClassName);
        }
    }else {
        for(std::vector<ClassProfile::ClassProfile>::size_type l = 0; l != leftovers.size(); l++){
            int placeInt1;
            if(0 < jp.containsMatch(inheritanceTree, leftovers[l].getInheritance(), placeInt1)){
                inheritanceTree[placeInt1].push_back(leftovers[l].getProfile());
                leftovers.erase(leftovers.begin()+l);
            }
        }   


        for(std::vector<ClassProfile>::size_type m = 0; m != inheritanceCounts[i].size(); m++){
            int placeInt2;
            if(0 < jp.containsMatch(inheritanceTree, inheritanceCounts[i][m].getInheritance(), placeInt2)){
                inheritanceTree[placeInt2].push_back(inheritanceCounts[i][m].getProfile());
            } else {
                leftovers.push_back(inheritanceCounts[i][m]);
            }
            
        }
        
        }
    }

   


    // TODO: Pass listClasses too Dyninst.
    //This is for Dyninst to use for iterating over it's functions
    std::cout << "This is printing from the list of Class" << endl;
    for( std::vector<ClassProfile::ClassProfile>::const_iterator i = listClasses.begin(); i != listClasses.end(); ++i){
        std::cout << "This is profile: ";
        std::cout << (*i).getProfile() << endl;
        vector<std::string> methods = (*i).getMethods();
        for( std::vector<string>::const_iterator j = methods.begin(); j != methods.end(); ++j){
             std::cout << *j << ' ' << std::endl;
         }
        }

   
    /* Call to dyninst passing pid and listClasses

    callToDyninst(pid, listClasses);
    */



// GET Process ID of launched exe  http://stackoverflow.com/questions/865152/how-can-i-get-a-process-handle-by-its-name-in-c
    /* UNCOMMENT
    std::string exeName = argv[1];
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

    createInstruments(profilemap, INHERITANCE TREE); 
    DynamicRunner(pid, exeName);
    */

    // CloseHandle(snapshot);

    




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
}




