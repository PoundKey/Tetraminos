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
```
