#include "DynamicRunner.h"

// DynamicRunner initializer
// sets up the app image and gets the functions from the
// app

// TODO - Need to take in another parameter - a pointer to a vector
// that holds the static class information - and use that to map
// class data to functions. Once we have that here, it will be 
// easy to identify when a function is called and play a note.
DynamicRunner::DynamicRunner(int pid, std::string exeName ) {

	std::vector<BPatch_process*> *getProcesses();

	BPatch *bpatch = new BPatch();
	// Attach the bpatch to the process given by pid
    appProc = bpatch->processAttach(exeName.c_str(), pid);
    // Get the process image
    appImage = appProc->getImage();

	// get the methods from the app image
    functions = *(appImage->getProcedures());
   
}

// The workhorse function
void DynamicRunner::analyse() {
	for (int i = 0; i < functions.size(); i++) {
		// Load up this function into a BPatch_function variable
		BPatch_function *this_function = functions[i];
		
		// setup a vector of funcpoints and set to the entry point of this_function
		// NOTE - the 'entry' point is essentially a memory address
		std::vector<BPatch_point *> *func_points;
		func_points = this_function->findPoint(BPatch_entry);
		// Create a snippet that calls printf every time a function is called
		std::vector<BPatch_snippet *> printfArgs;
		// Setup the funcString - format is "Function Called! Name is: _____\n"
		std::string funcString = "Function Called! Name is:";
		funcString.append(this_function->getName());
		funcString.append("\n");
		// Create a BPatch_constExpr using the funcString
		BPatch_snippet *fmt = new BPatch_constExpr(funcString.c_str());
		printfArgs.push_back(fmt);

		std::vector<BPatch_function *> printfFuncs;
		// Find printf in the system library
		appImage->findFunction("printf", printfFuncs);
		// create a BPatch_funcCallExpr using printf and the string argument defined earlier
		BPatch_funcCallExpr printfCall(*(printfFuncs[0]), printfArgs);
		std::string comparor = "apply_surface";
		// Here for speed purposes - "apply_surface" is the render function in pacman
		// so I am ignoring it for now - will find a better way to do this -- TODO
		if (this_function->getName().compare(comparor) != 0) {
			injectFuncIntoFunc(printfCall, func_points);
		}
	}

	// infinitely loop
	appProc->continueExecution();
	//while (!appProc->isTerminated()) {
	//
	//}
}

void DynamicRunner::injectFuncIntoFunc(BPatch_funcCallExpr funcToInject, std::vector<BPatch_point *> *func_points) {
	appProc->insertSnippet(funcToInject, *func_points);
}


