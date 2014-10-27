#include <stdio.h>
#include <vector>
#include "BPatch.h"
#include "BPatch_process.h"
#include "BPatch_function.h"
#include "BPatch_Vector.h"
#include "BPatch_thread.h"
#include "BPatch_point.h"
#include <iostream>
#include <string>

class DynamicRunner {

public:
	DynamicRunner(int pid, std::string exeName);
	void analyse();

private:
	BPatch_process *appProc;
	BPatch_image *appImage;
	std::vector<BPatch_function *> functions;
	std::vector<BPatch_point *> *points;

	void injectFuncIntoFunc(BPatch_funcCallExpr funcToInject, std::vector<BPatch_point *> *func_points);

	//void injectIntoFunction(BPatch_variableExpr *toInject, BPatch_arithExpr arithExpr, std::vector<BPatch_point *> *points);
};