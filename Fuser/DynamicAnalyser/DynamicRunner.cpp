#include "DynamicRunner.h"

DynamicRunner::DynamicRunner(int pid, std::string exeName ) {

	std::vector<BPatch_process*> *getProcesses();

	BPatch *bpatch = new BPatch();
	// Attach the bpatch to the process given by pid
    appProc = bpatch->processAttach(exeName.c_str(), pid);
    // Get the process image
    appImage = appProc->getImage();

    std::vector<BPatch_module *>* modules = appImage->getModules();
	BPatch_module *appMod = modules[0][0];

    functions = *(appMod->getProcedures());
    //printf("Did i get here?\n");
    for (int i = 0; i < functions.size(); i++) {
		BPatch_function *this_function = functions[i];
		//std::cout << "Function name: " << this_function->getName() << "\n";
		std::vector<BPatch_point *> *func_points;
		func_points = this_function->findPoint(BPatch_entry);
		// Create a snippet that calls printf with each effective address
		std::vector<BPatch_snippet *> printfArgs;
		std::string funcString = "Function Called! Name is:";
		funcString.append(this_function->getName());
		funcString.append("\n");
		BPatch_snippet *fmt = new BPatch_constExpr(funcString.c_str());
		printfArgs.push_back(fmt);
		//std::string fname = this_function->getName();
		BPatch_snippet *fmt2 = new BPatch_constExpr((const char*)this_function->getName().c_str());
		//printf("%s",this_function->getName().c_str());
		//printf("\n");
		//printfArgs.push_back(fmt2);
		std::vector<BPatch_function *> printfFuncs;
		appImage->findFunction("printf", printfFuncs);
		BPatch_funcCallExpr printfCall(*(printfFuncs[0]), printfArgs);
		//printf("Do I get to here??");
		std::string comparor = "apply_surface";
		if (this_function->getName().compare(comparor) != 0) {
			//std::cout << "This is apply_surface!\n";
			appProc->insertSnippet(printfCall, *func_points);
		}
		
		//printf("HELLO?\n");
	}
	appProc->continueExecution();
	while (!appProc->isTerminated()) {

	}
}

//void DynamicRunner::injectIntoFunction(BPatch_variableExpr *toInject, BPatch_arithExpr arithExpr, std::vector<BPatch_point *> *points) {
//
//}


