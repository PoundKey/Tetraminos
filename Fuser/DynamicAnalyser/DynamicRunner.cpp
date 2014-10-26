#include "DynamicRunner.h"

DynamicRunner::DynamicRunner(int pid, std::string exeName, ) {

	std::vector<BPatch_process*> *getProcesses();

	BPatch *bpatch = new BPatch();
	// Attach the bpatch to the process given by pid
    appProc = bpatch->processAttach(exeName.c_str(), pid);
    // Get the process image
    appProc->getImage();

    std::vector<BPatch_module *>* modules = appImage->getModules();
	BPatch_module *appMod = modules[0][0];

    functions = *(appMod->getProcedures());

    //for (int i = 0; i < functions.size(); i++) {
	//	BPatch_function *this_function = functions[i];
	//	std::cout << "Function name: " << this_function->getName() << "\n";
	//}
}

void DynamicRunner::injectIntoFunction(BPatch_variableExpr *toInject, BPatch_arithExpr arithExpr, std::vector<BPatch_point *> *points) {

}


