CPSC410
=======

To start the server, nevigate to the Tetraminos directory.
```bash
cd Tetraminos
npm start
```
NodeJS now is running at:
```
http://localhost:3000
```
Dynamic Analyser
================

In the real world, the entire system would be sitting on a web server somewhere, but for development purposes we are just using a local development server.

The Dynamic Analyser is reliant on the user having Dyninst installed on their system. It can be installed here: http://www.dyninst.org/downloads/dyninst-8.x

Once installation is complete, compile (on Ubuntu) using the following command (adjust source files as needed)

g++ -I/usr/local/dyninst/include [___.cpp files] -L/usr/local/dyninst/lib -ldyninstAPI -lcommon -lsymtabAPI -lpatchAPI -lparseAPI -lstackwalk -lpcontrol -linstructionAPI -ldynC_API -ldynDwarf -ldynElf -lsymLite -lsymtabAPI

Dynamic Analyser code exists in Fuser/DynamicAnalyser. Note that test.cpp and main.cpp are unused in the context of the entire project, but are there for testing purposes so that we don't need to compile and load an entire game every time we want to attach to a process and check that dyninst is working as it should.

Testing Process
================

1. Static Analyzer: Takes a directory of pre-computed xml class files as input; the output expected to be a single JSON object which contains the following format: {staticInfo: [ {className: value, method: [ ], field: [ ], inheritance: value, dependency: [ ]} ...] }
The console prints out error messages for class files not processed by the static parser.

2. Fuser: Takes in a single JSON file computed by the parser as input, assert the file conform s with the class pre-defined structure (Otherwise an exception is thrown). Along with each class profile creation, the consoles prints out it's corresponding string representation.

3. Framework (The wrapper): Triggers the automation process by clicking one of the 
codebase selection button. It calls the execution cmd to invoke the game (codebase), following by calling the fuser to fetch the game process id and further invocation on the dynamic analyzer. Again, each invocation prints the assertion statement on the console verifying that the control flow executes correctly according to the plan.

4. Dynamic Analyser: See last paragraph of Dynamic Analyser section for brief description of testing process.

Codebase
================
Ideally our analyzer works on every legitimate C++ code bases.
