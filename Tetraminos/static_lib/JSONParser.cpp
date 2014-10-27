#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "ClassProfile.cpp"
#include <iostream>
#include <map>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
// #include "Sonifier/OSCmessenger.cpp"
#include "../../Fuser/DynamicAnalyser/DynamicRunner.h"

int main(int argc, char * argv[]) {
    std::map<std::string,std::string> classmap;
    std::map<std::string,std::string>::iterator it;
    std::string jsonName = argv[2];


    //TODO: IMPORT JSON FILE
    // http://stackoverflow.com/questions/18107079/rapidjson-working-code-for-reading-document-from-file
    FILE * pFile = fopen (argv[2] , "r");
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
            std::cout << temp["className"].GetString() << "\n";

            // Create Profile
            cl.setProfile(temp["className"].GetString());
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
            const rapidjson::Value& tempInhert = temp["inheritance"];
            for (rapidjson::SizeType k = 0; k < tempInhert.Size(); k++){
                printf("inheritance[%d] = %s\n", k, tempInhert[k].GetString());
                // Line space for instrument creationtempField[h].GetString() to access field
                cl.setField(tempInhert[k].GetString()); 
            }
            const rapidjson::Value& tempDep = temp["dependency"];
            for (rapidjson::SizeType k = 0; k < tempDep.Size(); k++){
                printf("dependency[%d] = %s\n", k, tempDep[k].GetString());

                cl.setField(tempDep[k].GetString());
                // Line space for instrument creationtempField[h].GetString() to access field
            }
            listClasses.push_back(cl);
    }

    // Send list of profiles to OSCMessenger to create instruments
    createInstruments();


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

    DynamicRunner(pid, exeName);

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



