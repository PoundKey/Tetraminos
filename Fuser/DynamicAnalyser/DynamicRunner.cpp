#include "DynamicRunner.h"

// DynamicRunner initializer
// sets up the app image and gets the functions from the
// app

// TODO - Need to take in another parameter - a pointer to a vector
// that holds the static class information - and use that to map
// class data to functions. Once we have that here, it will be 
// easy to identify when a function is called and play a note.
DynamicRunner::DynamicRunner(int pid, std::vector<ClassProfile>& c) {

	std::vector<BPatch_process*> *getProcesses();

	BPatch *bpatch = new BPatch();
	// Attach the bpatch to the process given by pid
    appProc = bpatch->processAttach("exe", pid);
    // Get the process image
    appImage = appProc->getImage();
    classes = c;
	// get the methods from the class profile
   // for (int i = 0; i < classes.size(); i++) {
    //	std::vector<std::string> classFunctions = classes[i].getMethods();
    	// append the functions for each class onto the class profile functions
    //	functions.insert(functions.end(), classFunctions.begin(), classFunctions.end());
    //}
    //functions = *(appImage->getProcedures());
   
}

// The workhorse function
void DynamicRunner::analyze() {
	std::cout << "size is: " << classes.size() << "\n";
	for (int i = 0; i < classes.size(); i++) {
		ClassProfile thisClass = classes[i];
		// find the name of the class
		std::string className = thisClass.getProfile();
		std::vector<std::string> classFunctions = thisClass.getMethods();
		//printf("Are we here?\n");
		for (int s = 0; s < classFunctions.size(); s++) {
			std::string thisFuncName = classFunctions[s];
			std::vector<BPatch_function *> funcsMatchingName;
			std::string fullFuncName = className;
			fullFuncName.append("::");
			fullFuncName.append(thisFuncName);
			appImage->findFunction(fullFuncName.c_str(), funcsMatchingName);
			BPatch_function *this_function = funcsMatchingName[0];
			/**
			BPatch_module *funcMod = thisFunc->getModule();
			char nameBuffer[256];
			funcMod->getName(nameBuffer, 256);
			std::cout << "Module name for function: " << thisFuncName << " is: ";
			printf("%s", nameBuffer);
			printf("\n");
			**/
			
		

			// Load up this function into a BPatch_function variable
			//BPatch_function *this_function = functions[i];
			
			// setup a vector of funcpoints and set to the entry point of this_function
			// NOTE - the 'entry' point is essentially a memory address
			std::vector<BPatch_point *> *func_points;
			func_points = this_function->findPoint(BPatch_entry);
			// Create a snippet that calls printf every time a function is called
			std::vector<BPatch_snippet *> printfArgs;
			// Setup the funcString - format is "Function Called! Name is: _____\n"
			std::string funcString = className;
			funcString.append(",");
			funcString.append(this_function->getName());
			funcString.append(",on\n");
			// Create a BPatch_constExpr using the funcString
			BPatch_snippet *fmt = new BPatch_constExpr(funcString.c_str());
			printfArgs.push_back(fmt);

			std::vector<BPatch_function *> printfFuncs;
			// Find printf in the system library
			appImage->findFunction("printf", printfFuncs);
			// create a BPatch_funcCallExpr using printf and the string argument defined earlier
			BPatch_funcCallExpr printfCall(*(printfFuncs[0]), printfArgs);
			injectFuncIntoFunc(printfCall, func_points);

			funcString.erase(funcString.end()-3, funcString.end());
			funcString.append(",off\n");

			func_points = this_function->findPoint(BPatch_exit);
			printfArgs.pop_back();
			BPatch_snippet *fmt1 = new BPatch_constExpr(funcString.c_str());
			printfArgs.push_back(fmt1);

			BPatch_funcCallExpr printfCallEnd(*(printfFuncs[0]), printfArgs);

			injectFuncIntoFunc(printfCallEnd, func_points);
			/**
			std::string comparor = "apply_surface";
			// Here for speed purposes - "apply_surface" is the render function in pacman
			// so I am ignoring it for now - will find a better way to do this -- TODO
			if (this_function->getName().compare(comparor) != 0) {
				injectFuncIntoFunc(printfCall, func_points);
			}
			**/
			
		}
	}

	// infinitely loop
	appProc->continueExecution();
	while (!appProc->isTerminated()) {
	
	}
}

void DynamicRunner::injectFuncIntoFunc(BPatch_funcCallExpr funcToInject, std::vector<BPatch_point *> *func_points) {
	appProc->insertSnippet(funcToInject, *func_points);
}

/** 
* Find a function based on the function class name and the 
* function name in the mutatee
**/
BPatch_function* DynamicRunner::findFunctionWithClassName(
	std::string funcName, 
	std::string className, 
	std::vector<BPatch_function *> funcsMatchingName
	) {
	for (int i = 0; i < funcsMatchingName.size(); i++) {

	}
	return NULL;
}


