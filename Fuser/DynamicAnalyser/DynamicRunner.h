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
#include "../../Tetraminos/static_lib/ClassProfile.h"

class DynamicRunner {

public:
	DynamicRunner(int pid, std::vector<ClassProfile>& c);
	void analyze();

private:
	BPatch_process *appProc;
	BPatch_image *appImage;
	std::vector<ClassProfile> classes;
	std::vector<BPatch_point *> *points;

	void injectFuncIntoFunc(BPatch_funcCallExpr funcToInject, std::vector<BPatch_point *> *func_points);
	BPatch_function* findFunctionWithClassName(std::string funcName, std::string className, std::vector<BPatch_function *> funcsMatchingName);

	//void injectIntoFunction(BPatch_variableExpr *toInject, BPatch_arithExpr arithExpr, std::vector<BPatch_point *> *points);
};